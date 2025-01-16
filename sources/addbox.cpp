    #include "addbox.h"
#include "ui_addbox.h"
#include <QMessageBox>

addBox::addBox(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addBox)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &addBox::getVal);
}

addBox::~addBox()
{
    delete ui;
}

void addBox::getVal()
{
    QString userid=ui->lineEdit->text();
    QString password=ui->lineEdit_2->text();
    QString confirmation=ui->lineEdit_3->text();

    if(password != confirmation) {
        QMessageBox::critical(this, "Password Error", "Passkeys do not match" );
        return;
    }

    emit enterInfo(userid, password);
}

