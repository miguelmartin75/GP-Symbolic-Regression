#-------------------------------------------------
#
# Project created by QtCreator 2015-04-01T11:46:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = viewer
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
        qcustomplot.cpp \
    PointEditDialog.cpp

HEADERS  += MainWindow.hpp \
            qcustomplot.h \
    PointEditDialog.hpp

FORMS    += MainWindow.ui \
    PointEditDialog.ui
