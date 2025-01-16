#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private slots:
    void onLogin();

private:
    Ui::login *ui;
    QSqlDatabase db;
    void initializeDatabase();
};

#endif // LOGIN_H
