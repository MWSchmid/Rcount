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

#ifndef DATABASEWRITER_H
#define DATABASEWRITER_H

#include <QXmlStreamWriter>
#include <QMap>
#include "databaseitem.h"

//class databaseItem;
class database;

class databaseWriter
{
public:
    databaseWriter(database *data);

    bool writeFile(const QString &fileName);

private:
    void writeElement(databaseItem *element);

    database *data;
    QXmlStreamWriter writer;
    QMap<Type,QString> stringmap;
};

#endif // DATABASEWRITER_H
