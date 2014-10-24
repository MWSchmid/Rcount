/*
This file is part of Rcount.

Rcount is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Rcount is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

See <http://www.gnu.org/licenses/> for a a copy of the GNU General Public License.
*/

#include <QtGui/QApplication>
#include <QtCore/QCoreApplication>

//#include <getopt.h>
//#include "consoleRunner.h"

#include "../p502_GUI/dataAnalysisViewWidget/dataanalysisview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // initialize the GUI
    dataAnalysisView w;
    // show the gui
    w.show();
    return a.exec();
}

/*
void usage() {
    std::cerr << std::endl << "Usage is ./programname [-c projectFile (several files separated by comma)]" << std::endl <<
                 "-c <file> if given, the application runs on console, otherwise gui." << std::endl << std::flush;
    exit(8);
}

int c;
extern char *optarg;
extern int optind, optopt, opterr;

int main(int argc, char *argv[])
{
    // check command line
    bool commandLine = false;
    QString projectFiles = "";


    while ((c = getopt(argc, argv, "c:")) != -1) {
        switch(c) {
        case 'c':
            commandLine = true;
            projectFiles = optarg;
            break;
        case ':':
            std::cerr << "some stuff not specified" << std::endl << std::flush;
            break;
        case '?':
            std::cerr << "unknown argument" << std::endl << std::flush;
            usage();
        }
    }

    //! RUNNING ON COMMANDLINE
    if (commandLine)
    {
        QCoreApplication a(argc, argv);
        // initialize the runner (will do everything)
        consoleRunner runner(projectFiles);
        QObject::connect(&runner, SIGNAL(finished()), &a, SLOT(quit()), Qt::DirectConnection);
        runner.startAnalysis();
        runner.start();
        return a.exec();
    }
    //! RUNNING IN GUI
    else
    {
        QApplication a(argc, argv);
        // initialize the GUI
        dataAnalysisView w;
        // show the gui
        w.show();
        return a.exec();
    }


}
*/
