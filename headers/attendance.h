#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <QDialog>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QList>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QInputDialog>
#include <QSqlQuery>
#include <QSqlError>
#include "subject.h"

namespace Ui {
class attendance;
}

class SubjectBox : public QGroupBox
{
    Q_OBJECT

public:
    // Constructor to initialize with a subject number (for new subjects)
    explicit SubjectBox(const QString &user, const QString &subjectName, QWidget* parent = nullptr);
    explicit SubjectBox(QWidget* parent = nullptr);
    void setDetails(const QString &userid, const QString &name, int a, int t);
    void Ainc(){attended++;}
    void Tinc(){total++;}
    QString user;
    QString subjectName;
    void loadSubjectDetailsFromDatabase();
    void initializeUI();
    friend class attendance;
    
    ~SubjectBox();

private slots:
    void onSpinBoxValueChanged(int value);
    void onTotalSpinBoxValueChanged(int value);
    void onPresentClicked();
    void onAbsentClicked();
    void updateAttendanceText();
    void updateSubjectInDatabase();

private:
    int attended, total;
    QPushButton *presentButton, *absentButton;
    QSpinBox *attendedSpinBox, *totalSpinBox;
    QLabel *statusLabel, *percentageLabel;

    void updateSubjectInfo();  // Update the subject's attendance data
};

class attendance : public QDialog {
    Q_OBJECT

public:
    explicit attendance(const QString &userid, QWidget *parent = nullptr);
    void newSub();
    QList<subject*> loadSubjectsFromDatabase();
    void openSubjectDialog();
    QString user;
    ~attendance();

private slots:
    void newSubject();

private:
    Ui::attendance *ui;
    subject *deleteSubjects;
    QVBoxLayout *scrollLayout;
    QList<SubjectBox*> subjectBoxes;
    QList<subject*> subjects;
};

#endif // ATTENDANCE_H
