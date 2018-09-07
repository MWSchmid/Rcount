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

#ifndef SIMPLE_DATA_H
#include "simple_data.h" //! do not include while compiling (as it is inline)!
#endif

inline simple_data::simple_data()
{
    chrom = "";
    strand = "";
    start = 0;
    end = 0;
    source = "";
    feature = "";
    name = "";
    parent = "";
    score = 0;
    phase = "";
}

inline void simple_data::insert_data(
    QString& chromin,
    QString& strandin,
    uint& startin,
    uint& endin,
    QString& sourcein,
    QString& featurein,
    QString& namein,
    QString& parentin,
    float& scorein,
    QString& phasein)
{
    chrom = chromin;
    strand = strandin;
    start = startin;
    end = endin;
    source = sourcein;
    feature = featurein;
    name = namein;
    parent = parentin;
    score = scorein;
    phase = phasein;
}

inline void simple_data::add_exon(uint &startin, uint &endin, QString &featurein)
{
    exon_starts.push_back(startin);
    exon_ends.push_back(endin);
    exon_features.push_back(featurein);
}

inline void simple_data::add_utr(uint &startin, uint &endin, QString &featurein)
{
    utr_starts.push_back(startin);
    utr_ends.push_back(endin);
    utr_features.push_back(featurein);
}

inline void simple_data::get_splice_junctions()
{
    uint start = 0;
    uint end = 0;
    qSort(exon_starts.begin(), exon_starts.end(), qLess<uint>()); //! TODO: sort also the features
    qSort(exon_ends.begin(), exon_ends.end(), qLess<uint>()); //! TODO: sort also the features
    for (int i = 0; i < (exon_starts.size() - 1); ++i) {
        start = exon_ends.at(i) + 1; // first base of intron
        end = exon_starts.at(i+1) - 1; // last base of intron
        splice_starts.push_back(start);
        splice_ends.push_back(end);
        splice_features.push_back(QString("splice"));
    }
}

inline void simple_data::insert_assembled_feature(QString& featurein) {
    assembled_feature = featurein;
    QString newname;
    for (int i = 0; i < exon_features.size(); ++i) {
        newname = QObject::tr("%1|%2").arg(featurein, exon_features.at(i));
        assembled_exon_features.push_back(newname);
    }
    for (int i = 0; i < utr_features.size(); ++i) {
        newname = QObject::tr("%1|%2").arg(featurein, utr_features.at(i));
        assembled_utr_features.push_back(newname);
    }
    for (int i = 0; i < splice_features.size(); ++i) {
        newname = QObject::tr("%1|%2").arg(featurein, splice_features.at(i));
        assembled_splice_features.push_back(newname);
    }
}

//! here are two functions used for sorting a vector of simple data
// use this one first with qSort
inline bool SortSimpleDataByStart(const simple_data &s1, const simple_data &s2) { return s1.start < s2.start; }
// then this one with qStableSort
inline bool SortSimpleDataByChrom(const simple_data &s1, const simple_data &s2) { return s1.chrom.toLower() < s2.chrom.toLower(); }

