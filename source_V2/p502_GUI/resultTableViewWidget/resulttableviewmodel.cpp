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

#include <QtGui>

#include "resulttableviewmodel.h"

resultTableViewModel::resultTableViewModel(QString& fileName, QObject *parent) :
    QAbstractTableModel(parent)
{
    dataLoaded = false;

    // fixed column headers
    columnHeader << "sumUnAmb" << "sumAmb"<< "sumAllo"<< "sumDistUnAmb"<< "sumDistAmb"<< "sumDistAllo"<< "totalHits"<< "isValid";

    // reserve some space for the data vector
    dataVec.reserve(30000);

    // try to read the data
    readCounts(fileName);
}

resultTableViewModel::~resultTableViewModel()
{
    //! destructor?
}

int resultTableViewModel::rowCount(const QModelIndex &parent) const
{
    return(rowNames.count());
}

int resultTableViewModel::columnCount(const QModelIndex &parent) const
{
    return(columnHeader.count());
}

QVariant resultTableViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) { return(QVariant()); }

    if (role == Qt::TextAlignmentRole) { return(int(Qt::AlignRight | Qt::AlignVCenter)); }
    else if (role == Qt::DisplayRole) {
        QString out = dataVec.at(index.row()).at(index.column());
        return(out);
    }
    return(QVariant());
}

QVariant resultTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) { return(QVariant()); }

    if (orientation == Qt::Horizontal) { return(columnHeader.at(section)); }

    return(rowNames.at(section));
}

int resultTableViewModel::findRow(QString &rowName)
{
    return(rowNames.indexOf(rowName));
}

void resultTableViewModel::readCounts(QString &fileName)
{
    // check if data is already loaded
    if (dataLoaded) {
        return;
    }

    // the file and textstream
    QFile file;
    QTextStream in;
    file.setFileName(fileName);

    // check if file available (interrupt if not)
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    // set some stuff
    in.setDevice(&file);
    in.setCodec("UTF-8");

    // read the file
    QString rowName;
    QString line;
    QStringList fields;
    while ( !in.atEnd() ) {
        line = in.readLine();
        fields = line.split('\t');
        rowName = fields.takeFirst();
        rowNames.append(rowName);
        dataVec.append(fields);
    }
    if (rowNames.count() > 10) { dataLoaded = true; }

    // close and 'check' for error
    file.close();//! ADDED 2014
    if (file.error() != QFile::NoError) {return;}//! ADDED 2014
}

