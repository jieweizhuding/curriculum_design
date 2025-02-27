#ifndef DIALOG_DELETE_ARC_H
#define DIALOG_DELETE_ARC_H

#include <QDialog>

namespace Ui {
class Dialog_delete_arc;
}

class Dialog_delete_arc : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_delete_arc(QWidget *parent = nullptr);
    ~Dialog_delete_arc();

private:
    Ui::Dialog_delete_arc *ui;

signals:
    void get_texts_arc(QStringList sl);


public slots:
    void on_buttonBox_accepted();
};

#endif // DIALOG_DELETE_ARC_H
