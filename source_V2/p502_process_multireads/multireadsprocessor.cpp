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

#include "multireadsprocessor.h"
#include "processMultiClasses.h"

multireadsProcessor::multireadsProcessor()
{
    this->_allFine = true;
    this->_isCancelled = false;
    this->_progress = 0;
    this->start();
}

multireadsProcessor::~multireadsProcessor()
{
    // empty the queue of projects and add STOPTHREAD - this tells the tread to exit the forever loop
    {
        QMutexLocker locker(&this->_mutex);
        while (!this->_work.isEmpty()) {
            this->_work.dequeue();
        }
        this->_work.enqueue("STOPTHREAD");
        this->_workAdded.wakeOne();
    }
    // wait before calling the base class destructor
    this->wait();
}

void multireadsProcessor::addWork(QString workString) // will add some work to the queue
{
    QMutexLocker locker(&this->_mutex);
    this->_work.enqueue(workString);
    this->_workAdded.wakeOne();
}

void multireadsProcessor::run()
{
    QString workString;

    forever {
        {// check if there is something to process
            QMutexLocker locker(&this->_mutex);
            if (this->_work.isEmpty()) {
                this->_workAdded.wait(&this->_mutex);
            }
            // take a project from the queue
            workString = this->_work.dequeue();
            // here is a keyword that cancels the run and stops everything
            if (workString == "STOPTHREAD") {
                break;
            }
        }

        emit this->processStatus("starting");
        emit this->processProgress(static_cast<int>(this->_progress));
        this->_allFine = this->doProcess(workString);
        if (!this->_allFine) {
            if (this->_isCancelled) {
                this->_isCancelled = false;
                this->_allFine = true;
            } else {
                this->_errorMessage = "somewhere an error";
                emit this->errorMessage(this->_errorMessage);
            }
        }
        // for the queue display one could send now the workString back
        emit this->workFinished(workString);
        // check if all are processed
        {
            QMutexLocker locker(&this->_mutex);
            if (this->_work.isEmpty()) {
                emit this->idleAgain();
            }
        }
    }
}

quint64 multireadsProcessor::getNumberLines(QString fileName)
{
    seqan::BamStream reader(fileName.toStdString().c_str());
    seqan::BamAlignmentRecord record;
    quint64 numLines = 0;
    while (!atEnd(reader)) {
        seqan::readRecord(record, reader);
        if (seqan::hasFlagUnmapped(record)) { continue; }
        ++numLines;
        if (this->_isCancelled) { break; }
    }
    seqan::close(reader);
    return(numLines);
}

QMap<uint, QString> multireadsProcessor::getIDtoNAME(QString fileName)
{
    QMap<uint, QString> out;
    std::string chromName = "*";
    out[-1] = QString::fromStdString(chromName);
    seqan::BamStream reader(fileName.toStdString().c_str());
    for (uint chromID = 0; chromID < length(reader.header.sequenceInfos); ++chromID) {
        chromName = seqan::toCString(reader.header.sequenceInfos[chromID].i1);
        out[chromID] = QString::fromStdString(chromName);
    }
    seqan::close(reader);
    return(out);
}

bool multireadsProcessor::copy_driver(QString& bamfile, QString& new_bamfile, quint64 &numLines){
    quint64 linesProcessed = 0;
    float weight = 1;
    bool rval = true;
    seqan::BamStream reader(bamfile.toStdString().c_str());
    seqan::BamStream writer(new_bamfile.toStdString().c_str(), seqan::BamStream::WRITE);
    // Copy header.  The header is automatically written out before
    // the first record.
    writer.header = reader.header;

    seqan::BamAlignmentRecord record;
    while (!atEnd(reader)) {
        ++linesProcessed;
        if ((linesProcessed % 1000000) == 0) {
            this->_progress += (100000000.0) / (numLines);
            emit processProgress(static_cast<int>(this->_progress));
            if (this->_isCancelled) { rval = false; break; }
        }
        seqan::readRecord(record, reader);
        seqan::BamTagsDict tags(record.tags);
        if (seqan::setTagValue(tags, "XW", weight, 'f')) { seqan::writeRecord(writer, record);}
        else {std::cerr << "seltsam\n" << std::flush;}
    }

    seqan::close(reader);
    seqan::close(writer);
    return(rval);
}

bool multireadsProcessor::load_driver(QString& bamfile, multireadcollection& multi_reads, quint64 &numLines){
    bool rval = true;
    QString baiFile = bamfile + ".bai";
    QMap<uint, QString> idToName = this->getIDtoNAME(bamfile);
    QString mess;
    quint64 numProcessed = 0;
    seqan::BamStream reader(bamfile.toStdString().c_str());// the filehandler
    seqan::BamIndex<seqan::Bai> baiIndex;
    seqan::read(baiIndex, baiFile.toStdString().c_str());
    for (uint i = 0; i < seqan::length(reader.header.sequenceInfos); ++i) {
        mess = "processing: " + idToName.value(i);
        emit this->processStatus(mess);
        numProcessed = process_one_chromosome(reader, baiIndex, i, multi_reads);
        this->_progress += (numProcessed / static_cast<float>(numLines))*80; //! ONLY UP TO 80 PROGRESS LIKE THIS
        emit processProgress(static_cast<int>(this->_progress));
        if (this->_isCancelled) { rval = false; break; }
    } // process one chrom after the other
    seqan::close(reader);// close the reader

    // calculate weights
    emit this->processStatus("calculating weights");
    multi_reads.calculate_weights();
    if (this->_isCancelled) { rval = false; }
    return(rval);
}

bool multireadsProcessor::write_driver(QString& bamfile, multireadcollection& multi_reads, QString& new_bamfile, quint64 &numLines){
    std::string name; // name of the read
    quint64 numProcessed = 0;
    uint numal; // number of alignments for a given read
    int chromid; // id of the chromosome
    float weight; // the weight that should be inserted
    int alBegin;
    int alEnd;
    unsigned tagID;
    bool rval;

    emit this->processStatus("writing reads");
    seqan::BamStream reader(bamfile.toStdString().c_str());
    seqan::BamStream writer(new_bamfile.toStdString().c_str(), seqan::BamStream::WRITE);
    // Copy header.  The header is automatically written out before the first record.
    writer.header = reader.header;

    seqan::BamAlignmentRecord record;

    while (!atEnd(reader)) {
        seqan::readRecord(record, reader);
        if (seqan::hasFlagUnmapped(record)) { continue; }
        ++numProcessed;
        if ((numProcessed % 1000000) == 0) {
            this->_progress = 80 + (numProcessed / static_cast<float>(numLines)) * 20; //! 80 from before plus 20
            emit processProgress(static_cast<int>(this->_progress));
        }
        if (this->_isCancelled) { rval = false; break; }
        seqan::BamTagsDict tags(record.tags);
        // number of alignments
        if (seqan::findTagKey(tagID, tags, "NH")) {seqan::extractTagValue(numal, tags, tagID);}
        else {numal = 1;}		// write and continue if the read is unique
        if (numal == 1) { weight = 1; }
        else {
            name = seqan::toCString(record.qName);
            chromid = record.rID;
            alBegin = record.beginPos;
            alEnd = alBegin + seqan::getAlignmentLengthInRef(record);
            weight = multi_reads.get_weight(name, chromid, alBegin, alEnd);
        }
        // convert the float to something printable
        if (seqan::setTagValue(tags, "XW", weight, 'f')) { seqan::writeRecord(writer, record);}
        else {std::cerr << "seltsam\n" << std::flush;}
    }
    seqan::close(reader);
    seqan::close(writer);
    return(rval);
}


quint64 multireadsProcessor::process_one_chromosome(seqan::BamStream& reader, seqan::BamIndex<seqan::Bai>& baiIndex, uint& refNum, multireadcollection& multi_reads) {
    quint64 numProcessed = 0;
    // dummies
    int chromid = refNum;
    int chromlen = reader.header.sequenceInfos[refNum].i2;

    // PART1: chrom_coverage
    chrom_coverage chrom_cov(chromid, chromlen);
    numProcessed = chrom_cov.load_cov(reader, baiIndex, true);

    // PART2: load multi reads
    multi_reads.add_from_one_chromosome(reader, baiIndex, chrom_cov);
    return(numProcessed);
}


bool multireadsProcessor::doProcess(QString workString)
{
    QStringList temp = workString.split("|");
    QString readsInfile = temp.at(0);
    QString readsOutfile = temp.at(1);
    QString doWeight = temp.at(2);
    uint allocationDistance = temp.at(3).toUInt();
    quint64 numLines;
    //QMap<uint, QString> idToName;
    this->_progress = 0;
    emit processProgress(static_cast<int>(this->_progress));
    bool rval = false;

    // get number of lines
    emit this->processStatus("reading line numbers");
    numLines = this->getNumberLines(readsInfile);
    if (this->_isCancelled) { return(false); }

    // process or copy-only
    if (doWeight == "n") {
        rval = this->copy_driver(readsInfile, readsOutfile, numLines);
        return(true);
    } else {
        multireadcollection multiReads(allocationDistance);
        this->load_driver(readsInfile, multiReads, numLines);
        this->write_driver(readsInfile, multiReads, readsOutfile, numLines);
    }

    return(true);
}

void multireadsProcessor::cancelProcessing() {
    this->_isCancelled = true;
}
