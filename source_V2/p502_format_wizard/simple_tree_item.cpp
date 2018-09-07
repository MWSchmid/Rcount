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

/*
    simple_tree_item.cpp

    A container for items of data supplied by the simple tree model.
*/

#include <QStringList>

#include "simple_tree_item.h"


//! [0]
simple_tree_item::simple_tree_item(const QVector<QVariant> &data, simple_tree_item *parent)
{
    parentItem = parent;
    itemData = data;
}
//! [0]

//! [1]
simple_tree_item::~simple_tree_item()
{
    qDeleteAll(childItems);
}
//! [1]

//! [2]
simple_tree_item *simple_tree_item::child(int number)
{
    return childItems.value(number);
}
//! [2]

//! [3]
int simple_tree_item::childCount() const
{
    return childItems.count();
}
//! [3]

//! [4]
int simple_tree_item::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<simple_tree_item*>(this));

    return 0;
}
//! [4]

//! [5]
int simple_tree_item::columnCount() const
{
    return itemData.count();
}
//! [5]

//! [6]
QVariant simple_tree_item::data(int column) const
{
    return itemData.value(column);
}
//! [6]

//! [7]
bool simple_tree_item::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        simple_tree_item *item = new simple_tree_item(data, this);
        childItems.insert(position, item);
    }

    return true;
}
//! [7]

//! [8]
bool simple_tree_item::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (simple_tree_item *child, childItems)
        child->insertColumns(position, columns);

    return true;
}
//! [8]

//! [9]
simple_tree_item *simple_tree_item::parent()
{
    return parentItem;
}
//! [9]

//! [10]
bool simple_tree_item::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}
//! [10]

bool simple_tree_item::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    foreach (simple_tree_item *child, childItems)
        child->removeColumns(position, columns);

    return true;
}

//! [11]
bool simple_tree_item::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}
//! [11]
