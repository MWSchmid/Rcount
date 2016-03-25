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

#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H

//! make this as a general data analysis handler where one can store settings and so on
//! NOTE: the threading solution is from chapter 14 of the Qt Book (ImagePro Example)

#include "../dataStructure/databaseitem.h"
#include "../dataStructure/database.h"
#include "../bamHandler/bamhandler.h"
#include "../bamHandler/bamhelpers.h"
#include "readmapper.h"
#include "readcounter.h"

class dataAnalysis: public QThread
{ Q_OBJECT
public:
    //! info about the run
    bool _allFine;
    QString _errorMessage;
    float _progress;
    int _numLines;
    QString _projectName;
    bool _writeCounts;
    QString _readerStatsString;
    QString _writerStatsString;

    //! input and output files
    // database XML
    QString _dataBaseInfile;
    QString _dataBaseOutfile;
    // alignments BAM
    QString _readsInfile;
    QString _readsOutfile;
    // file with the results
    QString _countTableFile;

    //! parameters for mapping and counting
    bool _useMulti;
    bool _stranded;
    bool _antisense;
    int _minReads;
    int _maxDist;
    int _minBelowMaxDist;
    bool _useRegion;
    QString _regionStartName;
    int _regionStart;
    QString _regionEndName;
    int _regionEnd;

    //! database connection and settings
    database *_dataBase;
    QVector<QVariant> _headers;
    uint _indexStepSize;

    //! buffer and coordination related things
    // variables related to the buffer with the alignments that are unmapped
    int _bufferSizeBAM;
    QList<QtBamAlignment> _bufferBAM;
    QWaitCondition _bufferIsNotFullBAM;
    QWaitCondition _bufferIsNotEmptyBAM;
    QMutex _mutexBAM;
    int _usedSpaceBAM;
    // variables related to the buffer with the alignments that are mapped
    int _bufferSizeMAP;
    QList<QtBamAlignment> _bufferMAP;
    QWaitCondition _bufferIsNotFullMAP;
    QWaitCondition _bufferIsNotEmptyMAP;
    QMutex _mutexMAP;
    int _usedSpaceMAP;
    // variables related to the buffer with the alignments that shall be written down
    int _bufferSizeOUT;
    QList<QtBamAlignment> _bufferOUT;
    QWaitCondition _bufferIsNotFullOUT;
    QWaitCondition _bufferIsNotEmptyOUT;
    QMutex _mutexOUT;
    int _usedSpaceOUT;

    //! check if the db is loaded or not (used to delete it)
    bool _dataBasePresent;

    //! variables used for the thread control
    QQueue<QString> _projects; // a queue with the project files that shall be processed - this can also hold RUNCANCELLED - will emit the stopThreads() signal
    QWaitCondition _projectAdded;
    QMutex _mutex;


public:
    // constructor shall set some default values
    // in the destructor one needs to take care everything unnecessary is removed
    // especially: make sure that the database is destroyed if it was generated on the heap (do this things in the runner)
    dataAnalysis();
    ~dataAnalysis();
    void addProject(QString fileName); // will add a project to the queue

protected:
    void run();

private:
    bool readProject(const QString &fileName);
    bool writeProject(const QString &fileName);
    void readElement(QXmlStreamReader* reader);
    bool doAnalysis();
    void deleteDataBase(); // if one is present, it will be deleted

private slots:
    //! THIS ONES MAY NEED A MUTEX
    void readerLines(int numLines); // not used at the moment
    void writerLines(int numLines);
    void readerStats(QString statsString);
    void writerStats(QString statsString);

public slots:
    void cancelDataAnalysis();

signals:
    void errorMessage(QString message);
    void stopThreads();
    void analysisStatus(QString status); // signals what is going on
    void analysisProgress(int progress);
    void analysisCountsWritten(QString fileName);
    void everythingProcessed();
    void idleAgain();
    void readerStatsObtained(QString statsString);
    void writerStatsObtained(QString statsString);
};

#endif // DATAANALYSIS_H
