#include "dialog_delete.h"
#include "ui_dialog_delete.h"

Dialog_delete::Dialog_delete(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_delete)
{
    ui->setupUi(this);
}

Dialog_delete::~Dialog_delete()
{
    delete ui;
}



void Dialog_delete::on_buttonBox_accepted()
{
    QString s;
    QComboBox* p= this->findChild<QComboBox*>();
    s=p->currentText();
    if(!s.isEmpty())
    emit(get_texts(s));
    delete this;
}

