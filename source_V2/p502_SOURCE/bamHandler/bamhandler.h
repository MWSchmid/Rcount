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

#ifndef BAMHANDLER_H
#define BAMHANDLER_H

#include <QtCore>
#include <QString>
#include <iostream>
#include <zlib.h>

#define SEQAN_HAS_ZLIB 1
#include "../../seqan/bam_io.h"

#include "bamhelpers.h"

class bamHandler : public QThread
{Q_OBJECT
public:
    //! the function to set everything up - note that if requested, the reader can jump to a certain region
    bamHandler(QString fileNameRead,
               QString fileNameWrite,
               QString mode,
               const int *bufferSize,
               QList<QtBamAlignment> *buffer,
               QWaitCondition *bufferIsNotFull,
               QWaitCondition *bufferIsNotEmpty,
               QMutex *mutex,
               int *usedSpace,
               bool useRegion,
               QString &regionChromStart,
               int &regionStart,
               QString &regionChromEnd,
               int &regionEnd
               );

    int getNumberLines();
    void run();

private:
    QString _fileNameRead;
    QString _fileNameWrite;
    QString _mode;
    const int *_bufferSize;
    QList<QtBamAlignment> *_buffer;
    QWaitCondition *_bufferIsNotFull;
    QWaitCondition *_bufferIsNotEmpty;
    QMutex *_mutex;
    int *_usedSpace;
    bool _useRegion;
    QString _regionChromStart;
    int _regionStart;
    QString _regionChromEnd;
    int _regionEnd;
    bool _producerIsFinished;
    int _processedLines;
    bool _isCanceled;
    QtBamStats _readerStats;
    QtBamStats _writerStats;
    QString _readerStatsString;
    QString _writerStatsString;

    //! functions which are required for chromosome name and ID conversion
    void getIDtoNAME(seqan::BamStream &reader, QMap<int,QString>& IDtoNAME); // mainly for reading alignments
    void getNAMEtoID(seqan::BamStream &reader, QMap<QString,int>& NAMEtoID); // mainly for writing alignments

    //! functions to convert cigar data
    QVector<QtCigarOp> getQtCigar(seqan::String<seqan::CigarElement<> >& seqanCigar); // mainly for reading alignments
    seqan::String<seqan::CigarElement<> > getSeqanCigar(QVector<QtCigarOp>& QtCigar); // mainly for writing alignments

    //! functions to read into, and write from a buffer - are called by run()
    // a function that reads seqan::BamAlignmentRecord, converts them to QtBamAlignments, and stores them in a buffer
    void readAlignments();

    // a function that takes QtBamAlignments from a buffer, converts them to seqan::BamAlignmentRecord, and writes them to a file
    void writeAlignments();

signals:
    void detectedError(QString errorMessage);
    void xLinesProcessed(int lines);
    void statsObtained(QString statsString);

public slots:
    void producerFinished(); // this function is required for the threaded writing. It should somehow only be linked to one thread (if there are several producers, one would maybe have to make an observer thread ?)
    void runCanceled();
};

#endif // BAMHANDLER_H
