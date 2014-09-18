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

#include "projecttreeviewitem.h"

projectTreeViewItem::projectTreeViewItem(const QList<QVariant> &data, projectTreeViewItem *parent)
{
    parentItem = parent;
    itemData = data;
}

projectTreeViewItem::~projectTreeViewItem()
{
    qDeleteAll(childItems);
}

void projectTreeViewItem::appendChild(projectTreeViewItem *child)
{
    childItems.append(child);
}

projectTreeViewItem *projectTreeViewItem::child(int row)
{
    return(childItems.value(row));
}

int projectTreeViewItem::childCount() const
{
    return(childItems.count());
}

int projectTreeViewItem::columnCount() const
{
    return(itemData.count());
}

QVariant projectTreeViewItem::data(int column) const
{
    return(itemData.value(column));
}

int projectTreeViewItem::row() const
{
    if (parentItem) {
        return(parentItem->childItems.indexOf(const_cast<projectTreeViewItem*>(this)));
    } else {
        return(0);
    }
}

bool projectTreeViewItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row){
        delete childItems.takeAt(position);
    }

    return true;
}

projectTreeViewItem *projectTreeViewItem::parent()
{
    return(parentItem);
}

projectTreeViewItem *projectTreeViewItem::topParent(projectTreeViewItem *rootItem)
{
    if (parentItem == rootItem) {
        return(this);
    } else {
        return(parentItem->topParent(rootItem));
    }
}
