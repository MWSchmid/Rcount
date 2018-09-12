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

#include <QtGui>

#ifndef ANNOTATION_READER_H
#include "annotation_reader.h" //! do not include while compiling (as it is inline)!
#endif



inline annotationReader::annotationReader(QString& annofile, QString& annotype) {
    fakeBedLines.clear();
    filetype = annotype;
    file.setFileName(annofile);
    if(!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Can not open file for reading: "
                  << qPrintable(file.errorString()) << std::endl;
        return;
    }
    in.setDevice(&file);
    in.setCodec("UTF-8");
    linenumber = 0;
    //initialize also others variables?
}

inline annotationReader::~annotationReader() {
    file.close();
}

inline bool annotationReader::readLine() {
    bool rval = false;
    if (filetype == "BED") { rval = readBedLine(); }
    else if (filetype == "GTF") { rval = readGtfLine(); }
    else if (filetype == "GFF3") { rval = readGffLine(); }
    else { std::cerr << "unknown file type" << std::endl; }
    ++linenumber;
    return(rval);
}

inline QString annotationReader::getFileType() {
    return(filetype);
}

//! for BED reading (12 or 14 columns)
inline bool annotationReader::readBedLine() {
    bool rval = false;
    // check if there are some "fake lines" available - in this case read this one and leave the file as it is
    if (fakeBedLines.count() > 0) {
        QString line = fakeBedLines.takeFirst();
        QStringList fields = line.split('\t');
        chrom = fields.at(0);
        strand = fields.at(1);
        start = fields.at(2).toUInt();
        end = fields.at(3).toUInt();
        feature = fields.at(4);
        score = fields.at(5).toFloat();
        name = fields.at(6);
        parent = fields.at(7);
        source = "BED";
        phase = ".";
        rval = true;
    } else {
        if(!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split('\t');
            if (fields.size() >= 6) {
                chrom = fields.at(0);
                start = fields.at(1).toUInt();
                end = fields.at(2).toUInt()-1; //! this is a bit awkward, the start is 0-based, but the end not
                QString modelname = fields.at(3);
                if (fields.size() >= 13) {
                    name = fields.at(12);
                } else {
                    if (modelname.count('.') > 0) {
                        QStringList tempSplit = modelname.split('.');
                        tempSplit.removeLast();
                        name = tempSplit.join(".");
                    } else {
                        name = modelname;
                    }
                }
                QString tempscore = fields.at(4);
                if (tempscore == ".") { score = 0; }
                else { score = tempscore.toFloat(); }
                strand = fields.at(5);
                uint thickStart = fields.at(6).toUInt();
                uint thickEnd = fields.at(7).toUInt();
                QString modelFeature = "";
                if (thickStart == thickEnd) {
                    feature = "non-coding-gene";
                    modelFeature = "non-coding-RNA";
                } else {
                    feature = "gene";
                    modelFeature = "mRNA";
                }
                source = "BED";
                phase = ".";
                parent = "none";
                rval = true;
                // create the fake lines - so first the model
                QStringList toStore;
                toStore << chrom << strand << QString::number(start) << QString::number(end) << modelFeature << QString::number(score) << modelname << name;
                fakeBedLines << toStore.join("\t");
                // and now all the exons
                uint blockCount = fields.at(9).toUInt();
                QStringList blockSizes = fields.at(10).split(',');
                QStringList blockStarts = fields.at(11).split(',');
                uint exonStart = 0;
                uint exonEnd = 0;
                for (uint i = 0; i < blockCount; ++i) {
                    toStore.clear();
                    exonStart = start + blockStarts.at(i).toUInt();
                    exonEnd = exonStart + blockSizes.at(i).toUInt() - 1; //! this is a bit awkward, the start is 0-based, but the end not
                    toStore << chrom << strand << QString::number(exonStart) << QString::number(exonEnd) << "exon" << QString::number(score) << "none" << modelname;
                    fakeBedLines << toStore.join("\t");
                }
            } else {
                if (line.length() == 0) {
                    std::cerr << "skipping empty line" << std::endl << std::flush;
                    rval = this->readBedLine();
                }
                else { if (line.at(0) == '#') {
                        std::cerr << "skipping comment line:" << std::endl << line.toStdString() << std::endl << std::flush;
                        rval = this->readBedLine(); }
                }
            }
        }
    }
    return(rval);
}

//! for GTF reading
inline bool annotationReader::getGtfAttributesREGEXP(QString& str)
{
    bool rval = false;
    Attr.clear(); //very important
    QRegExp rx("(\\w+|\\w+\\.\\w+|\\w+\\.\\w+\\.\\w+|\\w+\\-\\w+|\\w+\\(\\w+\\)\\w+)");
    QString key = "none";
    int counter = 1;
    int pos = 0;
    while ((pos = rx.indexIn(str, pos)) != -1) {
        if ((counter % 2) != 0) { key = rx.cap(1); }
        else {Attr.insert(key, rx.cap(1));}
        pos += rx.matchedLength();
        ++counter;
    }
    rval = (counter != 1);
    if (Attr.contains("gene_id") && Attr.contains("transcript_id")) {
        if (Attr["transcript_id"] != "none") {
            if ((feature == "exon") || (feature == "CDS")) {
                name = "none";
                parent = Attr["transcript_id"];
            } else {
                name = Attr["transcript_id"];
                parent = Attr["gene_id"];
            }
        } else {
            name = Attr["gene_id"];
            parent = "none";
        }
    } else {
        name = "none";
        parent = "none";
    }
    return(rval);
}

inline bool annotationReader::getGtfAttributes(QString& str)
{
    bool rval = false;
    Attr.clear(); //very important
    QStringList pairs = str.split("; ", QString::SkipEmptyParts);
    QString key = "";
    QString val = "";
    foreach (QString pair, pairs) {
        QStringList keyval = pair.split(" \"", QString::SkipEmptyParts);
        if (keyval.length() != 2) {
            std::cerr << pair.toStdString() << std::endl << std::flush;
            continue;
        }
        key = keyval.at(0).simplified();
        val = keyval.at(1).simplified();
        key.remove(" ");
        val.remove(" ");
        val.remove("\"");
        //std::cerr << feature.toStdString() << "|\t|" << key.toStdString() << "|\t|" << val.toStdString() << "|" << std::endl << std::flush;
        Attr.insert(key, val);
    }
    rval = !Attr.isEmpty();
    if (Attr.contains("gene_id") && Attr.contains("transcript_id")) {
        if (Attr["transcript_id"] != "none") {
            if ((feature == "exon") || (feature == "CDS")) {
                name = "none";
                parent = Attr["transcript_id"];
            } else {
                name = Attr["transcript_id"];
                parent = Attr["gene_id"];
            }
        } else {
            name = Attr["gene_id"];
            parent = "none";
        }
    } else {
        if (Attr.contains("gene_id")) {
            name = Attr["gene_id"];
            parent = "none";
        } else {
            name = "none";
            parent = "none";
        }
    }
    return(rval);
}

inline bool annotationReader::readGtfLine() {
    bool rval = false;
    if(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split('\t');
        if (fields.length() == 9) {
            chrom = fields.takeFirst();
            source = fields.takeFirst();
            feature = fields.takeFirst();
            start = fields.takeFirst().toUInt();
            end = fields.takeFirst().toUInt();
            QString tempscore = fields.takeFirst();
            if (tempscore == ".") { score = 0; }
            else { score = tempscore.toFloat(); }
            strand = fields.takeFirst();
            phase = fields.takeFirst();
            QString attributes = fields.takeFirst();
            if (getGtfAttributes(attributes)) { rval = true; }
        } else {
            if (line.length() == 0) {
                std::cerr << "skipping empty line" << std::endl << std::flush;
                rval = this->readGtfLine();
            }
            else { if (line.at(0) == '#') {
                    std::cerr << "skipping comment line:" << std::endl << line.toStdString() << std::endl << std::flush;
                    rval = this->readGtfLine(); }
            }
        }
    }
    return(rval);
}

//! for GFF3 reading
inline bool annotationReader::getGffAttributes(QString& str) {
    bool rval = false;
    Attr.clear(); //very important
    QStringList pairs = str.split(';', QString::SkipEmptyParts);
    foreach (QString pair, pairs) {
        QStringList keyval = pair.split("=", QString::SkipEmptyParts);
        Attr.insert(keyval.at(0), keyval.at(1));
    }
    rval = (pairs.length() > 0);
    if (rval) {
        bool has_ID = Attr.contains("ID");
        bool has_Name = Attr.contains("Name");
        bool has_Parent = Attr.contains("Parent");
        if (has_ID && has_Name) {
            name = Attr["ID"];
            if (Attr["ID"] == Attr["Name"]) {
                if (has_Parent) { parent = Attr["Parent"]; }
                else { parent = "none"; }
            } else {
                parent = Attr["Name"];
            }
        } else {
            name = "none";
            if (has_Parent) {
                int pos = Attr["Parent"].indexOf(',');
                if (pos != -1) {
                    parent = Attr["Parent"].split(',').at(1);
                } else {
                    parent = Attr["Parent"];
                }
            } else {
                parent = "none";
            }
        }
    }
    return(rval);
}

inline bool annotationReader::readGffLine() {
    bool rval = false;
    if(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split('\t');
        if (fields.length() == 9) {
            chrom = fields.takeFirst();
            source = fields.takeFirst();
            feature = fields.takeFirst();
            start = fields.takeFirst().toUInt();
            end = fields.takeFirst().toUInt();
            QString tempscore = fields.takeFirst();
            if (tempscore == ".") { score = 0; }
            else { score = tempscore.toFloat(); }
            strand = fields.takeFirst();
            phase = fields.takeFirst();
            QString attributes = fields.takeFirst();
            if (getGffAttributes(attributes)) { rval = true; }
        } else {
            if (line.length() == 0) {
                std::cerr << "skipping empty line" << std::endl << std::flush;
                rval = this->readGffLine();
            }
            else { if (line.at(0) == '#') {
                    std::cerr << "skipping comment line:" << std::endl << line.toStdString() << std::endl << std::flush;
                    rval = this->readGffLine(); }
            }
        }
    }
    return(rval);
}

