#-------------------------------------------------
#
# Project created by QtCreator 2014-07-10T20:54:04
#
#-------------------------------------------------

QT += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = Equalizer_APO_Manager
TEMPLATE = app


SOURCES += main.cpp\
	mainwindow.cpp \
	filterfile.cpp \
	singleapplication.cpp

HEADERS  += mainwindow.h \
	filterfile.h \
	singleapplication.h

FORMS    += \
	mainwindow.ui

RESOURCES += \
	resources.qrc

win32:RC_ICONS += Icon.ico
