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

#include "../dataStructure/databaseitem.h"
#include "../dataStructure/database.h"
#include "../bamHandler/bamhelpers.h"

#include "readmapper.h"

readMapper::readMapper(const int *bufferSizeBAM, QList<QtBamAlignment> *bufferBAM, QWaitCondition *bufferIsNotFullBAM, QWaitCondition *bufferIsNotEmptyBAM, QMutex *mutexBAM, int *usedSpaceBAM,
                       const int *bufferSizeMAP, QList<QtBamAlignment> *bufferMAP, QWaitCondition *bufferIsNotFullMAP, QWaitCondition *bufferIsNotEmptyMAP, QMutex *mutexMAP, int *usedSpaceMAP,
                       database *dataBase, bool &stranded, bool &antisense, bool &multi)
{
    //! the pointer to the database
    this->_dataBase = dataBase;
    //! mappingmode
    this->_stranded = stranded;
    this->_antisense = antisense;
    this->_multi = multi;
    //! variables related to alignments that are unmapped
    this->_bufferSizeBAM = bufferSizeBAM;
    this->_bufferBAM = bufferBAM;
    this->_bufferIsNotFullBAM = bufferIsNotFullBAM;
    this->_bufferIsNotEmptyBAM = bufferIsNotEmptyBAM;
    this->_mutexBAM = mutexBAM;
    this->_usedSpaceBAM = usedSpaceBAM;
    this->_producerIsFinishedBAM = false;
    //! variables related to alignments that are mapped
    this->_bufferSizeMAP = bufferSizeMAP;
    this->_bufferMAP = bufferMAP;
    this->_bufferIsNotFullMAP = bufferIsNotFullMAP;
    this->_bufferIsNotEmptyMAP = bufferIsNotEmptyMAP;
    this->_mutexMAP = mutexMAP;
    this->_usedSpaceMAP = usedSpaceMAP;
    //! in case one interrupts the analysis
    this->_isCanceled = false;
}

// this one can be directly linked to the bamHandler
void readMapper::producerFinished() {
    this->_producerIsFinishedBAM = true;
    this->_mutexBAM->lock();
    this->_bufferIsNotEmptyBAM->wakeAll();
    this->_mutexBAM->unlock();
}

void readMapper::runCanceled()
{
    this->_isCanceled = true;
    //! TODO - implement this in a correct way
}

//! this version worked in tha bamHandlerTest (note that I had to put the wakeUpAll in the slot)
void readMapper::run()
{
    QtBamAlignment curItem;
    while ( true ) {
        //! fetch an alignment
        // start control the first buffer (BAM)
        this->_mutexBAM->lock();
        while ( (*(this->_usedSpaceBAM) == 0) && (!this->_producerIsFinishedBAM) ) {
            //std::cerr << "entering wait" << std::endl << std::flush;
            this->_bufferIsNotEmptyBAM->wait(this->_mutexBAM);
        }
        if ((*this->_usedSpaceBAM) == 0) {
            //std::cerr << "exiting loop" << std::endl << std::flush;
            this->_mutexBAM->unlock();
            break;
        }
        // take the alignment
        curItem = this->_bufferBAM->takeFirst();//! 8. march 2012 changed to takeFirst
        --*(this->_usedSpaceBAM);
        this->_bufferIsNotFullBAM->wakeAll();
        this->_mutexBAM->unlock();
        // end control the first buffer (BAM)

        //! map the alignment if it was aligned
        if (curItem.IsAligned()) {
            if ( (curItem._numal > 1) && !this->_multi ) {
                curItem.setIsSkipped(true);
            } else {
                //! skip all the second fragments of a PE read - in case of tophat the isSecondMate works
                if (curItem.IsSecondMate()) {
                    curItem.setIsSkipped(true);
                }
                else {
                    if (curItem.IsGapped()) { this->_dataBase->alignmentMapFragmentGapped(curItem, this->_stranded, this->_antisense); }
                    else { this->_dataBase->alignmentMapFragment(curItem, this->_stranded, this->_antisense); }
                }
            }
        } else {
            curItem.setIsSkipped(true);
        }
        //if (!curItem.IsSkipped) {}
        //! add the alignment to the buffer holding the mapped alignments
        // start control the second buffer (MAP)
        this->_mutexMAP->lock();
        while (*(this->_usedSpaceMAP) == *(this->_bufferSizeMAP))
            this->_bufferIsNotFullMAP->wait(this->_mutexMAP);
        this->_bufferMAP->append(curItem);
        ++*(this->_usedSpaceMAP);
        this->_bufferIsNotEmptyMAP->wakeAll();
        this->_mutexMAP->unlock();
        // end control the second buffer (MAP)
    }
}
