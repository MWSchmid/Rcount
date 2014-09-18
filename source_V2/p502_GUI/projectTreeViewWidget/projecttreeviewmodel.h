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

#ifndef PROJECTTREEVIEWMODEL_H
#define PROJECTTREEVIEWMODEL_H

#include <QAbstractItemModel>
#include <QSet>

class projectTreeViewItem;
class QXmlStreamReader;
class QXmlStreamWriter;

class projectTreeViewModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit projectTreeViewModel(QObject *parent = 0);
    ~projectTreeViewModel();
    //reimpl
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());
    //custom
    void appendProject(projectTreeViewItem* projectItem);
    void removeProject(QModelIndex &index);
    QModelIndex topParent(const QModelIndex &index) const;
    void addProject(QString &fileName);
    void addStats(QString &projectName, QStringList &lines);
    QString getCountFile(const QModelIndex &index) const;
    QString getFileName(const QModelIndex &index) const;
    QStringList getAllFileNames() const;

private:
    projectTreeViewItem *rootItem;

    // internal reader functions
    void readProject(QXmlStreamReader* reader, QString &fileName);
    void readElement(QXmlStreamReader* reader, projectTreeViewItem* parent, QString &fileName);
    // internal writer functions
    void writeProject(projectTreeViewItem* project);
    void writeElement(QXmlStreamWriter* writer, projectTreeViewItem *element);

};

#endif // PROJECTTREEVIEWMODEL_H
