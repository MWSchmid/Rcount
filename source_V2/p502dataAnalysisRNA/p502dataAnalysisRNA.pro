#-------------------------------------------------
#
# Project created by QtCreator 2012-01-19T10:56:23
#
#-------------------------------------------------

QT       += core gui xml

TARGET = Rcount-distribute
TEMPLATE = app

HEADERS += ../p502_SOURCE/dataStructure/databaseitem.h \
    ../p502_SOURCE/dataStructure/databasereader.h \
    ../p502_SOURCE/dataStructure/databasewriter.h \
    ../p502_SOURCE/dataStructure/database.h \
    #../p502_SOURCE/dataStructure/databasemodel.h \
    ../p502_SOURCE/dataStructure/temporaryresults.h \
    ../p502_SOURCE/dataStructure/mappingtreeitem.h \
    ../p502_SOURCE/fastaHandler/fastahandler.h \
    ../p502_SOURCE/bamHandler/bamhelpers.h \
    ../p502_SOURCE/bamHandler/bamhandler.h \
    ../p502_SOURCE/dataAnalysis/readmapper.h \
    ../p502_SOURCE/dataAnalysis/readcounter.h \
    ../p502_SOURCE/dataAnalysis/dataanalysis.h \
    ../p502_GUI/createProjectWidget/createprojectwidget.h \
    ../p502_GUI/projectTreeViewWidget/projecttreeview.h \
    ../p502_GUI/projectTreeViewWidget/projecttreeviewmodel.h \
    ../p502_GUI/projectTreeViewWidget/projecttreeviewitem.h \
    ../p502_GUI/resultTableViewWidget/resulttableviewmodel.h \
    ../p502_GUI/resultTableViewWidget/resulttableview.h \
    ../p502_GUI/dataAnalysisViewWidget/dataanalysisview.h \
    ../p502_GUI/analysisProgressWidget/analysisprogressview.h \
    consoleRunner.h \
    ../p502_GUI/analysisProgressWidget/printTimeAndMem.h # to turn this on, uncomment the lines in analysisprogressview.cpp. WORKS ONLY ON LINUX - IT READS THE MEM-INFO FROM /proc/...

SOURCES += ../p502_SOURCE/dataStructure/databaseitem.cpp \
    ../p502_SOURCE/dataStructure/databasereader.cpp \
    ../p502_SOURCE/dataStructure/databasewriter.cpp \
    ../p502_SOURCE/dataStructure/database.cpp \
    #../p502_SOURCE/dataStructure/databasemodel.cpp \
    ../p502_SOURCE/dataStructure/temporaryresults.cpp \
    ../p502_SOURCE/fastaHandler/fastahandler.cpp \
    ../p502_SOURCE/bamHandler/bamhandler.cpp \
    ../p502_SOURCE/dataAnalysis/readmapper.cpp \
    ../p502_SOURCE/dataAnalysis/readcounter.cpp \
    ../p502_SOURCE/dataAnalysis/dataanalysis.cpp \
    ../p502_GUI/createProjectWidget/createprojectwidget.cpp \
    ../p502_GUI/projectTreeViewWidget/projecttreeview.cpp \
    ../p502_GUI/projectTreeViewWidget/projecttreeviewmodel.cpp \
    ../p502_GUI/projectTreeViewWidget/projecttreeviewitem.cpp \
    ../p502_GUI/resultTableViewWidget/resulttableviewmodel.cpp \
    ../p502_GUI/resultTableViewWidget/resulttableview.cpp \
    ../p502_GUI/dataAnalysisViewWidget/dataanalysisview.cpp \
    ../p502_GUI/analysisProgressWidget/analysisprogressview.cpp \
    main.cpp

FORMS += ../p502_GUI/projectTreeViewWidget/projecttreeview.ui \
    ../p502_GUI/createProjectWidget/createprojectwidget.ui \
    ../p502_GUI/resultTableViewWidget/resulttableview.ui \
    ../p502_GUI/analysisProgressWidget/analysisprogressview.ui \

#RESOURCES += p502iconResources.qrc

