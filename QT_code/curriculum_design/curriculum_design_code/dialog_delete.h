#ifndef DIALOG_DELETE_H
#define DIALOG_DELETE_H

#include <QDialog>

namespace Ui {
class Dialog_delete;
}

class Dialog_delete : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_delete(QWidget *parent = nullptr);
    ~Dialog_delete();

private:
    Ui::Dialog_delete *ui;

signals:
    void get_texts(QString s);
private slots:
    void on_buttonBox_accepted();
};

#endif // DIALOG_DELETE_H
