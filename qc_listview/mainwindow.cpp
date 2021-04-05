#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->listView->pressed(int childindex);
    //connect(ui->listView, SIGNAL(pressed(const QModelIndex &index)), ui->listView, SLOT(pressed(const QModelIndex &data)));

    ui->listView->setItems("1,0,0,,主界面,,NULL");
    ui->listView->setItems("2,0,0,0xf03e,地图监控,,主界面");
    ui->listView->setItems("2,0,0,0xf03d,视频监控,,主界面");
    ui->listView->setItems("2,0,1,0xf108,设备监控,,主界面");

    ui->listView->setItems("1,0,0,,系统设置,,NULL");
    ui->listView->setItems("2,0,0,0xf2bb,防区信息,,系统设置");
    ui->listView->setItems("2,0,0,0xf0f9,位置调整,,系统设置");
    ui->listView->setItems("2,0,1,0xf242,地图编辑,,系统设置");

    ui->listView->setItems("1,0,0,,警情查询,,NULL");
    ui->listView->setItems("2,0,0,0xf271,记录查询,,警情查询");
    ui->listView->setItems("2,0,0,0xf140,图像查询,,警情查询");
    ui->listView->setItems("2,0,0,0xf0b1,视频查询,,警情查询");
    ui->listView->setItems("2,0,1,0xf207,数据回放,,警情查询");

    ui->listView->setItems("1,0,1,,帮助文档,,NULL");
    ui->listView->setItems("2,0,0,0xf0c2,系统帮助,,帮助文档");
    ui->listView->setItems("2,0,0,0xf075,关于系统,,帮助文档");
    ui->listView->setItems("2,0,1,0xf015,关于系统,,帮助文档");
}

MainWindow::~MainWindow()
{
    delete ui;
}
