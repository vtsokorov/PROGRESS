#-------------------------------------------------
#
# Project created by QtCreator 2016-12-15T08:45:04
#
#-------------------------------------------------

QT       += core gui sql printsupport
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PROGRESS
TEMPLATE = app

INCLUDEPATH += $$PWD/limereport/$${QT_VERSION}/include
DEPENDPATH  += $$PWD/limereport/$${QT_VERSION}/include


CONFIG(debug,debug|release) {
   contains(CONFIG,zint){
        LIBS += -L$$PWD/limereport/$${QT_VERSION}/debug -lQtZint
   }
   DESTDIR += $$PWD/bin/debug
   LIBS += -L$$PWD/limereport/$${QT_VERSION}/debug -llimereport

} else {
    contains(CONFIG,zint){
        LIBS += -L$$PWD/limereport/$${QT_VERSION}/release -lQtZint
  }
  DESTDIR += $$PWD/bin/release
  LIBS += -L$$PWD/limereport/$${QT_VERSION}/release -llimereport
}

SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    applicationmodel.cpp \
    simpleinputdialog.cpp \
    equipmentinputdialog.cpp \
    sellinginputdialog.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    applicationmodel.h \
    pglobal.h \
    simpleinputdialog.h \
    equipmentinputdialog.h \
    sellinginputdialog.h \
    querys.h \
    reports.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    simpleinputdialog.ui \
    equipmentinputdialog.ui \
    sellinginputdialog.ui

RESOURCES += \
    img.qrc

RC_FILE = app.rc
