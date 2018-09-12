/**************************************************************************
** This is quite similar to the simple_tree_item class.
** Instead of creating for each entry of a feature an own name,
** we use a QVariant vector, get senseful names for an index via enum
**************************************************************************/

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



#ifndef DATABASEITEM_H
#define DATABASEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

class database;
class databaseWriter;
class mappingTreeItem;

// define two types:
// RANGE are feature like exons (is something within?)
// GAP are feature like splice junctions (is something exactly corresponding to the border?)
enum Type { ROOT, LOCUS, TRANSCRIPT, EXON, INTRON, UTR };

class databaseItem
{
public:
    //! basic definitions, constructor and destructor
    Type type;

    // constructor - important to pass on type
    databaseItem(Type intype, const QVector<QVariant> &data, databaseItem *parent = 0);

    // destructor - if one element is deleted, all child elements must be deleted too
    ~databaseItem();

    //! access functions

    // access a specific child
    databaseItem *child(int number);

    // access the parent of the current item
    databaseItem *parent();

    // access one item in the data of the current item
    QVariant data(int column) const;

    // access the data vector using a pointer (points somehow to the first element-> access the others via [index])
    QVariant* dataPtr(); //! custom - NOTE: NO IDEA IF WORKING

    // get the child number of the current item
    int childNumber() const;

    // get the number of children of the current item
    int childCount() const;

    // get the number of columns (number data entries) of the current item
    int columnCount() const;

    //! custom accessors
    // get the level of the item
    int getLevel();

    //! new in the countsPerTranscript version, access the name of the item
    QString getName();

    //! modifier functions

    // change one data value
    bool setData(int column, const QVariant &value);

    // change one data value recursively
    bool RsetData(int column, const QVariant & value);

    // add a child
    bool addChild(databaseItem *child);

    //! potentially useful modifier functions for the GUI and future

    // insert a child at a specific position - maybe useful once for editing and painting
    bool insertChildren(int position, int count, int columns, Type childType);

    // remove a subset of children - maybe useful once for editing and painting
    bool removeChildren(int position, int count);

    // insert additional columns (either only this entry or all the children too)
    bool insertColumns(int position, int columns, bool childrentoo);

    // remove columns (either only this entry or all the children too)
    bool removeColumns(int position, int columns, bool childrentoo);

    /*!
      access to the topmost parent (LOCUS)
      */

    databaseItem* topParent();

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
    bool mapPosition(const uint &position, const uint &STARTCOL, const uint &ENDCOL);
    bool mapRange(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL);
    bool mapGap(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL);
    databaseItem* mapPositionPtr(const uint &position, const uint &STARTCOL, const uint &ENDCOL);
    databaseItem* mapRangePtr(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL);
    databaseItem* mapGapPtr(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL);
    //! strand specific
    bool mapPositionStrand(const uint &position, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL);
    bool mapRangeStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL);
    bool mapGapStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL);
    databaseItem* mapPositionPtrStrand(const uint &position, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL);
    databaseItem* mapRangePtrStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL);
    databaseItem* mapGapPtrStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL);

    /*!
      recursive position and read mappings:
      this versions map from the topmost element (eg LOCUS) down to the lowest element (eg EXON)
      it reports all matches at the lowest level in a QVector<databaseItem*>
      the void return takes an additional vector and fills it up (the other one is built on this)
      */

    //! not strand specific
    void RmapPosition(const uint &position, const uint &STARTCOL, const uint &ENDCOL, QVector<databaseItem*> &results);
    void RmapRange(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, QVector<databaseItem*> &results);
    void RmapGap(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, QVector<databaseItem*> &results);
    QVector<databaseItem*> RmapPositionVec(const uint &position, const uint &STARTCOL, const uint &ENDCOL);
    QVector<databaseItem*> RmapRangeVec(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL);
    QVector<databaseItem*> RmapGapVec(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL);

    //! strand specific
    void RmapPositionStrand(const uint &position, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, QVector<databaseItem*> &results);
    void RmapRangeStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, QVector<databaseItem*> &results);
    void RmapGapStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, QVector<databaseItem*> &results);
    QVector<databaseItem*> RmapPositionVecStrand(const uint &position, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL);
    QVector<databaseItem*> RmapRangeVecStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL);
    QVector<databaseItem*> RmapGapVecStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL);

    /*!
      mappings: BEST HITS
      recursive and non recursive
      implemented for:
      position (Cmeth)
      range (ungapped alignment)
      gap (gapped alignment)
      */

    /*!
      recursive position and read mappings:
      this versions map from the topmost element (eg LOCUS) down to the lowest element (eg EXON)
      it reports all matches at the lowest level in a QVector<databaseItem*>
      the void return takes an additional vector and fills it up (the other one is built on this)
      */

    //! not strand specific
    void bestRmapPosition(const uint &position, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL, QVector<databaseItem*> &results);
    void bestRmapRange(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL, QVector<databaseItem*> &results);
    void bestRmapGap(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL, QVector<databaseItem*> &results);
    QVector<databaseItem*> bestRmapPositionVec(const uint &position, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL);
    QVector<databaseItem*> bestRmapRangeVec(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL);
    QVector<databaseItem*> bestRmapGapVec(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL);

    //! strand specific
    void bestRmapPositionStrand(const uint &position, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL, QVector<databaseItem*> &results);
    void bestRmapRangeStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL, QVector<databaseItem*> &results);
    void bestRmapGapStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL, QVector<databaseItem*> &results);
    QVector<databaseItem*> bestRmapPositionVecStrand(const uint &position, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL);
    QVector<databaseItem*> bestRmapRangeVecStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL);
    QVector<databaseItem*> bestRmapGapVecStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL);


    //! mappingTree based functions with void return or list return- used in combination with alignments
    //! RECURSIVE - ALL LOCI - ALL FEATURES

    //! not strand specific
    void nestedRmapPosition(const uint &position, const uint &STARTCOL, const uint &ENDCOL, QList<mappingTreeItem> &topItemList);
    void nestedRmapRange(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, QList<mappingTreeItem> &topItemList);
    void nestedRmapGap(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, QList<mappingTreeItem> &topItemList);

    //! strand specific
    void nestedRmapPositionStrand(const uint &position, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, QList<mappingTreeItem> &topItemList);
    void nestedRmapRangeStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, QList<mappingTreeItem> &topItemList);
    void nestedRmapGapStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, QList<mappingTreeItem> &topItemList);

    //! mappingTree based functions with void return or list return- used in combination with alignments
    //! RECURSIVE - ONLY THE BEST AT EACH LEVEL, BUT ALL LEVELS

    //! not strand specific
    void bestNestedRmapPosition(const uint &position, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL, QList<mappingTreeItem> &topItemList);
    void bestNestedRmapRange(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL, QList<mappingTreeItem> &topItemList);
    void bestNestedRmapGap(const uint &start, const uint &end, const uint &STARTCOL, const uint &ENDCOL, const uint &PRIORITYCOL, QList<mappingTreeItem> &topItemList);

    //! strand specific
    void bestNestedRmapPositionStrand(const uint &position, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL, QList<mappingTreeItem> &topItemList);
    void bestNestedRmapRangeStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL, QList<mappingTreeItem> &topItemList);
    void bestNestedRmapGapStrand(const uint &start, const uint &end, const QString &strand, const uint &STARTCOL, const uint &ENDCOL, const uint &STRANDCOL, const uint &PRIORITYCOL, QList<mappingTreeItem> &topItemList);

    /*!
      other, yet unknown things
      */
    // calculate coverage
    // QVector<QVariant> get coverage(something to do this);

private:
    QList<databaseItem*> childItems;
    QVector<QVariant> itemData;
    databaseItem *parentItem;

    friend class database;
    friend class databaseWriter;
    friend class databaseReader;
};

#endif // DATABASEITEM_H
