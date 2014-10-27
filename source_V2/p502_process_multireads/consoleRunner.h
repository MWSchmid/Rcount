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

#include "processmultireads.h"

// a mini class to run the data analysis in a console
class consoleRunner : public QThread
{ Q_OBJECT

private:
    QString command;
    multireadsProcessor processor;

public:
    consoleRunner(QString &command){
        QStringList temp = command.split(",");
        this->command = temp.join("|");
        // set connections
        connect(&processor, SIGNAL(processStatus(QString)), this, SLOT(printStatus(QString)), Qt::DirectConnection);
        connect(&processor, SIGNAL(processProgress(int)), this, SLOT(printProgress(int)), Qt::DirectConnection);
        connect(&processor, SIGNAL(errorMessage(QString)), this, SLOT(printError(QString)), Qt::DirectConnection);
    }

    //~consoleRunner(){}

    void startAnalysis() {
        std::cerr << QTime::currentTime().toString().toStdString() << std::endl << std::flush;
        processor.addWork(this->command);
    }

protected:
    void run() {
        std::cerr << "running Rcount-multireads via the command line" << std::endl << std::flush;
        processor.addWork("STOPTHREAD");
        processor.wait();
        std::cerr << "stopping Rcount-multireads" << std::endl << std::flush;
    }

private slots:
    void printStatus(QString status){  std::cerr << qPrintable(QTime::currentTime().toString()) << " " << status.toStdString() << std::endl << std::flush; }
    void printProgress(int progress) { std::cerr << qPrintable(QTime::currentTime().toString()) << " " << progress << "%" << std::endl << std::flush; }
    void printError(QString errorMessage) { std::cerr << "ERROR: " << errorMessage.toStdString() << std::endl << std::flush; }
};

#endif // CONSOLERUNNER_H
