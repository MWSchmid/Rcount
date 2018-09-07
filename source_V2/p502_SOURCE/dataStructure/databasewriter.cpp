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
#include "databasewriter.h"

databaseWriter::databaseWriter(database *data)
{
    this->data = data;
    this->stringmap.insert(LOCUS, "locus");
    this->stringmap.insert(TRANSCRIPT, "transcript");
    this->stringmap.insert(EXON, "exon");
    this->stringmap.insert(INTRON, "intron");
    this->stringmap.insert(UTR, "utr");
}


bool databaseWriter::writeFile(const QString &fileName)
{
    bool rval = false;

    // try to open the file and check for errors
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        rval = false;
    }
    else {
        this->writer.setDevice(&file);
        this->writer.setAutoFormatting(true);


        // write elements
        this->writer.writeStartDocument();
        this->writer.writeStartElement("p502annotation");
        this->writer.writeAttribute("version", "2.0");
        foreach (databaseItem *element, this->data->rootItem->childItems) {
            this->writeElement(element);
        }
        this->writer.writeEndElement();
        this->writer.writeEndDocument();

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

void databaseWriter::writeElement(databaseItem *element)
{
    //
    this->writer.writeStartElement(this->stringmap.value(element->type));
    for (int i = 0; i < element->itemData.count(); ++i) {
        this->writer.writeAttribute(this->data->rootItem->itemData.at(i).toString(), element->itemData.at(i).toString());
    }
    foreach (databaseItem *child, element->childItems) {
        this->writeElement(child);
    }
    this->writer.writeEndElement();
}


//! OLD VERSION
/*
bool databaseWriter::writeFile(const QString &fileName)
{
    bool rval = false;

    // try to open the file and check for errors
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        rval = false;
    }
    else {
        this->writer.setDevice(&file);
        this->writer.setAutoFormatting(true);


        // write elements
        this->writer.writeStartDocument();
        this->writer.writeStartElement("p502annotation");
        this->writer.writeAttribute("version", "1.0");
        foreach (databaseItem *element, this->data->rootItem->childItems) {
            this->writeElement(element);
        }
        this->writer.writeEndElement();
        this->writer.writeEndDocument();

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

void databaseWriter::writeElement(databaseItem *element)
{
    //
    this->writer.writeStartElement(this->stringmap.value(element->type));
    for (int i = 0; i < element->itemData.count(); ++i) {
        this->writer.writeTextElement(this->data->rootItem->itemData.at(i).toString(), element->itemData.at(i).toString());
    }
    foreach (databaseItem *child, element->childItems) {
        this->writeElement(child);
    }
    this->writer.writeEndElement();
}
*/




