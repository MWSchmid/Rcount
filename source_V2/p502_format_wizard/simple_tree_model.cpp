/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

/***************************************************************************
** changed at the 12th of August 2011 by Marc Schmid
****************************************************************************/

#include <QtWidgets>

#include "simple_tree_item.h"
#include "simple_tree_model.h"

//! [0]
simple_tree_model::simple_tree_model(const QStringList &headers, const QString &data,
                     QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;

    rootItem = new simple_tree_item(rootData);
    setupModelData(data.split(QString("\n")), rootItem);
}
//! [0]

//! [1]
simple_tree_model::~simple_tree_model()
{
    delete rootItem;
}
//! [1]

//! [2]
int simple_tree_model::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}
//! [2]

QVariant simple_tree_model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    simple_tree_item *item = getItem(index);

    return item->data(index.column());
}

//! [3]
Qt::ItemFlags simple_tree_model::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
//! [3]

//! [4]
simple_tree_item *simple_tree_model::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        simple_tree_item *item = static_cast<simple_tree_item*>(index.internalPointer());
        if (item) return item;
    }
    return rootItem;
}
//! [4]

QVariant simple_tree_model::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

//! [5]
QModelIndex simple_tree_model::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();
//! [5]

//! [6]
    simple_tree_item *parentItem = getItem(parent);

    simple_tree_item *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
//! [6]

bool simple_tree_model::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool simple_tree_model::insertRows(int position, int rows, const QModelIndex &parent)
{
    simple_tree_item *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

//! [7]
QModelIndex simple_tree_model::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    simple_tree_item *childItem = getItem(index);
    simple_tree_item *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}
//! [7]

bool simple_tree_model::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool simple_tree_model::removeRows(int position, int rows, const QModelIndex &parent)
{
    simple_tree_item *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

//! [8]
int simple_tree_model::rowCount(const QModelIndex &parent) const
{
    simple_tree_item *parentItem = getItem(parent);

    return parentItem->childCount();
}
//! [8]

bool simple_tree_model::setData(const QModelIndex &index, const QVariant &value,
                        int role)
{
    if (role != Qt::EditRole)
        return false;

    simple_tree_item *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool simple_tree_model::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void simple_tree_model::setupModelData(const QStringList &lines, simple_tree_item *parent)
{
    QList<simple_tree_item*> parents;
    QList<uint> levels;
    uint curlevel;
    parents << parent;
    levels << 0;

    int number = 0;

    while (number < lines.count()) {
        QString lineData = lines[number];
        if (!lineData.isEmpty()) {
            // Read the column data from the line. Remember that the last entry is the level.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            curlevel = columnStrings.last().toUInt();
            columnStrings.pop_back();
            QVector<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column) { columnData << columnStrings[column]; }
            if (curlevel > levels.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.
                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    levels << curlevel;
                }
            } else {
                while (curlevel < levels.last() && parents.count() > 0) {
                    parents.pop_back();
                    levels.pop_back();
                }
            }
            // Append a new item to the current parent's list of children.
            simple_tree_item *parent = parents.last();
            parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
        }
        number++;
    }
}

QString simple_tree_model::save_treestructure() {
    QString out;
    QStringList lines;
    QString curline;
    QString header;
    QString priority;
    // read through the data and write a string equal to the one used in setupModelData
    //! TODO
    for (int lc = 0; lc < rootItem->childCount(); ++lc) {
        header = rootItem->child(lc)->data(0).toString();
        priority = rootItem->child(lc)->data(1).toString();
        curline = QObject::tr("%1\t%2\t0").arg(header, priority);
        lines << curline;
        for (int tc = 0; tc < rootItem->child(lc)->childCount(); ++tc) {
            header = rootItem->child(lc)->child(tc)->data(0).toString();
            priority = rootItem->child(lc)->child(tc)->data(1).toString();
            curline = QObject::tr("%1\t%2\t1").arg(header, priority);
            lines << curline;
            for (int sc = 0; sc < rootItem->child(lc)->child(tc)->childCount(); ++sc) {
                header = rootItem->child(lc)->child(tc)->child(sc)->data(0).toString();
                priority = rootItem->child(lc)->child(tc)->child(sc)->data(1).toString();
                curline = QObject::tr("%1\t%2\t2").arg(header, priority);
                lines << curline;
            }
        }
    }
    out = lines.join("\n");
    return(out);
}


