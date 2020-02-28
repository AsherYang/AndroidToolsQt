#-------------------------------------------------
#
# Project created by QtCreator 2019-09-09T10:40:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# AndroidTools For me(main.cpp use MainWindow.cpp)
TARGET = AndroidTools

# NfcTools For others(main.cpp use MinWindow.cpp)
#TARGET = NfcTools
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

LIBS += User32.LIB

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        minwindow.cpp

HEADERS += \
        mainwindow.h \
        minwindow.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md \
    copy.bat

RC_ICONS = logo.ico

include(./settings/settings.pri)
include(./logkeyword/log_keyword.pri)
include(./utils/utils.pri)
include(./copyfile/copy_file.pri)
#include(./translate/translate.pri)
#include(./adboperate/adb_operate.pri)
#include(./baseinfo/base_info.pri)
include(./nfc/nfc.pri)

#RESOURCES += \
#    res.qrc
