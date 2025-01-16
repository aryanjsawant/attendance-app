#ifndef UPDATEBOX_H
#define UPDATEBOX_H

#include <QDialog>

namespace Ui {
class updateBox;
}

class updateBox : public QDialog
{
    Q_OBJECT

signals:
    void newSub(const QString &rn);

public:
    explicit updateBox(QWidget *parent = nullptr);
    ~updateBox();

private slots:
    void onDelete();

private:
    Ui::updateBox *ui;
};

#endif // UPDATEBOX_H
