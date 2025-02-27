#include "dialog_arc.h"
#include "ui_dialog_arc.h"

Dialog_arc::Dialog_arc(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_arc)
{
    ui->setupUi(this);
}

Dialog_arc::~Dialog_arc()
{
    delete ui;
}

void Dialog_arc::on_buttonBox_accepted()
{
    QStringList sl;
    QList<QComboBox*> list=this->findChildren<QComboBox*>();
    for(int i=0;i<2;i++){
        sl<<list[i]->currentText();
    }
    emit get_arc_texts(sl);
    delete this;
}


void Dialog_arc::on_buttonBox_rejected()
{
    delete this;
}

