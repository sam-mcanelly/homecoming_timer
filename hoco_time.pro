#-------------------------------------------------
#
# Project created by QtCreator 2015-05-17T20:27:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "HoCo Time v0.03"
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    student.cpp \
    db_controller.cpp \
    db_sort.cpp

HEADERS  += mainwindow.h \
    student.h \
    db_controller.h \
    db_sort.h

FORMS    += mainwindow.ui
