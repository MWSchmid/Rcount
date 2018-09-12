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

#ifndef CONSOLERUNNER_H
#define CONSOLERUNNER_H

#include "../p502_SOURCE/dataAnalysis/dataanalysis.h"


// a mini class to run the data analysis in a console
class consoleRunner : public QThread
{ Q_OBJECT

private:
    QStringList _projectFiles;
    dataAnalysis analyzer;

public:
    consoleRunner(QString &projectFiles){
        this->_projectFiles = projectFiles.split(",");
        // set connections
        connect(&analyzer, SIGNAL(analysisStatus(QString)), this, SLOT(printStatus(QString)), Qt::DirectConnection);
        connect(&analyzer, SIGNAL(analysisProgress(int)), this, SLOT(printProgress(int)), Qt::DirectConnection);
        connect(&analyzer, SIGNAL(errorMessage(QString)), this, SLOT(printError(QString)), Qt::DirectConnection);
        connect(&analyzer, SIGNAL(writerStatsObtained(QString)), this, SLOT(printStats(QString)), Qt::DirectConnection);
    }

    //~consoleRunner(){}

    void startAnalysis() {
        foreach (QString fileName, this->_projectFiles) {
            analyzer.addProject(fileName);
        }
    }

protected:
    void run() {
        std::cerr << "running Rcount-distribute via the command line" << std::endl << std::flush;
        analyzer.addProject("STOPTHREAD");
        analyzer.wait();
        std::cerr << "stopping Rcount-distribute" << std::endl << std::flush;
    }

private slots:
    void printStats(QString stats){
        stats = stats.replace("|", "\n");
        std::cerr << qPrintable(QTime::currentTime().toString()) << " STATS " << std::endl << stats.toStdString() << std::endl << std::flush;
    }
    void printStatus(QString status){  std::cerr << qPrintable(QTime::currentTime().toString()) << " " << status.toStdString() << std::endl << std::flush; }
    void printProgress(int progress) { std::cerr << qPrintable(QTime::currentTime().toString()) << " " << progress << "%" << std::endl << std::flush; }
    void printError(QString errorMessage) { std::cerr << "ERROR: " << errorMessage.toStdString() << std::endl << std::flush; }
};

#endif // CONSOLERUNNER_H
