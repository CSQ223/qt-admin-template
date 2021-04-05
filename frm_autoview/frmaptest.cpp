#include <QtGlobal>
#include <QFontDatabase>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QDir>
#include <QFileDialog>
#include <thread>
#include "frmaptest.h"
#include "ui_frmaptest.h"

#include <QDebug>


frmAptest::frmAptest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmAptest)
{
    ui->setupUi(this);

    cloud_handle_ptr = CloudData::Instance();

    InitFont();

    InitForm();

    InitStyle();

    InitData();

}

frmAptest::~frmAptest()
{
    delete ui;

    for (int index = 0; index < 32; index++)
    {
        delete osswidgets[index].label_num_ptr;
        delete osswidgets[index].label_filename_ptr;
        delete osswidgets[index].label_filesize_ptr;
        delete osswidgets[index].label_filedate_ptr;
        delete osswidgets[index].btn_download_ptr;
        delete osswidgets[index].progress_download_ptr;
        delete osswidgets[index].progress_handle_ptr;
    }
}

void frmAptest::InitFont()
{
    //判断图形字体是否存在,不存在则加入
    QFontDatabase fontDb;
    if (!fontDb.families().contains("FontAwesome")) {
        int fontId = fontDb.addApplicationFont(":resources/font/fontawesome-webfont.ttf");
        QStringList fontName = fontDb.applicationFontFamilies(fontId);
        if (fontName.count() == 0) {
            //qDebug() << "load fontawesome-webfont.ttf error";
        }
    }

    if (fontDb.families().contains("FontAwesome")) {
        iconFont = QFont("FontAwesome");
#if (QT_VERSION >= QT_VERSION_CHECK(4,8,0))
        iconFont.setHintingPreference(QFont::PreferNoHinting);
#endif
    }
}

void frmAptest::InitForm()
{
    ui->btn_downloadall->setCheckable(true);
    ui->cbx_download_all->setEnabled(false);
    for (int i = 1901; i <= 2099; i++) {
        ui->cboxYear->addItem(QString("%1年").arg(i), QVariant(i));
    }
    //月份下拉框
    for (int i = 1; i <= 12; i++) {
        ui->cboxMonth->addItem(QString("%1月").arg(i), QVariant(i));
    }
    //上一年按钮
//    ui->btnPrevYear->setObjectName("btnPrevYear");
//    ui->btnPrevYear->setFixedWidth(35);
//    ui->btnPrevYear->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->btnPrevYear->setFont(iconFont);
    ui->btnPrevYear->setText(QChar(0xf060));

    //下一年按钮
//    ui->btnNextYear->setFixedWidth(35);
//    ui->btnNextYear->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->btnNextYear->setFont(iconFont);
    ui->btnNextYear->setText(QChar(0xf061));

    //上个月按钮
//    ui->btnPrevMonth->setObjectName("btnPrevMonth");
//    ui->btnPrevMonth->setFixedWidth(35);
//    ui->btnPrevMonth->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    ui->btnPrevMonth->setFont(iconFont);
    ui->btnPrevMonth->setText(QChar(0xf060));

    //下个月按钮
//    ui->btnNextMonth->setObjectName("btnNextMonth");
//    ui->btnNextMonth->setFixedWidth(35);
//    ui->btnNextMonth->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    ui->btnNextMonth->setFont(iconFont);
    ui->btnNextMonth->setText(QChar(0xf061));

    for (int index = 0; index < 32; index++)
    {
        OSSWidget widget;

        widget.label_num_ptr = new QLabel("test");
        widget.label_filename_ptr = new QLabel("test");
        widget.label_filesize_ptr = new QLabel("test");
        widget.label_filedate_ptr = new QLabel("test");

        widget.btn_download_ptr = new QPushButton("下载");
        widget.btn_download_ptr->setObjectName(QString("btn_download%1").arg(index));

        widget.progress_download_ptr =  new QProgressBar();

        widget.btn_handle_ptr = new QPushButton("接管统计");
        widget.btn_handle_ptr->setObjectName(QString("btn_analyse_%1").arg(index));
        widget.progress_handle_ptr =  new QProgressBar();

        widget.label_ga_totalkilometer_ptr = new QLabel("test");
        widget.label_ga_autokilometer_ptr = new QLabel("test");

        connect(widget.btn_download_ptr, SIGNAL(clicked()), this, SLOT(slot_btn_clicked()));
        connect(widget.btn_handle_ptr, SIGNAL(clicked()), this, SLOT(slot_btn_handle_clicked()));
        osswidgets.push_back(widget);
    }

    ui->tableWidget->setRowCount(32);
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置
    //
#if 0
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("序号")<<tr("文件名") \
              <<tr("文件大小")<<tr("更新日期")<<tr("下载")<<tr("下载进度") << tr("接管分析") <<tr("分析进度"));
#endif
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("序号")<<tr("文件名") \
              <<tr("文件大小")<<tr("更新日期")<<tr("下载")<<tr("下载进度") << tr("测试总里程(KM)") <<tr("驾驶总里程(KM)"));
    ui->tableWidget->verticalHeader()->setHidden(true);
    //int cloumwidth = ui->tableWidget->frameWidth();

    ui->tableWidget->setColumnWidth(0, 50);

#ifdef Q_OS_LINUX
// linux
    ui->tableWidget->setColumnWidth(1, 200);
    ui->tableWidget->setColumnWidth(2, 60);
    ui->tableWidget->setColumnWidth(3, 60);
#endif

#ifdef Q_OS_WIN32
// win

    ui->tableWidget->setColumnWidth(1, 300);
    ui->tableWidget->setColumnWidth(2, 80);
    ui->tableWidget->setColumnWidth(3, 80);
#endif

    ui->tableWidget->setColumnWidth(4, 70);
    ui->tableWidget->setColumnWidth(5, 150);
    ui->tableWidget->setColumnWidth(6, 70);
    ui->tableWidget->setColumnWidth(7, 150);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->hide();

}



void frmAptest::test()
{
    //qDebug() << "btn clicked." ;
}

void frmAptest::InitStyle()
{

}

void frmAptest::InitData()
{
    QDir dir;
    QString vehicleid;
    std::vector<ALIBABACLOUD_HANDLE::file_detail_st> filedetails;


    app_path_ = QCoreApplication::applicationDirPath();
    dir.setPath(app_path_ + "/temp");
    if (!dir.exists())
    {
        dir.mkdir(app_path_ + "/temp");
    }
//    handle_ptr = std::make_shared<ALIBABACLOUD_HANDLE::alibabacloud_handle>(AccessKeyId, AccessKeySecret, Endpoint, BucketName);

    date = QDate::currentDate();

    cbox_year = date.year();
    cbox_month = date.month();

    //设置为今天,设置变量防止重复触发
    ui->cboxYear->setCurrentIndex(ui->cboxYear->findText(QString("%1年").arg(cbox_year)));
    ui->cboxMonth->setCurrentIndex(ui->cboxMonth->findText(QString("%1月").arg(cbox_month)));

    ui->cbx_frametype->addItem("1");
    ui->cbx_frametype->addItem("2");
    ui->cbx_frametype->addItem("3");

    ui->cbx_vehicleid->addItem("temp");

    vehicleid = ui->cbx_vehicleid->currentText();

//    //CloudData *cloud_handle_ptr = CloudData::Instance();
    if (cloud_handle_ptr)
    {
        std::string strtime = "2020-06";
        std::string serach_directory = vehicleid.toStdString() + "/";
        bool result = cloud_handle_ptr->query_directory(serach_directory, filedetails);
        if (!result)
        {
            std::cout << "cloud_data_ptr query_directory error!" << std::endl;
            return;
        }

//        for (std::string::size_type i=0; i< filedetails.size(); i++)
//        {
//            std::cout << filedetails[i].name << ":" << filedetails[i].size << std::endl;
//        }

        bool ret = cloud_handle_ptr->query_directory(serach_directory, filedetails);
        if (!ret)
        {
            return;
        }
        oss_mutex_.lock();
        ossdatas.clear();
        for (std::string::size_type i=0; i< filedetails.size(); i++)
        {
            char time_buffer[40] = {0};
            unix_date_time time;

            if (filedetails[i].name.find(".csv") != std::string::npos && \
                filedetails[i].name.find(strtime) != std::string::npos)
            {
                OSSData data;
                data.seqnum = static_cast<int>(i);
                data.filename = filedetails[i].name;
                data.filesize = filedetails[i].size;
                sscanf(filedetails[i].date.c_str(), "%hu-%hhu-%hhuT%hhud:%hhu:%hhu.%uZ", \
                                            &time.year, &time.mon, &time.day, &time.hour, \
                                            &time.min, &time.sec, &time.msec);
                sprintf(time_buffer, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", time.year, \
                                        time.mon, time.day, time.hour, time.min, time.sec);
                data.filedate = time_buffer;
                data.is_downloading = false;
                data.download_status = 0;
                data.handle_status = 0;
                data.is_handling = false;
                ossdatas.push_back(data);
            }

        }
        oss_mutex_.unlock();
        UpdateTableWidget(1);
    }
//    osswidgets[0].progress_download_ptr->setValue(20);
//    osswidgets[0].progress_handle_ptr->setValue(20);

    timer_ptr = new QTimer(this);
    connect(timer_ptr, SIGNAL(timeout()), this, SLOT(slot_timeout()));
    timer_ptr->start(1000);

    timer_update_gadata_ptr = new QTimer(this);
    connect(timer_update_gadata_ptr, SIGNAL(timeout()), this, SLOT(slot_update_gadata_timeout()));
    timer_update_gadata_ptr->start(30000);
}

void frmAptest::slot_update_gadata_timeout()
{
    QString strDataType = ui->cbx_frametype->currentText();

}

void frmAptest::slot_timeout()
{

}

void frmAptest::UpdateTableWidget(int type)
{
    int index = 0;
    if (ossdatas.size() > 31)
    {
        return ;
    }

//    ui->tableWidget->clearContents();

//    ui->tableWidget->setRowCount(32);
//    ui->tableWidget->setColumnCount(7);
    oss_mutex_.lock();

    for (index = 0; index < osswidgets.size(); index++)
    {
        ui->tableWidget->removeCellWidget(index, 0);
        ui->tableWidget->removeCellWidget(index, 1);
        ui->tableWidget->removeCellWidget(index, 2);

        ui->tableWidget->removeCellWidget(index, 3);
        ui->tableWidget->removeCellWidget(index, 4);
        ui->tableWidget->removeCellWidget(index, 5);
        ui->tableWidget->removeCellWidget(index, 6);
        ui->tableWidget->removeCellWidget(index, 7);
    }

    for (auto iter : osswidgets)
    {
        delete iter.label_num_ptr;
        delete iter.label_filename_ptr;
        delete iter.label_filesize_ptr;
        delete iter.label_filedate_ptr;
        delete iter.btn_download_ptr;
        delete iter.progress_download_ptr;
        delete iter.btn_handle_ptr;
        delete iter.progress_handle_ptr;
        delete iter.label_ga_totalkilometer_ptr;
        delete iter.label_ga_autokilometer_ptr;
    }

    osswidgets.clear();

    for (int index = 0; index < 32; index++)
    {
        OSSWidget widget;

        widget.label_num_ptr = new QLabel("test");
        widget.label_filename_ptr = new QLabel("test");
        widget.label_filesize_ptr = new QLabel("test");
        widget.label_filedate_ptr = new QLabel("test");

        widget.btn_download_ptr = new QPushButton("下载");
        widget.btn_download_ptr->setObjectName(QString("btn_download%1").arg(index));
        widget.progress_download_ptr =  new QProgressBar();
        if (type == 1)
        {
            widget.btn_download_ptr->setEnabled(true);
            widget.progress_download_ptr->setEnabled(true);
        } else {
            widget.btn_download_ptr->setEnabled(false);
            widget.progress_download_ptr->setEnabled(false);
        }


        widget.btn_handle_ptr = new QPushButton("接管分析");
        widget.btn_handle_ptr->setObjectName(QString("btn_handle_%1").arg(index));
        widget.progress_handle_ptr =  new QProgressBar();
        if (type == 1)
        {
            widget.btn_handle_ptr->setEnabled(false);
            widget.progress_handle_ptr->setEnabled(false);
        } else {
            widget.btn_handle_ptr->setEnabled(true);
            widget.progress_handle_ptr->setEnabled(true);
        }

        widget.label_ga_totalkilometer_ptr = new QLabel("test");
        widget.label_ga_autokilometer_ptr = new QLabel("test");

        connect(widget.btn_download_ptr, SIGNAL(clicked()), this, SLOT(slot_btn_clicked()));
        connect(widget.btn_handle_ptr, SIGNAL(clicked()), this, SLOT(slot_btn_handle_clicked()));
        osswidgets.push_back(widget);
    }
    if (type == 1)
    {
        ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("序号")<<tr("文件名") \
                  <<tr("文件大小")<<tr("更新日期")<<tr("下载")<<tr("下载进度") << tr("测试总里程(KM)") <<tr("总里程(KM)"));
    } else {
        ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("序号")<<tr("文件名") \
                  <<tr("文件大小")<<tr("更新日期")<<tr("下载")<<tr("下载进度") << tr("接管分析") <<tr("分析进度"));
    }

    for (size_t index=0; index < ossdatas.size(); index++)
    {
        osswidgets[static_cast<int>(index)].label_num_ptr->setText(QString("%1").arg(ossdatas[index].seqnum));
        osswidgets[static_cast<int>(index)].label_num_ptr->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        osswidgets[static_cast<int>(index)].label_filename_ptr->setText(QString::fromStdString(ossdatas[index].filename));
        osswidgets[static_cast<int>(index)].label_filename_ptr->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        osswidgets[static_cast<int>(index)].label_filesize_ptr->setText(QString::fromStdString(ossdatas[index].filesize));
        osswidgets[static_cast<int>(index)].label_filesize_ptr->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        osswidgets[static_cast<int>(index)].label_filedate_ptr->setText(QString::fromStdString(ossdatas[index].filedate));
        osswidgets[static_cast<int>(index)].label_filedate_ptr->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        if (ossdatas[index].download_status == 1)
        {
            osswidgets[static_cast<int>(index)].progress_download_ptr->setValue(100);
        } else if (ossdatas[index].download_status == 2) {
            osswidgets[static_cast<int>(index)].progress_download_ptr->setValue(0);
            osswidgets[static_cast<int>(index)].progress_handle_ptr->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
        } else {
            osswidgets[static_cast<int>(index)].progress_download_ptr->setValue(0);
        }

        if (ossdatas[index].handle_status)
        {
            osswidgets[static_cast<int>(index)].progress_handle_ptr->setValue(100);
        } else {
            osswidgets[static_cast<int>(index)].progress_handle_ptr->setValue(0);
        }
        ui->tableWidget->setCellWidget(static_cast<int>(index), 0, osswidgets[static_cast<int>(index)].label_num_ptr);
        ui->tableWidget->setCellWidget(static_cast<int>(index), 1, osswidgets[static_cast<int>(index)].label_filename_ptr);
        ui->tableWidget->setCellWidget(static_cast<int>(index), 2, osswidgets[static_cast<int>(index)].label_filesize_ptr);
        ui->tableWidget->setCellWidget(static_cast<int>(index), 3, osswidgets[static_cast<int>(index)].label_filedate_ptr);
        //btn_download center show
        QWidget *btn_download_widget_ptr = new QWidget;
        QHBoxLayout *layout = new QHBoxLayout;
        layout->setSpacing(0);
        layout->setMargin(0);
        layout->addWidget(osswidgets[static_cast<int>(index)].btn_download_ptr);
        btn_download_widget_ptr->setLayout(layout);
        ui->tableWidget->setCellWidget(static_cast<int>(index), 4, btn_download_widget_ptr);
        ui->tableWidget->setCellWidget(static_cast<int>(index), 5, osswidgets[static_cast<int>(index)].progress_download_ptr);
        // btn_analyse center view.


        if (type == 1)
        {
            bool result = false;
            std::string filecontent;
            result = cloud_handle_ptr->download_files_to_memory(ossdatas[index].filename, filecontent);
            if (result)
            {
                Vehicle_DataInfos_st vehicle_datainfos;
                csv_handler csv_hdl;

                csv_hdl.convert_from_csv(filecontent, vehicle_datainfos);


                osswidgets[static_cast<int>(index)].label_ga_totalkilometer_ptr->setText(QString::number(vehicle_datainfos.ga_data.vehicle_total_mileage, 'f', 2));
                osswidgets[static_cast<int>(index)].label_ga_autokilometer_ptr->setText(QString::number(vehicle_datainfos.ga_data.vehicle_auto_mileage, 'f', 2));
            } else {
                osswidgets[static_cast<int>(index)].label_ga_totalkilometer_ptr->setText("0");
                osswidgets[static_cast<int>(index)].label_ga_autokilometer_ptr->setText("0");
            }


            ui->tableWidget->setCellWidget(static_cast<int>(index), 6, osswidgets[static_cast<int>(index)].label_ga_totalkilometer_ptr);
            osswidgets[static_cast<int>(index)].label_ga_totalkilometer_ptr->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->tableWidget->setCellWidget(static_cast<int>(index), 7, osswidgets[static_cast<int>(index)].label_ga_autokilometer_ptr);
            osswidgets[static_cast<int>(index)].label_ga_autokilometer_ptr->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        } else {
            QWidget *btn_analsye_widget_ptr = new QWidget;
            QHBoxLayout *layout_analyse = new QHBoxLayout;
            layout_analyse->setSpacing(0);
            layout_analyse->setMargin(0);
            layout_analyse->addWidget(osswidgets[static_cast<int>(index)].btn_handle_ptr);
            btn_analsye_widget_ptr->setLayout(layout_analyse);

            ui->tableWidget->setCellWidget(static_cast<int>(index), 6, btn_analsye_widget_ptr);
            ui->tableWidget->setCellWidget(static_cast<int>(index), 7, osswidgets[static_cast<int>(index)].progress_handle_ptr);
        }


    }
    oss_mutex_.unlock();
}

void frmAptest::slot_btn_clicked()
{

#ifdef Q_OS_LINUX
// linux
    QString savepath = "/";
#endif

#ifdef Q_OS_WIN32
// win
    QString savepath = "D:/";
#endif

    QString vehicleid = ui->cbx_vehicleid->currentText();
    QPushButton* btn_ptr = qobject_cast<QPushButton*>(sender());
    std::string pathname = app_path_.toStdString();
    std::string local_filename;
    QDir dir(app_path_ + "/temp/" + vehicleid);
    if (!dir.exists())
    {
        dir.mkdir(app_path_ + "/temp/" + vehicleid);
    }

//    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), savepath, tr("TEXT (*.asc)"));
//    if (path.isEmpty())
//    {
//        return;
//    }
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), savepath,
                                                     QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if (path.isEmpty())
    {
        return;
    }


    if ("btn_download0" == btn_ptr->objectName())
    {

//        QString path = QCoreApplication::applicationDirPath();

        std::string remote_directory = ossdatas[0].filename;
        std::string local_directory = pathname + "/temp/" + ossdatas[0].filename;
        std::size_t pos = ossdatas[0].filename.find("/", 0);//.find(vehicleid, 0);
        local_filename = path.toStdString() + "/" + ossdatas[0].filename.substr(pos + 1);

        ossdatas[0].is_downloading = 1;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[0].is_downloading = false;
                ossdatas[0].download_status = 1;
//                osswidgets[0].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[0].is_downloading = false;
                ossdatas[0].download_status = 2;
//                osswidgets[0].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download1" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[1].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[1].filename;

        std::size_t pos = ossdatas[1].filename.find("/", 0);//.find(vehicleid, 0);
        local_filename = path.toStdString() + "/" + ossdatas[1].filename.substr(pos + 1);

        ossdatas[1].is_downloading = true;
        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[1].is_downloading = false;
                ossdatas[1].download_status = 1;
//                osswidgets[1].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[1].is_downloading = false;
                ossdatas[1].download_status = 2;
//                osswidgets[1].progress_download_ptr->setValue(0);
            }
        }).detach();

    } else if ("btn_download2" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[2].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[2].filename;

        std::size_t pos = ossdatas[2].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[2].filename.substr(pos + 1);

        ossdatas[2].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[2].is_downloading = false;
                ossdatas[2].download_status = 1;
//                osswidgets[2].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[2].is_downloading = false;
                ossdatas[2].download_status = 2;
//                osswidgets[2].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download3" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[3].filename;
//        std::string local_directory = ossdatas[3].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[3].filename;

        std::size_t pos = ossdatas[3].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[3].filename.substr(pos + 1);

        ossdatas[3].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[3].is_downloading = false;
                ossdatas[3].download_status = 1;
//                osswidgets[3].progress_download_ptr->setValue(100);
            }  else {
//                ossdatas[3].is_downloading = false;
                ossdatas[3].download_status = 2;
//                osswidgets[3].progress_download_ptr->setValue(0);
            }
       }).detach();

    } else if ("btn_download4" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[4].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[4].filename;

        std::size_t pos = ossdatas[4].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[4].filename.substr(pos + 1);

        ossdatas[4].is_downloading = true;
        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[4].is_downloading = false;
                ossdatas[4].download_status = 1;
//                osswidgets[4].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[4].is_downloading = false;
                ossdatas[4].download_status = 2;
//                osswidgets[4].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download5" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[5].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[5].filename;

        std::size_t pos = ossdatas[5].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[5].filename.substr(pos + 1);

        ossdatas[5].is_downloading = true;
        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[5].is_downloading = false;
                ossdatas[5].download_status = 1;
//                osswidgets[5].progress_download_ptr->setValue(100);
            }  else {
//                ossdatas[5].is_downloading = false;
                ossdatas[5].download_status = 2;
//                osswidgets[5].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download6" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[6].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[6].filename;

        std::size_t pos = ossdatas[6].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[6].filename.substr(pos + 1);

        ossdatas[6].is_downloading = true;
        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[6].is_downloading = false;
                ossdatas[6].download_status = 1;
//                osswidgets[6].progress_download_ptr->setValue(100);
            }  else {
//                ossdatas[6].is_downloading = false;
                ossdatas[6].download_status = 2;
//                osswidgets[6].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download7" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[7].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[7].filename;

        std::size_t pos = ossdatas[7].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[7].filename.substr(pos + 1);

        ossdatas[7].is_downloading = true;
        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[7].is_downloading = false;
                ossdatas[7].download_status = 1;
//                osswidgets[7].progress_download_ptr->setValue(100);
            }  else {
//                ossdatas[7].is_downloading = false;
                ossdatas[7].download_status = 2;
//                osswidgets[7].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download8" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[8].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[8].filename;

        std::size_t pos = ossdatas[8].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[8].filename.substr(pos + 1);

        ossdatas[8].is_downloading = true;
        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[8].is_downloading = false;
                ossdatas[8].download_status = 1;
//                osswidgets[8].progress_download_ptr->setValue(100);
            }  else {
//                ossdatas[8].is_downloading = false;
                ossdatas[8].download_status = 2;
//                osswidgets[8].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download9" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[9].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[9].filename;

        std::size_t pos = ossdatas[9].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[9].filename.substr(pos + 1);

        ossdatas[9].is_downloading = true;
        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[9].is_downloading = false;
                ossdatas[9].download_status = 1;
//                osswidgets[9].progress_download_ptr->setValue(100);
            }else {
//                ossdatas[9].is_downloading = false;
                ossdatas[9].download_status = 2;
//                osswidgets[9].progress_download_ptr->setValue(0);
            }
       }).detach();
    } else if ("btn_download10" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[10].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[10].filename;

        std::size_t pos = ossdatas[10].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[10].filename.substr(pos + 1);

        ossdatas[10].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[10].is_downloading = false;
                ossdatas[10].download_status = 1;
//                osswidgets[10].progress_download_ptr->setValue(100);
            }  else {
//                ossdatas[10].is_downloading = false;
                ossdatas[10].download_status = 2;
//                osswidgets[10].progress_download_ptr->setValue(0);
            }
       }).detach();
    } else if ("btn_download11" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[11].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[11].filename;

        std::size_t pos = ossdatas[11].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[11].filename.substr(pos + 1);

        ossdatas[11].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[11].is_downloading = false;
                ossdatas[11].download_status = 1;
//                osswidgets[11].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[11].is_downloading = false;
                ossdatas[11].download_status = 2;
//                osswidgets[11].progress_download_ptr->setValue(0);
            }
       }).detach();
    } else if ("btn_download12" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[12].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[12].filename;

        std::size_t pos = ossdatas[12].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[12].filename.substr(pos + 1);

        ossdatas[12].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[12].is_downloading = false;
                ossdatas[12].download_status = 1;
//                osswidgets[12].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[12].is_downloading = false;
                ossdatas[12].download_status = 2;
//                osswidgets[12].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download13" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[13].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[13].filename;

        std::size_t pos = ossdatas[13].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[13].filename.substr(pos + 1);

        ossdatas[13].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[13].is_downloading = false;
                ossdatas[13].download_status = 1;
//                osswidgets[13].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[13].is_downloading = false;
                ossdatas[13].download_status = 2;
//                osswidgets[13].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download14" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[14].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[14].filename;

        std::size_t pos = ossdatas[14].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[14].filename.substr(pos + 1);

        ossdatas[14].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[14].is_downloading = false;
                ossdatas[14].download_status = 1;
//                osswidgets[14].progress_download_ptr->setValue(100);
            }else {
//                ossdatas[14].is_downloading = false;
                ossdatas[14].download_status = 2;
//                osswidgets[14].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download15" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[15].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[15].filename;

        std::size_t pos = ossdatas[15].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[15].filename.substr(pos + 1);

        ossdatas[15].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[15].is_downloading = false;
                ossdatas[15].download_status = 1;
//                osswidgets[15].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[15].is_downloading = false;
                ossdatas[15].download_status = 2;
//                osswidgets[15].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download16" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[16].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[16].filename;

        std::size_t pos = ossdatas[16].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[16].filename.substr(pos + 1);

        ossdatas[16].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[16].is_downloading = false;
                ossdatas[16].download_status = 1;
//                osswidgets[16].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[16].is_downloading = false;
                ossdatas[16].download_status = 2;
//                osswidgets[16].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download17" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[17].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[17].filename;

        std::size_t pos = ossdatas[17].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[17].filename.substr(pos + 1);

        ossdatas[17].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[17].is_downloading = false;
                ossdatas[17].download_status = 1;
//                osswidgets[17].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[17].is_downloading = false;
                ossdatas[17].download_status = 2;
//                osswidgets[17].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download18" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[18].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[18].filename;

        std::size_t pos = ossdatas[18].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[18].filename.substr(pos + 1);

        ossdatas[18].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[18].is_downloading = false;
                ossdatas[18].download_status = 1;
//                osswidgets[18].progress_download_ptr->setValue(100);
            }else {
//                ossdatas[18].is_downloading = false;
                ossdatas[18].download_status = 2;
//                osswidgets[18].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download19" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[19].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[19].filename;

        std::size_t pos = ossdatas[19].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[19].filename.substr(pos + 1);

        ossdatas[19].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[19].is_downloading = false;
                ossdatas[19].download_status = 1;
//                osswidgets[19].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[19].is_downloading = false;
                ossdatas[19].download_status = 2;
//                osswidgets[19].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download20" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[20].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[20].filename;

        std::size_t pos = ossdatas[20].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[20].filename.substr(pos + 1);

        ossdatas[20].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[20].is_downloading = false;
                ossdatas[20].download_status = 1;
//                osswidgets[20].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[20].is_downloading = false;
                ossdatas[20].download_status = 2;
//                osswidgets[20].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download21" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[21].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[21].filename;

        std::size_t pos = ossdatas[21].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[21].filename.substr(pos + 1);

        ossdatas[21].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[21].is_downloading = false;
                ossdatas[21].download_status = 1;
//                osswidgets[21].progress_download_ptr->setValue(100);
            }else {
//                ossdatas[21].is_downloading = false;
                ossdatas[21].download_status = 2;
//                osswidgets[21].progress_download_ptr->setValue(0);
            }
        }).detach();

    } else if ("btn_download22" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[22].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[22].filename;

        std::size_t pos = ossdatas[22].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[22].filename.substr(pos + 1);

        ossdatas[22].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[22].is_downloading = false;
                ossdatas[22].download_status = 1;
//                osswidgets[22].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[22].is_downloading = false;
                ossdatas[22].download_status = 2;
//                osswidgets[22].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download23" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[23].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[23].filename;

        std::size_t pos = ossdatas[23].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[23].filename.substr(pos + 1);

        ossdatas[23].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[23].is_downloading = false;
                ossdatas[23].download_status = 1;
//                osswidgets[23].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[23].is_downloading = false;
                ossdatas[23].download_status = 2;
//                osswidgets[23].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download24" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[24].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[24].filename;

        std::size_t pos = ossdatas[24].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[24].filename.substr(pos + 1);

        ossdatas[24].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[24].is_downloading = false;
                ossdatas[24].download_status = 1;
//                osswidgets[24].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[24].is_downloading = false;
                ossdatas[24].download_status = 2;
//                osswidgets[24].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download25" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[25].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[25].filename;

        std::size_t pos = ossdatas[25].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[25].filename.substr(pos + 1);

        ossdatas[25].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[25].is_downloading = false;
                ossdatas[25].download_status = 1;
//                osswidgets[25].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[25].is_downloading = false;
                ossdatas[25].download_status = 2;
//                osswidgets[25].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download26" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[26].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[26].filename;

        std::size_t pos = ossdatas[26].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[26].filename.substr(pos + 1);

        ossdatas[26].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[26].is_downloading = false;
                ossdatas[26].download_status = 1;
//                osswidgets[26].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[26].is_downloading = false;
                ossdatas[26].download_status = 2;
//                osswidgets[26].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download27" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[27].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[27].filename;

        std::size_t pos = ossdatas[27].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[27].filename.substr(pos + 1);

        ossdatas[27].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[27].is_downloading = false;
                ossdatas[27].download_status = 1;
//                osswidgets[27].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[27].is_downloading = false;
                ossdatas[27].download_status = 2;
//                osswidgets[27].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download28" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[28].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[28].filename;

        std::size_t pos = ossdatas[28].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[28].filename.substr(pos + 1);

        ossdatas[28].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[28].is_downloading = false;
                ossdatas[28].download_status = 1;
//                osswidgets[28].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[28].is_downloading = false;
                ossdatas[28].download_status = 2;
//                osswidgets[28].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download29" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[29].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[29].filename;

        std::size_t pos = ossdatas[29].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[29].filename.substr(pos + 1);

        ossdatas[29].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[29].is_downloading = false;
                ossdatas[29].download_status = 1;
//                osswidgets[29].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[29].is_downloading = false;
                ossdatas[29].download_status = 2;
//                osswidgets[29].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download30" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[30].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[30].filename;

        std::size_t pos = ossdatas[30].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[30].filename.substr(pos + 1);

        ossdatas[30].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[30].is_downloading = false;
                ossdatas[30].download_status = 1;
//                osswidgets[30].progress_download_ptr->setValue(100);
            }  else {
//                ossdatas[30].is_downloading = false;
                ossdatas[30].download_status = 2;
//                osswidgets[30].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else if ("btn_download31" == btn_ptr->objectName()){
        std::string remote_directory = ossdatas[31].filename;
        std::string local_directory = pathname + "/temp/" +ossdatas[31].filename;

        std::size_t pos = ossdatas[31].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[31].filename.substr(pos + 1);

        ossdatas[31].is_downloading = true;

        std::thread([this, local_filename, remote_directory]() {
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
//                ossdatas[31].is_downloading = false;
                ossdatas[31].download_status = 1;
//                osswidgets[31].progress_download_ptr->setValue(100);
            } else {
//                ossdatas[31].is_downloading = false;
                ossdatas[31].download_status = 1;
//                osswidgets[31].progress_download_ptr->setValue(0);
            }
        }).detach();
    } else {

    }
}

void frmAptest::handle_asc_file(std::string localfilename, std::string remotefilename)
{
    if (localfilename == "")
    {

    }

    if (remotefilename == "")
    {

    }
}

void frmAptest::slot_btn_handle_clicked()
{
    std::string local_filename;
    std::string remote_filename;
    std::string vehiclename;
#ifdef Q_OS_LINUX
// linux
    QString savepath = "/";
#endif

#ifdef Q_OS_WIN32
// win
    QString savepath = "D:/";
#endif

    QString vehicldid = ui->cbx_vehicleid->currentText();
    QPushButton* btn_ptr = qobject_cast<QPushButton*>(sender());
    //QString path = QFileDialog::getSaveFileName(this, tr("Save File"), savepath, tr("TEXT (*.csv)"));

    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), savepath,
                                                     QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if (path.isEmpty())
    {
        return;
    }

    local_filename = path.toStdString();
    vehiclename = vehicldid.toStdString();

    if (btn_ptr->objectName() == "btn_handle_0")
    {
        std::string filename;
        std::vector<std::string> strelems;
        //qDebug() << "btn_analyse_0 clicked."<< path;
        remote_filename = ossdatas[0].filename;
        split_to_string(remote_filename, "/", strelems);
//        qDebug() << QString::fromStdString(strelems[0]) << " "  << QString::fromStdString(strelems[1]) << " " << QString::fromStdString(strelems[2]);
        if (strelems.size() != 3)
        {
            ossdatas[0].is_handling = true;
            ossdatas[0].handle_status = 2;
            return;
        }
        filename = replace_str(strelems[2], "asc", "csv");
        local_filename = path.toStdString() + "/" + strelems[0] + "_" + filename;
        ossdatas[0].is_handling = true;
        std::thread([this, vehiclename, local_filename, remote_filename](){
            std::string content;
            char *content_ptr = nullptr;
            int64_t content_len = 0;
            //CloudData *cloud_handle_ptr = CloudData::Instance();
            CSVOperator csv_analyse(local_filename);
            std::vector<VehicleMonitorInfo> takeoverlist;


            bool ret = cloud_handle_ptr->download_files_to_memory(remote_filename, &content_ptr, content_len);
            if (ret)
            {
                for (int j=0; j< content_len; j++)
                {
                    if (content_ptr[j] == 0)
                    {
                        content_ptr[j] = 0x0A;
                    }
                }
                content = content_ptr;
            }
//            ossdatas[0].is_handling = false;
            csv_analyse.save(local_filename);
        }).detach();
    }
}
//char* frmAptest::ConvertUtcToLocalTime(struct tm* t2,const char* date){

// struct tm t;
// memset(&t,sizeof(t));
// t.tm_year = atoi(date)-1900;
// t.tm_mon = atoi(date+5)-1;
// t.tm_mday = atoi(date+8);
// t.tm_hour = atoi(date+11);
// t.tm_min = atoi(date+14);
// t.tm_sec = atoi(date+17);

// time_t tt = _mkgmtime64(&t);
// if(tt != -1){
// if(t2 == nullptr){
//  t2 = &t;
// }
// *t2 = *localtime(&tt);
// char* ds = (char*) malloc(24);
// memset(ds,24);
// sprintf(ds,"%.4d-%.2d-%.2d %.2d:%.2d:%.2d",t2->tm_year + 1900,t2->tm_mon + 1,t2->tm_mday,t2->tm_hour,t2->tm_min,t2->tm_sec);
// return ds;
// }
// return nullptr;
//}

void frmAptest::UpdateOSSView(const QString &strSearchDir,const QString &strDateTime, const QString &strDataType)
{
    
}


void frmAptest::on_btnPrevYear_clicked()
{
    int year = 0;
    int month = 0;
    QString stryear, strmonth, strDateTime, strDataType;
    QString vehicleid = ui->cbx_vehicleid->currentText();
    QString strSearchDir = vehicleid + "/";

    cbox_year -= 1;

    if (cbox_year < 1901)
    {
        cbox_year = 1901;
    }

    if (cbox_year > 2099)
    {
        cbox_year = 2099;
    }

    ui->cboxYear->setCurrentIndex(ui->cboxYear->findText(QString("%1年").arg(cbox_year)));


    year = ui->cboxYear->currentData().toInt();
    month = ui->cboxMonth->currentData().toInt();
    stryear = QString("%1").arg(year);
    strmonth = QString("%1").arg(month, 2, 10, QLatin1Char('0'));
    strDateTime = stryear + "-" + strmonth;

    strDataType = ui->cbx_frametype->currentText();


    UpdateOSSView(strSearchDir, strDateTime, strDataType);

}

void frmAptest::on_btnNextYear_clicked()
{
    int year = 0;
    int month = 0;
    QString stryear, strmonth, strDateTime, strDataType;
    QString vehicleid = ui->cbx_vehicleid->currentText();
    QString strSearchDir = vehicleid + "/";

    cbox_year += 1;

    if (cbox_year < 1901)
    {
        cbox_year = 1901;
    }

    if (cbox_year > 2099)
    {
        cbox_year = 2099;
    }

    ui->cboxYear->setCurrentIndex(ui->cboxYear->findText(QString("%1年").arg(cbox_year)));

    year = ui->cboxYear->currentData().toInt();
    month = ui->cboxMonth->currentData().toInt();
    stryear = QString("%1").arg(year);
    strmonth = QString("%1").arg(month, 2, 10, QLatin1Char('0'));
    strDateTime = stryear + "-" + strmonth;

    strDataType = ui->cbx_frametype->currentText();


    UpdateOSSView(strSearchDir, strDateTime, strDataType);

}

void frmAptest::on_btnPrevMonth_clicked()
{
    int year = 0;
    int month = 0;
    QString stryear, strmonth, strDateTime, strDataType;
    QString vehicleid = ui->cbx_vehicleid->currentText();
    QString strSearchDir = vehicleid + "/";

    cbox_month -= 1;
    if (cbox_month == 0)
    {
        cbox_month = 12;
    }

    if (cbox_month >12)
    {
        cbox_month = 1;
    }

    ui->cboxMonth->setCurrentIndex(ui->cboxMonth->findText(QString("%1月").arg(cbox_month)));

    year = ui->cboxYear->currentData().toInt();
    month = ui->cboxMonth->currentData().toInt();
    stryear = QString("%1").arg(year);
    strmonth = QString("%1").arg(month, 2, 10, QLatin1Char('0'));
    strDateTime = stryear + "-" + strmonth;

    strDataType = ui->cbx_frametype->currentText();


    UpdateOSSView(strSearchDir, strDateTime, strDataType);

}

void frmAptest::on_btnNextMonth_clicked()
{
    int year = 0;
    int month = 0;
    QString stryear, strmonth, strDateTime, strDataType;
    QString vehicleid = ui->cbx_vehicleid->currentText();
    QString strSearchDir = vehicleid + "/";

    cbox_month += 1;
    if (cbox_month == 0)
    {
        cbox_month = 12;
    }

    if (cbox_month >12)
    {
        cbox_month = 1;
    }

    ui->cboxMonth->setCurrentIndex(ui->cboxMonth->findText(QString("%1月").arg(cbox_month)));

    year = ui->cboxYear->currentData().toInt();
    month = ui->cboxMonth->currentData().toInt();
    stryear = QString("%1").arg(year);
    strmonth = QString("%1").arg(month, 2, 10, QLatin1Char('0'));
    strDateTime = stryear + "-" + strmonth;

    strDataType = ui->cbx_frametype->currentText();


    UpdateOSSView(strSearchDir, strDateTime, strDataType);

}


void frmAptest::on_cbx_frametype_currentIndexChanged(const QString &strtype)
{
    QString strDateTime;
    QString vehicleid = ui->cbx_vehicleid->currentText();
    QString strSearchDir = vehicleid + "/";
    int year = ui->cboxYear->currentData().toInt();
    int month = ui->cboxMonth->currentData().toInt();
    QString stryear = QString("%1").arg(year);
    QString strmonth = QString("%1").arg(month, 2, 10, QLatin1Char('0'));
    strDateTime = stryear + "-" + strmonth;
    for (std::string::size_type i =0; i < ossdatas.size(); i++)
    {
        ossdatas[i].is_downloading = false;
        ossdatas[i].download_status = 0;

        ossdatas[i].is_handling = false;
        ossdatas[i].handle_status = 0;
    }

    UpdateOSSView(strSearchDir, strDateTime, strtype);
}

void frmAptest::on_btn_handle_clicked()
{
    //int type = 0;
    int index = 1;
    size_t i = 0;
    std::vector<VehicleMonitorInfo> takeoverlist;
#ifdef Q_OS_LINUX
// linux
    QString savepath = "/";
#endif

#ifdef Q_OS_WIN32
// win
    QString savepath = "D:/";
#endif


    QString vehicleid = ui->cbx_vehicleid->currentText();
    QString str = ui->cbx_frametype->currentText();
    int year = ui->cboxYear->currentData().toInt();
    int month = ui->cboxMonth->currentData().toInt();
    QString stryear = QString("%1").arg(year);
    QString strmonth = QString("%1").arg(month, 2, 10, QLatin1Char('0'));



    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), savepath,
                                                     QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if (path.isEmpty())
    {
        return;
    }
//    csv_filename_ = path.toStdString() + "";

    if ("原始数据1" == str)
    {
       //type = 2;
       ui->btn_handle->setEnabled(true);
       csv_filename_ = path.toStdString() + "/" + vehicleid.toStdString() + "_SH-" + stryear.toStdString() + "-" + strmonth.toStdString() + ".csv";
    }
    else if ("原始数据2" == str)
    {
       //type = 3;
       ui->btn_handle->setEnabled(true);
       csv_filename_ =path.toStdString() + "/" + vehicleid.toStdString() + "_GA-" + stryear.toStdString() + "-" + strmonth.toStdString() + ".csv";
    }

    ui->btn_handle->setText("分析中...");
    ui->btn_handle->setEnabled(false);

    csv.setString(0, 1, "序号");
    csv.setString(0, 2, "车辆名称");
    csv.setString(0, 3, "时间");
    csv.setString(0, 4, "脱离地点");
    csv.setString(0, 5, "接管用时");
    csv.setString(0, 6, "车辆状态");
    csv.setString(0, 7, "外部环境");
    csv.setString(0, 8, "脱离原因");


    for (auto iter : ossdatas)
    {
        std::string content;
        char *content_ptr = nullptr;
        int64_t content_len = 0;
        bool flag = false;
        ossdatas[i].is_handling = true;

        bool ret = cloud_handle_ptr->download_files_to_memory(iter.filename, &content_ptr, content_len);
        if (ret)
        {
            for (int j=0; j< content_len; j++)
            {
                if (content_ptr[j] == 0)
                {
                    flag = true;
                    content_ptr[j] = 0x0A;
                }
            }
            content = content_ptr;
            if (flag)
            {
                std::cout << iter.filename << std::endl;
            }
            takeoverlist.clear();

            if (ret)
            {
                for (auto iter1 : takeoverlist)
                {
                    std::string strdatetime;
                    csv.setNumber(static_cast<uint32_t>(index), 1, index);
                    csv.setString(static_cast<uint32_t>(index), 2, vehicleid.toStdString());
                    date_to_string(iter1.time, strdatetime, DateStringType_MIN);
                    csv.setString(static_cast<uint32_t>(index), 3, strdatetime);
                    csv.setString(static_cast<uint32_t>(index), 4, "");
                    csv.setString(static_cast<uint32_t>(index), 5, "");
                    csv.setString(static_cast<uint32_t>(index), 6, "");
                    csv.setString(static_cast<uint32_t>(index), 7, "");
                    if (iter1.takeover == 2)
                    {
                        csv.setString(static_cast<uint32_t>(index), 8, "人工接管");
                    } else if(iter1.takeover == 3)
                    {
                        csv.setString(static_cast<uint32_t>(index), 8, "系统提示接管");
                    }
                    index++;
                }
            }
            ossdatas[i].handle_status = true;
            delete[] content_ptr;
        } else {
            ossdatas[i].handle_status = false;
        }
        i++;
    }
    csv.save(csv_filename_);

    ui->btn_handle->setText("接管信息统计");
    ui->btn_handle->setEnabled(true);

    return ;
#if 0
    for (auto iter : ossdatas)
    {
        std::string content;
        ossdatas[i].is_handling = true;
        bool ret = cloud_handle_ptr->download_files_to_memory(iter.filename, content);
        if (ret)
        {
            takeoverlist.clear();
            ret = dp.Process(content, takeoverlist);
            if (ret)
            {
                for (auto iter : takeoverlist)
                {
                    std::string strdatetime;
                    csv.setNumber(static_cast<uint32_t>(index), 1, index);
                    csv.setString(static_cast<uint32_t>(index), 2, vehicleid.toStdString());
                    date_to_string(iter.time, strdatetime, DateStringType_MIN);
                    csv.setString(static_cast<uint32_t>(index), 3, strdatetime);
                    csv.setString(static_cast<uint32_t>(index), 4, "");
                    csv.setString(static_cast<uint32_t>(index), 5, "");
                    csv.setString(static_cast<uint32_t>(index), 6, "");
                    csv.setString(static_cast<uint32_t>(index), 7, "");
                    if (iter.takeover == 2)
                    {
                        csv.setString(static_cast<uint32_t>(index), 8, "人工接管");
                    } else if(iter.takeover == 3)
                    {
                        csv.setString(static_cast<uint32_t>(index), 8, "系统提示接管");
                    }
                    index++;
                }
            }
            ossdatas[i].handle_status = true;
        } else {
            ossdatas[i].handle_status = false;
        }
        i++;
    }
//    std::cout << "csv_filename_:" << csv_filename_ << std::endl;
    csv.save(csv_filename_);
//

    ui->btn_handle->setText("接管信息统计");
    ui->btn_handle->setEnabled(true);
#endif
}

void frmAptest::on_cbx_vehicleid_currentIndexChanged(const QString &strVehicleId)
{
    QString strDateTime;
    QString vehicleid = ui->cbx_vehicleid->currentText();
    QString strSearchDir = vehicleid + "/";
    QString strType = ui->cbx_frametype->currentText();

    int year = ui->cboxYear->currentData().toInt();
    int month = ui->cboxMonth->currentData().toInt();
    QString stryear = QString("%1").arg(year);
    QString strmonth = QString("%1").arg(month, 2, 10, QLatin1Char('0'));
    strDateTime = stryear + "-" + strmonth;

    if (strVehicleId == "")
    {

    }

    for (std::string::size_type i =0; i < ossdatas.size(); i++)
    {
        ossdatas[i].download_status = false;
    }

    UpdateOSSView(strSearchDir, strDateTime, strType);
}

void frmAptest::on_btn_downloadall_clicked()
{
    std::string local_filename;
    std::string remote_filename;
    std::string vehiclename;
    QString savepath = "D:/";

    QString vehicldid = ui->cbx_vehicleid->currentText();

    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), savepath,
                                         QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if (path.isEmpty())
    {
        return;
    }

    local_filename = path.toStdString();
    vehiclename = vehicldid.toStdString();

    ui->btn_downloadall->setText("下载中...");
    ui->btn_downloadall->setChecked(true);


    for (std::string::size_type index=0; index < ossdatas.size(); index++)
    {
        std::string remote_directory = ossdatas[index].filename;

        std::size_t pos = ossdatas[index].filename.find("/", 0);
        local_filename = path.toStdString() + "/" + ossdatas[index].filename.substr(pos + 1);

        ossdatas[index].is_downloading = true;

        std::thread([this,index,  local_filename, remote_directory]()
        {
            //CloudData *cloud_handle_ptr = CloudData::Instance();

            if (cloud_handle_ptr->download_files(remote_directory, local_filename))
            {
                ossdatas[index].download_status = 1;
            } else {
                ossdatas[index].download_status = 1;
            }
        }).detach();
    }
}

void frmAptest::on_cboxYear_currentIndexChanged(const QString &arg1)
{
    int year = 0;
    int month = 0;
    QString stryear, strmonth, strDateTime, strDataType;
    QString vehicleid = ui->cbx_vehicleid->currentText();
    QString strSearchDir = vehicleid + "/";

    if (arg1 == "")
    {

    }

    year = ui->cboxYear->currentData().toInt();
    month = ui->cboxMonth->currentData().toInt();
    stryear = QString("%1").arg(year);
    strmonth = QString("%1").arg(month, 2, 10, QLatin1Char('0'));
    strDateTime = stryear + "-" + strmonth;

    strDataType = ui->cbx_frametype->currentText();
    cbox_year  = year;

    UpdateOSSView(strSearchDir, strDateTime, strDataType);
}

void frmAptest::on_cboxMonth_currentIndexChanged(const QString &arg1)
{
    int year = 0;
    int month = 0;
    QString stryear, strmonth, strDateTime, strDataType;
    QString vehicleid = ui->cbx_vehicleid->currentText();
    QString strSearchDir = vehicleid + "/";

    if (arg1 == "")
    {

    }

    year = ui->cboxYear->currentData().toInt();
    month = ui->cboxMonth->currentData().toInt();
    stryear = QString("%1").arg(year);
    strmonth = QString("%1").arg(month, 2, 10, QLatin1Char('0'));
    strDateTime = stryear + "-" + strmonth;

    strDataType = ui->cbx_frametype->currentText();
    cbox_month  = month;

    UpdateOSSView(strSearchDir, strDateTime, strDataType);
}

void frmAptest::on_tableWidget_cellEntered(int row, int column)
{
    if (row)
    {

    }

    if (column)
    {

    }
    //for (int index =0; index <= 7; index++)
    {
        //ui->tableWidget->setRowColor(row, QColor(193,210,240));
    }
}

void frmAptest::slot_btn_gather_down()
{
//    timer_update_gadata_ptr->stop();
}

void frmAptest::slot_btn_view_down()
{
//    if(!timer_update_gadata_ptr->isActive())
//    {
//        timer_update_gadata_ptr->start();
//    }

}
