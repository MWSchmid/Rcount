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

#ifndef READCOUNTER_H
#define READCOUNTER_H

#include <QtCore>
#include <iostream>

#include "../dataStructure/databaseitem.h"
#include "../dataStructure/database.h"
#include "../dataStructure/mappingtreeitem.h"
#include "../dataStructure/temporaryresults.h"
#include "../bamHandler/bamhelpers.h"

class readCounter: public QThread
{ Q_OBJECT
public:
    readCounter(const int *bufferSizeMAP, QList<QtBamAlignment> *bufferMAP, QWaitCondition *bufferIsNotFullMAP, QWaitCondition *bufferIsNotEmptyMAP, QMutex *mutexMAP, int *usedSpaceMAP,
                const int *bufferSizeOUT, QList<QtBamAlignment> *bufferOUT, QWaitCondition *bufferIsNotFullOUT, QWaitCondition *bufferIsNotEmptyOUT, QMutex *mutexOUT, int *usedSpaceOUT,
                database *dataBase, int &minReads, int &maxDist, int &minBelowMaxDist);
    void run();
private:
    //! a pointer to the dataBase that is used to map the reads
    database *_dataBase;
    //! storing temporary results
    //QHash<QString, temporaryResults> _tempData;
    QHash<QString, QHash<QString, temporaryResults> > _tempData;
    //! a list holding all ambiguous alignments
    QList<QtBamAlignment> _ambAlignments;
    //! variables related to filtering
    int _minReads; // will check bigger than -1 would use all reads
    int _maxDist;  // set to a very high value if not required
    int _minBelowMaxDist; // the minimal numbers of reads/hits below the maxDist
    //! variables related to the buffer with the alignments that are mapped
    const int *_bufferSizeMAP;
    QList<QtBamAlignment> *_bufferMAP;
    QWaitCondition *_bufferIsNotFullMAP;
    QWaitCondition *_bufferIsNotEmptyMAP;
    QMutex *_mutexMAP;
    int *_usedSpaceMAP;
    bool _producersAreFinishedMAP;
    //! variables related to the buffer with the alignments that shall be written down
    const int *_bufferSizeOUT;
    QList<QtBamAlignment> *_bufferOUT;
    QWaitCondition *_bufferIsNotFullOUT;
    QWaitCondition *_bufferIsNotEmptyOUT;
    QMutex *_mutexOUT;
    int *_usedSpaceOUT;
    //! in case one interrupts the analysis
    bool _isCanceled;
private:
    void countFirstRound(QtBamAlignment &al, mappingTreeItem &item);
    void countSecondRound(QtBamAlignment &al, mappingTreeItem &item, float &alloSum, int &numPassed);
    void allocate();
public slots:
    void producersAreFinished(); //! well - I thought that it may be possible with several mappers - no idea if it would work. With a single mapper, one can directly link it, otherwise one would require an additional slot in the dataAnalysis class and a respective signal
    void runCanceled();
};

#endif // READCOUNTER_H
