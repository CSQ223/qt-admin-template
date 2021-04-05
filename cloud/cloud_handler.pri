INCLUDEPATH += $$PWD/alibabacloud
INCLUDEPATH += $$PWD/alibabacloud/include



win32 {
    LIBS += -lalibabacloud-oss-cpp-sdk -L$$PWD/alibabacloud/lib/windows
    LIBS += -lcurl -L$$PWD/alibabacloud/lib/windows
    LIBS += -leay32 -L$$PWD/alibabacloud/lib/windows
    LIBS += -lssleay32 -L$$PWD/alibabacloud/lib/windows
    LIBS += -lzlibwapi -L$$PWD/alibabacloud/lib/windows
}
unix {
    LIBS += -lalibabacloud-oss-cpp-sdk -L$$PWD/alibabacloud/lib/linux
    LIBS += -lcurl
    LIBS += -lpthread
    LIBS += -lssl
    LIBS += -lcrypto
}



SOURCES     += $$PWD/alibabacloud/alibabacloud_handle.cpp
SOURCES     += $$PWD/cloud_handler.cpp

HEADERS     += $$PWD/alibabacloud/alibabacloud_handle.h
HEADERS     += $$PWD/cloud_handler.h
