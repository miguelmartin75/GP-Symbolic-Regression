#-------------------------------------------------
#
# Project created by QtCreator 2015-04-01T11:46:54
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

TARGET = viewer
TEMPLATE = app

LIBS += -L/Volumes/HDD/Programming/repos/school/gp/lib/build -lgp_symb_reg_s
INCLUDEPATH += ../lib/src
INCLUDEPATH += /usr/local/include

SOURCES += main.cpp\
        MainWindow.cpp \
        qcustomplot.cpp \
    PointEditDialog.cpp \
    PointListEditDialog.cpp \
    GeneratePointDialog.cpp \
    ConfigDialog.cpp \
    RunThread.cpp

HEADERS  += MainWindow.hpp \
            qcustomplot.h \
    PointEditDialog.hpp \
    PointListEditDialog.hpp \
    ErrorDialog.hpp \
    RunThread.hpp \
    GeneratePointDialog.hpp \
    ConfigDialog.hpp \
    Validator.hpp

FORMS    += MainWindow.ui \
    PointEditDialog.ui \
    PointEditDialog.ui \
    PointListEditDialog.ui \
    GeneratePointDialog.ui \
    ConfigDialog.ui
