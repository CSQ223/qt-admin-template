#-------------------------------------------------
#
# Project created by QtCreator 2020-04-26T19:10:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = aptestview
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $$PWD/include
win32 {
    LIBS += -lalibabacloud-oss-cpp-sdk -L$$PWD/lib
    LIBS += -lcurl -L$$PWD/lib
    LIBS += -leay32 -L$$PWD/lib
    LIBS += -lssleay32 -L$$PWD/lib
    LIBS += -lzlibwapi -L$$PWD/lib
}

unix{
    LIBS += $$PWD/libalibabacloud-oss-cpp-sdk.a
    LIBS += -lcurl
    LIBS += -lpthread
    LIBS += -lssl
    LIBS += -lcrypto
}
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        frmaptest.cpp \
        alibabacloud_handle.cpp \
    datahandle.cpp

HEADERS += \
        frmaptest.h \
        alibabacloud_handle.h \
    datahandle.h \
    rtmcodec.h

FORMS += \
        frmaptest.ui

RESOURCES += \
    main.qrc
