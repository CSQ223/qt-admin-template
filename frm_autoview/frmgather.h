#ifndef frmGather_H
#define frmGather_H

#include <string>
#include <vector>

#include <QWidget>
#include <QDate>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QVector>
#include <QMutex>
#include <QTimer>

#include "common.h"
#include "alibabacloud_handle.h"
#include "csv_handler.h"
#include "xlsx_handler.h"
#include "cloud_handler.h"

namespace Ui {
class frmGather;
}

struct OSSData_st
{
    std::string filename;
    std::string filesize;
    unix_date_time filedate;
};

class frmGather : public QWidget
{
    Q_OBJECT

public:
    struct OSSWidget
    {
        //序号
        QLabel *label_num_ptr;
        //文件名
        QLabel *label_filename_ptr;
        //文件大小
        QLabel *label_filesize_ptr;
        //更新日期
        QLabel *label_filedate_ptr;
        //下载
        QPushButton *btn_download_ptr;
        //天气信息
        QComboBox *cbx_weather_ptr;
        //下载进度
        QProgressBar *progress_download_ptr;
        //分析进度
        QPushButton *btn_handle_ptr;
        //分析进度
        QProgressBar *progress_handle_ptr;
    };

    struct Date_Weather_st
    {
        int seqnum;
        QString date_str;
        QString weather_str;
    };

    struct Date_Weather_Widget_st
    {
        //序号
        QLabel *label_num_ptr;
        //日期信息
        QLabel *label_datastr_ptr;
        //天气信息
//        QComboBox *cbx_weather_ptr;
        QLineEdit *lineedit_weather_ptr;
    };

    struct Vehicle_Progress_st
    {
        int seqnum;
        std::string vehiclename;
        bool is_handling;
        uint8_t handle_status;
    };

    struct Vehicle_Progress_Widget_st
    {
        //序号
        QLabel *label_num_ptr;
        //
        QLabel *label_vehiclename_ptr;
        //分析进度
        QProgressBar *progress_handle_ptr;
    };



    explicit frmGather(QWidget *parent = nullptr);
    ~frmGather();

    void handle_asc_file(std::string , std::string );
    void set_handle_mode(bool mode);
private slots:
    void test();
    void slot_timeout();

    void slot_btn_clicked();

    void slot_btn_handle_clicked();

    void on_btnPrevYear_clicked();

    void on_btnNextYear_clicked();

    void on_btnPrevMonth_clicked();

    void on_btnNextMonth_clicked();

    void on_cbx_frametype_currentIndexChanged(const QString &arg1);

    void on_btn_handle_clicked();

    void on_cbx_vehicleid_currentIndexChanged(const QString &arg1);

    void on_btn_downloadall_clicked();

    void on_checkBox_Foton_3051_stateChanged(int arg1);

    void on_checkBox_Foton_3052_stateChanged(int arg1);

    void on_checkBox_Foton_3053_stateChanged(int arg1);

    void on_checkBox_Foton_3054_stateChanged(int arg1);

    void on_checkBox_SQ_2012_stateChanged(int arg1);

    void slot_cbx_indexchanged(const QString &);

    void update_process_mode(bool);
    void on_cboxYear_currentIndexChanged(const QString &arg1);

    void on_cboxMonth_currentIndexChanged(const QString &arg1);

    void on_checkBox_reprocess_stateChanged(int arg1);

private:
    Ui::frmGather *ui;

    QFont iconFont;
    QDate date;                         //当前日期
    int cbox_year;
    int cbox_month;

    std::vector<std::string> vehicle_vec;
    std::map<std::string, std::vector<std::array<OSSData_st, 3>>> ossdata_map;

    // unique date map
    std::map<std::string, bool> date_map;
    QMutex oss_mutex_;
//    std::vector<OSSData> ossdatas;
//    QVector<OSSWidget> osswidgets;

    QWidget *lineedit_weather_widget_ptr;
    QHBoxLayout *layout;
    std::vector<Date_Weather_st> weather_vec;
    QVector<Date_Weather_Widget_st> weather_widget_vec;

    std::vector<Vehicle_Progress_st> vehicle_progress_vec;
    QVector<Vehicle_Progress_Widget_st> vehicle_progress_widget_vec;

    //Alibaba Cloud Handle
    //std::shared_ptr<ALIBABACLOUD_HANDLE::alibabacloud_handle> handle_ptr;
    CloudData *cloud_handle_ptr;


    // Data Analyse
    CSVOperator csv;
    std::string csv_filename_;
    QString app_path_;

    //progress precent
    int progress_value;
    // handle data status: -1/error 0/ok
    int handle_status;
    // 0: no need reprocess 1:need reprocess
    bool handle_mode;

    QTimer *timer_ptr;

    void InitFont();

    void InitForm();

    void InitStyle();

    void InitData();

    void UpdateTableWidget();

    void UpdateTableWidget(int year, int month);

    void UpdateOSSView(const QString &,const QString &, const QString &);

    bool Query_Vehicle_OSSData(int year, int mon);

    bool Query_Vehicle_Data(int year, int month, int type, std::map<std::string, std::vector<OSSData_st>> &oss_data_map);
};

#endif // frmGather_H
