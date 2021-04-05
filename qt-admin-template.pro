TEMPLATE = subdirs

QT       += core gui sql network xml
CONFIG   += C++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-admin-template
TEMPLATE = app
RC_ICONS = ./resources/images/favicon.ico
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += \
                $$PWD/cloud \
                $$PWD/common \
                $$PWD/csv \
                $$PWD/frm_autoview \
                $$PWD/qc_percenting \
                $$PWD/qc_navbutton \
                $$PWD/qc_listview \
                $$PWD/qxlsx


include ($$PWD/cloud/cloud_handler.pri)
include ($$PWD/common/common.pri)
include ($$PWD/csv/csv_handler.pri)
include ($$PWD/frm_autoview/autodataview.pri)
include ($$PWD/qc_percenting/qc_percenting.pri)
include ($$PWD/qc_navbutton/qc_navbutton.pri)
include ($$PWD/qc_listview/qc_listview.pri)
include ($$PWD/qxlsx/qxlsx.pri)


SOURCES += $$PWD/main.cpp 
SOURCES += $$PWD/iconhelper.cpp
SOURCES += $$PWD/mainwindow.cpp

RESOURCES += $$PWD/mainwindowres.qrc

FORMS += $$PWD/mainwindow.ui

HEADERS += $$PWD/mainwindow.h 
HEADERS += $$PWD/iconhelper.h
