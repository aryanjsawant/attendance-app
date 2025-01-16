#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

// Database file path
const QString dbFilePath = QCoreApplication::applicationDirPath() + "/database/Info.db";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , addbox(new addBox(this))
    , findbox(new findBox(this))
    , updatebox(new updateBox(this))
{
    ui->setupUi(this);

    // Initialize database
    initializeDatabase();

    // Connect signals to slots
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::addFun);
    connect(addbox, &addBox::enterInfo, this, &MainWindow::getInfo);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::searchFunction);
    connect(findbox, &findBox::onSearch, this, &MainWindow::onFind);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::deleteFunction);
    connect(updatebox, &updateBox::newSub, this, &MainWindow::onDelete);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete addbox;
    delete findbox;
    delete updatebox;
}

void MainWindow::initializeDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilePath);

    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", "Failed to open database: " + db.lastError().text());
        return;
    }

    db.close();
}

void MainWindow::addFun()
{
    addbox->show();
}

void MainWindow::getInfo(const QString &userid, const QString &password)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilePath);

    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", "Failed to open database: " + db.lastError().text());
        return;
    }

    QSqlQuery query;

    // Check if the user already exists
    query.prepare("SELECT COUNT(*) FROM users WHERE userid = ?");
    query.addBindValue(userid);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to execute query: " + query.lastError().text());
        db.close();
        return;
    }

    query.next();
    int userCount = query.value(0).toInt();

    if (userCount > 0) {
        QMessageBox::critical(this, "Error", "User with this ID already exists.");
        db.close();
        return;
    }

    // Insert the new user if it doesn't already exist
    query.prepare("INSERT INTO users (userid, password) VALUES (?, ?)");
    query.addBindValue(userid);
    query.addBindValue(password);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "User added successfully.");
    } else {
        QMessageBox::warning(this, "Database Error", "Failed to add user: " + query.lastError().text());
    }

    db.close();
    addbox->close();  // Assuming 'addbox' is a valid pointer to a UI element.
}

void MainWindow::searchFunction()
{
    findbox->show();
}

void MainWindow::onFind(const QString &userid)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilePath);

    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", "Failed to open database: " + db.lastError().text());
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT userid, password FROM users WHERE userid = ?");
    query.addBindValue(userid);

    if (query.exec() && query.next()) {
        QString foundUserId = query.value(0).toString();
        QString password = query.value(1).toString();

        QMessageBox::information(this, "User Found",
                                 "User ID: " + foundUserId + "\n" +
                                 "Password: " + password);
    } else {
        QMessageBox::warning(this, "Not Found", "User with ID " + userid + " not found.");
    }

    db.close();
    findbox->close();
}

void MainWindow::deleteFunction()
{
    updatebox->show();
}

void MainWindow::onDelete(const QString &rn)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilePath);

    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", "Failed to open database: " + db.lastError().text());
        return;
    }

    QSqlQuery query;
    // Prepare a query to delete the user with the given roll number
    query.prepare("DELETE FROM users WHERE userid = ?");
    query.addBindValue(rn);

    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            QMessageBox::information(this, "Success", "User with roll number " + rn + " deleted successfully.");
        } else {
            QMessageBox::warning(this, "Not Found", "User with roll number " + rn + " not found.");
        }
    } else {
        QMessageBox::warning(this, "Database Error", "Failed to delete user: " + query.lastError().text());
    }

    db.close();
    updatebox->close();
}
