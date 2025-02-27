//图形界面头文件
#include "mainwindow.h"
#include "dialog.h"
#include "./ui_mainwindow.h"
#include "dialog_delete_arc.h"
#include "dialog_tsort.h"
#include "dialog_arc.h"
#include "dialog_delete.h"

//自定义头文件
#include "my_graph.h"
#include "tsort.h"

//标准库
#include <math.h>
#include <algorithm>

//三方库
#include <ogdf/basic/graphics.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/LongestPathRanking.h>


//QT库
#include <qcombobox.h>
#include <qlistwidget.h>
#include <QTimer>


extern AOV::graph<Vertexinfo> graph;



MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textBrowser->setText("已保存");
    connect(textEdit->document(),&QTextDocument::modificationChanged,this,&MainWindow::show_save);

    infoLogger=spdlog::qt_color_logger_st("info_logger", ui->info_log_window,100,true);
    debugLogger=spdlog::qt_color_logger_st("debug_logger", ui->debug_log_window,500,true);
    errorLogger=spdlog::qt_color_logger_st("error_logger",ui->error_log_window,500,true);
    // 设置日志级别
    infoLogger->set_level(spdlog::level::info);
    debugLogger->set_level(spdlog::level::debug);
    errorLogger->set_level(spdlog::level::err);
}

MainWindow::~MainWindow()
{
    delete ui;
}




//保存显示函数(OK)
void MainWindow::show_save(bool ismodified)
{
    if(ismodified){
        //由0-1变化
        this->h=0;
    }
    ui->textBrowser->setText(ismodified?"未保存":"已保存");
}



//保存操作(OK)
bool MainWindow::maybeSave()
{
    infoLogger->info("正在执行保存操作");
    if (textEdit->document()->isModified()) {
        QMessageBox::StandardButton ret = QMessageBox::warning(
            this, tr("警告"),
            tr("文档已被修改。是否保存更改？"),
            QMessageBox::Save | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            // 如果打开文件，则修改对应文件内容，未打开文件则创建对应文件
            if(fileName->isEmpty()){
                QString _fileName = QFileDialog::getSaveFileName(this,
                                                                tr("新建文件"),
                                                                QString(),
                                                                tr("AOV文件 (*.aov)"));
                // 检查用户是否选择了文件名
                if (!_fileName.isEmpty()) {
                    // 检查并自动补全后缀（例如 .aov）
                    if (!_fileName.endsWith(".aov")) {
                        _fileName += ".aov";  // 默认后缀
                    }
                    // 创建文件
                    QFile file(_fileName);
                    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QTextStream out(&file);
                        QString content=textEdit->toPlainText();
                        out<<content;
                        file.close();
                        QMessageBox::information(this, tr("成功"), tr("文件已创建: %1").arg(_fileName));
                        std::string s="创建了文件";
                        s.append(_fileName.toStdString());
                        infoLogger->info(s);
                    } else {
                        QMessageBox::critical(this, tr("错误"), tr("无法创建文件！"));
                        errorLogger->error("创建文件失败");
                    }
                    *fileName=_fileName;
                }
            }
            infoLogger->info("成功保存");
            textEdit->document()->setModified(false);
            return true; // 假设保存成功
        } else if (ret == QMessageBox::Cancel) {
            infoLogger->info("取消保存");
            return false;
        }
    }
    infoLogger->info("保存操作结束");
    return true;
}

//关闭保存函数(OK)
void MainWindow::closeSave()
{
    infoLogger->info("正在执行关闭保存函数") ;
    maybeSave();
    if(!fileName->isEmpty()){
        if(textEdit->document()->isEmpty()){
            //删除打开的文件
            if (QFile::exists(*fileName)) { // 检查文件是否存在
                if (QFile::remove(*fileName)) { // 删除文件
                    infoLogger->info("正在删除空文件");
                    QMessageBox::information(nullptr, tr("成功"), tr("文件已删除: %1").arg(*fileName));
                } else {
                    QMessageBox::critical(nullptr, tr("错误"), tr("无法删除文件: %1").arg(*fileName));
                    errorLogger->error("无法删除文件");
                }
            } else {
                QMessageBox::warning(nullptr, tr("警告"), tr("文件不存在: %1").arg(*fileName));
                errorLogger->error("无法删除文件");
            }
        }else{
            //写入文件
            QFile file(*fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                QString content = textEdit->toPlainText();
                out << content;
                file.close();
                infoLogger->info("更改了文件内容");
            } else {
                QMessageBox::critical(this, tr("错误"), tr("无法创建文件！"));
                errorLogger->error("无法删除文件");
            }
        }
    }
    fileName->clear();
    delete(textEdit);
    textEdit=new QTextEdit();
    infoLogger->info("关闭保存函数结束");
}

//关闭操作(OK)
void MainWindow::closeEvent(QCloseEvent *event)
{
    closeSave();
}

//新建文件操作(OK)
void MainWindow::on_new_button_clicked()
{
    closeSave();
    infoLogger->info("正在新建文件");
    graph.get_m().clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);
    infoLogger->info("新建文件结束");
}

//打开文件操作(OK)
void MainWindow::on_open_button_clicked()
{

    closeSave();
    infoLogger->info("正在打开文件");
    // 打开文件对话框，并限制只能选择特定后缀（如 .txt 文件）
    *fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), QString(),
                                             tr("文本文件 (*.aov)"));

    // 检查是否选择了文件
    if (!fileName->isEmpty()) {
        // 获取文件后缀
        QFileInfo fileInfo(*fileName);
        QString suffix = fileInfo.suffix();

        // 判断文件后缀是否合法（例如必须是 .aov）
        if (suffix != "aov") {
            errorLogger->error("文件后缀错误");
            QMessageBox::warning(this, tr("错误"), tr("只能打开 .aov 文件！"));
            return;
        }

        // 打开文件并读取内容
        QFile file(*fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            textEdit->setText(file.readAll());
            textEdit->document()->setModified(false);
            //需要解析textEdit，生成graph并更新list表
            QString plainText = textEdit->toPlainText();
            QStringList lines = plainText.split("\n");
            QStringList content;
            size_t size=lines.size()-1;
            int i=0,j=0;
            for(;i<size;i++){
                if(lines[i]=="#"){
                    break;
                }
                content=lines[i].split(" ");
                graph.insert_node(content[0].toStdString(),content[1].toStdString());
                j=ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(j);
                ui->tableWidget->setItem(j,0,new QTableWidgetItem(content[0]));
                ui->tableWidget->setItem(j,1,new QTableWidgetItem(content[1]));
            }
            i++;
            for(;i<size;i++){
                content=lines[i].split(" ");
                graph.insert_arc(content[0].toStdString(),content[1].toStdString());
                j=ui->tableWidget_2->rowCount();
                ui->tableWidget_2->insertRow(j);
                ui->tableWidget_2->setItem(j,0,new QTableWidgetItem(content[0]));
                ui->tableWidget_2->setItem(j,1,new QTableWidgetItem(content[1]));
            }
            infoLogger->info("打开文件成功");
        } else {
            errorLogger->error("文件打开失败");
            QMessageBox::critical(this, tr("错误"), tr("文件无法打开！"));
        }
    }
    infoLogger->info("打开文件操作结束");
}

//保存按钮(OK)
void MainWindow::on_save_button_clicked()
{
    maybeSave();
}

//拓扑排序(OK)
void MainWindow::on_Tsort_button_clicked()
{
    infoLogger->info("开始拓扑排序");
    AOV::Tsort<Vertexinfo> t(graph);
    v_s.clear();
    v_s=t.topologicalSortKahn(t.graph,t.h);
    // v_s=t.topologicalSortDFS(t.graph,t.h);
    Dialog_Tsort * ptr=new Dialog_Tsort();
    QListWidget* p=ptr->findChild<QListWidget*>();
    if(t.h){
        this->h=1;
        debugLogger->debug("图中有环存在");
        QMessageBox::information(this,tr("错误"),tr("图中有环存在，请结合排序内容与图像修改图"));
        ptr->setWindowTitle("去除环后的拓扑排序");
    }else{
        this->h=2;
        ptr->setWindowTitle("拓扑排序显示");
    }
    for(std::string s:v_s){
        p->addItem(QString::fromStdString(s));
    }
    ptr->show();
}




//顶点添加函数(OK)
void MainWindow::fun(QStringList sl){
    infoLogger->info("开始添加顶点");
    if(!graph.insert_node(sl[0].toStdString(),sl[1].toStdString())){
        //uid重复,提醒重新输入
        debugLogger->debug("id重复");
        QMessageBox::information(nullptr,tr("错误"),tr("id重复"));
        return;
    }
    QString s;
    s.append(sl[0]);
    s.append(" ");
    s.append(sl[1]);
    s.append("\n");
    if(textEdit->document()->isEmpty()){
        s.append("#");
        s.append("\n");
        textEdit->insertPlainText(s);
    }else{
        textEdit->moveCursor(QTextCursor::Start);
        textEdit->insertPlainText(s);
    }
    int i=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(i);
    ui->tableWidget->setItem(i,0,new QTableWidgetItem(sl[0]));
    ui->tableWidget->setItem(i,1,new QTableWidgetItem(sl[1]));
    infoLogger->info("成功添加顶点");
}

//边添加函数(OK)
void MainWindow::fun_arc(QStringList sl)
{
    infoLogger->info("开始添加边");
    graph.insert_arc(sl[0].toStdString(),sl[1].toStdString());
    QString s;
    s.append(sl[0]);
    s.append(" ");
    s.append(sl[1]);
    s.append("\n");
    textEdit->moveCursor(QTextCursor::End);
    textEdit->insertPlainText(s);
    int i=ui->tableWidget_2->rowCount();
    ui->tableWidget_2->insertRow(i);
    ui->tableWidget_2->setItem(i,0,new QTableWidgetItem(sl[0]));
    ui->tableWidget_2->setItem(i,1,new QTableWidgetItem(sl[1]));
    infoLogger->info("添加边结束");
}

//顶点删除函数(OK)
void MainWindow::fun_del(QString s){
    infoLogger->info("开始删除顶点");
    graph.delete_node(s.toStdString());
    QTextCursor cursor = textEdit->textCursor();
    cursor.movePosition(QTextCursor::Start); // 将光标移至文档开头
    while(textEdit->find(s,QTextDocument::FindCaseSensitively)){
        // 获取当前匹配的光标
        QTextCursor matchCursor = textEdit->textCursor();
        // 将光标扩展到整行
        matchCursor.select(QTextCursor::LineUnderCursor);
        // 删除选中行
        matchCursor.removeSelectedText();
        // 删除行后的换行符
        matchCursor.deleteChar();
    }
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
            QTableWidgetItem* item = ui->tableWidget->item(row, 0);
            if (item && item->text() == s) {
                ui->tableWidget->removeRow(row); // 删除找到的行
                break; // 如果只需删除第一匹配项，可以直接返回
            }
    }
    for (int row = 0; row < ui->tableWidget_2->rowCount(); ++row) {
        for(int column=0;column<2;column++){
            QTableWidgetItem* item = ui->tableWidget_2->item(row, column);
            if (item && item->text() == s) {
                ui->tableWidget_2->removeRow(row); // 删除找到的行
            }
        }
    }
    infoLogger->info("删除顶点结束");
}

//边删除函数(OK)
void MainWindow::fun_del_arc(QStringList sl)
{
    infoLogger->info("开始删除边");
    if(!graph.delete_arc(sl[0].toStdString(),sl[1].toStdString())){
        debugLogger->debug("所选弧不存在");
        QMessageBox::information(this,tr("错误"),tr("该弧不存在"));
        return;
    }
    QTextCursor cursor = textEdit->textCursor();
    cursor.movePosition(QTextCursor::Start); // 将光标移至文档开头
    QString s=sl[0];
    s.append(" ");
    s.append(sl[1]);
    while(textEdit->find(s,QTextDocument::FindCaseSensitively)){
        // 获取当前匹配的光标
        QTextCursor matchCursor = textEdit->textCursor();
        // 将光标扩展到整行
        matchCursor.select(QTextCursor::LineUnderCursor);
        // 删除选中行
        matchCursor.removeSelectedText();
        // 删除行后的换行符
        matchCursor.deleteChar();
    }
    for (int row = 0; row < ui->tableWidget_2->rowCount(); ++row) {
            QTableWidgetItem* item = ui->tableWidget_2->item(row, 0);
            if (item && item->text() == sl[0]) {
                QTableWidgetItem* item = ui->tableWidget_2->item(row, 1);
                {
                    if(item&& item->text()==sl[1]){
                        ui->tableWidget_2->removeRow(row); // 删除找到的行
                        return;
                    }
                }
            }
    }
    infoLogger->info("边删除结束");
}



//顶点添加操作(OK)
void MainWindow::on_add_v_button_clicked()
{
    Dialog * diaptr=new Dialog();
    diaptr->setWindowTitle("顶点添加");
    diaptr->show();
    connect(diaptr,&Dialog::get_texts,this,&MainWindow::fun);
}

//边添加操作(OK)
void MainWindow::on_add_arc_button_clicked()
{
    Dialog_arc * diaptr_arc=new Dialog_arc();
    diaptr_arc->setWindowTitle("边添加");
    QList<QComboBox*>list= diaptr_arc->findChildren<QComboBox*>();
    for(auto &[uuid,node]:graph.get_m()){
        list[0]->addItem(QString::fromStdString(uuid));
        list[1]->addItem(QString::fromStdString(uuid));
    }
    diaptr_arc->show();
    connect(diaptr_arc,&Dialog_arc::get_arc_texts,this,&MainWindow::fun_arc);
}

//顶点删除操作(OK)
void MainWindow::on_delete_button_clicked()
{
    Dialog_delete *diaptr_del=new Dialog_delete();
    diaptr_del->setWindowTitle("顶点删除");
    QComboBox* p=diaptr_del->findChild<QComboBox*>();
    for(auto &[uuid,node]:graph.get_m()){
        p->addItem(QString::fromStdString(uuid));
    }
    diaptr_del->show();
    connect(diaptr_del,&Dialog_delete::get_texts,this,&MainWindow::fun_del);
}

//边删除操作(OK)
void MainWindow::on_delete_arc_button_clicked()
{
    Dialog_delete_arc* diaptr_del_arc=new Dialog_delete_arc();
    diaptr_del_arc->setWindowTitle("边删除");
    QList<QComboBox*> list=diaptr_del_arc->findChildren<QComboBox*>();
    for(auto &[uuid,node]:graph.get_m()){
        list[0]->addItem(QString::fromStdString(uuid));
        list[1]->addItem(QString::fromStdString(uuid));
    }
    diaptr_del_arc->show();
    connect(diaptr_del_arc,&Dialog_delete_arc::get_texts_arc,this,&MainWindow::fun_del_arc);
}






//图像绘制函数(OK)
void MainWindow::on_DAG_button_clicked(){
    AOV::graph<Vertexinfo> t_g;
    t_g.get_m()=graph.get_m();
    scene.clear();
    infoLogger->info("开始绘制图像");
    if(h==0){
        debugLogger->debug("未优先判断是否有环");
        QMessageBox::information(this,tr("警告"),tr("需要先判断图像是否有环"));
        return;
    }
    if(h==1){
        debugLogger->debug("图中有环时调用图像绘制");
        QMessageBox::information(this,tr("警告"),tr("图中有环存在,请结合图像手动删除环路"));
    }
    ogdf::Graph g;
    ogdf::GraphAttributes ga(t_g.switch_graph(g));

    // 层次化布局：Sugiyama 布局
    ogdf::SugiyamaLayout SL;
    SL.setRanking(new ogdf::LongestPathRanking);
    SL.setCrossMin(new ogdf::MedianHeuristic);
    SL.setLayout(new ogdf::OptimalHierarchyLayout);
    // 应用布局
    SL.call(ga);

    for (ogdf::node v : g.nodes) {
        double x = ga.x(v);
        double y = ga.y(v);

        // 获取节点编号
        QString label = QString::fromStdString(ga.label(v));

        if(std::find(v_s.begin(),v_s.end(),ga.label(v))!=v_s.end())
        // 在 Qt 中绘制节点
        QGraphicsEllipseItem *nodeItem = scene.addEllipse(x - 30, y - 30, 60, 60, QPen(Qt::black), QBrush(Qt::white));
        else
        QGraphicsEllipseItem *nodeItem = scene.addEllipse(x - 30, y - 30, 60, 60, QPen(Qt::black), QBrush(Qt::yellow));

        // 绘制节点编号
        QGraphicsTextItem *textItem = scene.addText(label);
        textItem->setDefaultTextColor(Qt::black);
        textItem->setFont(QFont("Arial", 10, QFont::Bold));
        textItem->setPos(x - 15, y - 15); // 调整文本位置，使其位于节点内部或旁边
    }
    for (ogdf::edge e : g.edges) {
        ogdf::node source = e->source();
        ogdf::node target = e->target();

        // 获取边的起点和终点坐标
        double x1 = ga.x(source);
        double y1 = ga.y(source);
        double x2 = ga.x(target);
        double y2 = ga.y(target);

        double dx=(x2-x1);
        double dy=y2-y1;
        double dz=std::sqrt(dx*dx+dy*dy);
        double ax=30/dz*dx;
        double ay=30/dz*dy;

        x1=x1+ax;
        y1=y1+ay;
        x2=x2-ax;
        y2=y2-ay;
        // 在 Qt 中绘制边
        QPainterPath path;
        path.moveTo(x1, y1);
        path.lineTo(x2, y2);

        // 创建箭头
        double arrowSize = 10;
        double angle = std::atan2(y2 - y1, x2 - x1);
        double arrowX1 = x2 - arrowSize * std::cos(angle + M_PI / 6);
        double arrowY1 = y2 - arrowSize * std::sin(angle + M_PI / 6);
        double arrowX2 = x2 - arrowSize * std::cos(angle - M_PI / 6);
        double arrowY2 = y2 - arrowSize * std::sin(angle - M_PI / 6);

        // 计算箭头点
        path.moveTo(x2, y2);            // 返回箭头底部
        path.lineTo(arrowX1, arrowY1);  // 第一个箭头点
        path.moveTo(x2, y2);            // 返回箭头底部
        path.lineTo(arrowX2, arrowY2);  // 第二个箭头点
        path.closeSubpath();            // 关闭路径

        // 绘制带箭头的边
        scene.addPath(path, QPen(Qt::black), QBrush(Qt::black));


        // scene.addLine(x1+ax, y1+ay, x2-ax, y2-ay, QPen(Qt::black));
    }
    view.setScene(&scene);
    view.setWindowTitle("DAG绘制");
    view.show();
    infoLogger->info("绘图结束");
}
