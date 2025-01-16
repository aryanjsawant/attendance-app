#include "updatebox.h"
#include "ui_updatebox.h"

updateBox::updateBox(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::updateBox)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked,this,&updateBox::onDelete);
}

updateBox::~updateBox()
{
    delete ui;
}

void updateBox::onDelete()
{
    QString userid = ui->lineEdit->text();
    emit newSub(userid);
}
