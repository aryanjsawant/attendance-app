#ifndef SUBJECT_H
#define SUBJECT_H

#include <QDialog>
#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class subject;
}

class subject : public QDialog
{
    Q_OBJECT

public:
    explicit subject(QWidget *parent = nullptr, const QString &userid = "");
    ~subject();

private slots:
    void onOkClicked();
    void onCancelClicked();

private:
    QString userid;
    Ui::subject *ui;
    QVBoxLayout *subjectLayout;
    QList<QCheckBox*> subjectCheckBoxes;
    void loadSubjectsFromDatabase();
    void deleteSelectedSubjects();
};

#endif // SUBJECT_H
