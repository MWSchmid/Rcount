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

#ifndef PROJECTTREEVIEWITEM_H
#define PROJECTTREEVIEWITEM_H

#include <QVariant>
#include <QList>

class projectTreeViewModel;

class projectTreeViewItem
{
public:
    projectTreeViewItem(const QList<QVariant> &data, projectTreeViewItem *parent);
    ~projectTreeViewItem();

    void appendChild(projectTreeViewItem *child);
    projectTreeViewItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    bool removeChildren(int position, int count);
    projectTreeViewItem *parent();
    projectTreeViewItem *topParent(projectTreeViewItem *rootItem);

private:
    QList<projectTreeViewItem*> childItems;
    QList<QVariant> itemData;
    projectTreeViewItem *parentItem;

    friend class projectTreeViewModel;
};

#endif // PROJECTTREEVIEWITEM_H
