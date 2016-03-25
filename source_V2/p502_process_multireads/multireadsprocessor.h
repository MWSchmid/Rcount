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

#ifndef MULTIREADSPROCESSOR_H
#define MULTIREADSPROCESSOR_H

#include <QtWidgets>
#include <QMap>
#include "processMultiClasses.h"
//#include "../seqan/stream.h"
//#include "../seqan/bam_io.h"

class multireadsProcessor: public QThread
{ Q_OBJECT
private:
    //! some stuff
    bool _allFine;
    QString _errorMessage;
    float _progress;
    bool _isCancelled;

    //! variables used for the thread control
    QQueue<QString> _work; // a queue with the project files that shall be processed
    QWaitCondition _workAdded;
    QMutex _mutex;

public:
    multireadsProcessor();
    ~multireadsProcessor();
    void addWork(QString workString); // will add some work to the queue

protected:
    void run();

private:
    bool doProcess(QString workString);
    quint64 getNumberLines(QString fileName);
    QMap<uint, QString> getIDtoNAME(QString fileName);
    bool copy_driver(QString& bamfile, QString& new_bamfile, quint64 &numLines);
    bool load_driver(QString& bamfile, multireadcollection& multi_reads, quint64 &numLines);
    bool write_driver(QString& bamfile, multireadcollection& multi_reads, QString& new_bamfile, quint64 &numLines);
    quint64 process_one_chromosome(seqan::BamStream& reader, seqan::BamIndex<seqan::Bai>& baiIndex, uint& refNum, multireadcollection& multi_reads);

public slots:
    void cancelProcessing();

signals:
    void errorMessage(QString message);
    void processStatus(QString status); // signals what is going on
    void processProgress(int progress);
    void idleAgain();
    void workFinished(QString workString);
};

#endif // MULTIREADSPROCESSOR_H
