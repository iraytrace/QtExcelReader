#-------------------------------------------------
#
# Project created by QtCreator 2015-09-14T10:39:12
#
#-------------------------------------------------

QT       += core gui axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = excelReader
TEMPLATE = app


SOURCES += main.cpp MainWindow.cpp RecentFiles.cpp VSLapp.cpp \
    ExcelTable.cpp

HEADERS  += MainWindow.h RecentFiles.h VSLapp.h \
    ExcelTable.h

FORMS    += MainWindow.ui

