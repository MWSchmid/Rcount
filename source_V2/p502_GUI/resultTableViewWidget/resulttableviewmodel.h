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

#ifndef RESULTTABLEVIEWMODEL_H
#define RESULTTABLEVIEWMODEL_H

#include <QAbstractTableModel>

#include <QStringList>
#include <QVector>

class resultTableViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit resultTableViewModel(QString& fileName, QObject *parent = 0);
    ~resultTableViewModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int findRow(QString &rowName);
    void readCounts(QString &fileName);

private:
    bool dataLoaded;
    QStringList columnHeader;
    QStringList rowNames;
    QVector<QStringList> dataVec;

};

#endif // RESULTTABLEVIEWMODEL_H
