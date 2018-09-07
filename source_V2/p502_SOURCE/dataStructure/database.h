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

#ifndef DATABASE_H
#define DATABASE_H

#include <QVariant>
#include <QVector>
#include <QPair>
#include <QHash>
#include <QTime>
#include <QTextStream>

//! ===========================================
//! TODO:
//! ACCESS ELEMENTS VIA NAME (NAMEINDEX)
//! RESULTS PER THREAD INSTEAD OF RETURN BY VALUE?
//! ===========================================

class databaseItem;
class databaseReader;
class databaseWriter;
class databaseModel;
class mappingTreeItem;
class temporaryResults;
class QtBamAlignment;

class database
{
public:
    database(const QVector<QVariant> &headers, const uint &indexStepSize = 10000);
    ~database();

    //! retrieve headers and step size
    uint getStepSize() const;
    QVector<QVariant> getHeaders() const;

    //! reading and writing the database
    bool readData(const QString &fileName);
    bool writeData(const QString &fileName);
//private:
    //void writeCountElement(QTextStream &out, databaseItem *item); //not required now as we just report loci
public:
    bool writeCountTable(const QString &fileName); // writes only the loci
    bool writeSampleWiseCountTables(const QString &fileName); // writes only the loci, but samplewise and not just overall

private:
    //! the rootItem of the database and the index
    databaseItem *rootItem;
    QHash< QPair<QString,uint>, QVector<databaseItem*> > CPindex;
    uint stepSize;

    //! compulsory columns
    uint NAMECOL;
    uint CHROMCOL;
    uint STRANDCOL;
    uint STARTCOL;
    uint ENDCOL;

    //! optional columns
    // standard GFF entries
    uint SOURCECOL;             // Ssource
    uint FEATURECOL;            // Sfeature
    uint ASSEMBLEDFEATURECOL;   // SassembledFeature
    uint PHASECOL;              // Sphase
    uint SCORECOL;              // Fscore
    // other entries
    uint PRIORITYCOL;       // Upriority   COLUMN with the priority
    uint SUMUNAMBCOL;       // Fsumunamb   COLUMN with sum of unambiguous hits - at any time
    uint SUMAMBCOL;         // Fsumamb     COLUMN with sum of ambiguous hits BEFORE allocation
    uint SUMALLOCOL;        // Fsumallo    COLUMN with sum of ambiguous hits AFTER allocation
    uint MINDISTUNAMBCOL;   // Udistunamb  COLUMN with sum of unambiguous hits in the first minDist bases
    uint MINDISTAMBCOL;     // Udistamb    COLUMN with sum of ambiguous hits BEFORE allocation in the first minDist bases
    uint MINDISTALLOCOL;    // Udistallo   COLUMN with sum of ambiguous hits AFTER allocation in the first minDist bases
    uint TOTCOL;            // Ftothits    COLUMN with sum of all hits (unamb and allocated amb)
    uint VALIDCOL;          // Uvalid      COLUMN with the info if the feature passed the last filter or not
    // other entries samplewise
    QHash<QString, uint> SAMPLEWISESUMUNAMBCOL;       // Fsumunamb   COLUMN with sum of unambiguous hits - at any time
    QHash<QString, uint> SAMPLEWISESUMAMBCOL;         // Fsumamb     COLUMN with sum of ambiguous hits BEFORE allocation
    QHash<QString, uint> SAMPLEWISESUMALLOCOL;        // Fsumallo    COLUMN with sum of ambiguous hits AFTER allocation
    QHash<QString, uint> SAMPLEWISEMINDISTUNAMBCOL;   // Udistunamb  COLUMN with sum of unambiguous hits in the first minDist bases
    QHash<QString, uint> SAMPLEWISEMINDISTAMBCOL;     // Udistamb    COLUMN with sum of ambiguous hits BEFORE allocation in the first minDist bases
    QHash<QString, uint> SAMPLEWISEMINDISTALLOCOL;    // Udistallo   COLUMN with sum of ambiguous hits AFTER allocation in the first minDist bases
    QHash<QString, uint> SAMPLEWISETOTCOL;            // Ftothits    COLUMN with sum of all hits (unamb and allocated amb)
    QHash<QString, uint> SAMPLEWISEVALIDCOL;          // Uvalid      COLUMN with the info if the feature passed the last filter or not

    //! for a fast lookup
    uint getOffset(const uint &position) const;
    void createIndex();

    //! some friends that need to have access
    friend class databaseReader;
    friend class databaseWriter;
    friend class databaseModel;

public:
    //! an old timeprint function
    void print_time(const QString &message) const;

    /*!
      basic data access
      */

    // get the length of an item
    int getLen(databaseItem *item) const;

    // get distance to START and END of an item
    int getStartDist(databaseItem *item, int &pos) const;
    int getEndDist(databaseItem *item, int &pos) const;

    // get distance to three or five prime end of an item
    int getThreePrimeDist(databaseItem *item, int &start, int &end) const;
    int getFivePrimeDist(databaseItem *item, int &start, int &end) const;

    // given a list of elements, search for the corresponding loci / transcripts (only unique)
    QList<databaseItem*> getLoci(QList<databaseItem*> &foi) const;
    QList<databaseItem*> getTranscripts(QList<databaseItem*> &foi) const;

    /*!
      basic data analysis modifiers - I decided to keep the database as passive as possible (all via temporaryResults)
      */
private:
    // this function is used to check if all the necessary columns for data upsate are present
    bool checkColumns() const;
    // this function is called to modify the data recursively -> would not be necessary with a name index
    void updateDataElement(databaseItem* item, const QHash<QString, temporaryResults> &tempData, const int &minReads, const int &minBelowMaxDist);
public:
    // insert the sampleWiseColumns
    bool insertSampleWiseColumns(const QHash<QString, QHash<QString, temporaryResults> > &tempData);
    // this is the updater that can be called from outside
    bool updateData(const QHash<QString, temporaryResults> &tempData, const int &minReads, const int &minBelowMaxDist);
    bool updateDataSampleWise(const QHash<QString, QHash<QString, temporaryResults> > &tempData, const int &minReads, const int &minBelowMaxDist);



    /*!
      VECTOR BASED RETURNS
      */

    /*!
      mapping reporting HITS TO ALL LOCI
      Lmap for locus mapping only
      */

    //! not strand specific
    // mapping with void return -> Supply a QVector
    void LmapPosition(const QString &chrom, const uint &position, QVector<databaseItem*> &results) const;
    void LmapRange(const QString &chrom, const uint &start, const uint &end, QVector<databaseItem*> &results) const;
    void LmapGap(const QString &chrom, const uint &start, const uint &end, QVector<databaseItem*> &results) const;
    // mapping with the results returned as a QVector (copying of vectors - goes via version above)
    QVector<databaseItem*> LmapPositionVec(const QString &chrom, const uint &position) const;
    QVector<databaseItem*> LmapRangeVec(const QString &chrom, const uint &start, const uint &end) const;
    QVector<databaseItem*> LmapGapVec(const QString &chrom, const uint &start, const uint &end) const;
    //! strand specific
    // mapping with void return -> Supply a QVector
    void LmapPositionStrand(const QString &chrom, const uint &position, const QString &strand, QVector<databaseItem*> &results) const;
    void LmapRangeStrand(const QString &chrom, const uint &start, const uint &end, const QString &strand, QVector<databaseItem*> &results) const;
    void LmapGapStrand(const QString &chrom, const uint &start, const uint &en, const QString &strandd, QVector<databaseItem*> &results) const;
    // mapping with the results returned as a QVector (copying of vectors - goes via version above)
    QVector<databaseItem*> LmapPositionVecStrand(const QString &chrom, const uint &position, const QString &strand) const;
    QVector<databaseItem*> LmapRangeVecStrand(const QString &chrom, const uint &start, const uint &end, const QString &strand) const;
    QVector<databaseItem*> LmapGapVecStrand(const QString &chrom, const uint &start, const uint &end, const QString &strand) const;

    /*!
      RECURSIVE mapping reporting HITS TO ALL FEATURES OF ALL LOCI - NOTE: TRIES TO FIND THE LOWEST LEVEL POSSIBLE
      Rmap for recursive mapping (for each locus the hits at the lowest level)
      the ones that are commented out would separate results from different loci
      similar commenting out is also done in the source file
      */

    //! not strand specific
    QVector<databaseItem*> RmapPosition(const QString &chrom, const uint &position) const;
    QVector<databaseItem*> RmapRange(const QString &chrom, const uint &start, const uint &end) const;
    QVector<databaseItem*> RmapGap(const QString &chrom, const uint &start, const uint &end) const;
    //QVector< QVector<databaseItem*> > RmapPosition(const QString &chrom, const uint &position) const;
    //QVector< QVector<databaseItem*> > RmapRange(const QString &chrom, const uint &start, const uint &end) const;
    //QVector< QVector<databaseItem*> > RmapGap(const QString &chrom, const uint &start, const uint &end) const;
    //! strand specific
    QVector<databaseItem*> RmapPositionStrand(const QString &chrom, const uint &position, const QString& strand) const;
    QVector<databaseItem*> RmapRangeStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand) const;
    QVector<databaseItem*> RmapGapStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand) const;
    //QVector< QVector<databaseItem*> > RmapPositionStrand(const QString &chrom, const uint &position, const QString& strand) const;
    //QVector< QVector<databaseItem*> > RmapRangeStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand) const;
    //QVector< QVector<databaseItem*> > RmapGapStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand) const;

    /*!
      mapping reporting HITS TO ONLY BEST LOCI
      Lmap for locus mapping only
      */

    //! not strand specific
    // mapping with void return -> Supply a QVector
    void bestLmapPosition(const QString &chrom, const uint &position, QVector<databaseItem*> &results) const;
    void bestLmapRange(const QString &chrom, const uint &start, const uint &end, QVector<databaseItem*> &results) const;
    void bestLmapGap(const QString &chrom, const uint &start, const uint &end, QVector<databaseItem*> &results) const;
    // mapping with the results returned as a QVector (copying of vectors - goes via version above)
    QVector<databaseItem*> bestLmapPositionVec(const QString &chrom, const uint &position) const;
    QVector<databaseItem*> bestLmapRangeVec(const QString &chrom, const uint &start, const uint &end) const;
    QVector<databaseItem*> bestLmapGapVec(const QString &chrom, const uint &start, const uint &end) const;
    //! strand specific
    // mapping with void return -> Supply a QVector
    void bestLmapPositionStrand(const QString &chrom, const uint &position, const QString& strand, QVector<databaseItem*> &results) const;
    void bestLmapRangeStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand, QVector<databaseItem*> &results) const;
    void bestLmapGapStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand, QVector<databaseItem*> &results) const;
    // mapping with the results returned as a QVector (copying of vectors - goes via version above)
    QVector<databaseItem*> bestLmapPositionVecStrand(const QString &chrom, const uint &position, const QString& strand) const;
    QVector<databaseItem*> bestLmapRangeVecStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand) const;
    QVector<databaseItem*> bestLmapGapVecStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand) const;

    /*!
      RECURSIVE mapping reporting HITS TO ONLY BEST LOWEST FEATURES POSSIBLE OF BEST LOCI
      Rmap for recursive mapping (for each locus the hits at the lowest level (therefrom the best)
      the ones that are commented out would separate results from different loci
      similar commenting out is also done in the source file
      */

    //! not strand specific
    QVector<databaseItem*> bestRmapPosition(const QString &chrom, const uint &position) const;
    QVector<databaseItem*> bestRmapRange(const QString &chrom, const uint &start, const uint &end) const;
    QVector<databaseItem*> bestRmapGap(const QString &chrom, const uint &start, const uint &end) const;
    //QVector< QVector<databaseItem*> > bestRmapPosition(const QString &chrom, const uint &position) const;
    //QVector< QVector<databaseItem*> > bestRmapRange(const QString &chrom, const uint &start, const uint &end) const;
    //QVector< QVector<databaseItem*> > bestRmapGap(const QString &chrom, const uint &start, const uint &end) const;
    //! strand specific
    QVector<databaseItem*> bestRmapPositionStrand(const QString &chrom, const uint &position, const QString& strand) const;
    QVector<databaseItem*> bestRmapRangeStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand) const;
    QVector<databaseItem*> bestRmapGapStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand) const;
    //QVector< QVector<databaseItem*> > bestRmapPositionStrand(const QString &chrom, const uint &position, const QString& strand) const;
    //QVector< QVector<databaseItem*> > bestRmapRangeStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand) const;
    //QVector< QVector<databaseItem*> > bestRmapGapStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand) const;

    //! mappingTreeItem based functions with void return - used in combination with alignments
    //! RECURSIVE - ALL LOCI - ALL FEATURES (this is the only recursive one that does not reduce to the lowest possible)

    //! not strand specific
    void nestedRmapPosition(const QString &chrom, const uint &position, QList<mappingTreeItem> &results) const;
    void nestedRmapRange(const QString &chrom, const uint &start, const uint &end, QList<mappingTreeItem> &results) const;
    void nestedRmapGap(const QString &chrom, const uint &start, const uint &end, QList<mappingTreeItem> &results) const;

    //! strand specific
    void nestedRmapPositionStrand(const QString &chrom, const uint &position, const QString& strand, QList<mappingTreeItem> &results) const;
    void nestedRmapRangeStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand, QList<mappingTreeItem> &results) const;
    void nestedRmapGapStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand, QList<mappingTreeItem> &results) const;

    //! mappingTreeItem based functions with void return - used in combination with alignments
    //! RECURSIVE - ONLY THE BEST AT EACH LEVEL, BUT ALL LEVELS (this is the only recursive one that does not reduce to the lowest possible)

    //! not strand specific
    void bestNestedRmapPosition(const QString &chrom, const uint &position, QList<mappingTreeItem> &results) const;
    void bestNestedRmapRange(const QString &chrom, const uint &start, const uint &end, QList<mappingTreeItem> &results) const;
    void bestNestedRmapGap(const QString &chrom, const uint &start, const uint &end, QList<mappingTreeItem> &results) const;

    //! strand specific
    void bestNestedRmapPositionStrand(const QString &chrom, const uint &position, const QString& strand, QList<mappingTreeItem> &results) const;
    void bestNestedRmapRangeStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand, QList<mappingTreeItem> &results) const;
    void bestNestedRmapGapStrand(const QString &chrom, const uint &start, const uint &end, const QString& strand, QList<mappingTreeItem> &results) const;

    /*!
      Mapping of a full read (all cigar operations) - called by readmapper:
      1. get the best loci, set mapFlag
      2. get the transcripts of those
      3. get the subfeatures of those transcripts, set mapFlag
      note that the pairedEnd and MatePair versions are yet to be implemented
      we map down everything as far as possible, keep all the mappings but note if there is a mapping at the lowest level
      */

    //! not strand or strand specific
    void alignmentMapFragment(QtBamAlignment &al, const bool& stranded, const bool& antisense) const;
    void alignmentMapFragmentGapped(QtBamAlignment &al, const bool& stranded, const bool& antisense) const;
    //void alignmentMapPairedEnd(QtBamAlignment &alA, QtBamAlignment &alB, const bool& stranded) const;
    //void alignmentMapMatePair(QtBamAlignment &alA, QtBamAlignment &alB, const bool& stranded) const;

};

#endif // DATABASE_H
