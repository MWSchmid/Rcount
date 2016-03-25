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

#include <QtWidgets>
#include <iostream>
#include "databaseitem.h"
#include "databasereader.h"
#include "databasewriter.h"
#include "database.h"
#include "mappingtreeitem.h"
#include "temporaryresults.h"
#include "../bamHandler/bamhelpers.h"

/*!
 things related to construction, data loading and so on
*/

// constructor
database::database(const QVector<QVariant> &headers, const uint &indexStepSize)
{
    if (headers.at(0) != "Sname") {
        std::cerr << "Database Error: the headers MUST include the attribute Sname" << std::endl;
    }

    // give initial nonsense values for the column numbers
    this->NAMECOL = 0; // this is really like this (see above)
    this->CHROMCOL = 0;
    this->STRANDCOL = 0;
    this->STARTCOL = 0;
    this->ENDCOL = 0;
    // optional columns - give a too big initial value
    this->SOURCECOL = headers.count();
    this->FEATURECOL = headers.count();
    this->ASSEMBLEDFEATURECOL = headers.count();
    this->PHASECOL = headers.count();
    this->SCORECOL = headers.count();
    this->PRIORITYCOL = headers.count();
    this->SUMUNAMBCOL = headers.count();
    this->SUMAMBCOL = headers.count();
    this->SUMALLOCOL = headers.count();
    this->MINDISTUNAMBCOL = headers.count();
    this->MINDISTAMBCOL = headers.count();
    this->MINDISTALLOCOL = headers.count();
    this->TOTCOL = headers.count();
    this->VALIDCOL = headers.count();

    // get the real column numbers
    for (int i = 0; i < headers.count(); ++i) {
        //! compulsory columns
        if (headers.at(i).toString() == "Schrom") { this->CHROMCOL = i; }
        else if (headers.at(i).toString() == "Sstrand") { this->STRANDCOL = i; }
        else if (headers.at(i).toString() == "Ustart") { this->STARTCOL = i; }
        else if (headers.at(i).toString() == "Uend") { this->ENDCOL = i; }
        //! optional columns - GFF
        else if (headers.at(i).toString() == "Ssource") { this->SOURCECOL = i; }
        else if (headers.at(i).toString() == "Sfeature") { this->FEATURECOL = i; }
        else if (headers.at(i).toString() == "SassembledFeature") { this->ASSEMBLEDFEATURECOL = i; }
        else if (headers.at(i).toString() == "Sphase") { this->PHASECOL = i; }
        else if (headers.at(i).toString() == "Fscore") { this->SCORECOL = i; }
        //! optional columns - analysis
        else if (headers.at(i).toString() == "Upriority") { this->PRIORITYCOL = i; }
        else if (headers.at(i).toString() == "Fsumunamb") { this->SUMUNAMBCOL = i; }
        else if (headers.at(i).toString() == "Fsumamb") { this->SUMAMBCOL = i; }
        else if (headers.at(i).toString() == "Fsumallo") { this->SUMALLOCOL = i; }
        else if (headers.at(i).toString() == "Udistunamb") { this->MINDISTUNAMBCOL = i; }
        else if (headers.at(i).toString() == "Udistamb") { this->MINDISTAMBCOL = i; }
        else if (headers.at(i).toString() == "Udistallo") { this->MINDISTALLOCOL = i; }
        else if (headers.at(i).toString() == "Ftothits") { this->TOTCOL = i; }
        else if (headers.at(i).toString() == "Uvalid") { this->VALIDCOL = i; }
        else { continue; }
    }
    // check if all the columns were found
    if ( (this->CHROMCOL == 0) || (this->STRANDCOL == 0) || (this->STARTCOL == 0) || (this->ENDCOL == 0) ) {
        std::cerr << "Database Error: the headers MUST include Schrom, Sstrand, Ustart, Uend" << std::endl;
    }

    this->stepSize = indexStepSize;
    this->rootItem = new databaseItem(ROOT, headers);
}

// destructor
database::~database()
{
    delete this->rootItem;
}

//! retrieve headers and step size
uint database::getStepSize() const
{
    return (this->stepSize);
}

QVector<QVariant> database::getHeaders() const
{
    return (this->rootItem->itemData);
}


//! reading and writing the database

// read an XML
bool database::readData(const QString &fileName)
{
    // init a reader
    databaseReader reader(this);

    // read through the file
    if ( reader.readFile(fileName) ) {
        this->createIndex();
        return(true);
    } else {
        return(false);
    }
}

// write an XML
bool database::writeData(const QString &fileName)
{
    // init a writer
    databaseWriter writer(this);

    // write all the entries
    return( writer.writeFile(fileName) );
}

// write a table with counts
bool database::writeCountTable(const QString &fileName)
{
    // check if the necessary columns exist
    bool rval = this->checkColumns();

    // only then write it
    if (rval) {
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                      << ": " << qPrintable(file.errorString())
                      << std::endl;
            rval = false;
        }
        else {
            QTextStream out(&file);
            out.setCodec("UTF-8");

            // File writing
            foreach (databaseItem* curItem, this->rootItem->childItems) {
                // skip the ones that are not valid
                if (curItem->data(this->VALIDCOL).toUInt() == 0) { continue; } //! TODO: remove if final!
                out <<
                       curItem->data(this->NAMECOL).toString() << '\t' <<
                       curItem->data(this->SUMUNAMBCOL).toFloat() << '\t' <<
                       curItem->data(this->SUMAMBCOL).toFloat() << '\t' <<
                       curItem->data(this->SUMALLOCOL).toFloat() << '\t' <<
                       curItem->data(this->MINDISTUNAMBCOL).toUInt() << '\t' <<
                       curItem->data(this->MINDISTAMBCOL).toUInt() << '\t' <<
                       curItem->data(this->MINDISTALLOCOL).toUInt() << '\t' <<
                       curItem->data(this->TOTCOL).toFloat() << '\t' <<
                       curItem->data(this->VALIDCOL).toUInt() << '\n';
            }
            out.flush();

            // close the file and check if it worked
            file.close();
            if (file.error() != QFile::NoError) {
                std::cerr << "Error: Cannot write file " << qPrintable(fileName)
                          << ": " << qPrintable(file.errorString())
                          << std::endl;
                rval = false;
            }
            else { rval = true; }
        }
    }

    return(rval);
}

/*!
  some old functions reimplemented X]
*/

void database::print_time(const QString &message) const
{
    std::cerr << "#TIME INFO#" << '\t' << qPrintable(message) << std::endl
              << "#TIME INFO#" << '\t' << qPrintable(QTime::currentTime().toString()) << std::endl << std::flush;
}

/*!
 index necessary for fast positional access
*/

uint database::getOffset(const uint& position) const
{
        uint x = (position / this->stepSize); // uses integer division
        return(x);
}


void database::createIndex()
{
    QString chrom; // chromosome of current locus
    uint start; // start of the current locus
    uint end; // end of the locus
    uint startStep;
    uint endStep;
    uint stepCounter;
    QPair<QString,uint> addKey; // the current key (this is a pair of chrom and the stepped end)
    QVector<databaseItem*> emptyVec; // an empty vector that should afterwards hold the pointers
    emptyVec.reserve(100);
    foreach (databaseItem* locus, this->rootItem->childItems) {
        chrom = locus->data(this->CHROMCOL).toString();
        start = locus->data(this->STARTCOL).toUInt();
        end = locus->data(this->ENDCOL).toUInt();
        startStep = this->getOffset(start);
        endStep = this->getOffset(end);
        for (stepCounter = startStep; stepCounter <= endStep; ++stepCounter) {
            addKey = qMakePair(chrom, stepCounter);
            if (!this->CPindex.contains(addKey)) { this->CPindex.insert(addKey, emptyVec); }
            this->CPindex[addKey].append(locus);
        }
    }
}


/*!
 things related to basic data access
*/

// get length of an item
int database::getLen(databaseItem *item) const
{
    int out;
    out = (item->data(this->ENDCOL).toInt() - item->data(this->STARTCOL).toInt());
    return(out);
}

// get distance to START or END of an item
int database::getStartDist(databaseItem *item, int &pos) const
{
    int rval = 0;
    if (item->type == ROOT) { rval = 0; }
    else if (item->type == LOCUS) {
        // check the smallest distance of one of the transcripts
        bool firstTranscript = true;
        int curDist;
        foreach(databaseItem* child, item->childItems) {
            if (child->type == TRANSCRIPT) {
                curDist = this->getStartDist(child, pos);
                if (firstTranscript) { firstTranscript = false; rval = curDist; }
                if (curDist < rval) { rval = curDist; }
            }
        }
    }
    else if (item->type == TRANSCRIPT) {
        // correct for introns and flanking regions
        foreach(databaseItem* child, item->childItems) {
            if (child->type == EXON) {
                if (child->data(this->ENDCOL).toInt() < pos) { rval += this->getLen(child); }
                if (child->mapPosition(pos, this->STARTCOL, this->ENDCOL)) { rval += this->getStartDist(child, pos); }
            }
        }
    }
    else {
        // simple distance for the lowest features
        rval = pos - item->data(this->STARTCOL).toInt();
    }
    // return
    return(rval);
}

int database::getEndDist(databaseItem *item, int &pos) const
{
    int rval = 0;
    if (item->type == ROOT) { rval = 0; }
    else if (item->type == LOCUS) {
        // check the smallest distance of one of the transcripts
        bool firstTranscript = true;
        int curDist;
        foreach(databaseItem* child, item->childItems) {
            if (child->type == TRANSCRIPT) {
                curDist = this->getEndDist(child, pos);
                if (firstTranscript) { firstTranscript = false; rval = curDist; }
                if (curDist < rval) { rval = curDist; }
            }
        }
    }
    else if (item->type == TRANSCRIPT) {
        // correct for introns and flanking regions
        foreach(databaseItem* child, item->childItems) {
            if (child->type == EXON) {
                if (child->data(this->STARTCOL).toInt() > pos) { rval += this->getLen(child); }
                if (child->mapPosition(pos, this->STARTCOL, this->ENDCOL)) { rval += this->getEndDist(child, pos); }
            }
        }
    }
    else {
        // simple distance for the lowest features
        rval = item->data(this->ENDCOL).toInt() - pos;
    }
    // return
    return(rval);
}

// get distance to three or five prime end
int database::getThreePrimeDist(databaseItem *item, int &start, int &end) const
{
    int rval = 0;
    if (item->data(this->STRANDCOL).toString() == "-") { rval = this->getStartDist(item, start); }
    else { rval = this->getEndDist(item, end); }
    return(rval);
}

int database::getFivePrimeDist(databaseItem *item, int &start, int &end) const
{
    int rval = 0;
    if (item->data(this->STRANDCOL).toString() == "-") { rval = this->getEndDist(item, end); }
    else { rval = this->getStartDist(item, start); }
    return(rval);
}

QList<databaseItem*> database::getLoci(QList<databaseItem*> &foi) const
{
    QList<databaseItem*> out;
    databaseItem* curEl; // the current element to potentially add (locus)
    bool alreadyAdded = false;

    foreach (databaseItem* curItem, foi) {
        curEl = curItem->topParent();
        foreach (databaseItem* prevEl, out) {
            if (curEl->data(0) == prevEl->data(0)) {
                alreadyAdded = true;
                break;
            }
        }
        if (!alreadyAdded) { out.append(curEl); }
    }

    return(out);
}

QList<databaseItem*> database::getTranscripts(QList<databaseItem*> &foi) const
{
    QList<databaseItem*> out;
    databaseItem* curEl; // the current element to potentially add (transcript)
    bool alreadyAdded = false;

    foreach (databaseItem* curItem, foi) {
        if (curItem->type == LOCUS || curItem->type == ROOT ) { continue; }
        else if (curItem->type == TRANSCRIPT) { curEl = curItem; }
        else { curEl = curItem->parent(); }
        foreach (databaseItem* prevEl, out) {
            if (curEl->data(0) == prevEl->data(0)) {
                alreadyAdded = true;
                break;
            }
        }
        if (!alreadyAdded) { out.append(curEl); }
    }
    return(out);
}


/*!
  basic data analysis modifiers - I decided to keep the database as passive as possible (all via temporaryResults)
  */

bool database::checkColumns() const
{
    bool rval = ( (this->SUMUNAMBCOL != this->rootItem->itemData.count()) &&
                  (this->SUMAMBCOL != this->rootItem->itemData.count()) &&
                  (this->SUMALLOCOL != this->rootItem->itemData.count()) &&
                  (this->MINDISTUNAMBCOL != this->rootItem->itemData.count()) &&
                  (this->MINDISTAMBCOL != this->rootItem->itemData.count()) &&
                  (this->MINDISTALLOCOL != this->rootItem->itemData.count()) &&
                  (this->TOTCOL != this->rootItem->itemData.count()) &&
                  (this->VALIDCOL != this->rootItem->itemData.count())
                );
    return(rval);
}

void database::updateDataElement(databaseItem* item, const QHash<QString, temporaryResults> &tempData, const int &minReads, const int &minBelowMaxDist)
{
    // the name of the item
    QString itemName = item->data(this->NAMECOL).toString();
    temporaryResults curRes;

    if (tempData.contains(itemName)) {
        curRes = tempData.value(itemName);
    }

    // update the element
    item->setData(this->SUMUNAMBCOL, curRes._sumUnAmb);
    item->setData(this->SUMAMBCOL, curRes._sumAmb);
    item->setData(this->SUMALLOCOL, curRes._sumAllo);
    item->setData(this->MINDISTUNAMBCOL, curRes._minDistUnAmb);
    item->setData(this->MINDISTAMBCOL, curRes._minDistAmb);
    item->setData(this->MINDISTALLOCOL, curRes._minDistAllo);
    item->setData(this->TOTCOL, (curRes._sumUnAmb + curRes._sumAllo));
    if (curRes.isValidSecondRound(minReads, minBelowMaxDist)) {
        item->setData(this->VALIDCOL, 1);
    } else {
        item->setData(this->VALIDCOL, 0);
    }

    // do it for all child elements
    foreach (databaseItem* child, item->childItems) {
        this->updateDataElement(child, tempData, minReads, minBelowMaxDist);
    }
}

bool database::updateData(const QHash<QString, temporaryResults> &tempData, const int &minReads, const int &minBelowMaxDist)
{
    // check if the columns exist
    bool rval = this->checkColumns();

    if (rval) {
        foreach (databaseItem* curItem, this->rootItem->childItems) {
            this->updateDataElement(curItem, tempData, minReads, minBelowMaxDist);
        }
    }

    return(rval);
}


/*!
  VECTOR BASED RETURNS
  */


/*!
  mapping reporting HITS TO ALL LOCI
  Lmap for locus mapping only
  */

//! not strand specific
void database::LmapPosition(const QString &chrom, const uint &position, QVector<databaseItem*> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(position));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        if ( element->mapPosition(position, this->STARTCOL, this->ENDCOL) ) {
            results.append(element);
        }
    }
}

void database::LmapRange(const QString &chrom, const uint &start, const uint &end, QVector<databaseItem*> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(start));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        if ( element->mapRange(start, end, this->STARTCOL, this->ENDCOL) ) {
            results.append(element);
        }
    }
}

void database::LmapGap(const QString &chrom, const uint &start, const uint &end, QVector<databaseItem*> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(start));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        if ( element->mapGap(start, end, this->STARTCOL, this->ENDCOL) ) {
            results.append(element);
        }
    }
}

QVector<databaseItem*> database::LmapPositionVec(const QString &chrom, const uint &position) const
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->LmapPosition(chrom, position, out);
    return(out);
}

QVector<databaseItem*> database::LmapRangeVec(const QString &chrom, const uint &start, const uint &end) const
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->LmapRange(chrom, start, end, out);
    return(out);
}

QVector<databaseItem*> database::LmapGapVec(const QString &chrom, const uint &start, const uint &end) const
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->LmapGap(chrom, start, end, out);
    return(out);
}

//! strand specific
void database::LmapPositionStrand(const QString &chrom, const uint &position, const QString &strand, QVector<databaseItem*> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(position));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        if ( element->mapPositionStrand(position, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL) ) {
            results.append(element);
        }
    }
}

void database::LmapRangeStrand(const QString &chrom, const uint &start, const uint &end, const QString &strand, QVector<databaseItem*> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(start));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        if ( element->mapRangeStrand(start, end, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL) ) {
            results.append(element);
        }
    }
}

void database::LmapGapStrand(const QString &chrom, const uint &start, const uint &end, const QString &strand, QVector<databaseItem*> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(start));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        if ( element->mapGapStrand(start, end, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL) ) {
            results.append(element);
        }
    }
}

QVector<databaseItem*> database::LmapPositionVecStrand(const QString &chrom, const uint &position, const QString &strand) const
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->LmapPositionStrand(chrom, position, strand, out);
    return(out);
}

QVector<databaseItem*> database::LmapRangeVecStrand(const QString &chrom, const uint &start, const uint &end, const QString &strand) const
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->LmapRangeStrand(chrom, start, end, strand, out);
    return(out);
}

QVector<databaseItem*> database::LmapGapVecStrand(const QString &chrom, const uint &start, const uint &end, const QString &strand) const
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->LmapGapStrand(chrom, start, end, strand, out);
    return(out);
}

/*!
  RECURSIVE mapping reporting HITS TO ALL FEATURES OF ALL LOCI
  Rmap for recursive mapping (for each locus the hits at the lowest level)
  the ones that are commented out would separate results from different loci
  similar commenting out is also done in the source file
  */

//! not strand specific
QVector<databaseItem*> database::RmapPosition(const QString &chrom, const uint &position) const
{
    //QVector< QVector<databaseItem*> > out;
    QVector<databaseItem*> out;
    out.reserve(10);
    QVector<databaseItem*> loci = this->LmapPositionVec(chrom, position);
    foreach (databaseItem* locus, loci) {
        out += locus->RmapPositionVec(position, this->STARTCOL, this->ENDCOL);
        //out.append(locus->RmapPositionVec(position, this->STARTCOL, this->ENDCOL));
    }
    return(out);
}

QVector<databaseItem*> database::RmapRange(const QString &chrom, const uint &start, const uint &end) const
{
    //QVector< QVector<databaseItem*> > out;
    QVector<databaseItem*> out;
    out.reserve(10);
    QVector<databaseItem*> loci = this->LmapRangeVec(chrom, start, end);
    foreach (databaseItem* locus, loci) {
        out += locus->RmapRangeVec(start, end, this->STARTCOL, this->ENDCOL);
        //out.append(locus->RmapRangeVec(start, end, this->STARTCOL, this->ENDCOL));
    }
    return(out);
}

QVector<databaseItem*> database::RmapGap(const QString &chrom, const uint &start, const uint &end) const
{
    //QVector< QVector<databaseItem*> > out;
    QVector<databaseItem*> out;
    out.reserve(10);
    QVector<databaseItem*> loci = this->LmapGapVec(chrom, start, end);
    foreach (databaseItem* locus, loci) {
        out += locus->RmapGapVec(start, end, this->STARTCOL, this->ENDCOL);
        //out.append(locus->RmapGapVec(start, end, this->STARTCOL, this->ENDCOL));
    }
    return(out);
}


//! strand specific
QVector<databaseItem*> database::RmapPositionStrand(const QString &chrom, const uint &position, const QString &strand) const
{
    //QVector< QVector<databaseItem*> > out;
    QVector<databaseItem*> out;
    out.reserve(10);
    QVector<databaseItem*> loci = this->LmapPositionVecStrand(chrom, position, strand);
    foreach (databaseItem* locus, loci) {
        out += locus->RmapPositionVecStrand(position, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL);
        //out.append(locus->RmapPositionVecStrand(position, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL));
    }
    return(out);
}

QVector<databaseItem*> database::RmapRangeStrand(const QString &chrom, const uint &start, const uint &end, const QString &strand) const
{
    //QVector< QVector<databaseItem*> > out;
    QVector<databaseItem*> out;
    out.reserve(10);
    QVector<databaseItem*> loci = this->LmapRangeVecStrand(chrom, start, end, strand);
    foreach (databaseItem* locus, loci) {
        out += locus->RmapRangeVecStrand(start, end, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL);
        //out.append(locus->RmapRangeVecStrand(start, end, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL));
    }
    return(out);
}

QVector<databaseItem*> database::RmapGapStrand(const QString &chrom, const uint &start, const uint &end, const QString &strand) const
{
    //QVector< QVector<databaseItem*> > out;
    QVector<databaseItem*> out;
    out.reserve(10);
    QVector<databaseItem*> loci = this->LmapGapVecStrand(chrom, start, end, strand);
    foreach (databaseItem* locus, loci) {
        out += locus->RmapGapVecStrand(start, end, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL);
        //out.append(locus->RmapGapVecStrand(start, end, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL));
    }
    return(out);
}


/*!
  mapping reporting HITS TO ONLY BEST LOCI
  Lmap for locus mapping only
  */

//! not strand specific
void database::bestLmapPosition(const QString &chrom, const uint &position, QVector<databaseItem*> &results) const
{
    //! TEST
    //std::cerr << "hallo3" << std::endl << std::flush;
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(position));
    //std::cerr << offset.first.toStdString() << '\t' << QString::number(offset.second).toStdString() << std::endl << std::flush;
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        //! TEST
        //std::cerr << "hallo4" << std::endl << std::flush;
        if ( element->mapPosition(position, this->STARTCOL, this->ENDCOL) ) {
            if ( !results.isEmpty() ) {
                if ( element->data(this->PRIORITYCOL).toUInt() > results.last()->data(this->PRIORITYCOL).toUInt() ) { continue; } // the previous one is better
                if ( element->data(this->PRIORITYCOL).toUInt() < results.last()->data(this->PRIORITYCOL).toUInt() ) { results.clear(); }
            }
            results.append(element); // append equal or better loci
        }
    }
}

void database::bestLmapRange(const QString &chrom, const uint &start, const uint &end, QVector<databaseItem*> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(start));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        if ( element->mapRange(start, end, this->STARTCOL, this->ENDCOL) ) {
            if ( !results.isEmpty() ) {
                if ( element->data(this->PRIORITYCOL).toUInt() > results.last()->data(this->PRIORITYCOL).toUInt() ) { continue; } // the previous one is better
                if ( element->data(this->PRIORITYCOL).toUInt() < results.last()->data(this->PRIORITYCOL).toUInt() ) { results.clear(); }
            }
        results.append(element); // append equal or better loci
        }
    }
}

void database::bestLmapGap(const QString &chrom, const uint &start, const uint &end, QVector<databaseItem*> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(start));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        if ( element->mapGap(start, end, this->STARTCOL, this->ENDCOL) ) {
            if ( !results.isEmpty() ) {
                if ( element->data(this->PRIORITYCOL).toUInt() > results.last()->data(this->PRIORITYCOL).toUInt() ) { continue; } // the previous one is better
                if ( element->data(this->PRIORITYCOL).toUInt() < results.last()->data(this->PRIORITYCOL).toUInt() ) { results.clear(); }
            }
            results.append(element); // append equal or better loci
        }
    }
}

QVector<databaseItem*> database::bestLmapPositionVec(const QString &chrom, const uint &position) const
{
    //! TEST
    //std::cerr << "hallo2" << std::endl << std::flush;
    QVector<databaseItem*> out;
    out.reserve(10);
    this->bestLmapPosition(chrom, position, out);
    return(out);
}

QVector<databaseItem*> database::bestLmapRangeVec(const QString &chrom, const uint &start, const uint &end) const
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->bestLmapRange(chrom, start, end, out);
    return(out);
}

QVector<databaseItem*> database::bestLmapGapVec(const QString &chrom, const uint &start, const uint &end) const
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->bestLmapGap(chrom, start, end, out);
    return(out);
}


//! strand specific
void database::bestLmapPositionStrand(const QString &chrom, const uint &position, const QString &strand, QVector<databaseItem*> &results) const
{
    //! TEST
    //std::cerr << "hallo3" << std::endl << std::flush;
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(position));
    //std::cerr << offset.first.toStdString() << '\t' << QString::number(offset.second).toStdString() << std::endl << std::flush;
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        //! TEST
        //std::cerr << "hallo4" << std::endl << std::flush;
        if ( element->mapPositionStrand(position, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL) ) {
            if ( !results.isEmpty() ) {
                if ( element->data(this->PRIORITYCOL).toUInt() > results.last()->data(this->PRIORITYCOL).toUInt() ) { continue; } // the previous one is better
                if ( element->data(this->PRIORITYCOL).toUInt() < results.last()->data(this->PRIORITYCOL).toUInt() ) { results.clear(); }
            }
            results.append(element); // append equal or better loci
        }
    }
}

void database::bestLmapRangeStrand(const QString &chrom, const uint &start, const uint &end, const QString &strand, QVector<databaseItem*> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(start));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        if ( element->mapRangeStrand(start, end, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL) ) {
            if ( !results.isEmpty() ) {
                if ( element->data(this->PRIORITYCOL).toUInt() > results.last()->data(this->PRIORITYCOL).toUInt() ) { continue; } // the previous one is better
                if ( element->data(this->PRIORITYCOL).toUInt() < results.last()->data(this->PRIORITYCOL).toUInt() ) { results.clear(); }
            }
        results.append(element); // append equal or better loci
        }
    }
}

void database::bestLmapGapStrand(const QString &chrom, const uint &start, const uint &end, const QString &strand, QVector<databaseItem*> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(start));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        if ( element->mapGapStrand(start, end, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL) ) {
            if ( !results.isEmpty() ) {
                if ( element->data(this->PRIORITYCOL).toUInt() > results.last()->data(this->PRIORITYCOL).toUInt() ) { continue; } // the previous one is better
                if ( element->data(this->PRIORITYCOL).toUInt() < results.last()->data(this->PRIORITYCOL).toUInt() ) { results.clear(); }
            }
            results.append(element); // append equal or better loci
        }
    }
}

QVector<databaseItem*> database::bestLmapPositionVecStrand(const QString &chrom, const uint &position, const QString &strand) const
{
    //! TEST
    //std::cerr << "hallo2" << std::endl << std::flush;
    QVector<databaseItem*> out;
    out.reserve(10);
    this->bestLmapPositionStrand(chrom, position, strand, out);
    return(out);
}

QVector<databaseItem*> database::bestLmapRangeVecStrand(const QString &chrom, const uint &start, const uint &end, const QString &strand) const
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->bestLmapRangeStrand(chrom, start, end, strand, out);
    return(out);
}

QVector<databaseItem*> database::bestLmapGapVecStrand(const QString &chrom, const uint &start, const uint &end, const QString &strand) const
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->bestLmapGapStrand(chrom, start, end, strand, out);
    return(out);
}


/*!
  RECURSIVE mapping reporting HITS TO ONLY BEST LOWEST FEATURES POSSIBLE OF BEST LOCI
  Rmap for recursive mapping (for each locus the hits at the lowest level (therefrom the best)
  the ones that are commented out would separate results from different loci
  similar commenting out is also done in the source file
  */

//! not strand specific
QVector<databaseItem*> database::bestRmapPosition(const QString &chrom, const uint &position) const
{
    //! TEST
    //std::cerr << "hallo1" << std::endl << std::flush;
    //QVector< QVector<databaseItem*> > out;
    QVector<databaseItem*> out;
    out.reserve(10);
    QVector<databaseItem*> loci = this->bestLmapPositionVec(chrom, position);
    foreach (databaseItem* locus, loci) {
        out += locus->bestRmapPositionVec(position, this->STARTCOL, this->ENDCOL, this->PRIORITYCOL);
        //out.append(locus->bestRmapPositionVec(position, this->STARTCOL, this->ENDCOL, this->PRIORITYCOL));
    }
    return(out);
}

QVector<databaseItem*> database::bestRmapRange(const QString &chrom, const uint &start, const uint &end) const
{
    //QVector< QVector<databaseItem*> > out;
    QVector<databaseItem*> out;
    out.reserve(10);
    QVector<databaseItem*> loci = this->bestLmapRangeVec(chrom, start, end);
    foreach (databaseItem* locus, loci) {
        out += locus->bestRmapRangeVec(start, end, this->STARTCOL, this->ENDCOL, this->PRIORITYCOL);
        //out.append(locus->bestRmapRangeVec(start, end, this->STARTCOL, this->ENDCOL, this->PRIORITYCOL));
    }
    return(out);
}

QVector<databaseItem*> database::bestRmapGap(const QString &chrom, const uint &start, const uint &end) const
{
    //QVector< QVector<databaseItem*> > out;
    QVector<databaseItem*> out;
    out.reserve(10);
    QVector<databaseItem*> loci = this->bestLmapGapVec(chrom, start, end);
    foreach (databaseItem* locus, loci) {
        out += locus->bestRmapGapVec(start, end, this->STARTCOL, this->ENDCOL, this->PRIORITYCOL);
        //out.append(locus->bestRmapGapVec(start, end, this->STARTCOL, this->ENDCOL, this->PRIORITYCOL));
    }
    return(out);
}

//! strand specific
QVector<databaseItem*> database::bestRmapPositionStrand(const QString &chrom, const uint &position, const QString &strand) const
{
    //! TEST
    //std::cerr << "hallo1" << std::endl << std::flush;
    //QVector< QVector<databaseItem*> > out;
    QVector<databaseItem*> out;
    out.reserve(10);
    QVector<databaseItem*> loci = this->bestLmapPositionVecStrand(chrom, position, strand);
    foreach (databaseItem* locus, loci) {
        out += locus->bestRmapPositionVecStrand(position, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL, this->PRIORITYCOL);
        //out.append(locus->bestRmapPositionVecStrand(position, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL, this->PRIORITYCOL));
    }
    return(out);
}

QVector<databaseItem*> database::bestRmapRangeStrand(const QString &chrom, const uint &start, const uint &end, const QString &strand) const
{
    //QVector< QVector<databaseItem*> > out;
    QVector<databaseItem*> out;
    out.reserve(10);
    QVector<databaseItem*> loci = this->bestLmapRangeVecStrand(chrom, start, end, strand);
    foreach (databaseItem* locus, loci) {
        out += locus->bestRmapRangeVecStrand(start, end, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL, this->PRIORITYCOL);
        //out.append(locus->bestRmapRangeVecStrand(start, end, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL, this->PRIORITYCOL));
    }
    return(out);
}

QVector<databaseItem*> database::bestRmapGapStrand(const QString &chrom, const uint &start, const uint &end, const QString &strand) const
{
    //QVector< QVector<databaseItem*> > out;
    QVector<databaseItem*> out;
    out.reserve(10);
    QVector<databaseItem*> loci = this->bestLmapGapVecStrand(chrom, start, end, strand);
    foreach (databaseItem* locus, loci) {
        out += locus->bestRmapGapVecStrand(start, end, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL, this->PRIORITYCOL);
        //out.append(locus->bestRmapGapVecStrand(start, end, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL, this->PRIORITYCOL));
    }
    return(out);
}






//! mappingTreeItem based functions with void return or list return- used in combination with alignments
//! RECURSIVE - ALL LOCI - ALL FEATURES (this is the only recursive one that does not reduce to the lowest possible)

//! not strand specific
void database::nestedRmapPosition(const QString &chrom, const uint &position, QList<mappingTreeItem> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(position));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        element->nestedRmapPosition(position, this->STARTCOL, this->ENDCOL, results);
    }
}

void database::nestedRmapRange(const QString &chrom, const uint &start, const uint &end, QList<mappingTreeItem> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(start));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        element->nestedRmapRange(start, end, this->STARTCOL, this->ENDCOL, results);
    }
}

void database::nestedRmapGap(const QString &chrom, const uint &start, const uint &end, QList<mappingTreeItem> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(start));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        element->nestedRmapGap(start, end, this->STARTCOL, this->ENDCOL, results);
    }
}

//! strand specific
void database::nestedRmapPositionStrand(const QString &chrom, const uint &position, const QString& strand, QList<mappingTreeItem> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(position));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        element->nestedRmapPositionStrand(position, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL, results);
    }
}

void database::nestedRmapRangeStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand, QList<mappingTreeItem> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(start));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        element->nestedRmapRangeStrand(start, end, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL, results);
    }
}

void database::nestedRmapGapStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand, QList<mappingTreeItem> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(start));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        element->nestedRmapGapStrand(start, end, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL, results);
    }
}

//! mappingTreeItem based functions with void return or list return- used in combination with alignments
//! RECURSIVE - ONLY THE BEST AT EACH LEVEL, BUT ALL LEVELS (this is the only recursive one that does not reduce to the lowest possible)

//! not strand specific
void database::bestNestedRmapPosition(const QString &chrom, const uint &position, QList<mappingTreeItem> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(position));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        element->bestNestedRmapPosition(position, this->STARTCOL, this->ENDCOL, this->PRIORITYCOL, results);
    }
}

void database::bestNestedRmapRange(const QString &chrom, const uint &start, const uint &end, QList<mappingTreeItem> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(start));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        element->bestNestedRmapRange(start, end, this->STARTCOL, this->ENDCOL, this->PRIORITYCOL, results);
    }
}

void database::bestNestedRmapGap(const QString &chrom, const uint &start, const uint &end, QList<mappingTreeItem> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(start));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        element->bestNestedRmapGap(start, end, this->STARTCOL, this->ENDCOL, this->PRIORITYCOL, results);
    }
}

//! strand specific
void database::bestNestedRmapPositionStrand(const QString &chrom, const uint &position, const QString& strand, QList<mappingTreeItem> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(position));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        element->bestNestedRmapPositionStrand(position, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL, this->PRIORITYCOL, results);
    }
}

void database::bestNestedRmapRangeStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand, QList<mappingTreeItem> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(start));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        element->bestNestedRmapRangeStrand(start, end, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL, this->PRIORITYCOL, results);
    }
}

void database::bestNestedRmapGapStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand, QList<mappingTreeItem> &results) const
{
    QPair<QString,uint> offset = qMakePair(chrom, this->getOffset(start));
    foreach (databaseItem* element, this->CPindex.value(offset)) {
        element->bestNestedRmapGapStrand(start, end, strand, this->STARTCOL, this->ENDCOL, this->STRANDCOL, this->PRIORITYCOL, results);
    }
}


/*!
  Mapping of a full read (all cigar operations):
  1. get the best loci, set mapFlag
  2. get the transcripts of those
  3. get the subfeatures of those transcripts, set mapFlag
  note that the pairedEnd and MatePair versions are yet to be implemented
  we map down everything as far as possible, keep all the mappings but note if there is a mapping at the lowest level
  */

//! not strand or strand specific
void database::alignmentMapFragment(QtBamAlignment &al, const bool& stranded, const bool& antisense) const
{
    //! this function assumes an xM read - or as well xMxDxM of xMxIxM (takes the end)
    // get the endposition
    int endPos = al.getEnd();

    // map from first to last position to annotation - clear the mappings afterwards
    if (stranded) {
        // get Strand NOTE that this would be faster via bool
        QString strand;
        if (antisense) {
            if (al.IsReverseStrand()) { strand = "+"; }
            else { strand = "-"; }
        } else {
            if (al.IsReverseStrand()) { strand = "-"; }
            else { strand = "+"; }
        }
        this->bestNestedRmapRangeStrand(al._refName, al._position, endPos, strand, al._mappings);
    } else {
        this->bestNestedRmapRange(al._refName, al._position, endPos, al._mappings);
    }

    // now check if they are real mappings or only partial mappings
    if (al._mappings.isEmpty()) {
        al.setIsSkipped(true);
    }
    else {
        al.setIsMappingToLocus(true); // this is a very rough mapping information. Does not tell if it is in a known exon or not
        // variables for iteration
        bool keepLocus;
        bool keepTrans;
        QList<mappingTreeItem>::iterator locIter;
        QList<mappingTreeItem>::iterator traIter;
        QList<mappingTreeItem>::iterator subIter;
        // remove the bad ones
        locIter = al._mappings.begin();
        while ( locIter != al._mappings.end() ) {
            keepLocus = false;
            traIter = (*locIter)._children.begin();
            while ( traIter != (*locIter)._children.end() ) {
                keepTrans = false;
                subIter = (*traIter)._children.begin();
                while ( subIter != (*traIter)._children.end() ) {
                    if ( (*subIter)._self->type == EXON ) {
                        al.setIsMappingToExon(true);
                        keepTrans = true;
                        keepLocus = true;
                        break;
                    }
                    ++subIter; // keep all the lowest features
                }
                // if transcript must be removed, do it and keep the iterator at the same spot, otherwise increase it
                if (!keepTrans) { traIter = (*locIter)._children.erase(traIter); }
                else { ++traIter; }
            }
            // if locus must be removed, do it and keep iterator at same spot, otherwise increase it
            if (!keepLocus) { locIter = al._mappings.erase(locIter); }
            else { ++locIter; }
        }
    }
    // set the ambiguous flag - this we do here as it should only consider perfect mappings
    if (al._mappings.count() > 1) { al.setIsAmbiguous(true); }
    else { al.setIsAmbiguous(false); }
}

void database::alignmentMapFragmentGapped(QtBamAlignment &al, const bool& stranded, const bool& antisense) const
{
    //! this function assumes xMyNzM... reads
    // we first do the mapping as for ungapped reads. From start to end. This will normally give us loci, transcripts, and eventually exons
    // after this first mapping, we go down to the transcripts and check the individual cigar operations on their subfeatures.
    // each ciger operation must map to a subfeature. Note that the M operations shall not match INTRON, but EXON. Other matches will be reported but not considered for validation
    // get the endposition
    int endPos = al.getEnd();

    // map from first to last position to annotation - clear the mappings afterwards
    if (stranded) {
        // get Strand NOTE that this would be faster via bool
        QString strand;
        if (antisense) {
            if (al.IsReverseStrand()) { strand = "+"; }
            else { strand = "-"; }
        } else {
            if (al.IsReverseStrand()) { strand = "-"; }
            else { strand = "+"; }
        }
        this->bestNestedRmapRangeStrand(al._refName, al._position, endPos, strand, al._mappings);
    } else {
        this->bestNestedRmapRange(al._refName, al._position, endPos, al._mappings);
    }

    // now go to the transcripts and check all cigar operations
    if (al._mappings.isEmpty()) { al.setIsSkipped(true); }
    else {
        al.setIsMappingToLocus(true); // this is a very rough mapping information. Does not tell if it is in a known exon or not
        // variables for iteration
        bool keepLocus;
        bool keepTrans;
        QList<mappingTreeItem>::iterator locIter;
        QList<mappingTreeItem>::iterator traIter;
        bool isMatch; // tells if cigar operation == M (otherwise N)
        bool validCigar; // tells if a cigar operation has a valid map
        mappingTreeItem newItem(0); // a potentially new item
        QVector<QtCigarOp>::iterator cigarIter;
        uint cs; // current start of a fragment
        uint ce; // current end of a fragment
        //! TODO and UNCLEAR - remove the bad ones - first delete address, then the element in the container ? or directly the element
        locIter = al._mappings.begin();
        while ( locIter != al._mappings.end() ) {
            keepLocus = false;
            traIter = (*locIter)._children.begin();
            while ( traIter != (*locIter)._children.end() ) {
                keepTrans = false;
                // clear the old mappings with the whole read
                (*traIter)._children.clear();
                // iterate over the cigar operations
                cs = al._position;
                for (cigarIter = al._cigarData.begin(); cigarIter != al._cigarData.end(); ++cigarIter) {
                    validCigar = false;
                    // check type of the current operation
                    if ( (*cigarIter)._type == 'M' ) { isMatch = true; }
                    else if ( (*cigarIter)._type == 'N' ) { isMatch = false; }
                    else if ( (*cigarIter)._type == 'D' ) { isMatch = true; } // a deletion in the reference is still kind of a match considering that the alignment positions are still according to the old reference
                    else if ( (*cigarIter)._type == 'I' ) { continue; } // an insert is not an intron - however - it should not extend the read (the alignment to the reference is actually shorter than the read length)
                    else if ( (*cigarIter)._type == 'S' ) { continue; } // clipping should be ignored - and the read should not be extended (the alignment to the reference is actually shorter than the read length)
                    else if ( (*cigarIter)._type == 'H' ) { continue; } // clipping should be ignored - and the read should not be extended (the alignment to the reference is actually shorter than the read length)
                    else { std::cerr << "unknown cigar operation" << (*cigarIter)._type.toLatin1() << std::endl << std::flush; continue; }
                    // get the end of the current operation
                    ce = cs + (*cigarIter)._length - 1; // minus one because start and end are both included in the database; here, the start is within, but adding the length, the end should not be included
                    //! map the fragment - NOTE THAT WE DONT CHECK STRAND ANYMORE
                    foreach (databaseItem* subItem, (*traIter)._self->childItems) {
                        if (isMatch) {
                            if ( subItem->mapRange(cs, ce, this->STARTCOL, this->ENDCOL) ) {
                                if ( subItem->type == EXON ) { validCigar = true; }
                                newItem._self = subItem; // change the pointer
                                (*traIter)._children.append(newItem); // append should hopefully make a real copy
                            }
                        } else {
                            if ( subItem->mapGap(cs, ce, this->STARTCOL, this->ENDCOL) ) {
                                if ( subItem->type == INTRON ) { validCigar = true; }
                                newItem._self = subItem; // change the pointer
                                (*traIter)._children.append(newItem); // append should hopefully make a real copy
                            }
                        }
                    }
                    // get the start of the next operation
                    cs = ce + 1; // we need the plus one again, as the start needs to be included
                    // exit if not valid Cigar
                    if (!validCigar) {
                        keepTrans = false;
                        break;
                    } else { keepTrans = true; }
                }
                // if transcript must be removed, do it and keep the iterator at the same spot, otherwise increase it
                if (!keepTrans) { traIter = (*locIter)._children.erase(traIter); }
                else { ++traIter; keepLocus = true; al.setIsMappingToSplice(true); }
            }
            // if locus must be removed, do it and keep iterator at same spot, otherwise increase it
            if (!keepLocus) { locIter = al._mappings.erase(locIter); }
            else { ++locIter; }
        }
    }
    // set the ambiguous flag - this we do here as it should only consider perfect mappings
    if (al._mappings.count() > 1) { al.setIsAmbiguous(true); }
    else { al.setIsAmbiguous(false); }
}

//void database::alignmentMapPairedEnd(QtBamAlignment &alA, QtBamAlignment &alB, const bool& stranded) const {}
//void database::alignmentMapMatePair(QtBamAlignment &alA, QtBamAlignment &alB, const bool& stranded) const {}



//! OLD FUNCTION WHERE ENTRIES ARE DIRECTLY MODIFIED
/*
  adding/counting of reads - called by readcounter

//! the first round - simple adding - we directly access the itemdata.
void database::countFirstRound(QtBamAlignment &al, mappingTreeItem *item)
{
    // get the minDist - you need the position of the aligment (note: if on forward strand, the endposition of the read is required)
    int end = al.getEnd();
    int minDist = this->getThreePrimeDist(item->_self, al._position, end);

    // replace it if necessary
    if ( ( item->_self->itemData[this->NUMMAPCOL].toUInt() > 0 ) && ( item->_self->itemData[this->MINDISTCOL].toInt() > minDist ) ) {
        item->_self->itemData[this->MINDISTCOL] = minDist;
    }

    // add the weight to the count and replace the mindist if necessary
    item->_self->itemData[this->NUMMAPCOL] = item->_self->itemData[this->NUMMAPCOL].toUInt() + 1;
    if (al.IsAmbiguous()) {
        item->_self->itemData[this->AMBCOL] = item->_self->itemData[this->AMBCOL].toFloat() + al._weight;
    } else {
        item->_self->itemData[this->UNAMBCOL] = item->_self->itemData[this->UNAMBCOL].toFloat() + al._weight;
    }

    // do the same for all children
    foreach (mappingTreeItem *child, item->_children) {
        this->countFirstRound(al, child);
    }
}
  */


