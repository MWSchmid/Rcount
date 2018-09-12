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

#include "bamhandler.h"
#include "bamhelpers.h"

#include "../../seqan/bam_io.h"

//! the function to set everything up
bamHandler::bamHandler(QString fileNameRead,
                       QString fileNameWrite,
                       QString mode,
                       const int *bufferSize,
                       QList<QtBamAlignment> *buffer,
                       QWaitCondition *bufferIsNotFull,
                       QWaitCondition *bufferIsNotEmpty,
                       QMutex *mutex,
                       int *usedSpace,
                       bool useRegion,
                       QString &regionChromStart,
                       int &regionStart,
                       QString &regionChromEnd,
                       int &regionEnd
                       )
{
    this->_fileNameRead = fileNameRead;
    this->_fileNameWrite = fileNameWrite;
    this->_mode = mode;
    this->_bufferSize = bufferSize;
    this->_buffer = buffer;
    this->_bufferIsNotFull = bufferIsNotFull;
    this->_bufferIsNotEmpty = bufferIsNotEmpty;
    this->_mutex = mutex;
    this->_usedSpace = usedSpace;
    this->_useRegion = useRegion;
    this->_regionChromStart = regionChromStart;
    this->_regionStart = regionStart;
    this->_regionChromEnd = regionChromEnd;
    this->_regionEnd = regionEnd;
    this->_producerIsFinished = false;
    this->_processedLines = 0;
    this->_readerStatsString = "";
    this->_writerStatsString = "";
    //! in case one interrupts the analysis
    this->_isCanceled = false;
}

int bamHandler::getNumberLines()
{
    seqan::BamStream reader(this->_fileNameRead.toStdString().c_str());
    seqan::BamAlignmentRecord record;
    int numLines = 0;
    QMap<QString,int> NAMEtoID;
    this->getNAMEtoID(reader, NAMEtoID);

    if (this->_useRegion) {
        std::string baiFile = this->_fileNameRead.toStdString() + ".bai";
        seqan::BamIndex<seqan::Bai> baiIndex;
        seqan::read(baiIndex, baiFile.c_str());
        bool region_has_alignments = false;
        if (!seqan::jumpToRegion(reader, region_has_alignments, NAMEtoID.value(this->_regionChromStart), this->_regionStart, this->_regionEnd, baiIndex)) {
            std::cerr << "could not jump to a certain region" << std::endl << std::flush;
        } else {
            if (region_has_alignments) {
                while (!atEnd(reader)) {
                    seqan::readRecord(record, reader);
                    if (record.rID == -1 || record.rID > NAMEtoID[this->_regionChromStart] || record.beginPos > this->_regionEnd) { break; }
                    if (record.beginPos < this->_regionStart) { continue; }
                    // extract the weight and the number of alignments
                    if (seqan::hasFlagUnmapped(record)) { continue; }
                    ++numLines;
                }
            }
        }
    } else {
        while (!atEnd(reader)) {
            seqan::readRecord(record, reader);
            if (seqan::hasFlagUnmapped(record)) { continue; }
            ++numLines;
        }
    }
    seqan::close(reader);

    return(numLines);
}

//! the signal that needs to be connected to the finished producer
void bamHandler::producerFinished() {
    this->_producerIsFinished = true;
    this->_mutex->lock();
    this->_bufferIsNotEmpty->wakeAll();
    this->_mutex->unlock();
}

//! two functions for chromName and chromID translation
void bamHandler::getIDtoNAME(seqan::BamStream &reader, QMap<int,QString>& IDtoNAME)
{
    std::string chromName = "*";
    IDtoNAME[-1] = QString::fromStdString(chromName);
    for (uint chromID = 0; chromID < length(reader.header.sequenceInfos); ++chromID) {
        chromName = seqan::toCString(reader.header.sequenceInfos[chromID].i1);
        IDtoNAME[chromID] = QString::fromStdString(chromName);
    }
}

void bamHandler::getNAMEtoID(seqan::BamStream &reader, QMap<QString,int>& NAMEtoID)
{
    std::string chromName = "*";
    NAMEtoID[QString::fromStdString(chromName)] = -1;
    for (uint chromID = 0; chromID < length(reader.header.sequenceInfos); ++chromID) {
        chromName = seqan::toCString(reader.header.sequenceInfos[chromID].i1);
        NAMEtoID[QString::fromStdString(chromName)] = chromID;
    }
}

//! functions to convert cigar data
QVector<QtCigarOp> bamHandler::getQtCigar(seqan::String<seqan::CigarElement<> >& seqanCigar)
{
    QtCigarOp curCigar;
    QVector<QtCigarOp> out;
    out.reserve(5);

    for (int i = 0; i < seqan::length(seqanCigar); ++i) {
        curCigar._type = QChar::fromLatin1(seqanCigar[i].operation);
        curCigar._length = seqanCigar[i].count;
        out.push_back(curCigar);
    }

    return(out);
}

seqan::String<seqan::CigarElement<> > bamHandler::getSeqanCigar(QVector<QtCigarOp>& QtCigar)
{
    char curOp;
    uint curCount;
    seqan::String<seqan::CigarElement<> > out;
    seqan::reserve(out, 5);

    QVector<QtCigarOp>::iterator cigarIter;
    for (cigarIter = QtCigar.begin(); cigarIter != QtCigar.end(); ++cigarIter) {
        curOp = cigarIter->_type.toLatin1();
        curCount = cigarIter->_length;
        seqan::appendValue(out, seqan::CigarElement<>(curOp, curCount));
    }

    return(out);
}

void bamHandler::runCanceled()
{
    this->_isCanceled = true;
}

//! the run function
void bamHandler::run()
{
    if ( this->_mode == "r" ) {
        this->readAlignments();
    } else if ( this->_mode == "w" ) {
        this->writeAlignments();
    } else {
        emit(this->detectedError("the mode is not valid"));
    }
}

//! the function that reads alignments
void bamHandler::readAlignments()
{
    seqan::BamStream reader(this->_fileNameRead.toStdString().c_str());
    seqan::BamAlignmentRecord record;
    QtBamAlignment QtAl;
    QMap<int,QString> IDtoNAME;
    QMap<QString,int> NAMEtoID;
    uint tagID;

    // retrieve the reference IDtoNAME and NAMEtoID translators
    this->getIDtoNAME(reader, IDtoNAME);
    this->getNAMEtoID(reader, NAMEtoID);

    // set a region if requested
    if (this->_useRegion) {
        std::string baiFile = this->_fileNameRead.toStdString() + ".bai";
        seqan::BamIndex<seqan::Bai> baiIndex;
        seqan::read(baiIndex, baiFile.c_str());
        bool region_has_alignments = false;
        if (!seqan::jumpToRegion(reader, region_has_alignments, NAMEtoID[this->_regionChromStart], this->_regionStart, this->_regionEnd, baiIndex)) {
            std::cerr << "could not jump to a certain region" << std::endl << std::flush;
        } else {
            if (region_has_alignments) {
                while (!atEnd(reader)) {
                    seqan::readRecord(record, reader);
                    if (record.rID == -1 || record.rID > NAMEtoID[this->_regionChromStart] || record.beginPos > this->_regionEnd) { break; }
                    if (record.beginPos < this->_regionStart) { continue; }
                    if (seqan::hasFlagUnmapped(record)) { continue; }
                    // extract the weight and the number of alignments
                    seqan::BamTagsDict tags(record.tags);
                    if (seqan::findTagKey(tagID, tags, "XW")) {seqan::extractTagValue(QtAl._weight, tags, tagID);}
                    else { QtAl._weight = 1; }
                    if (seqan::findTagKey(tagID, tags, "XM")) {seqan::extractTagValue(QtAl._mapFlag, tags, tagID);}
                    else { QtAl._mapFlag = 0; }
                    if (seqan::findTagKey(tagID, tags, "NH")) {seqan::extractTagValue(QtAl._numal, tags, tagID);}
                    else { QtAl._numal = 1; } //! ATTENTION - IT ASSUMES UNIQUE ALIGNMENT IF THE FLAG IS MISSING
                    QtAl._name = QString(seqan::toCString(record.qName));
                    QtAl._length = static_cast<int>(seqan::getAlignmentLengthInRef(record));
                    QtAl._queryBases = QString(seqan::toCString(record.seq));
                    QtAl._qualities = QString(seqan::toCString(record.qual));
                    QtAl._refName = IDtoNAME.value(static_cast<int>(record.rID));
                    QtAl._position = static_cast<int>(record.beginPos);
                    QtAl._mapQuality = static_cast<uint>(record.mapQ);
                    QtAl._alignmentFlag = static_cast<uint>(record.flag);
                    QtAl._cigarData = this->getQtCigar(record.cigar);
                    QtAl._bin = record.bin;
                    // set all the mapFlag values possible
                    if (QtAl._numal > 1) { QtAl.setIsMulti(true); }
                    if (QtAl._cigarData.count() > 1) {
                        for (QVector<QtCigarOp>::iterator cigarIter = QtAl._cigarData.begin(); cigarIter != QtAl._cigarData.end(); ++cigarIter) {
                            if ((*cigarIter)._type == 'N') {
                                QtAl.setIsGapped(true);
                                if (seqan::findTagKey(tagID, tags, "XS")) {seqan::extractTagValue(QtAl._spliceFlag, tags, tagID);} //! added XS tag
                                else { QtAl._spliceFlag = '.'; }// 46 is ASCII dot
                                break;
                            }
                        }
                    }
                    if (QtAl._weight == 0) { QtAl.setIsZeroWeight(true); }
                    // add to the reader stats
                    this->_readerStats.addAlignment(QtAl);
                    // push to the buffer
                    this->_mutex->lock();
                    while (*(this->_usedSpace) == *(this->_bufferSize)) {
                        this->_bufferIsNotFull->wait(this->_mutex);
                    }
                    this->_buffer->append(QtAl);
                    ++*(this->_usedSpace);
                    this->_bufferIsNotEmpty->wakeAll();
                    this->_mutex->unlock();
                    // count
                    ++this->_processedLines;
                    if ((this->_processedLines % 1000000) == 0) {
                        emit this->xLinesProcessed(this->_processedLines);
                    }
                    if (this->_isCanceled) { break; }
                }
            }
        }
    } else {
        // read alignments, convert, and append
        while (!atEnd(reader)) {
            /*if (this->_isCanceled) {
                std::cerr << "bamHandler::readAlignments: try to clear buffer" << std::endl << std::flush;
                this->_mutex->lock();
                this->_buffer->clear();
                this->_usedSpace = 0;
                this->_mutex->unlock();
                break;
            }*/
            seqan::readRecord(record, reader);
            if (seqan::hasFlagUnmapped(record)) { continue; }
            // extract the weight and the number of alignments
            seqan::BamTagsDict tags(record.tags);
            if (seqan::findTagKey(tagID, tags, "XW")) {seqan::extractTagValue(QtAl._weight, tags, tagID);}
            else { QtAl._weight = 1; }
            if (seqan::findTagKey(tagID, tags, "XM")) {seqan::extractTagValue(QtAl._mapFlag, tags, tagID);}
            else { QtAl._mapFlag = 0; }
            if (seqan::findTagKey(tagID, tags, "NH")) {seqan::extractTagValue(QtAl._numal, tags, tagID);}
            else { QtAl._numal = 1; } //! ATTENTION - IT ASSUMES UNIQUE ALIGNMENT IF THE FLAG IS MISSING
            // assign the rest
            QtAl._name = QString(seqan::toCString(record.qName));
            QtAl._length = static_cast<int>(seqan::getAlignmentLengthInRef(record));
            QtAl._queryBases = QString(seqan::toCString(record.seq));
            QtAl._qualities = QString(seqan::toCString(record.qual));
            QtAl._refName = IDtoNAME.value(static_cast<int>(record.rID));
            QtAl._position = static_cast<int>(record.beginPos);
            QtAl._mapQuality = static_cast<uint>(record.mapQ);
            QtAl._alignmentFlag = static_cast<uint>(record.flag);
            QtAl._cigarData = this->getQtCigar(record.cigar);
            QtAl._bin = record.bin;
            // set all the mapFlag values possible
            if (QtAl._numal > 1) { QtAl.setIsMulti(true); }
            if (QtAl._cigarData.count() > 1) { // TAKE ONLY THE ONES WITH A N AS GAPPED READS
                for (QVector<QtCigarOp>::iterator cigarIter = QtAl._cigarData.begin(); cigarIter != QtAl._cigarData.end(); ++cigarIter) {
                    if ((*cigarIter)._type == 'N') {
                        QtAl.setIsGapped(true);
                        if (seqan::findTagKey(tagID, tags, "XS")) {seqan::extractTagValue(QtAl._spliceFlag, tags, tagID);} //! added XS tag
                        else { QtAl._spliceFlag = '.'; }// 46 is ASCII dot
                        break;
                    }
                }
            }
            if (QtAl._weight == 0) { QtAl.setIsZeroWeight(true); }
            // add to the reader stats
            this->_readerStats.addAlignment(QtAl);
            // push to the buffer
            this->_mutex->lock();
            while (*(this->_usedSpace) == *(this->_bufferSize)) {
                this->_bufferIsNotFull->wait(this->_mutex);
            }
            this->_buffer->append(QtAl);
            ++*(this->_usedSpace);
            this->_bufferIsNotEmpty->wakeAll();
            this->_mutex->unlock();
            // count
            ++this->_processedLines;
            if ((this->_processedLines % 1000000) == 0) {
                emit this->xLinesProcessed(this->_processedLines);
            }
            if (this->_isCanceled) { break; }
        }
    }
    seqan::close(reader);
    this->_readerStatsString = this->_readerStats.getTreeViewString();
    emit this->statsObtained(this->_readerStatsString);
    std::cerr << "reader finished" << std::endl << std::flush;
}

//! the function that writes alignments
void bamHandler::writeAlignments()
{
    seqan::BamStream reader(this->_fileNameRead.toStdString().c_str());
    seqan::BamStream writer(this->_fileNameWrite.toStdString().c_str(), seqan::BamStream::WRITE);
    // Copy header.  The header is automatically written out before the first record.
    writer.header = reader.header;

    seqan::BamAlignmentRecord record;
    QtBamAlignment QtAl;
    QMap<QString,int> NAMEtoID;
    QMap<int,QString> IDtoNAME;
    QString XTtag; //! new in the countsPerTranscript version: report the gene and transcript IDs for each read in the BAM file
    //QByteArray XTtagByteArray; //! new in the countsPerTranscript version: report the gene and transcript IDs for each read in the BAM file
    //const char *XTtagCstring; //! new in the countsPerTranscript version: report the gene and transcript IDs for each read in the BAM file

    // retrieve the reference IDtoNAME and NAMEtoID translators
    this->getIDtoNAME(reader, IDtoNAME);
    this->getNAMEtoID(reader, NAMEtoID);

    //consume from a buffer, convert and write
    while ( true ) {
        this->_mutex->lock();
        // wait if there is more to come
        while ( (*(this->_usedSpace) == 0) && (!this->_producerIsFinished) ) {
            this->_bufferIsNotEmpty->wait(this->_mutex); // this also unlocks the mutex
        }
        // exit the loop if the buffer is empty
        if ((*this->_usedSpace) == 0) {
            this->_mutex->unlock();
            break;
        }
        seqan::clear(record);
        //! hier bin ich
        // otherwise take one alignment from the list
        QtAl = this->_buffer->takeFirst(); //! 8. march 2012 changed to takeFirst
        // make a new seqan::BamAlignmentRecord
        // make a new ::BamAlignment
        record.qName = QtAl._name.toStdString();
        record.flag = QtAl._alignmentFlag;
        record.mapQ = QtAl._mapQuality;
        record.beginPos = QtAl._position;
        record.bin = QtAl._bin;
        record.rID = NAMEtoID.value(QtAl._refName);
        record.cigar = this->getSeqanCigar(QtAl._cigarData);
        record.seq = QtAl._queryBases.toStdString();
        record.qual = QtAl._qualities.toStdString();
        //! extract the gene and transcript names
        XTtag.clear();
        QtAl.getGeneAndTranscriptNames(XTtag);
        //XTtagByteArray = XTtag.toLatin1();
        //XTtagCstring = XTtagByteArray.data();
        // add tags and write the new alignment
        std::cout << QtAl._name.toStdString() << '\t' << XTtag.toStdString() << std::endl << std::flush; //! the Z tag is not implemented -.-
        seqan::BamTagsDict tags(record.tags);
        if (QtAl.IsGapped()) {
            if (seqan::setTagValue(tags, "NH", static_cast<int>(QtAl._numal), 'i') && seqan::setTagValue(tags, "XW", QtAl._weight, 'f') && seqan::setTagValue(tags, "XM", static_cast<int>(QtAl._mapFlag), 'i') && seqan::setTagValue(tags, "XS", QtAl._spliceFlag, 'A')) {
                seqan::writeRecord(writer, record);
            } else { emit this->detectedError("alignment not written"); }
        } else {
            if (seqan::setTagValue(tags, "NH", static_cast<int>(QtAl._numal), 'i') && seqan::setTagValue(tags, "XW", QtAl._weight, 'f') && seqan::setTagValue(tags, "XM", static_cast<int>(QtAl._mapFlag), 'i')) {
                seqan::writeRecord(writer, record);
            } else { emit this->detectedError("alignment not written"); }
        } //! added XS tag
        --*(this->_usedSpace);
        this->_bufferIsNotFull->wakeAll();
        this->_mutex->unlock();
        // ad to writer stats
        this->_writerStats.addAlignment(QtAl);
        // count
        ++this->_processedLines;
        if ((this->_processedLines % 1000000) == 0) {
            emit this->xLinesProcessed(this->_processedLines);
        }
    }
    seqan::close(reader);
    seqan::close(writer);
    this->_writerStatsString = this->_writerStats.getTreeViewString();
    emit this->statsObtained(this->_writerStatsString);
    std::cerr << "writer finished" << std::endl << std::flush;
}





