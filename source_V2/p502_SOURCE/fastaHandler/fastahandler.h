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

#ifndef FASTAHANDLER_H
#define FASTAHANDLER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMap>
#include <iostream>


class fastaHandler {
public:
    // public member variables
    QMap<QString, QString > sequences;

    // constructor - does not do a lot
    fastaHandler();

    // destructor (close file normally)
    //~fastaHandler();

    // read the sequences and tell if success
    bool readSequences(const QString &fileName);

    // write the sequences and tell if success
    bool writeSequences(const QString &fileName);

};

#endif // FASTAHANDLER_H
