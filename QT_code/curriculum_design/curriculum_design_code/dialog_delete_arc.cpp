#include "dialog_delete_arc.h"
#include "ui_dialog_delete_arc.h"

Dialog_delete_arc::Dialog_delete_arc(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_delete_arc)
{
    ui->setupUi(this);
}

Dialog_delete_arc::~Dialog_delete_arc()
{
    delete ui;
}

void Dialog_delete_arc::on_buttonBox_accepted()
{
    QStringList sl;
    QList<QComboBox*> list= this->findChildren<QComboBox*>();
    for(int i=0;i<2;i++){
        sl<<list[i]->currentText();
    }
    if(!sl[0].isEmpty()&&!sl[1].isEmpty())
    emit(get_texts_arc(sl));
    delete this;
}

