#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_buttonBox_accepted()
{
    QStringList sl;
    QList<QLineEdit*> list=this->findChildren<QLineEdit*>();
    if(list[0]->text().isEmpty()||list[1]->text().isEmpty()){
        QMessageBox::information(this,tr("错误"),tr("输入框不能为空"));
        return;
    }
    for(int i=0;i<2;i++){
        sl<<list[i]->text();
    }
    emit get_texts(sl);
    delete this;
}

void Dialog::on_buttonBox_rejected()
{
    delete this;
}

