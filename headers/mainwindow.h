#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFileInfo>
#include <QCoreApplication>
#include "addbox.h"
#include "findbox.h"
#include "updatebox.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    const QString dbFilePath = QCoreApplication::applicationDirPath() + "/database/users.db";
    ~MainWindow();

private slots:
    void addFun();
    void getInfo(const QString &rn, const QString &n);
    void searchFunction();
    void onFind(const QString &rn);
    void deleteFunction();
    void onDelete(const QString &rn);

private:
    Ui::MainWindow *ui;
    addBox *addbox;
    findBox *findbox;
    updateBox *updatebox;
    void initializeDatabase();
};

#endif // MAINWINDOW_H
