QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
#指定编译生成的可执行文件到bin目录
#DESTDIR += $$PWD/bin
CONFIG(debug,debug|release){
    DESTDIR += $$PWD/bin/debug
}
CONFIG(release,debug|release){
    DESTDIR += $$PWD/bin/release
}
win32 {
    contains(QT_ARCH, i386) {
        CONFIG(debug,debug|release){
            TARGET = QtTranslate_debug_x86
        }
        CONFIG(release,debug|release){
            TARGET = QtTranslate_release_x86
        }
    }else {

        CONFIG(debug,debug|release){
            TARGET= QtTranslate_debug_x64
        }
        CONFIG(release,debug|release){
            TARGET= QtTranslate_release_x64
        }
    }
}
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/BaiduTranslateApi.cpp \
    src/BaseApi.cpp \
    src/ComputerInfo.cpp \
    src/JesonFileManage.cpp \
    src/QtTranslate.cpp \
    src/TencentTranslateApi.cpp \
    src/TranslateAPI.cpp \
    src/qaesencryption.cpp \
    src/log/LogHandler.cpp

HEADERS += \
    src/BaiduTranslateApi.h \
    src/BaseApi.h \
    src/ComputerInfo.h \
    src/JesonFileManage.h \
    src/QtTranslate.h \
    src/TencentTranslateApi.h \
    src/TranslateAPI.h \
    src/api.h \
    src/qaesencryption.h \
    src/log/LogHandler.h \
    src/log/Singleton.h

FORMS += \
    src/QtTranslate.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
