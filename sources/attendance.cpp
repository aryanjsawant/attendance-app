#include "attendance.h"
#include "subject.h"
#include "ui_attendance.h"
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTextStream>
#include <QIcon>

attendance::attendance(const QString& userid, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::attendance)
    , deleteSubjects(new subject(parent, userid))
{
    ui->setupUi(this);

    // Setup scroll area
    QWidget* scrollContent = new QWidget(ui->scrollArea);
    scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->setAlignment(Qt::AlignTop);
    scrollContent->setLayout(scrollLayout);
    ui->scrollArea->setWidget(scrollContent);
    QString iconPath = QCoreApplication::applicationDirPath();
    ui->pushButton_2->setIcon(QIcon(iconPath + "/media/trash.png"));

    user = userid;
    // Database connection
    const QString dbFilePath = QCoreApplication::applicationDirPath() + "/database/users.db";
    QSqlDatabase db = QSqlDatabase::database();
    db.setDatabaseName(dbFilePath);

    if (!db.open()) {
        QMessageBox::warning(this, "Error", "Failed to open database: " + db.lastError().text());
        return;
    }

    // Load subjects from database
    QSqlQuery query;
    query.prepare("SELECT * FROM subjects WHERE userid = ?");
    query.addBindValue(user);

    if (!query.exec()) {
        QMessageBox::warning(this, "Error", "Failed to execute query: " + query.lastError().text());
        db.close();
        return;
    }

    while (query.next()) {
        QString subjectName = query.value("name").toString();
        int attended = query.value("attended").toInt();
        int total = query.value("total").toInt();

        SubjectBox* s1 = new SubjectBox(user, subjectName);
        s1->setDetails(user, subjectName, attended, total);

        // Reconnect signals for loaded SubjectBoxes
        connect(s1->presentButton, &QPushButton::clicked, s1, &SubjectBox::onPresentClicked);
        connect(s1->absentButton, &QPushButton::clicked, s1, &SubjectBox::onAbsentClicked);

        subjectBoxes.append(s1);
        scrollLayout->addWidget(s1);
    }

    connect(ui->pushButton, &QPushButton::clicked, this, &attendance::newSubject);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &attendance::openSubjectDialog);

    db.close();
}

void attendance::openSubjectDialog()
{
    
    subject *subDlg = new subject(this, user);
    subDlg->exec();
    delete subDlg;
    QLayoutItem *child;
    while ((child = scrollLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }
    // Database connection
    const QString dbFilePath = QCoreApplication::applicationDirPath() + "/database/users.db";
    QSqlDatabase db = QSqlDatabase::database();
    db.setDatabaseName(dbFilePath);

    if (!db.open()) {
        QMessageBox::warning(this, "Error", "Failed to open database: " + db.lastError().text());
        return;
    }

    // Load subjects from database
    QSqlQuery query;
    query.prepare("SELECT * FROM subjects WHERE userid = ?");
    query.addBindValue(user);

    if (!query.exec()) {
        QMessageBox::warning(this, "Error", "Failed to execute query: " + query.lastError().text());
        db.close();
        return;
    }

    while (query.next()) {
        QString subjectName = query.value("name").toString();
        int attended = query.value("attended").toInt();
        int total = query.value("total").toInt();

        SubjectBox* s1 = new SubjectBox(user, subjectName);
        s1->setDetails(user, subjectName, attended, total);

        // Reconnect signals for loaded SubjectBoxes
        connect(s1->presentButton, &QPushButton::clicked, s1, &SubjectBox::onPresentClicked);
        connect(s1->absentButton, &QPushButton::clicked, s1, &SubjectBox::onAbsentClicked);

        subjectBoxes.append(s1);
        scrollLayout->addWidget(s1);
    }

    db.close();
}

void attendance::newSubject() {
    QString newSubjectName = QInputDialog::getText(this, "New Subject", "Enter subject name:");

    if (newSubjectName.isEmpty()) {
        return;  // No subject name entered
    }

    const QString dbFilePath = QCoreApplication::applicationDirPath() + "/database/users.db";
    QSqlDatabase db = QSqlDatabase::database();
    db.setDatabaseName(dbFilePath);

    if (!db.open()) {
        QMessageBox::warning(this, "Error", "Failed to open database: " + db.lastError().text());
        return;
    }

    // Check if the subject already exists for this user
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM subjects WHERE name = ? AND userid = ?");
    checkQuery.addBindValue(newSubjectName);
    checkQuery.addBindValue(this->user);

    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Duplicate Subject", "Subject already exists!");
        return;
    }

    // Insert the new subject
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO subjects (name, attended, total, userid) VALUES (?, ?, ?, ?)");
    insertQuery.addBindValue(newSubjectName);
    insertQuery.addBindValue(0);
    insertQuery.addBindValue(0);
    insertQuery.addBindValue(this->user);

    if (!insertQuery.exec()) {
        QMessageBox::warning(this, "Error", "Failed to insert subject: " + insertQuery.lastError().text());
    } else {
        // Create a new SubjectBox and add it to the UI
        SubjectBox* newBox = new SubjectBox(this->user, newSubjectName, this);
        subjectBoxes.append(newBox);
        scrollLayout->addWidget(newBox);
    }

    db.close();
}

attendance::~attendance()
{
    qDeleteAll(subjectBoxes);
    delete deleteSubjects;
    delete ui;
}

// SubjectBox class modifications
SubjectBox::SubjectBox(const QString& user, const QString& subjectName, QWidget* parent) 
    : QGroupBox(parent), user(user), subjectName(subjectName), attended(0), total(0) 
{
    initializeUI();
}

SubjectBox::SubjectBox(QWidget* parent) 
    : QGroupBox(parent), attended(0), total(0) 
{
    initializeUI();
}

void SubjectBox::initializeUI() {
    setTitle(subjectName);
    setFixedHeight(80);
    setMinimumWidth(271);

    // Initialize widgets
    presentButton = new QPushButton("P", this);
    presentButton->setGeometry(30, 20, 21, 21);
    presentButton->setStyleSheet("QPushButton{font-size:20px;color:black;background:rgb(77, 255, 23);}");

    absentButton = new QPushButton("A", this);
    absentButton->setGeometry(90, 20, 21, 21);
    absentButton->setStyleSheet("QPushButton{font-size:20px;color:black;background:rgb(245, 10, 10);}");

    attendedSpinBox = new QSpinBox(this);
    attendedSpinBox->setGeometry(130, 20, 31, 22);

    totalSpinBox = new QSpinBox(this);
    totalSpinBox->setGeometry(130, 50, 31, 22);

    statusLabel = new QLabel(this);
    statusLabel->setGeometry(10, 60, 131, 16);

    percentageLabel = new QLabel(this);
    percentageLabel->setGeometry(230, 40, 21, 21);

    // Connect signals
    connect(attendedSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));
    connect(totalSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onTotalSpinBoxValueChanged(int)));
    connect(presentButton, SIGNAL(clicked()), this, SLOT(onPresentClicked()));
    connect(absentButton, SIGNAL(clicked()), this, SLOT(onAbsentClicked()));
}

void SubjectBox::setDetails(const QString& user, const QString& subjectName, int attended, int total) {
    this->user = user;
    this->subjectName = subjectName;
    this->attended = attended;
    this->total = total;

    // Update UI
    setTitle(subjectName);
    attendedSpinBox->setValue(attended);
    totalSpinBox->setValue(total);
    updateAttendanceText();
}

void SubjectBox::loadSubjectDetailsFromDatabase() {
    const QString dbFilePath = QCoreApplication::applicationDirPath() + "/database/users.db";
    QSqlDatabase db = QSqlDatabase::database();
    db.setDatabaseName(dbFilePath);

    if (!db.open()) {
        QMessageBox::warning(this, "Error", "Failed to open database: " + db.lastError().text());
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT attended, total FROM subjects WHERE name = ? AND userid = ?");
    query.addBindValue(this->subjectName);
    query.addBindValue(this->user);

    if (query.exec() && query.next()) {
        attended = query.value(0).toInt();
        total = query.value(1).toInt();

        attendedSpinBox->setValue(attended);
        totalSpinBox->setValue(total);

        updateAttendanceText();
    }

    db.close();
}


SubjectBox::~SubjectBox() {}

void SubjectBox::onSpinBoxValueChanged(int value)
{
    if (value > total) {
        QMessageBox::warning(this, "Invalid input", "Attended classes should be less than or equal to total classes");
        attendedSpinBox->setValue(attended);
        return;
    }
    attended = value;
    updateAttendanceText();
}

void SubjectBox::onTotalSpinBoxValueChanged(int value)
{
    if (value < attended) {
        QMessageBox::warning(this, "Invalid input", "Total classes should be greater than or equal to attended classes");
        totalSpinBox->setValue(total);
        return;
    }
    total = value;
    updateAttendanceText();
    updateSubjectInDatabase();
}

void SubjectBox::onPresentClicked()
{
    Ainc(),Tinc();
    attendedSpinBox->setValue(attended);
    totalSpinBox->setValue(total);
    updateAttendanceText();
    updateSubjectInDatabase();
}

void SubjectBox::onAbsentClicked()
{
    Tinc();
    totalSpinBox->setValue(total);
    updateAttendanceText();
}

void SubjectBox::updateAttendanceText()
{
    if (total == 0) {
        percentageLabel->setText("0%");
        statusLabel->setText("You can miss 0 lectures");
        return;
    }

    double attendance = ((double)attended / total) * 100;
    percentageLabel->setText(QString::number(attendance) + "%");

    if (attendance < 75) {
        int x = ceil((0.75 * total - attended) / 0.25);
        statusLabel->setText("You need to attend " + QString::number(x) + " lectures");
    } else {
        int x = floor((attended - 0.75 * total) / 0.75);
        statusLabel->setText("You can miss " + QString::number(x) + " lectures");
    }
}

void SubjectBox::updateSubjectInDatabase() {
    QSqlDatabase db = QSqlDatabase::database();  // Use existing connection

    if (!db.isOpen()) {
        QMessageBox::warning(this, "Error", "Database connection is not open.");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE subjects SET attended = ?, total = ? WHERE name = ? AND userid = ?");
    query.addBindValue(attended);
    query.addBindValue(total);
    query.addBindValue(subjectName);
    query.addBindValue(user);

    if (!query.exec()) {
        QMessageBox::warning(this, "Error", "Failed to update subject: " + query.lastError().text());
    }
}
