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

#ifndef ANNOTATION_READER_H
#define ANNOTATION_READER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMap>
#include <iostream>

//! definition of a base class file reader - no template - just with a different type given as argument
class annotationReader {
private:
    // private member variable (the data stream and the file)
    QString filetype;
    QFile file;
    QTextStream in;
    // read a line and tell if success - as base class default, this is the gtf/gff style
    bool readBedLine();
    bool readGtfLine();
    bool readGffLine();
    // get the attributes from a line (important for the GTFs), as base class this returns false (not used anywhere)
    bool getGtfAttributesREGEXP(QString& str);
    bool getGtfAttributes(QString& str);
    bool getGffAttributes(QString& str);

public:
    //! member variables - the current line
    // coordinates
    QString chrom;
    QString strand;
    uint start;
    uint end;
    // feature source, type, name and parent name
    QString source;
    QString feature;
    QString name;
    QString parent;
    // all attributes
    QMap<QString,QString> Attr;
    // score of the feature
    float score;
    // phase of the feature (only used for CDS)
    QString phase;
    // just the line number
    uint linenumber;
    // BED files have multiple exons per line - so instead of rewriting the data loading, we just imitate multiple lines
    QStringList fakeBedLines;
    /* the entries of such a fake BedLine are:
    chrom = fields.at(0);
    strand = fields.at(1);
    start = fields.at(2).toUInt();
    end = fields.at(3).toUInt();
    feature = fields.at(4);
    score = fields.at(5).toFloat();
    name = fields.at(6);
    parent = fields.at(7);
    */


    //! member functions
    // constructor (open and check the file)
    annotationReader(QString& annofile, QString& annotype);

    // destructor (close file normally)
    ~annotationReader();

    // read a line and tell if success - as base class default, this is the gtf/gff style
    bool readLine();

    // get the file type
    QString getFileType();
};

//! INCLUDE THE SOURCE CODE (INLINE FILE)
#include "annotation_reader.cpp"


#endif // ANNOTATION_READER_H
