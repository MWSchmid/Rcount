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

#ifndef SIMPLE_DATA_H
#define SIMPLE_DATA_H

#include <QString>
#include <QMap>
#include <QVector>
#include <QtAlgorithms>

//! NOTE THAT THIS CLASS IS USED EQUALLY FOR GENE AND TRANSCRIPT
class simple_data
{
public:
    // fully public (like a struct)
    // members for the feature itself
    QString chrom;
    QString strand;
    uint start;
    uint end;
    QString source;
    QString feature;
    QString assembled_feature;
    QString name;
    QString parent;
    float score;
    QString phase;
    // collections for the children
    QVector<uint> exon_starts;
    QVector<uint> exon_ends;
    QVector<QString> exon_features;
    QVector<QString> assembled_exon_features;
    QVector<uint> splice_starts;
    QVector<uint> splice_ends;
    QVector<QString> splice_features;
    QVector<QString> assembled_splice_features;
    QVector<uint> utr_starts;
    QVector<uint> utr_ends;
    QVector<QString> utr_features;
    QVector<QString> assembled_utr_features;

    // functions
    simple_data();
    void insert_data(
        QString& chromin,
        QString& strandin,
        uint& startin,
        uint& endin,
        QString& sourcein,
        QString& featurein,
        QString& namein,
        QString& parentin,
        float& scorein,
        QString& phasein);

    // destructor?

    // add a child
    void add_exon(uint& startin, uint& endin, QString& featurein);
    void add_utr(uint& startin, uint& endin, QString& featurein);

    // calculate splice junctions if relevant
    void get_splice_junctions();

    // insert assembled feature (eg gene|mRNA and gene|mRNA|exon)
    void insert_assembled_feature(QString& featurein);

};

//! here are two functions used for sorting a vector of simple data
// use this one first with qSort
bool SortSimpleDataByStart(const simple_data &s1, const simple_data &s2);
// then this one with qStableSort
bool SortSimpleDataByChrom(const simple_data &s1, const simple_data &s2);

//! INCLUDE THE SOURCE CODE (INLINE FILE)
#include "simple_data.cpp"


#endif // SIMPLE_DATA_H

