#ifndef DIALOG_ARC_H
#define DIALOG_ARC_H

#include <QDialog>

namespace Ui {
class Dialog_arc;
}

class Dialog_arc : public QDialog
{
    Q_OBJECT

signals:
    void get_arc_texts(QStringList sl);


public:
    explicit Dialog_arc(QWidget *parent = nullptr);
    ~Dialog_arc();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Dialog_arc *ui;
};

#endif // DIALOG_ARC_H
