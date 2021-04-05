#ifndef FRMAPTEST_H
#define FRMAPTEST_H

#include <string>
#include <vector>

#include <QWidget>
#include <QDate>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QVector>
#include <QMutex>
#include <QTimer>


#include "alibabacloud_handle.h"
#include "csvoperator.h"
#include "csv_handler.h"
#include "cloud_handler.h"

namespace Ui {
class frmAptest;
}

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
    //下载进度
    QProgressBar *progress_download_ptr;
    //分析进度
    QPushButton *btn_handle_ptr;
    //分析进度
    QProgressBar *progress_handle_ptr;

    //公安测试总里程(KM)
    QLabel *label_ga_totalkilometer_ptr;

    //公安自动驾驶里程(KM)
    QLabel *label_ga_autokilometer_ptr;
};

struct OSSData
{
    int seqnum;
    std::string filename;
    std::string filesize;
    std::string filedate;
    bool is_downloading;
    uint8_t download_status;
    bool is_handling;
    uint8_t handle_status;
};

class frmAptest : public QWidget
{
    Q_OBJECT

public:
    explicit frmAptest(QWidget *parent = 0);
    ~frmAptest();

    void handle_asc_file(std::string , std::string );

private slots:
    void test();

    void slot_timeout();

    void slot_update_gadata_timeout();

    void slot_btn_clicked();

    void slot_btn_handle_clicked();

    void on_btnPrevYear_clicked();

    void on_btnNextYear_clicked();

    void on_btnPrevMonth_clicked();

    void on_btnNextMonth_clicked();

    void slot_btn_gather_down();

    void slot_btn_view_down();

    void on_cbx_frametype_currentIndexChanged(const QString &arg1);

    void on_btn_handle_clicked();

    void on_cbx_vehicleid_currentIndexChanged(const QString &arg1);

    void on_btn_downloadall_clicked();

    void on_cboxYear_currentIndexChanged(const QString &arg1);

    void on_cboxMonth_currentIndexChanged(const QString &arg1);

    void on_tableWidget_cellEntered(int row, int column);

private:
    Ui::frmAptest *ui;

    QFont iconFont;
    QDate date;                         //当前日期
    int cbox_year;
    int cbox_month;

    QMutex oss_mutex_;
    std::vector<OSSData> ossdatas;
    QVector<OSSWidget> osswidgets;

//    std::shared_ptr<ALIBABACLOUD_HANDLE::alibabacloud_handle> handle_ptr;
    CloudData *cloud_handle_ptr;

    // Data Analyse
    CSVOperator csv;
    std::string csv_filename_;
    QString app_path_;

    QTimer *timer_ptr;
    QTimer *timer_update_gadata_ptr;

    void InitFont();

    void InitForm();

    void InitStyle();

    void InitData();

    void UpdateTableWidget(int type);

    void UpdateOSSView(const QString &,const QString &, const QString &);
};

#endif // FRMAPTEST_H
