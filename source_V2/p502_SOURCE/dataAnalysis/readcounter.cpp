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
#include "../dataStructure/mappingtreeitem.h"
#include "../dataStructure/temporaryresults.h"
#include "../bamHandler/bamhelpers.h"

#include "readcounter.h"

readCounter::readCounter(const int *bufferSizeMAP, QList<QtBamAlignment> *bufferMAP, QWaitCondition *bufferIsNotFullMAP, QWaitCondition *bufferIsNotEmptyMAP, QMutex *mutexMAP, int *usedSpaceMAP,
                         const int *bufferSizeOUT, QList<QtBamAlignment> *bufferOUT, QWaitCondition *bufferIsNotFullOUT, QWaitCondition *bufferIsNotEmptyOUT, QMutex *mutexOUT, int *usedSpaceOUT,
                         database *dataBase, int &minReads, int &maxDist, int &minBelowMaxDist)
{
    //! the pointer to the database
    this->_dataBase = dataBase;
    //! temporary data
    this->_tempData.clear();
    //! a list holding all ambiguous alignments
    this->_ambAlignments.clear();
    this->_ambAlignments.reserve(2000000);
    //! variables related to filtering
    this->_minReads = minReads;
    this->_maxDist = maxDist;
    this->_minBelowMaxDist = minBelowMaxDist;
    //! variables related to alignments that are mapped
    this->_bufferSizeMAP = bufferSizeMAP;
    this->_bufferMAP = bufferMAP;
    this->_bufferIsNotFullMAP = bufferIsNotFullMAP;
    this->_bufferIsNotEmptyMAP = bufferIsNotEmptyMAP;
    this->_mutexMAP = mutexMAP;
    this->_usedSpaceMAP = usedSpaceMAP;
    this->_producersAreFinishedMAP = false;
    //! variables related to the buffer with the alignments that shall be written down
    this->_bufferSizeOUT = bufferSizeOUT;
    this->_bufferOUT = bufferOUT;
    this->_bufferIsNotFullOUT = bufferIsNotFullOUT;
    this->_bufferIsNotEmptyOUT = bufferIsNotEmptyOUT;
    this->_mutexOUT = mutexOUT;
    this->_usedSpaceOUT = usedSpaceOUT;
    //! in case one interrupts the analysis
    this->_isCanceled = false;
}

// this one here should not be link to one producer only, but several of them
void readCounter::producersAreFinished() {
    this->_producersAreFinishedMAP = true;
    this->_mutexMAP->lock();
    this->_bufferIsNotEmptyMAP->wakeAll();
    this->_mutexMAP->unlock();
}


//! the first round - simple adding - we directly access the itemdata.
void readCounter::countFirstRound(QtBamAlignment &al, mappingTreeItem &item)
{
    // get the name of the feature (0 is the forced NameCol)
    QString itemName = item._self->data(0).toString();

    // get the minDist - you need the position of the aligment (note: if on forward strand, the endposition of the read is required)
    int end = al.getEnd();
    int minDist = this->_dataBase->getThreePrimeDist(item._self, al._position, end);
    bool addToMinDist =  minDist < this->_maxDist;

    // check if the sample and the itemName already exist in the tempData
    if ( !this->_tempData.contains(al._sample) ) {
        QHash<QString, temporaryResults> newHash;
        this->_tempData.insert(al._sample, newHash);
    }

    // add an empty entry if it does not exist yet
    if ( !this->_tempData[al._sample].contains(itemName) ) {
        temporaryResults newEntry;
        this->_tempData[al._sample].insert(itemName, newEntry);
    }

    // add the weight to the count and replace the mindist if necessary
    this->_tempData[al._sample][itemName].firstRound(al._weight, addToMinDist, al.IsAmbiguous());

    // do the same for all children
    foreach (mappingTreeItem child, item._children) {
        this->countFirstRound(al, child);
    }
}

//! the function that adds the allocated ambiguous hits
void readCounter::countSecondRound(QtBamAlignment &al, mappingTreeItem &item, float &alloSum, int &numPassed)
{
    // get the name of the feature (0 is the forced NameCol)
    QString itemName = item._self->data(0).toString();

    // get the minDist - you need the position of the aligment (note: if on forward strand, the endposition of the read is required)
    int end = al.getEnd();
    int minDist = this->_dataBase->getThreePrimeDist(item._self, al._position, end);
    bool addToMinDist =  minDist < this->_maxDist;

    // get the new weight
    float newWeight;
    if (alloSum == 0) { newWeight = (1.0 / numPassed)*al._weight; }
    else { newWeight = (this->_tempData[al._sample].value(itemName)._sumUnAmb / alloSum)*al._weight; }

    // add it and replace the mindist if necessary
    this->_tempData[al._sample][itemName].secondRound(newWeight, addToMinDist);

    // do the same for all children
    foreach (mappingTreeItem child, item._children) {
        this->countSecondRound(al, child, alloSum, numPassed);
    }
}

//! the function that allocates ambiguous alignments
void readCounter::allocate()
{
    // variables
    QString curName;
    float alloSum;
    int numPassed;
    QList<mappingTreeItem> toAdd;

    foreach (QtBamAlignment al, this->_ambAlignments) {
        // get the sum of unambiguous alignments and the number of valid features
        alloSum = 0;
        numPassed = 0;
        toAdd.clear();
        foreach (mappingTreeItem curLocusBranch, al._mappings) {
            curName = curLocusBranch._self->data(0).toString(); // 0 is by force the name column
            if ( this->_tempData[al._sample].contains(curName) ) {
                // check if it mapping is valid from the first round
                if ( this->_tempData[al._sample][curName].isValidFirstRound(this->_minReads, this->_minBelowMaxDist) ) {
                    alloSum += this->_tempData[al._sample].value(curName)._sumUnAmb;
                    toAdd.append(curLocusBranch);
                }
            }
        }
        // add the counts of the ones that are valid
        numPassed = toAdd.count();
        foreach (mappingTreeItem curLocusBranch, toAdd) {
            this->countSecondRound(al, curLocusBranch, alloSum, numPassed);
        }
    }
    // remove all ambiguous alignments from storage
    this->_ambAlignments.clear();
}

void readCounter::runCanceled() {
    this->_isCanceled = true;
    //! TODO - implement this in a correct way
}

//! while running, do the whole job: first normal adding, then filter, then distribute ambiguous
void readCounter::run() {

    QtBamAlignment curItem;
    bool worked;
    //QString curName;

    //! fetch and add (via temporaryResults)
    while ( true ) {
        //! fetch
        // start control first buffer (MAP)
        this->_mutexMAP->lock();
        while ( (*(this->_usedSpaceMAP) == 0) && (!this->_producersAreFinishedMAP) ) {
            this->_bufferIsNotEmptyMAP->wait(this->_mutexMAP);
        }
        if ((*this->_usedSpaceMAP) == 0) {
            this->_mutexMAP->unlock();
            break;
        }
        curItem = this->_bufferMAP->takeFirst(); //! 8. march 2012 changed to takeFirst
        --*(this->_usedSpaceMAP);
        this->_bufferIsNotFullMAP->wakeAll();
        this->_mutexMAP->unlock();
        // end control first buffer (MAP)

        // if a valid mapping, add the counts
        if ( !curItem.IsSkipped() ) {
            // iterate over the mappings and add/update
            // Variant A: temporaryResults (may be faster than the multiple QVariant conversions and a bit more convenient)
            // Variant B: the database entry directly (no idea if this is really save)
            foreach (mappingTreeItem curLocusBranch, curItem._mappings) {
                //curName = curLocusBranch._self->data(0).toString();
                //std::cerr << curName.toStdString() << std::endl << std::flush;
                this->countFirstRound(curItem, curLocusBranch);
            }
            // store ambiguous alignments
            if ( curItem.IsAmbiguous() ) {
                //! in case we move to another chromosome, process the previous alignments to save memory
                if ( !this->_ambAlignments.isEmpty() ) {
                    if (curItem._refName != this->_ambAlignments.last()._refName) {
                        this->allocate();
                    }
                }
                this->_ambAlignments.append(curItem);
                //! REMOVE THE SEQUENCES TO SAVE MEMORY - already done while reading now
                this->_ambAlignments.last()._name = "";
                //this->_ambAlignments.last()._alignedBases = "";
                this->_ambAlignments.last()._qualities = "";
                this->_ambAlignments.last()._queryBases = "";
            }
        }

        // start control second buffer (OUT)
        this->_mutexOUT->lock();
        while (*(this->_usedSpaceOUT) == *(this->_bufferSizeOUT))
            this->_bufferIsNotFullOUT->wait(this->_mutexOUT);
        this->_bufferOUT->append(curItem);
        ++*(this->_usedSpaceOUT);
        this->_bufferIsNotEmptyOUT->wakeAll();
        this->_mutexOUT->unlock();
        // end control second buffer (OUT)
    }

    //! distribute ambiguous and add (first filtering is done here)
    if (!this->_isCanceled) {
        std::cerr << "allocating" << std::endl << std::flush;
        this->allocate();
    }

    //! finally update the database (second filtering is done here) -> maybe one should call this outside of this class
    if (!this->_isCanceled) {
        std::cerr << "updating database" << std::endl << std::flush;
        worked = this->_dataBase->updateDataSampleWise(this->_tempData, this->_minReads, this->_minBelowMaxDist);
    }

    // also the temporary data
    this->_tempData.clear();

    //! TODO: emit signal if it did not work

    std::cerr << "counter finished" << std::endl << std::flush;
}
