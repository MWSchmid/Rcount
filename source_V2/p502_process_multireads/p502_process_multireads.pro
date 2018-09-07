#-------------------------------------------------
#
# Project created by QtCreator 2014-06-21T15:21:46
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = Rcount-multireads
TEMPLATE = app


SOURCES += main.cpp\
        processmultireads.cpp\
        multireadsprocessor.cpp \
        analysisprogressview.cpp \
        processmultireadsview.cpp

HEADERS  += processmultireads.h \
            processMultiClasses.h \
            multireadsprocessor.h \
            analysisprogressview.h \
            processmultireadsview.h \
            printTimeAndMem.h \ # to turn this on, uncomment the lines in analysisprogressview.cpp. WORKS ONLY ON LINUX - IT READS THE MEM-INFO FROM /proc/...
    consoleRunner.h

FORMS    += analysisprogressview.ui \
            processmultireadsview.ui

INCLUDEPATH+= ../zlib-1.2.8 \
              ../

LIBS += -lz
