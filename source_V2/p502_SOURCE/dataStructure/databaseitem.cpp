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

#include "databaseitem.h"
#include "mappingtreeitem.h"
//! TEST
#include "iostream"

// constructor
databaseItem::databaseItem(Type intype, const QVector<QVariant> &data, databaseItem *parent)
{
    this->type = intype;
    this->parentItem = parent;
    this->itemData = data;
}

// destructor
databaseItem::~databaseItem()
{
    qDeleteAll(this->childItems);
}



//! access functions



// access a specific child
databaseItem *databaseItem::child(int number)
{
    return(this->childItems.value(number));
}

// access the parent of the current item
databaseItem *databaseItem::parent()
{
    return(this->parentItem);
}

// access one item in the data of the item
QVariant databaseItem::data(int column) const
{
    return(this->itemData.value(column));
}

// access the data vector using a pointer (points somehow to the first element)
QVariant* databaseItem::dataPtr() {
    return(this->itemData.data());
} //! NOTE: NO IDEA IF WORKING

// get the child number of the current item
int databaseItem::childNumber() const
{
    if (this->parentItem) {
        return(this->parentItem->childItems.indexOf(const_cast<databaseItem*>(this)));
    }
    else {
        return(0);
    }

}

// get the number of children of the current item
int databaseItem::childCount() const
{
    return(this->childItems.count());
}

// get the number of columns (number data entries) of the current item
int databaseItem::columnCount() const
{
    return(this->itemData.count());
}

//! custom accessors
// get the level of the item
int databaseItem::getLevel()
{
    if (this->type == ROOT) { return(0); }
    else if (this->type == LOCUS) { return(1); }
    else if (this->type == TRANSCRIPT) { return(2); }
    else { return(3); }
}




//! modifier functions



// change one data value
bool databaseItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= this->itemData.size()) {
        return(false);
    }
    else {
        this->itemData[column] = value;
        return(true);
    }
}

// change one data value recursively
bool databaseItem::RsetData(int column, const QVariant & value)
{
    bool rval = true;
    if (column < 0 || column >= this->itemData.size()) {
        rval = false;
    }
    else {
        this->itemData[column] = value;
        foreach(databaseItem* child, this->childItems) {
            if (!child->RsetData(column, value)) {
                rval = false;
                break;
            }
        }
    }
    return(rval);
}

// add a child (here its a whole database item)
bool databaseItem::addChild(databaseItem *child)
{
    if (child) {
        this->childItems += child;
        return(true);
    }
    else {
        return(false);
    }
}

//! potentially useful modifier functions for the GUI and future

// insert a child at a specific position - maybe useful once for editing and painting
bool databaseItem::insertChildren(int position, int count, int columns, Type childType)
{
    if ((position < 0) || (position > this->childItems.size())) {
        return(false);
    }
    else {
        for (int row = 0; row < count; ++row) {
            QVector<QVariant> data(columns);
            databaseItem *item = new databaseItem(childType, data, this);
            this->childItems.insert(position, item);
        }
        return(true);
    }
}

// remove a subset of children - maybe useful once for editing and painting
bool databaseItem::removeChildren(int position, int count)
{
    if ((position < 0) || (position + count > this->childItems.size())) {
        return(false);
    }
    else {
        for (int row = 0; row < count; ++row) { delete this->childItems.takeAt(position); }
        return(true);
    }
}

// insert additional columns (either only this entry or all the children too)
bool databaseItem::insertColumns(int position, int columns, bool childrentoo)
{
    if ((position < 0) || (position > this->itemData.size())) {
        return(false);
    }
    else {
        for (int column = 0; column < columns; ++column) { this->itemData.insert(position, QVariant()); }
        if (childrentoo) {
            foreach (databaseItem *child, this->childItems) { child->insertColumns(position, columns, true); }
        }
        return(true);
    }
}


// remove columns (either only this entry or all the children too)
bool databaseItem::removeColumns(int position, int columns, bool childrentoo)
{
    if ((position < 0) || (position + columns > this->itemData.size())) {
        return(false);
    }
    else {
        for (int column = 0; column < columns; ++column) { this->itemData.remove(position); }
        if (childrentoo) {
            foreach (databaseItem *child, this->childItems) { child->removeColumns(position, columns, childrentoo); }
        }
        return(true);
    }
}


/*!
  access to the topmost parent (LOCUS)
  */

databaseItem* databaseItem::topParent()
{
    if (this->type == LOCUS) {
        return(this);
    }
    else if (this->type == TRANSCRIPT) {
        return(this->parentItem);
    } else {
        return(this->parentItem->parentItem);
    }
}


/*!
  mappings: ALL HITS
  recursive and non recursive
  implemented for:
  position (Cmeth)
  range (ungapped alignment)
  gap (gapped alignment)
  */

/*!
  non recursive position and read mappings:
  bool return for mapping success
  databaseItem* return to keep a pointer to the element
  */

//! not strand specific
bool databaseItem::mapPosition(const uint &position, const uint &STARTCOL, const uint &ENDCOL)
{
    if ((this->itemData.value(STARTCOL).toUInt() <= position) && (this->itemData.value(ENDCOL).toUInt() >= position)) { return(true); }
    else { return(false); }
}

bool databaseItem::mapRange(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL)
{
    if ((this->itemData.value(STARTCOL).toUInt() <= start) && (this->itemData.value(ENDCOL).toUInt() >= end)) { return(true); }
    else { return(false); }
}

bool databaseItem::mapGap(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL)
{
    if (this->type == INTRON) {
        if ((this->itemData.value(STARTCOL).toUInt() == start) && (this->itemData.value(ENDCOL).toUInt() == end)) {
            return(true);
        } else {
            return(false);
        }
    }
    else if ((this->type == LOCUS) || (this->type == TRANSCRIPT)) {
        return( this->mapRange(start, end, STARTCOL, ENDCOL) );
    }
    else {
        return(false);
    }
}

databaseItem* databaseItem::mapPositionPtr(const uint &position, const uint &STARTCOL, const uint &ENDCOL)
{
    if ( this->mapPosition(position, STARTCOL, ENDCOL) ) { return(this); }
    else { return(0); }
}

databaseItem* databaseItem::mapRangePtr(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL)
{
    if ( this->mapRange(start, end, STARTCOL, ENDCOL) ) { return(this); }
    else { return(0); }
}

databaseItem* databaseItem::mapGapPtr(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL)
{
    if ( this->mapGap(start, end, STARTCOL, ENDCOL) ) { return(this); }
    else { return(0); }
}

//! strand specific
bool databaseItem::mapPositionStrand(const uint &position, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL)
{
    if ((this->itemData.value(STRANDCOL).toString() == strand) && (this->itemData.value(STARTCOL).toUInt() <= position) && (this->itemData.value(ENDCOL).toUInt() >= position)) { return(true); }
    else { return(false); }
}

bool databaseItem::mapRangeStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL)
{
    if ((this->itemData.value(STRANDCOL).toString() == strand) && (this->itemData.value(STARTCOL).toUInt() <= start) && (this->itemData.value(ENDCOL).toUInt() >= end)) { return(true); }
    else { return(false); }
}

bool databaseItem::mapGapStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL)
{
    if (this->type == INTRON) {
        if ((this->itemData.value(STRANDCOL).toString() == strand) && (this->itemData.value(STARTCOL).toUInt() == start) && (this->itemData.value(ENDCOL).toUInt() == end)) {
            return(true);
        } else {
            return(false);
        }
    }
    else if ((this->type == LOCUS) || (this->type == TRANSCRIPT)) {
        return( this->mapRangeStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL) );
    }
    else {
        return(false);
    }
}

databaseItem* databaseItem::mapPositionPtrStrand(const uint &position, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL)
{
    if ( this->mapPositionStrand(position, strand, STARTCOL, ENDCOL, STRANDCOL) ) { return(this); }
    else { return(0); }
}

databaseItem* databaseItem::mapRangePtrStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL)
{
    if ( this->mapRangeStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL) ) { return(this); }
    else { return(0); }
}

databaseItem* databaseItem::mapGapPtrStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL)
{
    if ( this->mapGapStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL) ) { return(this); }
    else { return(0); }
}


/*!
  recursive position and read mappings:
  this versions map from the topmost element (LOCUS) down to the lowest element (eg EXON)
  it reports all matches at the lowest (possible) level in a QVector<databaseItem*>
  the void return takes an additional vector and fills it up (the other one is built on this)
  */

//! not strand specific
void databaseItem::RmapPosition(const uint &position, const uint &STARTCOL, const uint &ENDCOL, QVector<databaseItem*> &results)
{
    if ( this->mapPosition(position, STARTCOL, ENDCOL) ) {
        int resultcount = results.count();
        foreach(databaseItem* child, this->childItems) {
            child->RmapPosition(position, STARTCOL, ENDCOL, results);
        }
        if (resultcount == results.count())  {results.append(this); }
    }
}

void databaseItem::RmapRange(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, QVector<databaseItem*> &results)
{
    if ( this->mapRange(start, end, STARTCOL, ENDCOL) ) {
        int resultcount = results.count();
        foreach(databaseItem* child, this->childItems) {
            child->RmapRange(start, end, STARTCOL, ENDCOL, results);
        }
        if (resultcount == results.count())  {results.append(this); }
    }
}

void databaseItem::RmapGap(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, QVector<databaseItem*> &results)
{
    if ( this->mapGap(start, end, STARTCOL, ENDCOL) ) {
        int resultcount = results.count();
        foreach(databaseItem* child, this->childItems) {
            child->RmapGap(start, end, STARTCOL, ENDCOL, results);
        }
        if (resultcount == results.count())  {results.append(this); }
    }
}

QVector<databaseItem*> databaseItem::RmapPositionVec(const uint &position, const uint &STARTCOL, const uint &ENDCOL) //NOTE: POINTER / REFERENCE / VALUE?
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->RmapPosition(position, STARTCOL, ENDCOL, out);
    return(out);
}

QVector<databaseItem*> databaseItem::RmapRangeVec(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL) //NOTE: POINTER / REFERENCE / VALUE?
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->RmapRange(start, end, STARTCOL, ENDCOL, out);
    return(out);
}

QVector<databaseItem*> databaseItem::RmapGapVec(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL) //NOTE: POINTER / REFERENCE / VALUE?
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->RmapGap(start, end, STARTCOL, ENDCOL, out);
    return(out);
}

//! strand specific
void databaseItem::RmapPositionStrand(const uint &position, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, QVector<databaseItem*> &results)
{
    if ( this->mapPositionStrand(position, strand, STARTCOL, ENDCOL, STRANDCOL) ) {
        int resultcount = results.count();
        foreach(databaseItem* child, this->childItems) {
            child->RmapPositionStrand(position, strand, STARTCOL, ENDCOL, STRANDCOL, results);
        }
        if (resultcount == results.count())  {results.append(this); }
    }
}

void databaseItem::RmapRangeStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, QVector<databaseItem*> &results)
{
    if ( this->mapRangeStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL) ) {
        int resultcount = results.count();
        foreach(databaseItem* child, this->childItems) {
            child->RmapRangeStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL, results);
        }
        if (resultcount == results.count())  {results.append(this); }
    }
}

void databaseItem::RmapGapStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, QVector<databaseItem*> &results)
{
    if ( this->mapGapStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL) ) {
        int resultcount = results.count();
        foreach(databaseItem* child, this->childItems) {
            child->RmapGapStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL, results);
        }
        if (resultcount == results.count())  {results.append(this); }
    }
}

QVector<databaseItem*> databaseItem::RmapPositionVecStrand(const uint &position, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL) //NOTE: POINTER / REFERENCE / VALUE?
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->RmapPositionStrand(position, strand, STARTCOL, ENDCOL, STRANDCOL, out);
    return(out);
}

QVector<databaseItem*> databaseItem::RmapRangeVecStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL) //NOTE: POINTER / REFERENCE / VALUE?
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->RmapRangeStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL, out);
    return(out);
}

QVector<databaseItem*> databaseItem::RmapGapVecStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL) //NOTE: POINTER / REFERENCE / VALUE?
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->RmapGapStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL, out);
    return(out);
}


/*!
  mappings: BEST HITS
  recursive
  implemented for:
  position (Cmeth)
  range (ungapped alignment)
  gap (gapped alignment)
  */

/*!
  recursive position and read mappings:
  this versions map from the topmost element (LOCUS) down to the lowest element (eg EXON)
  it reports all matches with the highest priority in a QVector<databaseItem*>
  the void return takes an additional vector and fills it up (the other one is built on this)
  */

//! not strand specific
void databaseItem::bestRmapPosition(const uint &position, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL, QVector<databaseItem*> &results)
{
    if ( this->mapPosition(position, STARTCOL, ENDCOL) ) {
        foreach(databaseItem* child, this->childItems) {
            child->bestRmapPosition(position, STARTCOL, ENDCOL, PRIORITYCOL, results);
        }
        if ( !results.isEmpty() ) {
            if ( this->data(PRIORITYCOL).toUInt() <= results.last()->data(PRIORITYCOL).toUInt() ) {
                //std::cerr << this->data(0).toString().toStdString() << std::endl << std::flush;
                if ( this->data(PRIORITYCOL).toUInt() < results.last()->data(PRIORITYCOL).toUInt() ) { results.clear(); }
                results.append(this);
            }
        }
        else { results.append(this); }
    }
}

void databaseItem::bestRmapRange(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL, QVector<databaseItem*> &results)
{
    if ( this->mapRange(start, end, STARTCOL, ENDCOL) ) {
        foreach(databaseItem* child, this->childItems) {
            child->bestRmapRange(start, end, STARTCOL, ENDCOL, PRIORITYCOL, results);
        }
        if ( !results.isEmpty() ) {
            if ( this->data(PRIORITYCOL).toUInt() <= results.last()->data(PRIORITYCOL).toUInt() ) {
                if ( this->data(PRIORITYCOL).toUInt() < results.last()->data(PRIORITYCOL).toUInt() ) { results.clear(); }
                results.append(this);
            }
        }
        else { results.append(this); }
    }
}

void databaseItem::bestRmapGap(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL, QVector<databaseItem*> &results)
{
    if ( this->mapGap(start, end, STARTCOL, ENDCOL) ) {
        foreach(databaseItem* child, this->childItems) {
            child->bestRmapGap(start, end, STARTCOL, ENDCOL, PRIORITYCOL, results);
        }
        if ( !results.isEmpty() ) {
            if ( this->data(PRIORITYCOL).toUInt() <= results.last()->data(PRIORITYCOL).toUInt() ) {
                if ( this->data(PRIORITYCOL).toUInt() < results.last()->data(PRIORITYCOL).toUInt() ) { results.clear(); }
                results.append(this);
            }
        }
        else { results.append(this); }
    }
}

QVector<databaseItem*> databaseItem::bestRmapPositionVec(const uint &position, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL)
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->bestRmapPosition(position, STARTCOL, ENDCOL, PRIORITYCOL, out);
    return(out);
}

QVector<databaseItem*> databaseItem::bestRmapRangeVec(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL)
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->bestRmapRange(start, end, STARTCOL, ENDCOL, PRIORITYCOL, out);
    return(out);
}

QVector<databaseItem*> databaseItem::bestRmapGapVec(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL)
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->bestRmapGap(start, end, STARTCOL, ENDCOL, PRIORITYCOL, out);
    return(out);
}

//! strand specific
void databaseItem::bestRmapPositionStrand(const uint &position, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL, QVector<databaseItem*> &results)
{
    if ( this->mapPositionStrand(position, strand, STARTCOL, ENDCOL, STRANDCOL) ) {
        foreach(databaseItem* child, this->childItems) {
            child->bestRmapPositionStrand(position, strand, STARTCOL, ENDCOL, STRANDCOL, PRIORITYCOL, results);
        }
        if ( !results.isEmpty() ) {
            if ( this->data(PRIORITYCOL).toUInt() <= results.last()->data(PRIORITYCOL).toUInt() ) {
                //std::cerr << this->data(0).toString().toStdString() << std::endl << std::flush;
                if ( this->data(PRIORITYCOL).toUInt() < results.last()->data(PRIORITYCOL).toUInt() ) { results.clear(); }
                results.append(this);
            }
        }
        else { results.append(this); }
    }
}

void databaseItem::bestRmapRangeStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL, QVector<databaseItem*> &results)
{
    if ( this->mapRangeStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL) ) {
        foreach(databaseItem* child, this->childItems) {
            child->bestRmapRangeStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL, PRIORITYCOL, results);
        }
        if ( !results.isEmpty() ) {
            if ( this->data(PRIORITYCOL).toUInt() <= results.last()->data(PRIORITYCOL).toUInt() ) {
                if ( this->data(PRIORITYCOL).toUInt() < results.last()->data(PRIORITYCOL).toUInt() ) { results.clear(); }
                results.append(this);
            }
        }
        else { results.append(this); }
    }
}

void databaseItem::bestRmapGapStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL, QVector<databaseItem*> &results)
{
    if ( this->mapGapStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL) ) {
        foreach(databaseItem* child, this->childItems) {
            child->bestRmapGapStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL, PRIORITYCOL, results);
        }
        if ( !results.isEmpty() ) {
            if ( this->data(PRIORITYCOL).toUInt() <= results.last()->data(PRIORITYCOL).toUInt() ) {
                if ( this->data(PRIORITYCOL).toUInt() < results.last()->data(PRIORITYCOL).toUInt() ) { results.clear(); }
                results.append(this);
            }
        }
        else { results.append(this); }
    }
}

QVector<databaseItem*> databaseItem::bestRmapPositionVecStrand(const uint &position, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL)
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->bestRmapPositionStrand(position, strand, STARTCOL, ENDCOL, STRANDCOL, PRIORITYCOL, out);
    return(out);
}

QVector<databaseItem*> databaseItem::bestRmapRangeVecStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL)
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->bestRmapRangeStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL, PRIORITYCOL, out);
    return(out);
}

QVector<databaseItem*> databaseItem::bestRmapGapVecStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL)
{
    QVector<databaseItem*> out;
    out.reserve(10);
    this->bestRmapGapStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL, PRIORITYCOL, out);
    return(out);
}







//! mappingTree based functions with void return or list return- used in combination with alignments
//! RECURSIVE - ALL LOCI - ALL FEATURES

//! not strand specific
void databaseItem::nestedRmapPosition(const uint &position, const uint &STARTCOL, const uint &ENDCOL, QList<mappingTreeItem> &topItemList)
{
    if ( this->mapPosition(position, STARTCOL, ENDCOL) ) {
        mappingTreeItem newTop(this);
        foreach(databaseItem* child, this->childItems) {
            child->nestedRmapPosition(position, STARTCOL, ENDCOL, newTop._children);
        }
        topItemList.append(newTop);
    }
}

void databaseItem::nestedRmapRange(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, QList<mappingTreeItem> &topItemList)
{
    if ( this->mapRange(start, end, STARTCOL, ENDCOL) ) {
        mappingTreeItem newTop(this);
        foreach(databaseItem* child, this->childItems) {
            child->nestedRmapRange(start, end, STARTCOL, ENDCOL, newTop._children);
        }
        topItemList.append(newTop);
    }
}

void databaseItem::nestedRmapGap(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, QList<mappingTreeItem> &topItemList)
{
    if ( this->mapGap(start, end, STARTCOL, ENDCOL) ) {
        mappingTreeItem newTop(this);
        foreach(databaseItem* child, this->childItems) {
            child->nestedRmapGap(start, end, STARTCOL, ENDCOL, newTop._children);
        }
        topItemList.append(newTop);
    }
}


//! strand specific
void databaseItem::nestedRmapPositionStrand(const uint &position, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, QList<mappingTreeItem> &topItemList)
{
    if ( this->mapPositionStrand(position, strand, STARTCOL, ENDCOL, STRANDCOL) ) {
        mappingTreeItem newTop(this);
        foreach(databaseItem* child, this->childItems) {
            child->nestedRmapPositionStrand(position, strand, STARTCOL, ENDCOL, STRANDCOL, newTop._children);
        }
        topItemList.append(newTop);
    }
}

void databaseItem::nestedRmapRangeStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, QList<mappingTreeItem> &topItemList)
{
    if ( this->mapRangeStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL) ) {
        mappingTreeItem newTop(this);
        foreach(databaseItem* child, this->childItems) {
            child->nestedRmapRangeStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL, newTop._children);
        }
        topItemList.append(newTop);
    }
}

void databaseItem::nestedRmapGapStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, QList<mappingTreeItem> &topItemList)
{
    if ( this->mapGapStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL) ) {
        mappingTreeItem newTop(this);
        foreach(databaseItem* child, this->childItems) {
            child->nestedRmapGapStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL, newTop._children);
        }
        topItemList.append(newTop);
    }
}


//! mappingTree based functions with void return or list return- used in combination with alignments
//! RECURSIVE - ONLY THE BEST AT EACH LEVEL, BUT ALL LEVELS

//! not strand specific
void databaseItem::bestNestedRmapPosition(const uint &position, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL, QList<mappingTreeItem> &topItemList)
{
    if ( this->mapPosition(position, STARTCOL, ENDCOL) ) {
        mappingTreeItem newTop(this);
        foreach(databaseItem* child, this->childItems) {
            child->bestNestedRmapPosition(position, STARTCOL, ENDCOL, PRIORITYCOL, newTop._children);
        }
        if ( !topItemList.isEmpty() ) {
            if ( this->data(PRIORITYCOL).toUInt() <= topItemList.last()._self->data(PRIORITYCOL).toUInt() ) {
                if ( this->data(PRIORITYCOL).toUInt() < topItemList.last()._self->data(PRIORITYCOL).toUInt() ) { topItemList.clear(); } // remove all with lower priorities
                topItemList.append(newTop);
            }
        }
        else { topItemList.append(newTop); }
    }
}

void databaseItem::bestNestedRmapRange(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL, QList<mappingTreeItem> &topItemList)
{
    if ( this->mapRange(start, end, STARTCOL, ENDCOL) ) {
        mappingTreeItem newTop(this);
        foreach(databaseItem* child, this->childItems) {
            child->bestNestedRmapRange(start, end, STARTCOL, ENDCOL, PRIORITYCOL, newTop._children);
        }
        if ( !topItemList.isEmpty() ) {
            if ( this->data(PRIORITYCOL).toUInt() <= topItemList.last()._self->data(PRIORITYCOL).toUInt() ) {
                if ( this->data(PRIORITYCOL).toUInt() < topItemList.last()._self->data(PRIORITYCOL).toUInt() ) { topItemList.clear(); } // remove all with lower priorities
                topItemList.append(newTop);
            }
        }
        else { topItemList.append(newTop); }
    }
}

void databaseItem::bestNestedRmapGap(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL, QList<mappingTreeItem> &topItemList)
{
    if ( this->mapGap(start, end, STARTCOL, ENDCOL) ) {
        mappingTreeItem newTop(this);
        foreach(databaseItem* child, this->childItems) {
            child->bestNestedRmapGap(start, end, STARTCOL, ENDCOL, PRIORITYCOL, newTop._children);
        }
        if ( !topItemList.isEmpty() ) {
            if ( this->data(PRIORITYCOL).toUInt() <= topItemList.last()._self->data(PRIORITYCOL).toUInt() ) {
                if ( this->data(PRIORITYCOL).toUInt() < topItemList.last()._self->data(PRIORITYCOL).toUInt() ) { topItemList.clear(); } // remove all with lower priorities
                topItemList.append(newTop);
            }
        }
        else { topItemList.append(newTop); }
    }
}


//! strand specific
void databaseItem::bestNestedRmapPositionStrand(const uint &position, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL, QList<mappingTreeItem> &topItemList)
{
    if ( this->mapPositionStrand(position, strand, STARTCOL, ENDCOL, STRANDCOL) ) {
        mappingTreeItem newTop(this);
        foreach(databaseItem* child, this->childItems) {
            child->bestNestedRmapPositionStrand(position, strand, STARTCOL, ENDCOL, STRANDCOL, PRIORITYCOL, newTop._children);
        }
        if ( !topItemList.isEmpty() ) {
            if ( this->data(PRIORITYCOL).toUInt() <= topItemList.last()._self->data(PRIORITYCOL).toUInt() ) {
                if ( this->data(PRIORITYCOL).toUInt() < topItemList.last()._self->data(PRIORITYCOL).toUInt() ) { topItemList.clear(); } // remove all with lower priorities
                topItemList.append(newTop);
            }
        }
        else { topItemList.append(newTop); }
    }
}

void databaseItem::bestNestedRmapRangeStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL, QList<mappingTreeItem> &topItemList)
{
    if ( this->mapRangeStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL) ) {
        mappingTreeItem newTop(this);
        foreach(databaseItem* child, this->childItems) {
            child->bestNestedRmapRangeStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL, PRIORITYCOL, newTop._children);
        }
        if ( !topItemList.isEmpty() ) {
            if ( this->data(PRIORITYCOL).toUInt() <= topItemList.last()._self->data(PRIORITYCOL).toUInt() ) {
                if ( this->data(PRIORITYCOL).toUInt() < topItemList.last()._self->data(PRIORITYCOL).toUInt() ) { topItemList.clear(); } // remove all with lower priorities
                topItemList.append(newTop);
            }
        }
        else { topItemList.append(newTop); }
    }
}

void databaseItem::bestNestedRmapGapStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL, QList<mappingTreeItem> &topItemList)
{
    if ( this->mapGapStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL) ) {
        mappingTreeItem newTop(this);
        foreach(databaseItem* child, this->childItems) {
            child->bestNestedRmapGapStrand(start, end, strand, STARTCOL, ENDCOL, STRANDCOL, PRIORITYCOL, newTop._children);
        }
        if ( !topItemList.isEmpty() ) {
            if ( this->data(PRIORITYCOL).toUInt() <= topItemList.last()._self->data(PRIORITYCOL).toUInt() ) {
                if ( this->data(PRIORITYCOL).toUInt() < topItemList.last()._self->data(PRIORITYCOL).toUInt() ) { topItemList.clear(); } // remove all with lower priorities
                topItemList.append(newTop);
            }
        }
        else { topItemList.append(newTop); }
    }
}
