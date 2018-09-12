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

#ifndef MAPPINGTREE_H
#define MAPPINGTREE_H

#include <QList>
#include <QStringList>
#include <iostream>

#include "../dataStructure/databaseitem.h"

//! well - its not the whole tree anymore, but an item of it

class mappingTreeItem
{
public:
    //data members
    databaseItem* _self; // the pointer to the entry in the database
    QList<mappingTreeItem> _children; // pointers to the children with valid mappings

    //constructor
    mappingTreeItem(databaseItem* self) {
        this->_self = self;
        this->_children.clear();
    }

    //! new in the countsPerTranscript version get gene and transcript names
    QString getNames() {
        QString out;
        QStringList temp;
        temp << this->_self->getName();
        foreach (mappingTreeItem child, this->_children) {
            temp << child._self->getName();
        }
        out = temp.join(",");
        return(out);
    }

    //! HM - DOES A DEFAULT CONSTRUCTOR WORK?
    // destructor - if one element is deleted, all child elements must be deleted too
    //~mappingTreeItem() {
        //this->_self = 0; // make the pointer a pointer to 0 to remove it (we do not want to remove the database item
        //this->_children.clear(); // does this work?
    //}
};

#endif // MAPPINGTREE_H
