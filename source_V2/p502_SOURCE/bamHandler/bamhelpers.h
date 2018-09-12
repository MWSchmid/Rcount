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

#ifndef BAMHELPERS_H
#define BAMHELPERS_H

#include <QtCore>
#include <QString>
#include <QStringList>
#include <iostream>

#include "../dataStructure/mappingtreeitem.h"

class databaseItem;

/*!
  This are some constants for alignment flag queries
*/
const int QT_ALIGNMENT_PAIRED              = 0x0001;
const int QT_ALIGNMENT_PROPER_PAIR         = 0x0002;
const int QT_ALIGNMENT_UNMAPPED            = 0x0004;
const int QT_ALIGNMENT_MATE_UNMAPPED       = 0x0008;
const int QT_ALIGNMENT_REVERSE_STRAND      = 0x0010;
const int QT_ALIGNMENT_MATE_REVERSE_STRAND = 0x0020;
const int QT_ALIGNMENT_READ_A              = 0x0040;//this was_1
const int QT_ALIGNMENT_READ_B              = 0x0080;//this was_2
const int QT_ALIGNMENT_SECONDARY           = 0x0100;
const int QT_ALIGNMENT_QC_FAILED           = 0x0200;
const int QT_ALIGNMENT_DUPLICATE           = 0x0400;

/*!
  This are some constants for the map flag entries
  They will be written out as a XM:i tag
*/

const int QT_MAPPING_MULTI              = 0x0001; // the if multiple mappings or not
const int QT_MAPPING_SKIP               = 0x0002; // tells if skipped or not
const int QT_MAPPING_GAPPED             = 0x0004; // the read has gaps
const int QT_MAPPING_WEIGHT             = 0x0008; // tells if weight is zero (is mainly useful in combination with others)
const int QT_MAPPING_LOCUS              = 0x0010; // the read maps to a known locus
const int QT_MAPPING_EXON               = 0x0020; // the read maps to a known exon
const int QT_MAPPING_SPLICE             = 0x0040; // the read maps to a known splice junction
const int QT_MAPPING_AMBIGUOUS          = 0x0080; // the read maps ambiguous


/*!
  This is a header only class for cigar operations
*/
class QtCigarOp {
public:
    // data members
    QChar _type;   // Operation type (MIDNSHP) - only M and N are considered at the moment - added D and I in 2014
    uint _length; // Operation length (number of bases)

    // constructor
    QtCigarOp(QChar type = 'M', uint length = 0) {
        this->_type = type;
        this->_length = length;
    }
};

/*!
  This here is a header only class for a bam alignment.
  It contains the most important things from the Bamtools alignment class.
  All values are however stored in a Qt-native format
*/

class QtBamAlignment {
public:
    //! member variables
    QString _name;          // Read name
    int _length;            // Query length
    QString _queryBases;    // 'Original' sequence (as reported from sequencing machine)
    QString _qualities;     // FASTQ qualities (ASCII characters, not numeric values)
    QString _refName;       // the name of the reference sequence belonging to the refID
    int _position;          // Position (0-based) where alignment starts
    uint _mapQuality;       // Mapping quality score
    uint _bin;              // BAM (standard) index bin number for this alignment
    uint _alignmentFlag;    // Alignment bit-flag - see Is<something>() methods to query this value, SetIs<something>() methods to manipulate
    QVector<QtCigarOp> _cigarData; // CIGAR operations for this alignment
    float _weight;          // the weight of the read (if given, otherwise defaults to 1) - this is the XW TAG
    uint _numal;            // the number of alignments - this is the NH tag
    uint _mapFlag;          // the flag from the mapping (bit-flag) - access via Is<Something>() and change with SetIs<Something>()
    unsigned char _spliceFlag;       // this is the XS flag from topHat - ASCII code -> 46: dot, 43: plus, 45: minus
    //! added XS tag
    //! not existing anymore with seqan:
    //QString _alignedBases;  // 'Aligned' sequence (includes any indels, padding, clipping)
    //QString _mateRefName;   // the name of the reference sequence where alignment's mate was aligned
    //int _matePosition;      // Position (0-based) where alignment's mate starts
    //int _insertSize;        // Mate-pair insert size

    // a structure holding the mappings in a tree-like form
    QList<mappingTreeItem> _mappings; // note that in readmapper, all loci irrespective of the priority are added - however, isAmbiguous will only return true if two or more items with the same&highest priority are mapped

    // constructor does nothing but create a new mappingItem (the rootItem)
    QtBamAlignment() {
        this->_mappings.clear();
    }

    // destructor - necessary?

    // get number of splices
    //int getNumSplice() { return(this->_cigarData.count()/2); } // integer division (i want to truncate/floor it) -is only true if only M/N operations

    // get the number of cigar operations or num gaps
    int getNumCop() { return(this->_cigarData.count()); }

    // get the endposition (CLOSED INTERVAL)
    int getEnd() {
        int out = this->_position - 1; // minus one because start and end are both included in the database; here, the start is within, but adding the length, the end should not be included
        // iterate over the cigar Operations
        for (QVector<QtCigarOp>::iterator cigarIter = this->_cigarData.begin(); cigarIter != this->_cigarData.end(); ++cigarIter) {
            if ( ((*cigarIter)._type == 'I') || ((*cigarIter)._type == 'S') || ((*cigarIter)._type == 'H') ) { continue; } // inserts and clipping should not be added
            out += cigarIter->_length;
        }
        return(out);
    }

    //! new in the countsPerTranscript version get gene and transcript names
    void getGeneAndTranscriptNames(QString& XTtag) {
        QStringList mappingGenesAndTranscripts;
        foreach (mappingTreeItem gene, this->_mappings) {
            mappingGenesAndTranscripts << gene.getNames();
        }
        XTtag.clear();
        XTtag = mappingGenesAndTranscripts.join(";");
    }

    // alignment flag queries -> same as in bamtools (instead of Mapped I use Aligned)
    bool IsDuplicate() const            { return ( (this->_alignmentFlag & QT_ALIGNMENT_DUPLICATE)     != 0 ); }
    bool IsFailedQC() const             { return ( (this->_alignmentFlag & QT_ALIGNMENT_QC_FAILED)     != 0 ); }
    bool IsFirstMate() const            { return ( (this->_alignmentFlag & QT_ALIGNMENT_READ_A)        != 0 ); }
    bool IsAligned() const              { return ( (this->_alignmentFlag & QT_ALIGNMENT_UNMAPPED)      == 0 ); }
    bool IsMateAligned() const          { return ( (this->_alignmentFlag & QT_ALIGNMENT_MATE_UNMAPPED) == 0 ); }
    bool IsMateReverseStrand() const    { return ( (this->_alignmentFlag & QT_ALIGNMENT_MATE_REVERSE_STRAND)  != 0 ); }
    bool IsPaired() const               { return ( (this->_alignmentFlag & QT_ALIGNMENT_PAIRED)        != 0 ); }
    bool IsPrimaryAlignment() const     { return ( (this->_alignmentFlag & QT_ALIGNMENT_SECONDARY)     == 0 ); }
    bool IsProperPair() const           { return ( (this->_alignmentFlag & QT_ALIGNMENT_PROPER_PAIR)   != 0 ); }
    bool IsReverseStrand() const        { return ( (this->_alignmentFlag & QT_ALIGNMENT_REVERSE_STRAND)       != 0 ); }
    bool IsSecondMate() const           { return ( (this->_alignmentFlag & QT_ALIGNMENT_READ_B)        != 0 ); }

    // map flag queries
    bool IsMulti() const                { return ( (this->_mapFlag & QT_MAPPING_MULTI)          != 0 ); }
    bool IsSkipped() const              { return ( (this->_mapFlag & QT_MAPPING_SKIP)           != 0 ); }
    bool IsGapped() const               { return ( (this->_mapFlag & QT_MAPPING_GAPPED)         != 0 ); }
    bool IsZeroWeight() const           { return ( (this->_mapFlag & QT_MAPPING_WEIGHT)         != 0 ); }
    bool IsMappingToLocus() const       { return ( (this->_mapFlag & QT_MAPPING_LOCUS)          != 0 ); }
    bool IsMappingToExon() const        { return ( (this->_mapFlag & QT_MAPPING_EXON)           != 0 ); }
    bool IsMappingToSplice() const      { return ( (this->_mapFlag & QT_MAPPING_SPLICE)         != 0 ); }
    bool IsAmbiguous() const            { return ( (this->_mapFlag & QT_MAPPING_AMBIGUOUS)      != 0 ); }


    // map flag modifiers
    void setIsMulti(bool ok) {
        if (ok) { this->_mapFlag |=  QT_MAPPING_MULTI; }
        else    { this->_mapFlag &= ~QT_MAPPING_MULTI; }
    }
    void setIsSkipped(bool ok) {
        if (ok) { this->_mapFlag |=  QT_MAPPING_SKIP; }
        else    { this->_mapFlag &= ~QT_MAPPING_SKIP; }
    }
    void setIsGapped(bool ok) {
        if (ok) { this->_mapFlag |=  QT_MAPPING_GAPPED; }
        else    { this->_mapFlag &= ~QT_MAPPING_GAPPED; }
    }
    void setIsZeroWeight(bool ok) {
        if (ok) { this->_mapFlag |=  QT_MAPPING_WEIGHT; }
        else    { this->_mapFlag &= ~QT_MAPPING_WEIGHT; }
    }
    void setIsMappingToLocus(bool ok) {
        if (ok) { this->_mapFlag |=  QT_MAPPING_LOCUS; }
        else    { this->_mapFlag &= ~QT_MAPPING_LOCUS; }
    }
    void setIsMappingToExon(bool ok) {
        if (ok) { this->_mapFlag |=  QT_MAPPING_EXON; }
        else    { this->_mapFlag &= ~QT_MAPPING_EXON; }
    }
    void setIsMappingToSplice(bool ok) {
        if (ok) { this->_mapFlag |=  QT_MAPPING_SPLICE; }
        else    { this->_mapFlag &= ~QT_MAPPING_SPLICE; }
    }
    void setIsAmbiguous(bool ok) {
        if (ok) { this->_mapFlag |=  QT_MAPPING_AMBIGUOUS; }
        else    { this->_mapFlag &= ~QT_MAPPING_AMBIGUOUS; }
    }
};

/*!
  This is a header only class that holds bamStats and can decode them
  */
class QtBamStats {
private:
    // data members
    QVector<qreal> _stats;
    // some decoders (one can take the position in the vector as a _mapFlag)
    bool IsMulti(int &index) const                { return ( (index & QT_MAPPING_MULTI)          != 0 ); }
    bool IsSkipped(int &index) const              { return ( (index & QT_MAPPING_SKIP)           != 0 ); } // also not too important
    bool IsGapped(int &index) const               { return ( (index & QT_MAPPING_GAPPED)         != 0 ); }
    bool IsZeroWeight(int &index) const           { return ( (index & QT_MAPPING_WEIGHT)         != 0 ); } // well - this is not really of use as += weight X]
    bool IsMappingToLocus(int &index) const       { return ( (index & QT_MAPPING_LOCUS)          != 0 ); }
    bool IsMappingToExon(int &index) const        { return ( (index & QT_MAPPING_EXON)           != 0 ); }
    bool IsMappingToSplice(int &index) const      { return ( (index & QT_MAPPING_SPLICE)         != 0 ); }
    bool IsAmbiguous(int &index) const            { return ( (index & QT_MAPPING_AMBIGUOUS)      != 0 ); }

public:
    // simple constructor
    QtBamStats(){ this->_stats.fill(0, 255); }
    // default destructor
    ~QtBamStats(){}
    // add an alignment to the stats
    void addAlignment(QtBamAlignment &al) { this->_stats[al._mapFlag] += al._weight; }
    // create a String that can be used to send as a signal to somewhere else. Its meant to make a treeView out of it (add it in the projecttreeview)
    QString getTreeViewString() {
        QMap<QString, qreal> tempMap;
        QString name;
        for (int index = 0; index != this->_stats.count(); ++index) {
            // skip all the empty entries
            if (this->_stats.at(index) == 0) { continue; }
            // top level - unique or multi
            if (this->IsMulti(index))                   { name = "0-multiple"; }
            else                                        { name = "0-unique"; }
            if (!tempMap.contains(name)) { tempMap.insert(name, 0); }
            tempMap[name] += this->_stats.at(index);
            // first level - locus or intergenic
            if (this->IsMappingToLocus(index))          { name += "-1-locus";
                if (!tempMap.contains(name)) { tempMap.insert(name, 0); }
                tempMap[name] += this->_stats.at(index);
                // second level - exon, splice, or no match
                if (this->IsMappingToExon(index))           { name += "-2-exon"; }
                else if (this->IsMappingToSplice(index))    { name += "-2-splice"; }
                else                                        { name += "-2-unmatched"; }
                if (!tempMap.contains(name)) { tempMap.insert(name, 0); }
                tempMap[name] += this->_stats.at(index);
                // this level - ambiguous or not
                if (this->IsAmbiguous(index))                   { name += "-3-ambiguous"; }
                else                                            { name += "-3-unambiguous"; }
                if (!tempMap.contains(name)) { tempMap.insert(name, 0); }
                tempMap[name] += this->_stats.at(index);
            }
            else                                        { name += "-1-intergenic";
                if (!tempMap.contains(name)) { tempMap.insert(name, 0); }
                tempMap[name] += this->_stats.at(index);
                if (this->IsGapped(index))                  { name += "-2-gapped"; }
                else                                        { name += "-2-ungapped"; }
                if (!tempMap.contains(name)) { tempMap.insert(name, 0); }
                tempMap[name] += this->_stats.at(index);
            }
        }
        //  now they should somehow be sorted and concatenated
        QString out = "";
        QList<QString> sortedKeys = tempMap.keys();
        qSort(sortedKeys.begin(), sortedKeys.end(), qLess<QString>());
        foreach (QString key, sortedKeys) {
            out += key;
            out += "-";
            out += QString::number(tempMap.value(key), 'f', 2);
            out += "|";
        }
        out.chop(1);
        //std::cerr << out.toStdString() << std::endl << std::flush;
        return(out);
    }
};
/*
0-multiple-21|
0-multiple-1-locus-21|
0-multiple-1-locus-2-exon-15|
0-multiple-1-locus-2-exon-3-unambiguous-15|
0-multiple-1-locus-2-unmatched-6|
0-multiple-1-locus-2-unmatched-3-unambiguous-6|
0-unique-5|
0-unique-1-intergenic-2|
0-unique-1-intergenic-2-ungapped-2|
0-unique-1-locus-3|
0-unique-1-locus-2-exon-1|
0-unique-1-locus-2-exon-3-unambiguous-1|
0-unique-1-locus-2-unmatched-2|
0-unique-1-locus-2-unmatched-3-unambiguous-2
*/
#endif // BAMHELPERS_H
