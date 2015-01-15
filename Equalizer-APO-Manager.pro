TEMPLATE = app

QT += core qml quick

SOURCES += main.cpp \
    singleapplication.cpp \
    guihandler.cpp \
	equalizerfuncs.cpp \
    windowfuncs.cpp

RESOURCES += qml.qrc \
    images.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    singleapplication.h \
    guihandler.h \
    equalizerfuncs.h \
    windowfuncs.h

