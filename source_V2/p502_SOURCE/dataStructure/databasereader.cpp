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

#include "databaseitem.h"
#include "database.h"
#include "databasereader.h"

databaseReader::databaseReader(database *data)
{
    this->data = data;
    this->typemap.insert("locus", LOCUS);
    this->typemap.insert("transcript", TRANSCRIPT);
    this->typemap.insert("exon", EXON);
    this->typemap.insert("intron", INTRON);
    this->typemap.insert("utr", UTR);
}

bool databaseReader::readFile(const QString &fileName)
{
    bool rval = false;

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
        this->reader.setDevice(&file);

        // read the first element
        this->reader.readNext();

        // go through the file
        while (!this->reader.atEnd()) {
            if (this->reader.isStartElement()) {
                if ((this->reader.name() == "p502annotation") && (this->reader.attributes().value("version").toString() == "2.0")) {
                    this->readAnnotationTree();
                }
                else if ((this->reader.name() == "p502annotation") && (this->reader.attributes().value("version").toString() == "1.0")) {
                    this->readAnnotationTreeV1();
                }
                else { this->reader.raiseError(QObject::tr("Not a Rcount/p502 annotation file with version 2.0 or 1.0 - instead: %1").arg(this->reader.attributes().value("version").toString())); }
            }
            else { this->reader.readNext(); }
        }

        // close the file and check for errors
        file.close();
        if (this->reader.hasError()) {
            std::cerr << "Error: Failed to parse file "
                      << qPrintable(fileName) << ": "
                      << this->reader.error() << " - "
                      << qPrintable(this->reader.errorString()) << std::endl;
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

void databaseReader::readAnnotationTree()
{
    this->reader.readNext();
    while ( !this->reader.atEnd() ) {
        if ( this->reader.isStartElement() ) {
            if (this->reader.name() == "locus") { this->readElement(LOCUS, this->data->rootItem); }
            else { this->reader.raiseError(QObject::tr("Got an unexpected element: %1").arg(this->reader.name().toString())); }
        }
        else {
            this->reader.readNext();
        }
    }
}

//! NOTE: now the headers are taken from the rootItem -> all entries have the same
//! number of columns - this is not necessary (but at the moment easier)
void databaseReader::readElement(Type nodeType,  databaseItem *parent)
{
    // a vector holding the data to set up a new databaseItem
    Type newtype;

    // init a new item
    databaseItem *item = new databaseItem(nodeType, this->data->rootItem->itemData, parent);
    // to make sure that every data column has a default value, go through the headers of the root item
    // this is only important if one gives a header that is not in the annotation file
    for (int i = 0; i < this->data->rootItem->itemData.count(); ++i ) {
        if (this->data->rootItem->itemData.value(i).toString().at(0) == QChar('U')) { item->setData(i, uint(0)); }
        else if (this->data->rootItem->itemData.value(i).toString().at(0) == QChar('I'))  { item->setData(i, int(0)); }
        else if (this->data->rootItem->itemData.value(i).toString().at(0) == QChar('F'))  { item->setData(i, float(0)); }
        else { item->setData(i, QString("")); }
    }
    // link it to the parent
    parent->addChild(item);

    // read the attributes
    foreach (QXmlStreamAttribute att, this->reader.attributes()) {
        int ind = this->data->rootItem->itemData.indexOf(att.name().toString());
        if ( ind >= 0 ) {
            if (att.name().at(0) == QChar('U')) { item->setData(ind, att.value().toString().toUInt()); }
            else if (att.name().at(0) == QChar('I'))  { item->setData(ind, att.value().toString().toInt()); }
            else if (att.name().at(0) == QChar('F'))  { item->setData(ind, att.value().toString().toFloat()); }
            else { item->setData(ind, att.value().toString()); }
        }
        else { continue; }
    }

    // read the next element and continue to loop
    //std::cerr << this->reader.name().toString().toStdString() << std::endl << std::flush;
    this->reader.readNext();

    while (!this->reader.atEnd()) {

        // check if we have the end of an element
        if (this->reader.isEndElement()) {
            this->reader.readNext();
            break;
        }

        // if it is a start element, process it, otherwise just read on
        if (this->reader.isStartElement()) {
            if (this->typemap.contains(this->reader.name().toString())) {
                // get the type of the new feature
                newtype = this->typemap.value(this->reader.name().toString());
                // and read it in
                this->readElement(newtype, item);
            }
        }
        else { this->reader.readNext(); }
    }
    //std::cerr << item->data(0).toString().toStdString() << std::endl << std::flush;
}

//! THE OLDER FILE VERSION

void databaseReader::readAnnotationTreeV1()
{
    this->reader.readNext();
    while ( !this->reader.atEnd() ) {
        if ( this->reader.isStartElement() ) {
            if (this->reader.name() == "locus") { this->readElementV1(LOCUS, this->data->rootItem); }
            else { this->reader.raiseError(QObject::tr("Got an unexpected element: %1").arg(this->reader.name().toString())); }
        }
        else {
            this->reader.readNext();
        }
    }
}

//! NOTE: now the headers are taken from the rootItem -> all entries have the same
//! number of columns - this is not necessary (but at the moment easier)
void databaseReader::readElementV1(Type nodeType,  databaseItem *parent)
{
    // a vector holding the data to set up a new databaseItem
    Type newtype;

    // init a new item
    databaseItem *item = new databaseItem(nodeType, this->data->rootItem->itemData, parent);
    // to make sure that every data column has a default value, go through the headers of the root item
    // this is only important if one gives a header that is not in the annotation file
    for (int i = 0; i < this->data->rootItem->itemData.count(); ++i ) {
        if (this->data->rootItem->itemData.value(i).toString().at(0) == QChar('U')) { item->setData(i, uint(0)); }
        else if (this->data->rootItem->itemData.value(i).toString().at(0) == QChar('I'))  { item->setData(i, int(0)); }
        else if (this->data->rootItem->itemData.value(i).toString().at(0) == QChar('F'))  { item->setData(i, float(0)); }
        else { item->setData(i, QString("")); }
    }
    // link it to the parent
    parent->addChild(item);

    // read the next element and continue to loop
    //std::cerr << this->reader.name().toString().toStdString() << std::endl << std::flush;
    this->reader.readNext();

    while (!this->reader.atEnd()) {

        // check if we have the end of an element
        if (this->reader.isEndElement()) {
            this->reader.readNext();
            break;
        }

        // if it is a start element, process it, otherwise just read on
        if (this->reader.isStartElement()) {

            if (this->typemap.contains(this->reader.name().toString())) {
                // get the type of the new feature
                newtype = this->typemap.value(this->reader.name().toString());
                // and read it in
                this->readElementV1(newtype, item);
            }

            else { //! NOTE THAT AT THE MOMENT IT MUST BE CORRECTLY SORTED!
                int ind = this->data->rootItem->itemData.indexOf(this->reader.name().toString());
                if ( ind >= 0 ) {
                    if (this->reader.name().at(0) == QChar('U')) { item->setData(ind, this->reader.readElementText().toUInt()); }
                    else if (this->reader.name().at(0) == QChar('I'))  { item->setData(ind, this->reader.readElementText().toInt()); }
                    else if (this->reader.name().at(0) == QChar('F'))  { item->setData(ind, this->reader.readElementText().toFloat()); }
                    else { item->setData(ind, this->reader.readElementText()); }
                    if (this->reader.isEndElement()) { this->reader.readNext(); }
                }
                else {
                    this->skipUnknownElementV1();
                }
            }
        }
        else { this->reader.readNext(); }
    }
    //std::cerr << item->data(0).toString().toStdString() << std::endl << std::flush;
}

void databaseReader::skipUnknownElementV1()
{
    this->reader.readNext();
    while (!this->reader.atEnd()) {
        // check if we have the end of an element
        if (this->reader.isEndElement()) {
            this->reader.readNext();
            break;
        }
        // if it is a start element, process it, otherwise just read on
        if (this->reader.isStartElement()) {
            this->skipUnknownElementV1();
        } else {
            this->reader.readNext();
        }
    }
}

