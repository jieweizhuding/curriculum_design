#ifndef DIALOG_TSORT_H
#define DIALOG_TSORT_H

#include <QDialog>

namespace Ui {
class Dialog_Tsort;
}

class Dialog_Tsort : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Tsort(QWidget *parent = nullptr);
    ~Dialog_Tsort();

private:
    Ui::Dialog_Tsort *ui;
};

#endif // DIALOG_TSORT_H
