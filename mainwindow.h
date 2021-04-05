#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QToolButton>
#include <QMainWindow>
#include "frmaptest.h"
#include "frmgather.h"
#include "cloud_handler.h"
#include "qc_listview.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString title;
    QString copyright;
    QString version;
    bool reprocess_mode;
    QMap<QString, QString> configs;

    QToolButton *minButton;
    QToolButton *maxButton;
    QToolButton *closeButton;

    QList<QWidget *> widgets;

    QWidget *frm_aptest_ptr;
    QWidget *frm_gather_ptr;

    std::string AccessKeyId;
    std::string AccessKeySecret;
    std::string Endpoint;
    std::string BucketName;

    CloudData *clouddata_ptr;

    void paintEvent(QPaintEvent *);

    void initStyle();
    void initForm();
    void initData();

    void loadConfig(QString fileName, QMap<QString, QString> &configs);

private slots:
    void windowClosed();
    void windowMinimize();
    void windowMaximize();
    void slot_listview_pressed(const QString &, const QString &);
//    void on_pushButton_device_monitor_clicked();
//    void on_pushButton_map_monitor_clicked();
//    void on_pushButton_data_monitor_clicked();
//    void on_pushButton_line_monitor_clicked();
//    void on_pushButton_data_query_clicked();
//    void on_pushButton_system_monitor_clicked();
    void on_btn_view_clicked();
    void on_btn_gather_clicked();
signals:
    void update_process_mode(bool);
};

#endif // MAINWINDOW_H
