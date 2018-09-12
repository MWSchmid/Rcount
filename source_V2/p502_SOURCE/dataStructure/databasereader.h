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

#ifndef DATABASEREADER_H
#define DATABASEREADER_H

#include <QXmlStreamReader>
#include <QMap>
#include "databaseitem.h"

//class databaseItem;
class database;

class databaseReader
{
public:
    databaseReader(database *data);

    bool readFile(const QString &fileName);

private:
    void readAnnotationTree();
    void readElement(Type nodeType,  databaseItem *parent);
    //! older file version V1
    void readAnnotationTreeV1();
    void readElementV1(Type nodeType,  databaseItem *parent);
    void skipUnknownElementV1();

    database *data;
    QXmlStreamReader reader;
    QMap<QString,Type> typemap;

};

#endif // DATABASEREADER_H
