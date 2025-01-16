#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include "mainwindow.h"
#include "attendance.h"
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QSqlQuery>
#include <QSqlError>

// Function to initialize the database and create the table if necessary
void login::initializeDatabase() {
    // Create the necessary directories if they don't exist
    QString dbPath = QCoreApplication::applicationDirPath() + "/database";
    QDir dir;
    if (!dir.exists(dbPath)) {
        dir.mkpath(dbPath);
    }

    // Set up the SQLite database using the member variable 'db'
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath + "/users.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", "Failed to open the database: " + db.lastError().text());
        return;
    }

    QSqlQuery query;
    // Create the 'users' table if it doesn't exist
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS users (
            userid TEXT PRIMARY KEY,
            password TEXT NOT NULL
        )
    )";
    if (!query.exec(createTableQuery)) {
        QMessageBox::critical(this, "Database Error", "Failed to create table: " + query.lastError().text());
        return;
    }

    // Create the 'subjects' table if it doesn't exist
    QString createSubjectsTableQuery = R"(
        CREATE TABLE IF NOT EXISTS subjects (
            name TEXT,
            userid TEXT NOT NULL,
            attended INTEGER NOT NULL DEFAULT 0,
            total INTEGER NOT NULL DEFAULT 0,
            PRIMARY KEY (name, userid),
            FOREIGN KEY (userid) REFERENCES users(userid) ON DELETE CASCADE
        )
    )";

    if (!query.exec(createSubjectsTableQuery)) {
        QMessageBox::critical(this, "Database Error", "Failed to create subjects table: " + query.lastError().text());
        return;
    }
}

login::login(QWidget *parent)
    : QDialog(parent), ui(new Ui::login) {
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &login::onLogin);
    initializeDatabase();
}

login::~login() {
    db.close();
    delete ui;
}

void login::onLogin() {
    QString uName = ui->lineEdit->text();
    QString uCode = ui->lineEdit_2->text();

    if (!db.isOpen()) {
        QMessageBox::critical(this, "Error", "Database is not open.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT userid FROM users WHERE userid = ? AND password = ?");
    query.addBindValue(uName);
    query.addBindValue(uCode);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to execute query: " + query.lastError().text());
        return;
    }

    if (uName == "admin" && uCode == "213") {
        MainWindow *w = new MainWindow();
        w->show();
        this->close();
    } else {
        if (query.next()) {
            attendance *w = new attendance(uName);
            w->show();
            this->close();
        } else {
            QMessageBox::warning(this, "Error Box", "Invalid username or password.");
        }
    }
}
