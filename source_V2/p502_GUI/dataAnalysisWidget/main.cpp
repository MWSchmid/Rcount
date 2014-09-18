#include <QtGui/QApplication>
#include "dataanalysiswidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dataAnalysisWidget w;
    w.show();

    return a.exec();
}
