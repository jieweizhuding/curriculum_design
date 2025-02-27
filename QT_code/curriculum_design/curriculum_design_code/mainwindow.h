#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//QT库
#include <QMainWindow>
#include <QTextEdit>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QGraphicsView>


#include "spdlog/spdlog.h"
#include "spdlog/sinks/qt_sinks.h"

typedef std::string Vertexinfo;


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    std::shared_ptr<spdlog::logger> infoLogger;
    std::shared_ptr<spdlog::logger> debugLogger;
    std::shared_ptr<spdlog::logger> errorLogger;
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    //h判断有环或无环，0-未知，1-有环，2-无环
    int h=0;
    bool issaved;
    std::vector<std::string> v_s;
    QGraphicsView view;
    QGraphicsScene scene;
    bool maybeSave();
    void closeSave();
    QTextEdit * textEdit=new QTextEdit();
    QString* fileName=new QString();

private slots:
    void show_save(bool ismodified);
    void fun(QStringList sl);
    void fun_arc(QStringList sl);
    void fun_del(QString s);
    void fun_del_arc(QStringList sl);
    void on_add_v_button_clicked();
    void on_add_arc_button_clicked();
    void on_new_button_clicked();
    void on_open_button_clicked();
    void on_save_button_clicked();
    void on_DAG_button_clicked();
    void on_delete_button_clicked();
    void on_delete_arc_button_clicked();
    void on_Tsort_button_clicked();
};
#endif // MAINWINDOW_H
