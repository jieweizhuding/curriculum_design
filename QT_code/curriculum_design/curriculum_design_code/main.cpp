#include "mainwindow.h"
#include "my_graph.h"
#include <QApplication>
#include "spdlog/spdlog.h"

AOV::graph<Vertexinfo> graph;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("DAG绘制");
    w.show();
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v" );
    return a.exec();
}
