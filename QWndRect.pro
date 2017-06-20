#-------------------------------------------------
#
# Project created by QtCreator 2017-05-15T15:47:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QWndRect
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    mysquare.cpp \
    graphicview.cpp \
    scene.cpp

HEADERS  += mainwindow.h \
    mysquare.h \
    graphicview.h \
    scene.h \
    globaldef.h

FORMS    += mainwindow.ui

RESOURCES += \
    imgrc.qrc

DISTFILES +=

INCLUDEPATH +=/usr/include/libxml2

INCLUDEPATH +=../OSIReader
unix|win32: LIBS += -lxml2

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-QWndRect-Desktop_Qt_5_8_0_GCC_64bit-Debug/release/ -lOSIReader
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-QWndRect-Desktop_Qt_5_8_0_GCC_64bit-Debug/debug/ -lOSIReader
else:unix: LIBS += -L$$PWD/../build-QWndRect-Desktop_Qt_5_8_0_GCC_64bit-Debug/ -lOSIReader

INCLUDEPATH += $$PWD/../build-QWndRect-Desktop_Qt_5_8_0_GCC_64bit-Debug
DEPENDPATH += $$PWD/../build-QWndRect-Desktop_Qt_5_8_0_GCC_64bit-Debug

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-QWndRect-Desktop_Qt_5_8_0_GCC_64bit-Debug/release/ -ljpeg
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-QWndRect-Desktop_Qt_5_8_0_GCC_64bit-Debug/debug/ -ljpeg
else:unix: LIBS += -L$$PWD/../build-QWndRect-Desktop_Qt_5_8_0_GCC_64bit-Debug/ -ljpeg

INCLUDEPATH += $$PWD/../build-QWndRect-Desktop_Qt_5_8_0_GCC_64bit-Debug
DEPENDPATH += $$PWD/../build-QWndRect-Desktop_Qt_5_8_0_GCC_64bit-Debug

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-QWndRect-Desktop_Qt_5_8_0_GCC_64bit-Debug/release/ -ltiff
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-QWndRect-Desktop_Qt_5_8_0_GCC_64bit-Debug/debug/ -ltiff
else:unix: LIBS += -L$$PWD/../build-QWndRect-Desktop_Qt_5_8_0_GCC_64bit-Debug/ -ltiff

INCLUDEPATH += $$PWD/../build-QWndRect-Desktop_Qt_5_8_0_GCC_64bit-Debug
DEPENDPATH += $$PWD/../build-QWndRect-Desktop_Qt_5_8_0_GCC_64bit-Debug
