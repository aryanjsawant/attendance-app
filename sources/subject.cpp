#include "subject.h"
#include "ui_subject.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlDatabase>

subject::subject(QWidget *parent, const QString &userid) :
    QDialog(parent),
    ui(new Ui::subject),
    userid(userid)
{
    ui->setupUi(this);

    // Set up the layout for dynamically adding radio buttons
    subjectLayout = new QVBoxLayout(ui->scrollArea->widget());
    ui->scrollArea->widget()->setLayout(subjectLayout);

    // Load subjects from database
    loadSubjectsFromDatabase();

    // Connect the OK and Cancel buttons
    connect(ui->okButton, &QPushButton::clicked, this, &subject::onOkClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &subject::onCancelClicked);
}

subject::~subject()
{
    delete ui;
}

void subject::loadSubjectsFromDatabase()
{
    const QString dbFilePath = QCoreApplication::applicationDirPath() + "/database/users.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilePath);

    if (!db.open()) {
        QMessageBox::warning(this, "Error", "Failed to open database: " + db.lastError().text());
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT name FROM subjects WHERE userid = ?");
    query.addBindValue(userid);

    if (!query.exec()) {
        QMessageBox::warning(this, "Error", "Failed to fetch subjects: " + query.lastError().text());
        db.close();
        return;
    }

    // Add checkboxes for each subject
    while (query.next()) {
        QString subjectName = query.value("name").toString();
        QCheckBox *checkBox = new QCheckBox(subjectName, this);
        subjectCheckBoxes.append(checkBox);
        subjectLayout->addWidget(checkBox);
    }

    db.close();
}

void subject::onOkClicked()
{
    deleteSelectedSubjects();
    accept();
}

void subject::onCancelClicked()
{
    reject();
}

void subject::deleteSelectedSubjects()
{
    const QString dbFilePath = QCoreApplication::applicationDirPath() + "/database/users.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilePath);

    if (!db.open()) {
        QMessageBox::warning(this, "Error", "Failed to open database: " + db.lastError().text());
        return;
    }

    for (QCheckBox *checkBox : subjectCheckBoxes) {
        if (checkBox->isChecked()) {
            QString subjectName = checkBox->text();
            QSqlQuery query;
            query.prepare("DELETE FROM subjects WHERE name = ? AND userid = ?");
            query.addBindValue(subjectName);
            query.addBindValue(userid);

            if (!query.exec()) {
                QMessageBox::warning(this, "Error", "Failed to delete subject: " + query.lastError().text());
            }
        }
    }

    db.close();
}
