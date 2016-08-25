#-------------------------------------------------
#
# Project created by QtCreator 2016-08-23T13:59:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YaoCalendar
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mycalendarwidget.cpp \
    sidebar.cpp \
    calendarevent.cpp \
    eventdialog.cpp \
    eventstorage.cpp \
    eventmaphelper.cpp

HEADERS  += mainwindow.h \
    mycalendarwidget.h \
    sidebar.h \
    eventmap.h \
    calendarevent.h \
    eventdialog.h \
    types.h \
    eventstorage.h \
    eventmaphelper.h

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    res.qrc
