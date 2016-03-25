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

#ifndef READMAPPER_H
#define READMAPPER_H

#include <QtCore>
#include <iostream>

#include "../dataStructure/databaseitem.h"
#include "../dataStructure/database.h"
#include "../bamHandler/bamhelpers.h"

/*!
  this class is the base class for the read mapper classes defined in readMapperVariants.h/cpp. It implements everything except the mapAlignment function.
  This virtual function is then defined in the derived classes, to handle the alignment according to its properties (fragment, PE, MP, stranded/not-stranded).
*/

class readMapper: public QThread
{ Q_OBJECT
public:
    readMapper(const int *bufferSizeBAM, QList<QtBamAlignment> *bufferBAM, QWaitCondition *bufferIsNotFullBAM, QWaitCondition *bufferIsNotEmptyBAM, QMutex *mutexBAM, int *usedSpaceBAM,
               const int *bufferSizeMAP, QList<QtBamAlignment> *bufferMAP, QWaitCondition *bufferIsNotFullMAP, QWaitCondition *bufferIsNotEmptyMAP, QMutex *mutexMAP, int *usedSpaceMAP,
               database *dataBase, bool &stranded, bool &antisense, bool &multi);
    void run();
private:
    //! a pointer to the dataBase that is used to map the reads
    database *_dataBase;
    //! mappingmode (stranded or not)
    bool _stranded;
    bool _antisense; //if stranded test for opposite strands between gene and read
    bool _multi;
    //! variables related to the buffer with the alignments that are unmapped
    const int *_bufferSizeBAM;
    QList<QtBamAlignment> *_bufferBAM;
    QWaitCondition *_bufferIsNotFullBAM;
    QWaitCondition *_bufferIsNotEmptyBAM;
    QMutex *_mutexBAM;
    int *_usedSpaceBAM;
    bool _producerIsFinishedBAM;
    //! variables related to the buffer with the alignments that are mapped
    const int *_bufferSizeMAP;
    QList<QtBamAlignment> *_bufferMAP;
    QWaitCondition *_bufferIsNotFullMAP;
    QWaitCondition *_bufferIsNotEmptyMAP;
    QMutex *_mutexMAP;
    int *_usedSpaceMAP;
    //! in case one interrupts the analysis
    bool _isCanceled;
public slots:
    void producerFinished();
    void runCanceled();
};

#endif // READMAPPER_H
