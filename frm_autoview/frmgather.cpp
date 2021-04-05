#include <QFontDatabase>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QDir>
#include <QFileDialog>

#include <thread>


#include "frmgather.h"
#include "ui_frmgather.h"

#include <QDebug>


#if 0
static void date_to_string(unix_date_time tmdate, std::string &strdate, DateStringType_en type)
{
    char temp[20] = {0};
    switch (type)
    {
    // 2020/06/08
    case DateStringType_en::DateStringType_DAY:
        strdate = std::to_string(tmdate.year) + '/' + std::to_string(tmdate.mon) + '/' + std::to_string(tmdate.day);
        break;
    // 2020-06-08-21
    case DateStringType_en::DateStringType_HOUR:
        strdate = std::to_string(tmdate.year) + '-' + std::to_string(tmdate.mon) + '-' + std::to_string(tmdate.day) + '-' + std::to_string(tmdate.min);
        break;
    // 2020.06.08 21:30
    case DateStringType_en::DateStringType_MIN:
        sprintf(temp, "%d.%02d.%02d %02d:%02d", tmdate.year, tmdate.mon, tmdate.day, tmdate.hour, tmdate.min);
        strdate = temp;
//        strdate = std::to_string(tmdate.year) + '.' + std::to_string(tmdate.mon) + '.' + std::to_string(tmdate.day) + ' ' + std::to_string(tmdate.hour) + ':' + std::to_string(tmdate.min);
        break;
    // 2020-06-08-21-30-30
    case DateStringType_en::DateStringType_SECOND:
        strdate = std::to_string(tmdate.year) + '-' + std::to_string(tmdate.mon) + '-' + std::to_string(tmdate.day) + '-' + std::to_string(tmdate.hour) + '-' + std::to_string(tmdate.min) + '-' + std::to_string(tmdate.sec);
        break;
    default:
        break;
    }
}

static void split_to_string(const std::string str, const std::string delim, std::vector<std::string> &strelems)
{
    strelems.clear();
    size_t pos = 0;
    size_t len = str.length();
    size_t delim_len = delim.length();
    if (delim_len == 0)
    {
        return ;
    }

    while (pos < len)
    {
        int find_pos = str.find(delim, pos);
        if (find_pos < 0)
        {
            strelems.push_back(str.substr(pos, len - pos));
            break;
        }
        strelems.push_back(str.substr(pos, find_pos - pos));
        pos = find_pos + delim_len;
    }
}

static std::string& replace_str(std::string &str,const std::string &old_value,const std::string &new_value)
{
    while(true)
    {
        std::string::size_type pos(0);
        if((pos=str.find(old_value))!= std::string::npos)
        {
            str.replace(pos,old_value.length(),new_value);
        } else {
            break;
        }
    }

    return str;
}
#endif
static void unique_datestr(const std::map<std::string, std::vector<std::array<OSSData_st, 3>>> &ossdata_map, std::map<std::string, bool> &date_map)
{
    for (auto &iter : ossdata_map)
    {
        for (auto iter1 : iter.second)
        {
            char temp[15] = {0};
            std::string date_str;
            sprintf(temp, "%04d/%02d/%02d", std::get<0>(iter1).filedate.year, std::get<0>(iter1).filedate.mon, std::get<0>(iter1).filedate.day);
            date_str = temp;
            date_map[date_str] = true;
        }
    }
}

frmGather::frmGather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmGather)
{
    ui->setupUi(this);

    handle_status = 0;

    cloud_handle_ptr = CloudData::Instance();

    InitFont();

    InitForm();

    InitStyle();

    InitData();
}

frmGather::~frmGather()
{
    delete ui;

    for (int index = 0; index < 30; index++)
    {
        delete weather_widget_vec[index].label_num_ptr;
        delete weather_widget_vec[index].label_datastr_ptr;
        delete weather_widget_vec[index].lineedit_weather_ptr;
    }

    for (int index = 0; index < 30; index++)
    {
        delete vehicle_progress_widget_vec[index].label_num_ptr;
        delete vehicle_progress_widget_vec[index].label_vehiclename_ptr;
        delete vehicle_progress_widget_vec[index].progress_handle_ptr;
    }
}

void frmGather::update_process_mode(bool mode)
{
    //handle_mode = mode;
}

void frmGather::set_handle_mode(bool mode)
{
    //handle_mode = mode;
}

void frmGather::InitFont()
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

void frmGather::InitForm()
{
    ui->cbx_frametype->setEnabled(false);
    ui->cbx_vehicleid->setEnabled(false);

    ui->checkBox_Foton_3051->setChecked(true);
    ui->checkBox_Foton_3052->setChecked(true);
    ui->checkBox_Foton_3053->setChecked(true);
    ui->checkBox_Foton_3054->setChecked(true);
    ui->checkBox_SQ_2012->setChecked(true);

    vehicle_vec.push_back("1");
    vehicle_vec.push_back("2");
    vehicle_vec.push_back("3");
    vehicle_vec.push_back("4");

    ui->btn_downloadall->setCheckable(true);
    ui->btn_handle->setCheckable(true);

    ui->widget_progress->setValue(0);

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
        Date_Weather_Widget_st widget;

        widget.label_num_ptr = new QLabel("test");
        widget.label_datastr_ptr = new QLabel("test");

        widget.lineedit_weather_ptr = new QLineEdit();
        widget.lineedit_weather_ptr->setObjectName(QString("lineedit_weather%1").arg(index));
//        widget.lineedit_weather_ptr->addItem("晴");
//        widget.lineedit_weather_ptr->addItem("阴");
//        widget.lineedit_weather_ptr->addItem("多云");
//        widget.lineedit_weather_ptr->addItem("小雨");
//        widget.lineedit_weather_ptr->addItem("中雨");
//        widget.lineedit_weather_ptr->addItem("大雨");
//        widget.lineedit_weather_ptr->addItem("阴转小雨");
//        widget.lineedit_weather_ptr->addItem("多云转阴");
//        widget.lineedit_weather_ptr->addItem("雷阵雨");
//        widget.lineedit_weather_ptr->addItem("冰雹");
//        widget.lineedit_weather_ptr->addItem("小雪");
//        widget.lineedit_weather_ptr->addItem("中雪");
//        widget.lineedit_weather_ptr->addItem("大雪");


        connect(widget.lineedit_weather_ptr, SIGNAL(textChanged(const QString &)), this, SLOT(slot_cbx_indexchanged(const QString &)));
//        connect(widget.btn_handle_ptr, SIGNAL(clicked()), this, SLOT(slot_btn_handle_clicked()));
        weather_widget_vec.push_back(widget);
    }

    for (int index = 0; index < 32; index++)
    {
        Vehicle_Progress_Widget_st widget;

        widget.label_num_ptr = new QLabel("test");
        widget.label_vehiclename_ptr = new QLabel("test");

        widget.progress_handle_ptr = new QProgressBar();


//        connect(widget.btn_download_ptr, SIGNAL(clicked()), this, SLOT(slot_btn_clicked()));
//        connect(widget.btn_handle_ptr, SIGNAL(clicked()), this, SLOT(slot_btn_handle_clicked()));
        vehicle_progress_widget_vec.push_back(widget);
    }


    //int cloumwidth = 0;
    ui->tableWidget->setRowCount(32);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置

    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("日期")<<tr("天气信息")<<tr("")<<tr("日期")<<tr("天气信息"));
    ui->tableWidget->verticalHeader()->setHidden(true);
//    cloumwidth = ui->tableWidget->frameWidth();
    ui->tableWidget->setColumnWidth(0, 80);
    ui->tableWidget->setColumnWidth(1, 100);
    ui->tableWidget->setColumnWidth(2, 50);
    ui->tableWidget->setColumnWidth(3, 80);
    ui->tableWidget->setColumnWidth(4, 100);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->hide();

    ui->tableWidget_result->setRowCount(32);
    ui->tableWidget_result->setColumnCount(3);
    ui->tableWidget_result->setHorizontalHeaderLabels(QStringList()<<tr("序号")<<tr("车辆名称")<<tr("分析进度"));
    ui->tableWidget_result->verticalHeader()->setHidden(true);
//    cloumwidth = ui->tableWidget_result->frameWidth();
    ui->tableWidget_result->setColumnWidth(0, 50);
    ui->tableWidget_result->setColumnWidth(1, 100);
    ui->tableWidget_result->setColumnWidth(2, 160);
    ui->tableWidget_result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_result->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_result->verticalHeader()->hide();
}



void frmGather::test()
{
    //qDebug() << "btn clicked." ;
}

void frmGather::InitStyle()
{

}

void frmGather::InitData()
{
    QDir dir;
//    QString vehicleid;
    progress_value = 0;


    app_path_ = QCoreApplication::applicationDirPath();
    dir.setPath(app_path_ + "/temp");
    if (!dir.exists())
    {
        dir.mkdir(app_path_ + "/temp");
    }



    date = QDate::currentDate();

    cbox_year = date.year();
    cbox_month = date.month();

    handle_mode = false;

    //设置为今天,设置变量防止重复触发
    ui->cboxYear->setCurrentIndex(ui->cboxYear->findText(QString("%1年").arg(cbox_year)));
    ui->cboxMonth->setCurrentIndex(ui->cboxMonth->findText(QString("%1月").arg(cbox_month)));

    ui->cbx_frametype->addItem("data1");
    ui->cbx_frametype->addItem("data2");
    ui->cbx_frametype->addItem("data3");

    ui->cbx_vehicleid->addItem("temp");

//    vehicleid = ui->cbx_vehicleid->currentText();

//    Query_vehicle_ossdata(date.year(), date.month());
//
//    unique_datestr(ossdata_map, date_map);
//    osswidgets[0].progress_download_ptr->setValue(20);
//    osswidgets[0].progress_handle_ptr->setValue(20);

    UpdateTableWidget(date.year(), date.month());

    UpdateTableWidget();

    timer_ptr = new QTimer(this);
    connect(timer_ptr, SIGNAL(timeout()), this, SLOT(slot_timeout()));
    timer_ptr->start(1000);
}

bool frmGather::Query_Vehicle_OSSData(int year, int month)
{
    //CloudData *cloud_handle_ptr = CloudData::Instance();

    if (cloud_handle_ptr)
    {
        char temp[8] = {0};

        std::string strtime;
        sprintf(temp, "%04d-%02d", year, month);
        strtime = temp;

        ossdata_map.clear();

        for (auto iter : vehicle_vec)
        {
            bool ret = false;
            std::vector<std::array<OSSData_st, 3>> data_vec;
            std::string csv_serach_directory = iter + "/";
            std::vector<ALIBABACLOUD_HANDLE::file_detail_st> csv_filedetails;

            //csv files
            ret = cloud_handle_ptr->query_directory(csv_serach_directory, csv_filedetails);
            if (!ret)
            {
                return false;
            }

            //asc files
//            ret = cloud_handle_ptr->query_directory(asc_serach_directory, asc_filedeatiles);
//            if (!ret)
//            {
//                return false;
//            }
//            oss_mutex_.lock();
            //ossdatas.clear();

            for (size_t i=0; i< csv_filedetails.size(); i++)
            {
                if (csv_filedetails[i].name.find(".csv") != std::string::npos && \
                    csv_filedetails[i].name.find(strtime) != std::string::npos)
                {
                    char temp[50] = {0};
                    unix_date_time time;
                    OSSData_st csv_data, sh_asc_data, ga_asc_data;

                    sscanf(csv_filedetails[i].date.c_str(), "%hu-%hhu-%hhuT%hhu:%hhu:%hhu.%uZ", \
                                                &time.year, &time.mon, &time.day, &time.hour, \
                                                &time.min, &time.sec, &time.msec);
                    csv_data.filename = csv_filedetails[i].name;
                    csv_data.filesize = csv_filedetails[i].size;
                    csv_data.filedate = time;

                    sprintf(temp, "%s/asc/SH_%04d-%02d-%02d.asc", iter.c_str(), time.year, time.mon, time.day);
                    sh_asc_data.filename = temp;
                    sh_asc_data.filesize = csv_filedetails[i].size;
                    sh_asc_data.filedate = time;

                    memset(temp, 0x00, sizeof(temp));
                    sprintf(temp, "%s/asc/GA_%04d-%02d-%02d.asc", iter.c_str(), time.year, time.mon, time.day);
                    ga_asc_data.filename = temp;
                    ga_asc_data.filesize = csv_filedetails[i].size;
                    ga_asc_data.filedate = time;
                    data_vec.push_back({csv_data, sh_asc_data, ga_asc_data});

                }
            }
            ossdata_map.insert(std::pair<std::string, std::vector<std::array<OSSData_st, 3>>>(iter, data_vec));
        }


//        oss_mutex_.unlock();
//        UpdateTableWidget(1);
    } else {
        return false;
    }

    return true;
}

// type :{1: sh and ga 2: sh 3: ga}
bool frmGather::Query_Vehicle_Data(int year, int month, int type, std::map<std::string, std::vector<OSSData_st>> &oss_data_map)
{
    //CloudData *cloud_handle_ptr = CloudData::Instance();

    if (cloud_handle_ptr)
    {
        char temp[8] = {0};

        std::string strtime;
        sprintf(temp, "%04d-%02d", year, month);
        strtime = temp;

        ossdata_map.clear();

        for (auto iter : vehicle_vec)
        {
            std::vector<OSSData_st> data_vec;
            std::string serach_directory = iter + "/";
            std::vector<ALIBABACLOUD_HANDLE::file_detail_st> filedetails;

            if((type ==2) || (type == 3))
            {
                serach_directory += "asc/";
            }

            bool ret = cloud_handle_ptr->query_directory(serach_directory, filedetails);
            if (!ret)
            {
                return false;
            }
//            oss_mutex_.lock();
            //ossdatas.clear();
            for (size_t i=0; i< filedetails.size(); i++)
            {
                bool result = false;

                if (type == 1)
                {
                   if (filedetails[i].name.find(".csv") != std::string::npos && \
                       filedetails[i].name.find(strtime) != std::string::npos \
                       )
                   {
                        result = true;
                   }
                }
                else if (type == 2)
                {
                    if (filedetails[i].name.find("SH_") != std::string::npos &&\
                        filedetails[i].name.find(".asc") != std::string::npos && \
                        filedetails[i].name.find(strtime, 0) != std::string::npos \
                       )
                    {
                        result = true;
                    }
                }
                else if (type == 3)
                {
                    if (filedetails[i].name.find("GA_") != std::string::npos && \
                        filedetails[i].name.find(".asc") != std::string::npos && \
                        filedetails[i].name.find(strtime, 0) != std::string::npos \
                       )
                    {
                        result = true;
                    }
                }

                if (result)
                {
                    unix_date_time time;
                    OSSData_st data;
                    data.filename = filedetails[i].name;
                    data.filesize = filedetails[i].size;

                    sscanf(filedetails[i].date.c_str(), "%hu-%hhu-%hhuT%hhu:%hhu:%hhu.%uZ", \
                                                &time.year, &time.mon, &time.day, &time.hour, \
                                                &time.min, &time.sec, &time.msec);
                    data.filedate = time;

                    data_vec.push_back(data);
                }

            }

            oss_data_map.insert(std::pair<std::string, std::vector<OSSData_st>>(iter, data_vec));
        }


//        oss_mutex_.unlock();
//        UpdateTableWidget(1);
    } else {
        return false;
    }

    return true;
}

void frmGather::slot_timeout()
{
    ui->widget_progress->setValue(progress_value);

    if (progress_value >= 100)
    {
        ui->btn_handle->setEnabled(true);
        ui->btn_downloadall->setEnabled(true);
        ui->btn_handle->setChecked(false);
        ui->btn_downloadall->setChecked(false);

        ui->btn_handle->setText("淞泓数据汇总");
        ui->btn_downloadall->setText("公安数据汇总");
    }

    if (handle_status < 0)
    {

        ui->widget_progress->setTextColor(Qt::red);
        ui->widget_progress->setPieColorStart(Qt::red);

        ui->widget_progress->setValue(100);
    }
#if 0
    int i = 0;
    for (i=0; i < ossdatas.size(); i++)
    {
        if (ossdatas[i].is_downloading == 1)
        {
            break;
        }

    }
    if (i < ossdatas.size())
    {
        if (ossdatas[i].download_status == 1)
        {
            osswidgets[i].progress_download_ptr->setValue(cloud_handle_ptr->get_download_percent());
            if (cloud_handle_ptr->get_download_percent() >= 100)
            {
                ossdatas[i].is_downloading = false;
            }

            if((i == (ossdatas.size() - 1)) && ui->btn_downloadall->isChecked())
            {
                ui->btn_downloadall->setText("下载完毕");
            }
        } else if (ossdatas[i].download_status == 2) {
            ossdatas[i].is_downloading = false;
            ossdatas[i].download_status = 0;
            osswidgets[i].progress_download_ptr->setValue(100);
            osswidgets[i].progress_download_ptr->setStyleSheet("QProgressBar::chunk{background:red}");
        }
    }

    for (i=0; i < ossdatas.size(); i++)
    {
        if (ossdatas[i].is_handling)
        {
            //std::cout << i << " is_handling" << std::endl;
            break;
        }
    }

    if (i < ossdatas.size())
    {
        if (ossdatas[i].handle_status == 1)
        {
            osswidgets[i].progress_handle_ptr->setValue(cloud_handle_ptr->get_download_percent());
            if (cloud_handle_ptr->get_download_percent() >= 100)
            {
//                std::cout << i << " is_handling ok" << std::endl;
                ossdatas[i].is_handling = false;
            }
            ossdatas[i].handle_status = 0;

            if((i == (ossdatas.size() - 1)) && ui->btn_handle->isChecked())
            {
                ui->btn_handle->setText("分析完毕");
            }
        } else if (ossdatas[i].handle_status == 2) {
            ossdatas[i].is_handling = false;
            ossdatas[i].handle_status = 0;
            osswidgets[i].progress_handle_ptr->setValue(100);
            osswidgets[i].progress_handle_ptr->setStyleSheet("QProgressBar::chunk{background:red}");
        }

    }
#endif
}

void frmGather::UpdateTableWidget(int year, int month)
{
    size_t index = 0;

    date_map.clear();

    Query_Vehicle_OSSData(year, month);

    unique_datestr(ossdata_map, date_map);

//    ui->tableWidget->clearContents();
    for (index = 0; index < weather_vec.size(); index++)
    {
        ui->tableWidget->removeCellWidget(static_cast<int>(index), 0);
        ui->tableWidget->removeCellWidget(static_cast<int>(index), 1);
        ui->tableWidget->removeCellWidget(static_cast<int>(index), 2);

        ui->tableWidget->removeCellWidget(static_cast<int>(index), 3);
        ui->tableWidget->removeCellWidget(static_cast<int>(index), 4);
    }

    for (auto iter : weather_widget_vec)
    {
        delete iter.label_num_ptr;
        delete iter.label_datastr_ptr;
        delete iter.lineedit_weather_ptr;
    }

//    delete layout;
//    delete lineedit_weather_widget_ptr;

    weather_vec.clear();
    weather_widget_vec.clear();




    oss_mutex_.lock();
//    for (auto &iter : date_map)
//    {
//        Date_Weather_st weather;

//        weather.seqnum = index++;
//        weather.date_str = QString::fromStdString(iter.first);
//        weather.weather_str = "晴";

//        weather_vec.push_back(weather);
//    }
    int days = get_month_days(year, month);

    for (int index1 = 0; index1 < days; index1++)
    {
        Date_Weather_Widget_st widget;

        widget.label_num_ptr = new QLabel("test");
        widget.label_datastr_ptr = new QLabel("test");

        widget.lineedit_weather_ptr = new QLineEdit();
        widget.lineedit_weather_ptr->setObjectName(QString("lineedit_weather%1").arg(index1));
//        widget.lineedit_weather_ptr->addItem("晴");
//        widget.lineedit_weather_ptr->addItem("阴");
//        widget.lineedit_weather_ptr->addItem("多云");
//        widget.lineedit_weather_ptr->addItem("小雨");
//        widget.lineedit_weather_ptr->addItem("中雨");
//        widget.lineedit_weather_ptr->addItem("大雨");
//        widget.lineedit_weather_ptr->addItem("阴转小雨");
//        widget.lineedit_weather_ptr->addItem("多云转阴");
//        widget.lineedit_weather_ptr->addItem("雷阵雨");
//        widget.lineedit_weather_ptr->addItem("冰雹");
//        widget.lineedit_weather_ptr->addItem("小雪");
//        widget.lineedit_weather_ptr->addItem("中雪");
//        widget.lineedit_weather_ptr->addItem("大雪");


        connect(widget.lineedit_weather_ptr, SIGNAL(textChanged(const QString &)), this, SLOT(slot_cbx_indexchanged(const QString &)));
//        connect(widget.btn_handle_ptr, SIGNAL(clicked()), this, SLOT(slot_btn_handle_clicked()));
        weather_widget_vec.push_back(widget);
    }

    for (int index1 = 0; index1 < days; index1++)
    {
        Date_Weather_st weather;
        char temp[15] = {0};
        std::string date_str;

        sprintf(temp, "%04d/%02d/%02d", year, month, index1+1);
        date_str = temp;

        weather.seqnum = index1;

        weather.date_str = QString::fromStdString(date_str);
        weather.weather_str = "未知";

        weather_vec.push_back(weather);
    }

    oss_mutex_.unlock();

    int index1 = 0;
    bool step = false;

    for (index = 0; index < weather_vec.size(); index++)
    {
        lineedit_weather_widget_ptr = new QWidget;
        layout = new QHBoxLayout;
        layout->setSpacing(5);
        layout->setMargin(0);
        layout->addWidget(weather_widget_vec[static_cast<int>(index)].lineedit_weather_ptr);
        lineedit_weather_widget_ptr->setLayout(layout);

        weather_widget_vec[static_cast<int>(index)].label_num_ptr->setText(QString("%1").arg(static_cast<int>(index)));
        weather_widget_vec[static_cast<int>(index)].label_num_ptr->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        weather_widget_vec[static_cast<int>(index)].label_datastr_ptr->setText(weather_vec[index].date_str);
        weather_widget_vec[static_cast<int>(index)].label_datastr_ptr->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        weather_widget_vec[static_cast<int>(index)].lineedit_weather_ptr->setText("未知");
//        weather_widget_vec[static_cast<int>(index)].lineedit_weather_ptr->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//        ui->tableWidget->setCellWidget(static_cast<int>(index), 0, weather_widget_vec[static_cast<int>(index)].label_num_ptr);
        if (step == false)
        {
            ui->tableWidget->setCellWidget(index1, 0, weather_widget_vec[static_cast<int>(index)].label_datastr_ptr);
            ui->tableWidget->setCellWidget(index1, 1, lineedit_weather_widget_ptr);
            step = true;
        } else {
            ui->tableWidget->setCellWidget(index1, 3, weather_widget_vec[static_cast<int>(index)].label_datastr_ptr);
            ui->tableWidget->setCellWidget(index1, 4, lineedit_weather_widget_ptr);

            step = false;
            index1++;
        }
    }

//    oss_mutex_.unlock();

}

void frmGather::UpdateTableWidget()
{
    size_t index = 0;

//    ui->tableWidget_result->clearContents();

    for (index = 0; index < weather_vec.size(); index++)
    {
        ui->tableWidget_result->removeCellWidget(static_cast<int>(index), 0);
        ui->tableWidget_result->removeCellWidget(static_cast<int>(index), 1);
        ui->tableWidget_result->removeCellWidget(static_cast<int>(index), 2);
    }

    for (auto iter : vehicle_progress_widget_vec)
    {
        delete iter.label_num_ptr;
        delete iter.label_vehiclename_ptr;
        delete iter.progress_handle_ptr;
    }

    vehicle_progress_vec.clear();
    vehicle_progress_widget_vec.clear();

    oss_mutex_.lock();
    for (auto &iter : ossdata_map)
    {
        Vehicle_Progress_st vehicle_progress;

        vehicle_progress.seqnum = static_cast<int>(index++);
        vehicle_progress.vehiclename = iter.first;
        vehicle_progress.is_handling = false;
        vehicle_progress.handle_status = 0;

        vehicle_progress_vec.push_back(vehicle_progress);
    }


    oss_mutex_.unlock();


    for (index = 0; index < 30; index++)
    {
        Vehicle_Progress_Widget_st widget;

        widget.label_num_ptr = new QLabel("test");
        widget.label_vehiclename_ptr = new QLabel("test");

        widget.progress_handle_ptr = new QProgressBar();


//        connect(widget.btn_download_ptr, SIGNAL(clicked()), this, SLOT(slot_btn_clicked()));
//        connect(widget.btn_handle_ptr, SIGNAL(clicked()), this, SLOT(slot_btn_handle_clicked()));
        vehicle_progress_widget_vec.push_back(widget);
    }

    index = 0;
    for (auto iter = vehicle_progress_vec.begin(); iter != vehicle_progress_vec.end(); iter++, index++)
    {
        vehicle_progress_widget_vec[static_cast<int>(index)].label_num_ptr->setText(QString("%1").arg(index));
        vehicle_progress_widget_vec[static_cast<int>(index)].label_num_ptr->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        vehicle_progress_widget_vec[static_cast<int>(index)].label_vehiclename_ptr->setText(QString::fromStdString(iter->vehiclename));
        vehicle_progress_widget_vec[static_cast<int>(index)].label_vehiclename_ptr->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//        weather_widget_vec[static_cast<int>(index)].lineedit_weather_ptr->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);


        if (iter->handle_status == 1)
        {
            vehicle_progress_widget_vec[static_cast<int>(index)].progress_handle_ptr->setValue(100);
        } else if (iter->handle_status == 2) {
            vehicle_progress_widget_vec[static_cast<int>(index)].progress_handle_ptr->setValue(0);
//            vehicle_progress_widget_vec[static_cast<int>(index)].progress_handle_ptr->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
        } else {
            vehicle_progress_widget_vec[static_cast<int>(index)].progress_handle_ptr->setValue(0);
        }


        ui->tableWidget_result->setCellWidget(static_cast<int>(index), 0, vehicle_progress_widget_vec[static_cast<int>(index)].label_num_ptr);
        ui->tableWidget_result->setCellWidget(static_cast<int>(index), 1, vehicle_progress_widget_vec[static_cast<int>(index)].label_vehiclename_ptr);

        ui->tableWidget_result->setCellWidget(static_cast<int>(index), 2, vehicle_progress_widget_vec[static_cast<int>(index)].progress_handle_ptr);

    }

//    oss_mutex_.unlock();

}

void frmGather::slot_btn_clicked()
{

}

void frmGather::handle_asc_file(std::string localfilename, std::string remotefilename)
{
    if (localfilename == "")
    {

    }

    if (remotefilename == "")
    {

    }
}

void frmGather::slot_cbx_indexchanged(const QString &str)
{
    oss_mutex_.lock();
    QLineEdit* lineedit_ptr = qobject_cast<QLineEdit*>(sender());

    if (lineedit_ptr->objectName() == "lineedit_weather0")
    {
        weather_vec[0].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather1") {
        weather_vec[1].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather2") {
        weather_vec[2].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather3") {
        weather_vec[3].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather4") {
        weather_vec[4].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather5") {
        weather_vec[5].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather6") {
        weather_vec[6].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather7") {
        weather_vec[7].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather8") {
        weather_vec[8].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather9") {
        weather_vec[9].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather10") {
        weather_vec[10].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather11") {
        weather_vec[11].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather12") {
        weather_vec[12].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather13") {
        weather_vec[13].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather14") {
        weather_vec[14].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather15") {
        weather_vec[15].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather16") {
        weather_vec[16].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather17") {
        weather_vec[17].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather18") {
        weather_vec[18].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather19") {
        weather_vec[19].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather20") {
        weather_vec[20].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather21") {
        weather_vec[21].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather22") {
        weather_vec[22].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather23") {
        weather_vec[23].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather24") {
        weather_vec[24].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather25") {
        weather_vec[25].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather26") {
        weather_vec[26].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather27") {
        weather_vec[27].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather28") {
        weather_vec[28].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather29") {
        weather_vec[29].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather30") {
        weather_vec[30].weather_str = str;
    } else if (lineedit_ptr->objectName() == "lineedit_weather31"){
        weather_vec[31].weather_str = str;
    }
    oss_mutex_.unlock();
}

void frmGather::slot_btn_handle_clicked()
{

}
//char* frmGather::ConvertUtcToLocalTime(struct tm* t2,const char* date){

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
// if(t2 == NULL){
//  t2 = &t;
// }
// *t2 = *localtime(&tt);
// char* ds = (char*) malloc(24);
// memset(ds,24);
// sprintf(ds,"%.4d-%.2d-%.2d %.2d:%.2d:%.2d",t2->tm_year + 1900,t2->tm_mon + 1,t2->tm_mday,t2->tm_hour,t2->tm_min,t2->tm_sec);
// return ds;
// }
// return NULL;
//}

void frmGather::UpdateOSSView(const QString &strSearchDir,const QString &strDateTime, const QString &strDataType)
{
    if (strSearchDir == "")
    {

    }
    if (strDateTime == "")
    {

    }

    if (strDataType == "")
    {

    }
#if 0
    int type = 0;
    std::string strtime;

    strtime = strDateTime.toStdString();

    if ("淞泓公安监管数据" == strDataType)
    {
        type = 1;
        ui->btn_handle->setEnabled(false);
        ui->btn_downloadall->setEnabled(true);
        ui->btn_downloadall->setChecked(false);
    }
    else if ("淞泓原始数据" == strDataType)
    {
        type = 2;
        ui->btn_handle->setEnabled(true);
        ui->btn_downloadall->setEnabled(false);
        ui->btn_downloadall->setChecked(false);
    }
    else if ("公安原始数据" == strDataType)
    {
        type = 3;
        ui->btn_handle->setEnabled(true);
        ui->btn_downloadall->setEnabled(false);
        ui->btn_downloadall->setChecked(false);
    }



    if (cloud_handle_ptr)
    {
        bool  ret = false;
        std::string strDir = strSearchDir.toStdString();
        std::vector<ALIBABACLOUD_HANDLE::file_detail_st> filedetails;

        if((type ==2) || (type == 3))
        {
            strDir += "asc/";
        }
        ret = cloud_handle_ptr->query_directory(strDir, filedetails);


        if (ret)
        {
            int index = 0;
            oss_mutex_.lock();
            ossdatas.clear();

            for (int i=0; i< filedetails.size(); i++)
            {
                bool result = false;
                if (type == 1)
                {
                   if (filedetails[i].name.find(".csv") != std::string::npos && \
                       filedetails[i].name.find(strtime) != std::string::npos \
                       )
                   {
                        result = true;
                   }
                }
                else if (type == 2)
                {
                    if (filedetails[i].name.find("SH_") != std::string::npos &&\
                        filedetails[i].name.find(".asc") != std::string::npos && \
                        filedetails[i].name.find(strtime, 0) != std::string::npos \
                       )
                    {
                        result = true;
                    }
                }
                else if (type == 3)
                {
                    if (filedetails[i].name.find("GA_") != std::string::npos && \
                        filedetails[i].name.find(".asc") != std::string::npos && \
                        filedetails[i].name.find(strtime, 0) != std::string::npos \
                       )
                    {
                        result = true;
                    }
                }
                if (result)
                {
                    char time_buffer[50] = {0};
                    unix_date_time time;
                    OSSData data;
                    data.seqnum = index++;
                    data.filename = filedetails[i].name;
                    data.filesize = filedetails[i].size;

                    sscanf(filedetails[i].date.c_str(), "%d-%d-%dT%d:%d:%d.%dZ", \
                                                &time.year, &time.mon, &time.day, &time.hour, \
                                                &time.min, &time.sec, &time.msec);
                    sprintf(time_buffer, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", time.year, \
                                            time.mon, time.day, (time.hour+8), time.min, time.sec);
                    data.filedate = time_buffer;
                    data.download_status = 0;
                    data.is_downloading = false;
                    data.handle_status = 0;
                    data.is_handling = false;
                    ossdatas.push_back(data);
                }

            }
            oss_mutex_.unlock();
            UpdateTableWidget(type);
        }

    }
#endif
}


void frmGather::on_btnPrevYear_clicked()
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


    UpdateTableWidget(year, month);

    UpdateTableWidget();

}

void frmGather::on_btnNextYear_clicked()
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

    UpdateTableWidget(year, month);

    UpdateTableWidget();

}

void frmGather::on_btnPrevMonth_clicked()
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

    UpdateTableWidget(year, month);

    UpdateTableWidget();

}

void frmGather::on_btnNextMonth_clicked()
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


    UpdateTableWidget(year, month);

    UpdateTableWidget();

}


void frmGather::on_cbx_frametype_currentIndexChanged(const QString &strtype)
{
    if (strtype == "")
    {

    }
#if 0
    QString strDateTime;
    QString vehicleid = ui->cbx_vehicleid->currentText();
    QString strSearchDir = vehicleid + "/";
    int year = ui->cboxYear->currentData().toInt();
    int month = ui->cboxMonth->currentData().toInt();
    QString stryear = QString("%1").arg(year);
    QString strmonth = QString("%1").arg(month, 2, 10, QLatin1Char('0'));
    strDateTime = stryear + "-" + strmonth;
    for (int i =0; i < ossdatas.size(); i++)
    {
        ossdatas[i].is_downloading = false;
        ossdatas[i].download_status = 0;

        ossdatas[i].is_handling = false;
        ossdatas[i].handle_status = 0;
    }

    UpdateOSSView(strSearchDir, strDateTime, strtype);
#endif
}

void frmGather::on_btn_handle_clicked()
{
    std::string filepathname;
#ifdef Q_OS_LINUX
// linux
    QString savepath = "/";
#endif

#ifdef Q_OS_WIN32
// win
    QString savepath = "D:/";
#endif

    int year = ui->cboxYear->currentData().toInt();
    int month = ui->cboxMonth->currentData().toInt();

    handle_status =  0;
    progress_value = 0;
    ui->btn_handle->setChecked(true);
    ui->btn_handle->setEnabled(false);
    ui->widget_progress->setValue(0);

    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), savepath,
                                                     QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if (path.isEmpty())
    {
        ui->btn_handle->setChecked(false);
        ui->btn_handle->setEnabled(true);
        return;
    }

    filepathname = path.toStdString() + "/SH-" + ui->cboxYear->currentData().toString().toStdString() + ui->cboxMonth->currentData().toString().toStdString() + ".xlsx";
    ui->btn_handle->setText("数据汇总中...");
    std::thread([this, filepathname, year, month]() {
        bool result;
        std::map<std::string, std::string> weather_map;
        std::map<std::string, std::map<std::string, Vehicle_DataInfo_st>> vehicle_data_map;
        //CloudData *cloud_handle_ptr = CloudData::Instance();
        std::shared_ptr<xlsx_handler> xslx_handle_ptr = std::make_shared<xlsx_handler>();

        for (size_t index =0; index < weather_vec.size(); index++)
        {
            weather_map[weather_vec[index].date_str.toStdString()] = weather_vec[index].weather_str.toStdString();
        }

        result = Query_Vehicle_OSSData(year, month);
        if (!result)
        {
            handle_status = -1;
            return;
        }

        progress_value = 2;
        for (auto &iter : ossdata_map)
        {
            std::map<std::string, Vehicle_DataInfo_st> vehicle_datainfo_map;
            for (auto iter1 : iter.second)
            {
                bool result = false;
                std::string filecontent;
                result = cloud_handle_ptr->download_files_to_memory(std::get<0>(iter1).filename, filecontent);
                if (result)
                {
                    char temp[20] = {0};
                    std::string date_str;
                    Vehicle_DataInfos_st vehicle_datainfos;
                    Vehicle_DataInfo_st vehicle_datainfo;
                    csv_handler csv_hdl;

                    csv_hdl.convert_from_csv(filecontent, vehicle_datainfos);
                    sprintf(temp, "%04d/%02d/%02d", vehicle_datainfos.sh_data.vehicle_date.year, vehicle_datainfos.sh_data.vehicle_date.mon, vehicle_datainfos.sh_data.vehicle_date.day );
                    date_str = temp;
                    vehicle_datainfo.weather_data = weather_map[date_str];
                    vehicle_datainfo.vehicle_data = vehicle_datainfos.sh_data;
                    vehicle_datainfo_map.insert(std::pair<std::string, Vehicle_DataInfo_st>(date_str, vehicle_datainfo));

                } else {
                    handle_status = -1;
                    return;
                }

                if (!handle_mode)
                {
                    if (progress_value <= 95)
                    {
                        progress_value += 1;
                    }

                }

            }


            vehicle_data_map.insert(std::pair<std::string, std::map<std::string, Vehicle_DataInfo_st>>(iter.first, vehicle_datainfo_map));
        }

        if (handle_mode)
        {
            progress_value = 5;
        }


//        std::cout << "###############" << std::endl;
//        for (auto &iter : vehicle_data_map)
//        {
//            for (auto &iter1 : iter.second)
//            {
//                std::cout << iter1.first << ":" << iter1.second.vehicle_data.vehicle_sliphands.size() << std::endl;
//            }
//            std::cout << "====================" << std::endl;
//        }
//        std::cout << "**********************" << std::endl;
        if (handle_mode)
        {
            //download sh ascfile and process
            for (auto &iter : ossdata_map)
            {
                std::map<std::string, Vehicle_DataInfo_st> temp_date_map = vehicle_data_map[iter.first];
                std::vector<VehicleMonitorInfo> takeoverlist;

                for (auto iter1 : iter.second)
                {
                    std::string content;
                    char *content_ptr = nullptr;
                    int64_t content_len = 0;

                    std::string temp_date;
                    std::vector<Sliphand_Data_st> sliphand_vec;
                    char temp[20] ={0};

                    sprintf(temp, "%04d/%02d/%02d", std::get<1>(iter1).filedate.year, std::get<1>(iter1).filedate.mon, std::get<1>(iter1).filedate.day);
                    temp_date = temp;

                    //ossdatas[i].is_handling = true;
                    bool ret = cloud_handle_ptr->download_files_to_memory(std::get<1>(iter1).filename, &content_ptr, content_len);
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

                        takeoverlist.clear();

                        if (ret)
                        {
                            for (auto iter2 : takeoverlist)
                            {
                                Sliphand_Data_st sliphand;

                                sliphand.sliphand_begin_time = iter2.time;
                                sliphand.sliphand_end_time = iter2.time;
                                sliphand.sliphand_distance = 0;
                                sliphand_vec.push_back(sliphand);
                            }
                        }
                        delete[] content_ptr;
        //                ossdatas[i].handle_status = true;
                    } else {
        //                ossdatas[i].handle_status = false;
                        //handle_status = - 1;
                        //return;
                    }

                    if (progress_value <= 95)
                    {
                        progress_value += 1;
                    }

                    temp_date_map[temp_date].vehicle_data.vehicle_sliphands.clear();
                    temp_date_map[temp_date].vehicle_data.vehicle_sliphands = sliphand_vec;
                    temp_date_map[temp_date].weather_data = weather_map[temp_date];

                }

                vehicle_data_map[iter.first] = temp_date_map;
            }
        }

        if (progress_value < 90)
        {
            progress_value = 90;
        }

//        std::cout << "######################" << std::endl;
//        for (auto &iter : vehicle_data_map)
//        {
//            std::cout << iter.first << std::endl;

//            for (auto &iter1 : iter.second)
//            {
//                std::cout << iter1.first << ":" << iter1.second.vehicle_data.vehicle_sliphands.size() << std::endl;
//            }
//            std::cout << "=====================" << std::endl;
//        }
//        std::cout << "**********************" << std::endl;

        xslx_handle_ptr->export_to_excel(weather_map, vehicle_data_map);
        xslx_handle_ptr->save(filepathname);
        progress_value = 100;

    }).detach();
}

void frmGather::on_cbx_vehicleid_currentIndexChanged(const QString &strVehicleId)
{
    if (strVehicleId == "")
    {

    }
#if 0
    QString strDateTime;
    QString vehicleid = ui->cbx_vehicleid->currentText();
    QString strSearchDir = vehicleid + "/";
    QString strType = ui->cbx_frametype->currentText();

    int year = ui->cboxYear->currentData().toInt();
    int month = ui->cboxMonth->currentData().toInt();
    QString stryear = QString("%1").arg(year);
    QString strmonth = QString("%1").arg(month, 2, 10, QLatin1Char('0'));
    strDateTime = stryear + "-" + strmonth;
    for (int i =0; i < ossdatas.size(); i++)
    {
        ossdatas[i].download_status = false;
    }

    UpdateOSSView(strSearchDir, strDateTime, strType);
#endif
}

void frmGather::on_btn_downloadall_clicked()
{
    std::string filepathname;

#ifdef Q_OS_LINUX
// linux
    QString savepath = "/";
#endif

#ifdef Q_OS_WIN32
// win
    QString savepath = "D:/";
#endif

    int year = ui->cboxYear->currentData().toInt();
    int month = ui->cboxMonth->currentData().toInt();

    handle_status =  0;
    progress_value = 0;
    ui->btn_downloadall->setChecked(true);
    ui->btn_downloadall->setEnabled(false);
    ui->widget_progress->setValue(0);

    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), savepath,
                                                     QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if (path.isEmpty())
    {
        ui->btn_downloadall->setChecked(false);
        ui->btn_downloadall->setEnabled(true);
        return;
    }
    handle_status =  0;
    filepathname = path.toStdString() + "/GA-" + ui->cboxYear->currentData().toString().toStdString() + ui->cboxMonth->currentData().toString().toStdString() + ".xlsx";

    ui->btn_downloadall->setText("数据汇总中...");
    std::thread([this, filepathname, year, month]() {
        bool result;
        std::map<std::string, std::string> weather_map;
        std::map<std::string, std::map<std::string, Vehicle_DataInfo_st>> vehicle_data_map;
        //CloudData *cloud_handle_ptr = CloudData::Instance();
        std::shared_ptr<xlsx_handler> xslx_handle_ptr = std::make_shared<xlsx_handler>();

        for (size_t index =0; index < weather_vec.size(); index++)
        {
            weather_map[weather_vec[index].date_str.toStdString()] = weather_vec[index].weather_str.toStdString();
        }

        result = Query_Vehicle_OSSData(year, month);
        if (!result)
        {
            handle_status = -1;
            return;
        }

        progress_value = 2;
        for (auto &iter : ossdata_map)
        {
            std::map<std::string, Vehicle_DataInfo_st> vehicle_datainfo_map;
            for (auto iter1 : iter.second)
            {
                bool result = false;
                std::string filecontent;
                result = cloud_handle_ptr->download_files_to_memory(std::get<0>(iter1).filename, filecontent);
                if (result)
                {
                    char temp[20] = {0};
                    std::string date_str;
                    Vehicle_DataInfos_st vehicle_datainfos;
                    Vehicle_DataInfo_st vehicle_datainfo;
                    csv_handler csv_hdl;
                    csv_hdl.convert_from_csv(filecontent, vehicle_datainfos);
                    sprintf(temp, "%04d/%02d/%02d", vehicle_datainfos.ga_data.vehicle_date.year, vehicle_datainfos.ga_data.vehicle_date.mon, vehicle_datainfos.ga_data.vehicle_date.day );
                    date_str = temp;
                    vehicle_datainfo.weather_data = weather_map[date_str];
                    vehicle_datainfo.vehicle_data = vehicle_datainfos.ga_data;
                    vehicle_datainfo_map.insert(std::pair<std::string, Vehicle_DataInfo_st>(date_str, vehicle_datainfo));

                } else {
                    //handle_status = -1;
                    //return;
                }

                if (!handle_mode)
                {
                    if (progress_value <= 95)
                    {
                        progress_value += 1;
                    }

                }
            }
            vehicle_data_map.insert(std::pair<std::string, std::map<std::string, Vehicle_DataInfo_st>>(iter.first, vehicle_datainfo_map));
        }

        if (handle_mode)
        {
            progress_value = 5;
        }


//        std::cout << "###############" << std::endl;
//        for (auto &iter : vehicle_data_map)
//        {
//            for (auto &iter1 : iter.second)
//            {
//                std::cout << iter1.first << ":" << iter1.second.vehicle_data.vehicle_sliphands.size() << std::endl;
//            }
//            std::cout << "====================" << std::endl;
//        }
//        std::cout << "**********************" << std::endl;

        //download sh ascfile and process
        if (handle_mode)
        {
            for (auto &iter : ossdata_map)
            {
                std::map<std::string, Vehicle_DataInfo_st> temp_date_map = vehicle_data_map[iter.first];
                std::vector<VehicleMonitorInfo> takeoverlist;

                for (auto iter1 : iter.second)
                {
                    std::string content;
                    char *content_ptr = nullptr;
                    int64_t content_len = 0;

                    std::string temp_date;
                    std::vector<Sliphand_Data_st> sliphand_vec;
                    char temp[20] ={0};

                    sprintf(temp, "%04d/%02d/%02d", std::get<2>(iter1).filedate.year, std::get<2>(iter1).filedate.mon, std::get<2>(iter1).filedate.day);
                    temp_date = temp;

                    //ossdatas[i].is_handling = true;
                    bool ret = cloud_handle_ptr->download_files_to_memory(std::get<2>(iter1).filename, &content_ptr, content_len);
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

                        takeoverlist.clear();

                        if (ret)
                        {
                            for (auto iter2 : takeoverlist)
                            {
                                Sliphand_Data_st sliphand;

                                sliphand.sliphand_begin_time = iter2.time;
                                sliphand.sliphand_end_time = iter2.time;
                                sliphand.sliphand_distance = 0;
                                sliphand_vec.push_back(sliphand);
                            }
                        }
        //                ossdatas[i].handle_status = true;
                        delete[] content_ptr;
                    } else {
        //                ossdatas[i].handle_status = false;
        //                handle_status = -1;
        //                return;
                    }

                    if (progress_value <= 95)
                    {
                        progress_value += 1;
                    }

                    temp_date_map[temp_date].vehicle_data.vehicle_sliphands.clear();
                    temp_date_map[temp_date].vehicle_data.vehicle_sliphands = sliphand_vec;
                    temp_date_map[temp_date].weather_data = weather_map[temp_date];

                }

                vehicle_data_map[iter.first] = temp_date_map;
            }

        }


        if (progress_value < 90)
        {
            progress_value = 90;
        }

//        std::cout << "######################" << std::endl;
//        for (auto &iter : vehicle_data_map)
//        {
//            std::cout << iter.first << std::endl;

//            for (auto &iter1 : iter.second)
//            {
//                std::cout << iter1.first << ":" << iter1.second.vehicle_data.vehicle_sliphands.size() << std::endl;
//            }
//            std::cout << "=====================" << std::endl;
//        }
//        std::cout << "**********************" << std::endl;



        xslx_handle_ptr->export_to_excel(weather_map, vehicle_data_map);

        xslx_handle_ptr->save(filepathname);

        progress_value = 100;

    }).detach();
}

void frmGather::on_checkBox_Foton_3051_stateChanged(int checkState)
{
    int year = 0;
    int month = 0;

    if (checkState)
    {

    }
    vehicle_vec.clear();

    if (ui->checkBox_Foton_3051->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-1");
    }

    if (ui->checkBox_Foton_3052->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-2");
    }

    if (ui->checkBox_Foton_3053->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-3");
    }

    if (ui->checkBox_Foton_3054->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-4");
    }

    if (ui->checkBox_SQ_2012->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("Shaanqi-2012");
    }

    year = ui->cboxYear->currentData().toInt();
    month = ui->cboxMonth->currentData().toInt();

    UpdateTableWidget(year, month);

    UpdateTableWidget();
}

void frmGather::on_checkBox_Foton_3052_stateChanged(int checkState)
{
    int year = 0;
    int month = 0;

    if (checkState)
    {

    }
    vehicle_vec.clear();

    if (ui->checkBox_Foton_3051->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-1");
    }

    if (ui->checkBox_Foton_3052->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-2");
    }

    if (ui->checkBox_Foton_3053->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-3");
    }

    if (ui->checkBox_Foton_3054->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-4");
    }

    year = ui->cboxYear->currentData().toInt();
    month = ui->cboxMonth->currentData().toInt();

    UpdateTableWidget(year, month);

    UpdateTableWidget();
}

void frmGather::on_checkBox_Foton_3053_stateChanged(int checkState)
{
    int year = 0;
    int month = 0;

    if (checkState)
    {

    }
    vehicle_vec.clear();

    if (ui->checkBox_Foton_3051->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-1");
    }

    if (ui->checkBox_Foton_3052->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-2");
    }

    if (ui->checkBox_Foton_3053->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-3");
    }

    if (ui->checkBox_Foton_3054->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-4");
    }

    if (ui->checkBox_SQ_2012->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("Shaanqi-2012");
    }

    year = ui->cboxYear->currentData().toInt();
    month = ui->cboxMonth->currentData().toInt();

    UpdateTableWidget(year, month);

    UpdateTableWidget();
}

void frmGather::on_checkBox_Foton_3054_stateChanged(int checkState)
{
    int year = 0;
    int month = 0;

    if (checkState)
    {

    }

    vehicle_vec.clear();

    if (ui->checkBox_Foton_3051->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-1");
    }

    if (ui->checkBox_Foton_3052->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-2");
    }

    if (ui->checkBox_Foton_3053->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-3");
    }

    if (ui->checkBox_Foton_3054->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-4");
    }

    year = ui->cboxYear->currentData().toInt();
    month = ui->cboxMonth->currentData().toInt();

    UpdateTableWidget(year, month);

    UpdateTableWidget();
}

void frmGather::on_checkBox_SQ_2012_stateChanged(int checkState)
{
    int year = 0;
    int month = 0;

    if (checkState)
    {

    }

    vehicle_vec.clear();

    if (ui->checkBox_Foton_3051->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-1");
    }

    if (ui->checkBox_Foton_3052->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-2");
    }

    if (ui->checkBox_Foton_3053->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-3");
    }

    if (ui->checkBox_Foton_3054->checkState() == Qt::Checked)
    {
        vehicle_vec.push_back("data-4");
    }


    year = ui->cboxYear->currentData().toInt();
    month = ui->cboxMonth->currentData().toInt();

    UpdateTableWidget(year, month);

    UpdateTableWidget();
}

void frmGather::on_cboxYear_currentIndexChanged(const QString &stryear)
{
    int year = 0;
    int month = 0;

    if (stryear == "")
    {

    }

    year = ui->cboxYear->currentData().toInt();
    month = ui->cboxMonth->currentData().toInt();

    cbox_year = year;

    UpdateTableWidget(year, month);

    UpdateTableWidget();
}

void frmGather::on_cboxMonth_currentIndexChanged(const QString &strmonth)
{
    int year = 0;
    int month = 0;

    if (strmonth == "")
    {

    }


    year = ui->cboxYear->currentData().toInt();
    month = ui->cboxMonth->currentData().toInt();

    cbox_month = month;

    UpdateTableWidget(year, month);

    UpdateTableWidget();
}

void frmGather::on_checkBox_reprocess_stateChanged(int arg1)
{
    if (arg1)
    {

    }

    if (ui->checkBox_reprocess->checkState() == Qt::Checked)
    {
        handle_mode = true;
    } else {
        handle_mode = false;
    }
}
