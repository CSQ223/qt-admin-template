#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    nav_ptr = new CustomNavListView(this);

//    this->setCentralWidget(nav_ptr);
//    connect(ui->listView, SIGNAL(pressed(const QModelIndex &index)), ui->listView, SLOT(pressed(const QModelIndex &index)));
//    ui->listView->
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listView_pressed(const QModelIndex &index)
{
    qDebug() << "on_listView_pressed";
}

void MainWindow::on_listView_2_pressed(const QModelIndex &index)
{
    qDebug() << "on_listView_2_pressed";
}

void MainWindow::on_pushButton_clicked()
{
    QString items = "test";
    ui->listView->setItems(items);
}
