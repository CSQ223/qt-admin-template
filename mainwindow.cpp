#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QDomDocument>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qc_navbutton.h"
#include "qc_listview.h"
#include "iconhelper.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowTitleHint);

    initStyle();

    initData();

    initForm();

    clouddata_ptr = CloudData::Instance();

    frm_aptest_ptr = new frmAptest(this);
    widgets.append(frm_aptest_ptr);
    connect(ui->btn_gather, SIGNAL(clicked()), frm_aptest_ptr, SLOT(slot_btn_gather_down()));
    connect(ui->btn_view, SIGNAL(clicked()), frm_aptest_ptr, SLOT(slot_btn_view_down()));

    frm_gather_ptr = nullptr;

    //foreach (QWidget *w, widgets) {
    ui->stackedWidget->addWidget(frm_aptest_ptr);
    //}

    ui->stackedWidget->setCurrentIndex(0);
    ui->btn_view->setChecked(true);
}

MainWindow::~MainWindow()
{
    if (NULL != frm_aptest_ptr)
    {
        delete frm_aptest_ptr;
    }

    if (NULL != frm_gather_ptr)
    {
        delete frm_gather_ptr;
    }

    widgets.clear();

    delete ui;
}

void MainWindow::loadConfig(QString fileName, QMap<QString, QString> &configs)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        return;
    }

    //将文件填充到dom容器
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return;
    }

    file.close();

    QDomElement root = doc.documentElement();
    if (root.tagName() == "property")
    {
        QDomNode node = root.firstChild();
        //QDomElement element = node.toElement();

        while (!node.isNull())
        {
            if (node.isElement())
            {
                QDomElement e = node.toElement();

                configs[e.nodeName()] = e.text();
            }

            node = node.nextSibling();
            //element = node.toElement();
        }
    }
}

void MainWindow::initData()
{
    //AutoVehicleData.ini
    reprocess_mode = false;

    loadConfig("qt-admin-template.ini", configs);

    for (auto iter = configs.begin(); iter != configs.end(); iter++)
    {
        qDebug() << iter.key() << ":" << iter.value();

        if (iter.key() == "AccessKeyId")
        {
            AccessKeyId = iter.value().toStdString();
        }

        if (iter.key() == "AccessKeySecret")
        {
            AccessKeySecret = iter.value().toStdString();
        }

        if (iter.key() == "Endpoint")
        {
            Endpoint = iter.value().toStdString();
        }

        if (iter.key() == "BucketName")
        {
            BucketName = iter.value().toStdString();
        }

        if (iter.key() == "title")
        {
            title = iter.value();
        }

        if (iter.key() == "copyright")
        {
            copyright = iter.value();
        }
    }

    CloudData::AccessKeyId = AccessKeyId;
    CloudData::AccessKeySecret = AccessKeySecret;
    CloudData::Endpoint = Endpoint;
    CloudData::BucketName = BucketName;
}

void MainWindow::initForm()
{
    //int wid = width();

    setWindowTitle(title);

    ui->btn_gather->setCheckable(true);
    ui->btn_view->setCheckable(true);
    ui->label_title->setText(title);
    ui->label_version_coyright->setText(title + "| "+ copyright +" | 版本号:");
    ui->label_version->setText(configs["version"]);

    ui->listView->setItems("1,0,0,,主界面,,NULL");
    ui->listView->setItems("2,0,0,0xf03e,数据基础操作,,主界面");
    ui->listView->setItems("2,0,0,0xf03d,数据汇总操作,,主界面");
    ui->listView->setItems("2,0,1,0xf108,设备监控,,主界面");

    ui->listView->setItems("1,0,0,,系统设置,,NULL");
    ui->listView->setItems("2,0,0,0xf0f9,防区信息,,系统设置");
    ui->listView->setItems("2,0,0,0xf0f9,位置调整,,系统设置");
    ui->listView->setItems("2,0,1,0xf0f9,地图编辑,,系统设置");

    ui->listView->setItems("1,0,0,,警情查询,,NULL");
    ui->listView->setItems("2,0,0,0xf0b1,记录查询,,警情查询");
    ui->listView->setItems("2,0,0,0xf0b1,图像查询,,警情查询");
    ui->listView->setItems("2,0,0,0xf0b1,视频查询,,警情查询");
    ui->listView->setItems("2,0,1,0xf0b1,数据回放,,警情查询");

    ui->listView->setItems("1,0,1,,帮助文档,,NULL");
    ui->listView->setItems("2,0,0,0xf0c2,系统帮助,,帮助文档");
    ui->listView->setItems("2,0,0,0xf075,关于系统,,帮助文档");
    ui->listView->setItems("2,0,1,0xf015,关于系统,,帮助文档");

    connect(ui->listView, SIGNAL(pressed(const QString &, const QString &)), this, SLOT(slot_listview_pressed(const QString &, const QString &)));
    //设置整体圆角
    //    ui->widgetNav5->setStyleSheet(".QWidget{background:#292929;border:1px solid #292929;border-radius:20px;}");
}

void MainWindow::initStyle()
{
    //加载样式表
    //    QFile file(":/resources/qss/psblack.css");
    //    QFile file(":/resources/qss/flatwhite.css");
    QFile file(":/resources/qss/lightblue.css");
    //    QFile file(":/resources/qss/orangeyellow.css");
    if (file.open(QFile::ReadOnly))
    {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    //int wid = width();
    //    minButton->setGeometry(wid-65, 5, 20, 20);
    //    closeButton->setGeometry(wid-25, 5, 20, 20);
    //    maxButton->setGeometry(wid-45, 5, 20, 20);
}

void MainWindow::windowClosed()
{
    this->close();
}

void MainWindow::windowMaximize()
{
    this->showMaximized();
}

void MainWindow::windowMinimize()
{
    this->showMinimized();
}

void MainWindow::slot_listview_pressed(const QString &text, const QString &parenttext)
{
    qDebug() << "text:" << text << "parenttext:" << parenttext;
    if ((text == "数据基础操作") && (parenttext == "主界面"))
    {
        on_btn_view_clicked();
    } else if ((text == "数据汇总操作") && (parenttext == "主界面"))
    {
        on_btn_gather_clicked();
    }

}

void MainWindow::on_btn_view_clicked()
{
    ui->btn_view->setChecked(true);
    ui->btn_gather->setChecked(false);
    ui->stackedWidget->setCurrentIndex(0);

    if (configs["reprocess_mode"] == "true")
    {
        //frm_gather_ptr->set_handle_mode(true);
        emit update_process_mode(true);
    }
    else
    {
        emit update_process_mode(false);
    }
}

void MainWindow::on_btn_gather_clicked()
{

    if (frm_gather_ptr == NULL)
    {
        frm_gather_ptr = new frmGather(this);
        widgets.append(frm_gather_ptr);
        connect(this, SIGNAL(update_process_mode(bool)), frm_gather_ptr, SLOT(update_process_mode(bool)));

        ui->stackedWidget->addWidget(frm_gather_ptr);
    }

    ui->btn_view->setChecked(false);
    ui->btn_gather->setChecked(true);
    ui->stackedWidget->setCurrentIndex(1);

    if (configs["reprocess_mode"] == "true")
    {
        //frm_gather_ptr->set_handle_mode(true);
        emit update_process_mode(true);
    }
    else
    {
        emit update_process_mode(false);
    }

}


