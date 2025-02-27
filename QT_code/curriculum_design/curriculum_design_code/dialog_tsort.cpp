#include "dialog_tsort.h"
#include "ui_dialog_tsort.h"

Dialog_Tsort::Dialog_Tsort(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_Tsort)
{
    ui->setupUi(this);
}

Dialog_Tsort::~Dialog_Tsort()
{
    delete ui;
}
