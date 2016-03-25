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

#include "projecttreeviewmodel.h"
#include "projecttreeviewitem.h"

projectTreeViewModel::projectTreeViewModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "project variable" << "value";
    rootItem = new projectTreeViewItem(rootData, 0);
}

projectTreeViewModel::~projectTreeViewModel()
{
    delete rootItem;
}

QVariant projectTreeViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return(QVariant());
    }

    if (role != Qt::DisplayRole) {
        return(QVariant());
    }

    projectTreeViewItem *item = static_cast<projectTreeViewItem*>(index.internalPointer());

    return(item->data(index.column()));
}

Qt::ItemFlags projectTreeViewModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return(0);
    } else {
        return(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }
}

QVariant projectTreeViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return(rootItem->data(section));
    } else {
        return(QVariant());
    }
}

QModelIndex projectTreeViewModel::index(int row, int column, const QModelIndex &parent) const
{
     if (!hasIndex(row, column, parent)) {
         return(QModelIndex());
     }

     projectTreeViewItem *parentItem;
     if (!parent.isValid()) {
         parentItem = rootItem;
     } else {
         parentItem = static_cast<projectTreeViewItem*>(parent.internalPointer());
     }

     projectTreeViewItem *childItem = parentItem->child(row);
     if (childItem) {
         return(createIndex(row, column, childItem));
     } else {
         return(QModelIndex());
     }
}

QModelIndex projectTreeViewModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return(QModelIndex());
    }

    projectTreeViewItem *childItem = static_cast<projectTreeViewItem*>(index.internalPointer());
    projectTreeViewItem *parentItem = childItem->parent();

    if (parentItem == rootItem) {
        return(QModelIndex());
    }

    return(createIndex(parentItem->row(), 0, parentItem));
}

QModelIndex projectTreeViewModel::topParent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return(QModelIndex());
    }

    projectTreeViewItem *childItem = static_cast<projectTreeViewItem*>(index.internalPointer());
    projectTreeViewItem *topParentItem = childItem->topParent(rootItem);

    if (topParentItem == rootItem) {
        return(QModelIndex()); // well, this should not happen
    }

    return(createIndex(topParentItem->row(), 0, topParentItem));
}

int projectTreeViewModel::rowCount(const QModelIndex &parent) const
{
    projectTreeViewItem *parentItem;
    if (parent.column() > 0) {
        return(0);
    }

    if (!parent.isValid()) {
        parentItem = rootItem;
    } else {
        parentItem = static_cast<projectTreeViewItem*>(parent.internalPointer());
    }

    return(parentItem->childCount());
}

int projectTreeViewModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return static_cast<projectTreeViewItem*>(parent.internalPointer())->columnCount();
    } else {
        return rootItem->columnCount();
    }
}

QString projectTreeViewModel::getCountFile(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return(QString());
    }

    projectTreeViewItem *curItem = static_cast<projectTreeViewItem*>(index.internalPointer());
    projectTreeViewItem *topItem = curItem->topParent(rootItem);

    foreach (projectTreeViewItem* part, topItem->childItems) {
        if (part->itemData.at(0) == "files") {
            foreach (projectTreeViewItem* subPart, part->childItems) {
                if (subPart->itemData.at(0) == "countTableFile") {
                    return(subPart->itemData.at(1).toString());
                }
            }
        }
    }

    return(QString());
}

QString projectTreeViewModel::getFileName(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return(QString());
    }

    projectTreeViewItem *curItem = static_cast<projectTreeViewItem*>(index.internalPointer());
    projectTreeViewItem *topItem = curItem->topParent(rootItem);
    return(topItem->data(1).toString());
}

QStringList projectTreeViewModel::getAllFileNames() const
{
    QStringList out;
    foreach (projectTreeViewItem *curItem, rootItem->childItems) {
        out << curItem->data(1).toString();
    }
    return(out);
}

bool projectTreeViewModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    bool success = false;
    projectTreeViewItem *parentItem;
    if (parent.isValid()) {
        parentItem= static_cast<projectTreeViewItem*>(parent.internalPointer());
    } else {
        parentItem = rootItem;
    }
    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();
    return(success);
}

void projectTreeViewModel::appendProject(projectTreeViewItem* projectItem)
{
    QModelIndex parent = QModelIndex();
    int position = rootItem->childCount();
    beginInsertRows(parent, position, position);
    rootItem->appendChild(projectItem);
    endInsertRows();
}

void projectTreeViewModel::removeProject(QModelIndex &index)
{
    QModelIndex topItemIndex = topParent(index);
    removeRow(topItemIndex.row());
}

void projectTreeViewModel::addStats(QString &projectName, QStringList &lines)
{
    //! get the project where the stats belong to
    projectTreeViewItem* project;
    foreach (projectTreeViewItem* projectIter, rootItem->childItems) {
        if (projectName == projectIter->data(0).toString()) {
            project = projectIter;
            break;
        }
    }
    QModelIndex projectIndex = createIndex(project->row(), 0, project);
    //! remove old stats if present
    foreach (projectTreeViewItem* subPart, project->childItems) {
        if ("stats" == subPart->data(0).toString()) {
            removeRow(subPart->row(), projectIndex);
            break;
        }
    }
    //! create a new branch
    QList<QVariant> curData;
    curData << "stats" << "";
    projectTreeViewItem* parentStats = new projectTreeViewItem(curData, project);
    // fill the new branch
    QList<projectTreeViewItem*> parents;
    QList<uint> levels;
    uint curLevel;
    QString curCount;
    QString curName;
    parents << parentStats;
    levels << 0;
    while (!lines.isEmpty()) {
        curData.clear();
        QString lineData = lines.takeFirst();
        QStringList fields = lineData.split("-");
        curCount = fields.takeLast();
        curName = fields.takeLast();
        curLevel = fields.takeLast().toUInt();
        curData << curName << curCount;
        if (curLevel > levels.last()) {
            // The last child of the current parent is now the new parent
            // unless the current parent has no children.
            if (parents.last()->childCount() > 0) {
                parents << parents.last()->child(parents.last()->childCount()-1);
                levels << curLevel;
            }
        } else {
            while (curLevel < levels.last() && parents.count() > 0) {
                parents.pop_back();
                levels.pop_back();
            }
        }
        // Append a new item to the current parent's list of children.
        projectTreeViewItem *parent = parents.last();
        projectTreeViewItem *child = new projectTreeViewItem(curData, parent);
        parent->appendChild(child);
    }
    //! insert the new branch
    int position = project->childCount();
    beginInsertRows(projectIndex, position, position);
    project->appendChild(parentStats);
    endInsertRows();
    //! write the project
    writeProject(project);
}

void projectTreeViewModel::addProject(QString &fileName)
{
    // init the XML reader
    QXmlStreamReader reader;

    // try to open the file and check for errors
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
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
                    readProject(&reader, fileName);
                } else {
                    reader.raiseError(QObject::tr("Not a p502 project file with version 1.0 - instead: %1").arg(reader.attributes().value("version").toString()));
                }
            } else {
                reader.readNext();
            }
        }

        // close the file and check for errors
        file.close();
        if (reader.hasError()) {
            std::cerr << "Error: Failed to parse file "
                      << qPrintable(fileName) << ": "
                      << reader.error() << " - "
                      << qPrintable(reader.errorString()) << std::endl;
        }
        if (file.error() != QFile::NoError) {
            std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                      << ": " << qPrintable(file.errorString())
                      << std::endl;
        }
    }
    // emit the signal that the data was changed
}

void projectTreeViewModel::readProject(QXmlStreamReader* reader, QString &fileName)
{
    reader->readNext();
    while ( !reader->atEnd() ) {
        if ( reader->isStartElement() ) {
            readElement(reader, rootItem, fileName);
        }
        else {
            reader->readNext();
        }
    }
}

void projectTreeViewModel::readElement(QXmlStreamReader* reader, projectTreeViewItem* parent, QString &fileName)
{
    // current data entries and item
    QList<QVariant> curData;
    projectTreeViewItem *curItem;

    if (reader->name().toString() == "name") {
        curData << reader->attributes().value("entry").toString() << fileName;
        curItem = new projectTreeViewItem(curData, parent);
        appendProject(curItem);
    } else {
        curData << reader->name().toString() << reader->attributes().value("entry").toString();
        curItem = new projectTreeViewItem(curData, parent);
        parent->appendChild(curItem);
    }

    // read next element and then loop on
    reader->readNext();
    while (!reader->atEnd()) {
        // check if we have the end of an element
        if (reader->isEndElement()) {
            reader->readNext();
            break;
        }
        // if it is a start element, process it, otherwise just read on
        if (reader->isStartElement()) { readElement(reader, curItem, fileName); }
        else { reader->readNext(); }
    }
}

void projectTreeViewModel::writeProject(projectTreeViewItem* project)
{
    bool rval = false;
    QString fileName = project->data(1).toString();

    // init the XML writer
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
        writer.writeAttribute("entry", project->data(0).toString());
        foreach (projectTreeViewItem* child, project->childItems) {
            writeElement(&writer, child);
        }
        writer.writeEndElement();

        // end header
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

    //return(rval);
}

void projectTreeViewModel::writeElement(QXmlStreamWriter* writer, projectTreeViewItem *element)
{
    QString elementName = element->data(0).toString();
    QString elementValue = element->data(1).toString();
    writer->writeStartElement(elementName);
    writer->writeAttribute("entry", elementValue);
    foreach (projectTreeViewItem *child, element->childItems) {
        writeElement(writer, child);
    }
    writer->writeEndElement();
}
