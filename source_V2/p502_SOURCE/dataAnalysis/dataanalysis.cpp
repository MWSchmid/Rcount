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
#include <QtXml>
#include <iostream>

#include "../dataStructure/databaseitem.h"
#include "../dataStructure/database.h"
#include "../bamHandler/bamhandler.h"
#include "../bamHandler/bamhelpers.h"
#include "readmapper.h"
#include "readcounter.h"

#include "dataanalysis.h"

//! make this as a general data analysis handler where one can store settings and so on

dataAnalysis::dataAnalysis()
{
    //! runner info
    this->_allFine = false;
    this->_errorMessage = "not started";
    this->_progress = 0;
    this->_numLines = 0;
    this->_projectName = "newProject";
    this->_writeCounts = false;
    this->_readerStatsString = "";
    this->_writerStatsString = "";

    //! input and output files
    // database XML
    this->_dataBaseInfile = "";
    this->_dataBaseOutfile = "";
    // alignments BAM
    this->_readsInfile = "";
    this->_readsOutfile = "";
    // file with the results
    this->_countTableFile = "";

    //! parameters for mapping and counting
    this->_useMulti = true;
    this->_stranded = false;
    this->_antisense = false;
    this->_minReads = 5;
    this->_maxDist = 250;
    this->_minBelowMaxDist = 1; // this is equal to disabled...
    this->_useRegion = false;
    this->_regionStartName = "";// some nonsense region for the bamreader
    this->_regionStart = 0;// some nonsense region for the bamreader
    this->_regionEndName = "";// some nonsense region for the bamreader
    this->_regionEnd = 0;// some nonsense region for the bamreader

    //! database connection and settings
    //this->_dataBase
    this->_headers.clear();
    this->_headers << "Sname" << "Schrom" << "Sstrand" << "Ustart" << "Uend" << "Sfeature" << "SassembledFeature" << "Upriority" << "Fsumunamb" << "Fsumamb" << "Fsumallo" << "Udistunamb" << "Udistamb" << "Udistallo" << "Ftothits" << "Uvalid";
    this->_indexStepSize = 10000;

    //! buffer and coordination related things
    // variables related to the buffer with the alignments that are unmapped
    this->_bufferSizeBAM = 200000;
    this->_bufferBAM.clear();
    this->_usedSpaceBAM = 0;
    // variables related to the buffer with the alignments that are mapped
    this->_bufferSizeMAP = 200000;
    this->_bufferMAP.clear();
    this->_usedSpaceMAP = 0;
    // variables related to the buffer with the alignments that shall be written down
    this->_bufferSizeOUT = 200000;
    this->_bufferOUT.clear();
    this->_usedSpaceOUT = 0;
    //
    this->_dataBasePresent = false;

    //! thread related variables - leave them as they are, but start this thread
    this->start();
}


dataAnalysis::~dataAnalysis()
{
    // stop the other threads
    emit this->stopThreads();
    // empty the queue of projects and add STOPTHREAD - this tells the tread to exit the forever loop
    {
        QMutexLocker locker(&this->_mutex);
        while (!this->_projects.isEmpty()) {
            this->_projects.dequeue();
        }
        this->_projects.enqueue("STOPTHREAD");
        this->_projectAdded.wakeOne();
    }
    // wait before calling the base class destructor
    this->wait();
}

void dataAnalysis::addProject(QString fileName)
{
    QMutexLocker locker(&this->_mutex);
    this->_projects.enqueue(fileName);
    this->_projectAdded.wakeOne();
}

// the implementation of run (just named like this at the moment) - starts all threads necessary
void dataAnalysis::run()
{
    QString fileName = "";

    forever {
        // check if there is something to process
        {
            QMutexLocker locker(&this->_mutex);
            if (this->_projects.isEmpty()) {
                this->_projectAdded.wait(&this->_mutex);
            }
            // take a project from the queue
            fileName = this->_projects.dequeue();
            // here is a keyword that cancels the run and stops everything
            if (fileName == "STOPTHREAD") {
                break;
            }
        }
        // read the projectFile to get the name and notify
        this->readProject(fileName);
        emit this->analysisStatus(QObject::tr("%1: started").arg(this->_projectName));
        emit this->analysisProgress(this->_progress);
        this->_allFine = this->doAnalysis();
        if (!this->_allFine) {
            this->_errorMessage = "somewhere an error";
            emit this->errorMessage(this->_errorMessage);
        }
        // check if all are processed
        {
            QMutexLocker locker(&this->_mutex);
            if (this->_projects.isEmpty()) {
                emit this->everythingProcessed();
            }
        }
    }
    // we reach here when the destructor is called - we should delete the database
    this->deleteDataBase();
}



void dataAnalysis::readElement(QXmlStreamReader* reader)
{
    // the current value
    // read all the elements
    reader->readNext();
    while (!reader->atEnd()) {
        if (reader->isStartElement()) {
            if      (reader->name() == "name")              { this->_projectName =  reader->attributes().value("entry").toString();}
            else if (reader->name() == "dataBaseInfile")    { this->_dataBaseInfile = reader->attributes().value("entry").toString();}
            else if (reader->name() == "dataBaseOutfile")   { this->_dataBaseOutfile = reader->attributes().value("entry").toString();}
            else if (reader->name() == "readsInfile")       { this->_readsInfile = reader->attributes().value("entry").toString();}
            else if (reader->name() == "readsOutfile")      { this->_readsOutfile = reader->attributes().value("entry").toString();}
            else if (reader->name() == "countTableFile")    { this->_countTableFile = reader->attributes().value("entry").toString();}
            else if (reader->name() == "multi")             { this->_useMulti = (reader->attributes().value("entry").toString() == "true");}
            else if (reader->name() == "stranded")          {
                this->_stranded = (reader->attributes().value("entry").toString() != "false");
                this->_antisense = (reader->attributes().value("entry").toString() == "antisense");
            }
            else if (reader->name() == "minReads")          { this->_minReads = reader->attributes().value("entry").toString().toInt();}
            else if (reader->name() == "maxDist")           { this->_maxDist = reader->attributes().value("entry").toString().toInt();}
            else if (reader->name() == "minBelowMaxDist")   { this->_minBelowMaxDist = reader->attributes().value("entry").toString().toInt();}
            else if (reader->name() == "useRegion")         { this->_useRegion = (reader->attributes().value("entry").toString() == "true");}
            else if (reader->name() == "regionStartName")   { this->_regionStartName = reader->attributes().value("entry").toString();}
            else if (reader->name() == "regionStart")       { this->_regionStart = reader->attributes().value("entry").toString().toInt();}
            else if (reader->name() == "regionEndName")     { this->_regionEndName = reader->attributes().value("entry").toString();}
            else if (reader->name() == "regionEnd")         { this->_regionEnd = reader->attributes().value("entry").toString().toInt();}
            else if (reader->name() == "indexStepSize")     { this->_indexStepSize = reader->attributes().value("entry").toString().toInt();}
            else if (reader->name() == "bufferSizeBAM")     { this->_bufferSizeBAM = reader->attributes().value("entry").toString().toInt();}
            else if (reader->name() == "bufferSizeMAP")     { this->_bufferSizeMAP = reader->attributes().value("entry").toString().toInt();}
            else if (reader->name() == "bufferSizeOUT")     { this->_bufferSizeOUT = reader->attributes().value("entry").toString().toInt();}
        }
        reader->readNext();
    }
}

// in future - read/write project file from xml
bool dataAnalysis::readProject(const QString &fileName)
{
    bool rval = false;

    // an XML stream reader
    QXmlStreamReader reader;

    // try to open the file and check for errors
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        rval = false;
    }
    else {
        // set the device
        reader.setDevice(&file);

        // read the first element
        reader.readNext();

        // go through the file
        while (!reader.atEnd()) {
            if (reader.isStartElement()) {
                if ((reader.name() == "p502project") && (reader.attributes().value("version").toString() == "1.0")) {
                    this->readElement(&reader);
                }
                else { reader.raiseError(QObject::tr("Not a p502project file with version 1.0 - instead: %1").arg(reader.attributes().value("version").toString())); }
            }
            else { reader.readNext(); }
        }

        // close the file and check for errors
        file.close();
        if (reader.hasError()) {
            std::cerr << "Error: Failed to parse file "
                      << qPrintable(fileName) << ": "
                      << reader.error() << " - "
                      << qPrintable(reader.errorString()) << std::endl;
            rval = false;
        }
        else if (file.error() != QFile::NoError) {
            std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                      << ": " << qPrintable(file.errorString())
                      << std::endl;
            rval = false;
        }
        else { rval = true; }
    }

    return(rval);
}

bool dataAnalysis::writeProject(const QString &fileName)
{
    bool rval = false;

    // an XML stream writer
    QXmlStreamWriter writer;

    // try to open the file and check for errors
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        rval = false;
    }
    else {
        writer.setDevice(&file);
        writer.setAutoFormatting(true);


        // write header
        writer.writeStartDocument();
        writer.writeStartElement("p502project");
        writer.writeAttribute("version", "1.0");

        // write elements
        writer.writeStartElement("name");
        writer.writeAttribute("entry", this->_projectName);
        writer.writeStartElement("files");
        writer.writeAttribute("entry", "");
        {
            writer.writeStartElement("dataBaseInfile");
            writer.writeAttribute("entry", this->_dataBaseInfile);
            writer.writeEndElement();
            writer.writeStartElement("dataBaseOutfile");
            writer.writeAttribute("entry", this->_dataBaseOutfile);
            writer.writeEndElement();
            writer.writeStartElement("readsInfile");
            writer.writeAttribute("entry", this->_readsInfile);
            writer.writeEndElement();
            writer.writeStartElement("readsOutfile");
            writer.writeAttribute("entry", this->_readsOutfile);
            writer.writeEndElement();
            writer.writeStartElement("countTableFile");
            writer.writeAttribute("entry", this->_countTableFile);
            writer.writeEndElement();
        }
        writer.writeEndElement();
        writer.writeStartElement("parameters");
        writer.writeAttribute("entry", "");
        {
            writer.writeStartElement("multi");
            if (this->_useMulti) { writer.writeAttribute("entry", QString("true")); }
            else { writer.writeAttribute("entry", QString("false")); }
            writer.writeEndElement();
            writer.writeStartElement("stranded");
            if (this->_stranded) {
                if (this->_antisense) { writer.writeAttribute("entry", QString("antisense")); }
                else { writer.writeAttribute("entry", QString("sense")); }
            }
            else { writer.writeAttribute("entry", QString("false")); }
            writer.writeEndElement();
            writer.writeStartElement("minReads");
            writer.writeAttribute("entry", QString::number(this->_minReads));
            writer.writeEndElement();
            writer.writeStartElement("maxDist");
            writer.writeAttribute("entry", QString::number(this->_maxDist));
            writer.writeEndElement();
            writer.writeStartElement("minBelowMaxDist");
            writer.writeAttribute("entry", QString::number(this->_minBelowMaxDist));
            writer.writeEndElement();
        }
        writer.writeEndElement();
        writer.writeStartElement("region");
        writer.writeAttribute("entry", "");
        {
            writer.writeStartElement("useRegion");
            if (this->_useRegion) { writer.writeAttribute("entry", QString("true")); }
            else { writer.writeAttribute("entry", QString("false")); }
            writer.writeEndElement();
            writer.writeStartElement("regionStartName");
            writer.writeAttribute("entry", this->_regionStartName);
            writer.writeEndElement();
            writer.writeStartElement("regionStart");
            writer.writeAttribute("entry", QString::number(this->_regionStart));
            writer.writeEndElement();
            writer.writeStartElement("regionEndName");
            writer.writeAttribute("entry", this->_regionEndName);
            writer.writeEndElement();
            writer.writeStartElement("regionEnd");
            writer.writeAttribute("entry", QString::number(this->_regionEnd));
            writer.writeEndElement();
        }
        writer.writeEndElement();
        writer.writeStartElement("settings");
        writer.writeAttribute("entry", "");
        {
            writer.writeStartElement("indexStepSize");
            writer.writeAttribute("entry", QString::number(this->_indexStepSize));
            writer.writeEndElement();
            writer.writeStartElement("bufferSizeBAM");
            writer.writeAttribute("entry", QString::number(this->_bufferSizeBAM));
            writer.writeEndElement();
            writer.writeStartElement("bufferSizeMAP");
            writer.writeAttribute("entry", QString::number(this->_bufferSizeMAP));
            writer.writeEndElement();
            writer.writeStartElement("bufferSizeOUT");
            writer.writeAttribute("entry", QString::number(this->_bufferSizeOUT));
            writer.writeEndElement();
        }
        writer.writeEndElement();
        writer.writeEndElement();

        // end the document
        writer.writeEndElement();
        writer.writeEndDocument();

        // close the file and check for errors
        file.close();
        if (file.error() != QFile::NoError) {
            std::cerr << "Error: Cannot write file " << qPrintable(fileName)
                      << ": " << qPrintable(file.errorString())
                      << std::endl;
            rval = false;
        }
        else { rval = true; }
    }
    return(rval);
}

bool dataAnalysis::doAnalysis()
{
    bool rval = false;

    this->deleteDataBase();

    //! load the database
    this->_writeCounts = true;
    this->_progress = 0;
    emit this->analysisStatus(QObject::tr("%1: reading database").arg(this->_projectName));
    emit this->analysisProgress(this->_progress);
    this->_dataBase = new database(this->_headers, this->_indexStepSize);
    this->_dataBase->readData(this->_dataBaseInfile);
    this->_dataBasePresent = true;
    this->_progress += 1;

    //! initialize everything
    emit this->analysisStatus(QObject::tr("%1: starting threads").arg(this->_projectName));
    emit this->analysisProgress(this->_progress);
    // open a bamhandler as reader
    bamHandler producer(this->_readsInfile,
                        this->_readsOutfile,
                        QString("r"),
                        &this->_bufferSizeBAM,
                        &this->_bufferBAM,
                        &this->_bufferIsNotFullBAM,
                        &this->_bufferIsNotEmptyBAM,
                        &this->_mutexBAM,
                        &this->_usedSpaceBAM,
                        this->_useRegion,
                        this->_regionStartName,
                        this->_regionStart,
                        this->_regionEndName,
                        this->_regionEnd);

    // initialize the readmapper
    readMapper mapper(&this->_bufferSizeBAM,
                      &this->_bufferBAM,
                      &this->_bufferIsNotFullBAM,
                      &this->_bufferIsNotEmptyBAM,
                      &this->_mutexBAM,
                      &this->_usedSpaceBAM,
                      &this->_bufferSizeMAP,
                      &this->_bufferMAP,
                      &this->_bufferIsNotFullMAP,
                      &this->_bufferIsNotEmptyMAP,
                      &this->_mutexMAP,
                      &this->_usedSpaceMAP,
                      this->_dataBase,
                      this->_stranded,
                      this->_antisense,
                      this->_useMulti);

    // initialize the readcounter
    readCounter counter(&this->_bufferSizeMAP,
                        &this->_bufferMAP,
                        &this->_bufferIsNotFullMAP,
                        &this->_bufferIsNotEmptyMAP,
                        &this->_mutexMAP,
                        &this->_usedSpaceMAP,
                        &this->_bufferSizeOUT,
                        &this->_bufferOUT,
                        &this->_bufferIsNotFullOUT,
                        &this->_bufferIsNotEmptyOUT,
                        &this->_mutexOUT,
                        &this->_usedSpaceOUT,
                        this->_dataBase,
                        this->_minReads,
                        this->_maxDist,
                        this->_minBelowMaxDist);

    // open a bamwriter
    bamHandler consumer(this->_readsInfile,
                        this->_readsOutfile,
                        QString("w"),
                        &this->_bufferSizeOUT,
                        &this->_bufferOUT,
                        &this->_bufferIsNotFullOUT,
                        &this->_bufferIsNotEmptyOUT,
                        &this->_mutexOUT,
                        &this->_usedSpaceOUT,
                        this->_useRegion,
                        this->_regionStartName,
                        this->_regionStart,
                        this->_regionEndName,
                        this->_regionEnd);

    //! set the connections from this thread to the others (if run is canceled)
    //! hm - stopping them in parallel does not really work... is not implemented at the moment
    // at the moment all producers simply empty their buffer and exit. This should trigger the consumers to follow
    connect(this, SIGNAL(stopThreads()), &producer, SLOT(runCanceled()), Qt::DirectConnection); //! NOTE THAT THE OTHER ONES WILL KEEP ON GOING UNTIL BUFFERS ARE EMPTY
    //connect(this, SIGNAL(stopThreads()), &mapper, SLOT(runCanceled()), Qt::DirectConnection); // not necessary
    //connect(this, SIGNAL(stopThreads()), &counter, SLOT(runCanceled()), Qt::DirectConnection); // not necessary
    //connect(this, SIGNAL(stopThreads()), &consumer, SLOT(runCanceled()), Qt::DirectConnection); // not necessary

    //! set the connections NOTE: if this is not a QThread, use QObject::connect()
    connect(&producer, SIGNAL(finished()), &mapper, SLOT(producerFinished()), Qt::DirectConnection);
    connect(&mapper, SIGNAL(finished()), &counter, SLOT(producersAreFinished()), Qt::DirectConnection); // direct connection as we have only one mapper
    connect(&counter, SIGNAL(finished()), &consumer, SLOT(producerFinished()), Qt::DirectConnection);

    //! signal
    this->_progress += 1;
    emit this->analysisStatus(QObject::tr("%1: setting up").arg(this->_projectName));
    emit this->analysisProgress(this->_progress);

    //! count the number of lines
    this->_numLines = producer.getNumberLines();

    //! connect the processed lines counters (only the writer)
    //connect(this->_producer, SIGNAL(xLinesProcessed(int)), this, SLOT(readerLines(int)), Qt::DirectConnection);
    connect(&consumer, SIGNAL(xLinesProcessed(int)), this, SLOT(writerLines(int)), Qt::DirectConnection);

    //! connect the stats (only the writer)
    //connect(&producer, SIGNAL(statsObtained(QString)), this, SLOT(readerStats(QString)), Qt::DirectConnection);
    connect(&consumer, SIGNAL(statsObtained(QString)), this, SLOT(writerStats(QString)), Qt::DirectConnection);

    //! signal
    this->_progress += 1;
    emit this->analysisStatus(QObject::tr("%1: processing").arg(this->_projectName));
    emit this->analysisProgress(this->_progress);

    //! start the threads
    producer.start();
    mapper.start();
    counter.start();
    consumer.start();

    //! wait for them
    producer.wait();
    mapper.wait();
    counter.wait();
    consumer.wait();

    //! write the table with the counts
    if (this->_writeCounts) {
        rval = this->_dataBase->writeCountTable(this->_countTableFile);
        emit this->analysisCountsWritten(this->_countTableFile);
        this->_progress = 100;
        emit this->analysisStatus(QObject::tr("%1: finished").arg(this->_projectName));
        emit this->analysisProgress(this->_progress);
    }

    this->deleteDataBase();

    emit this->idleAgain();

    //! return
    return(rval);
}

void dataAnalysis::deleteDataBase()
{
    if (this->_dataBasePresent) {
        std::cerr << "try to clear dataBase" << std::endl << std::flush;
        delete this->_dataBase;
        this->_dataBasePresent = false;
    }
}
//! it would maybe work to do all the buffers and mutexes locally and use terminate to kill them (with non-local variables it would not work to run it again)
void dataAnalysis::cancelDataAnalysis()
{
    //emit this->analysisStatus(QObject::tr("not implemented"));
    emit this->analysisStatus(QObject::tr("cancelling"));
    emit this->stopThreads();
    this->_writeCounts = false;
}

void dataAnalysis::readerStats(QString statsString)
{
    this->_readerStatsString = this->_projectName;
    this->_readerStatsString += "|";
    this->_readerStatsString += statsString;
    emit this->readerStatsObtained(this->_readerStatsString);
}

void dataAnalysis::writerStats(QString statsString)
{
    this->_writerStatsString = this->_projectName;
    this->_writerStatsString += "|";
    this->_writerStatsString += statsString;
    emit this->writerStatsObtained(this->_writerStatsString);
}

//! THIS MAY NEED A MUTEX
// note that this slot is not used at the moment
void dataAnalysis::readerLines(int numLines) {
    int curPro = ((numLines/100000)*100) / (this->_numLines/100000); // the /100000 is necessary for large files (goes otherwise over the boundaries of a 32 bit int)
    if (curPro > 98) { this->_progress = 98; }
    else { this->_progress = curPro; }
    emit this->analysisProgress(this->_progress);
}

//! THIS MAY NEED A MUTEX
void dataAnalysis::writerLines(int numLines) {
    int curPro = ((numLines/100000)*100) / (this->_numLines/100000);// the /100000 is necessary for large files (goes otherwise over the boundaries of a 32 bit int)
    if (curPro > 98) { this->_progress = 98; }
    else { this->_progress = curPro; }
    emit this->analysisProgress(this->_progress);
}
