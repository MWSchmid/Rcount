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

#ifndef SIMPLE_DATA_COLLECTION_H
#define SIMPLE_DATA_COLLECTION_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QMap>
#include <QSet>

#ifndef SIMPLE_DATA_H
#include "simple_data.h"
#endif

#ifndef ANNOTATION_READER_H
#include "../p502_SOURCE/annotation_reader/annotation_reader.h"
#endif

class simple_data_collection
{
private:
    QString filename;
    QString filetype;
    QMap<QString, uint> priorities;
    QVector<simple_data> loci; // all loci
    QVector<simple_data> transcripts; // all transcripts
    QMap<QString, uint> maptranscripts; // transcript name to its index in the vector
    QMap<QString, QString> lototr; //loci to transcripts, multimap
    QStringList structure; // like a file for the viewer
    uint three_prime;
    uint five_prime;
    bool useZeroBased;
    bool useEnsemblLoader;

public:
    simple_data_collection();

    //! simple update functions
    void set_zeroBased(bool& useZeroBasedIn);
    void set_filename(QString& filenamein);
    void set_filetype(QString& filetypein);
    void set_3_prime(uint& in);
    void set_5_prime(uint& in);
    void set_ensembl_loader(bool& useEnsemblLoaderIn);
    void clear_all();

    //! simple access function
    QString get_structure();

public:
    //! reclaim the values from the treeview -> separate; not implemented
    bool update_treestructure(QString treestructure);
    bool update_priorities();

    //! load the data and build the correct order
    bool load_data();

private:
    //! load the structure for the treeview (the simple way using load_data first)
    bool load_structure();
    bool load_data_ensembl();
    bool load_data_generic();

public:
    //! function to write the new annotation file - V1
    bool write_xml_V1(QString& outfilein);
    void write_simple_data_V1(QXmlStreamWriter *out, simple_data *dat);
    void write_vector_data_V1(
        QXmlStreamWriter *out,
        simple_data *dat,
        QString *feature,
        QString *assembledFeature,
        uint *start,
        uint *end,
        int *number);
    //! function to write the new annotation file - V2
    bool write_xml_V2(QString& outfilein);
    void write_simple_data_V2(QXmlStreamWriter *out, simple_data *dat);
    void write_vector_data_V2(
        QXmlStreamWriter *out,
        simple_data *dat,
        QString *feature,
        QString *assembledFeature,
        uint *start,
        uint *end,
        int *number);
};

//! INCLUDE THE SOURCE CODE (INLINE FILE)
#include "simple_data_collection.cpp"

#endif // SIMPLE_DATA_COLLECTION_H
