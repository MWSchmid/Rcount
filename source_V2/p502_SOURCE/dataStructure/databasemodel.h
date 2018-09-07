#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

//#include <QtGui/QStylePlugin>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

/****************************************************************
** this is first attempt for a model that displays the data
** structure used in p502. It should not be used for raw data
** analyses, but for displaying the underlying data in different
** ways. Planned is a table or tree and a genome browser.
** Data analysis should however be done with a separate class,
** which has direct access to the data structure. Like this, it
** is easier (and likely faster) to do the data analysis part.
*****************************************************************/

class databaseItem;
class databaseReader;
class databaseWriter;
class database;

class databaseModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    // constructor
    databaseModel(database *data,QObject *parent = 0);

    // destructor
    ~databaseModel();


    //! some reimplemented virtual functions

    //
    QModelIndex index(int row, int column, const QModelIndex &parent) const;

    //
    QModelIndex parent(const QModelIndex &child) const;

    // return data from an entry -> this is more important in the future
    QVariant data(const QModelIndex &index, int role) const;

    // access the header data
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

    //bool insertColumns(int position, int columns, const QModelIndex &parent = QModelIndex());

    //bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex());

    //bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    //bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    //! other functions

    // an index to use via Chrom: Pos -> needs  to be a persistent model index!
    bool createCPindex();

private:

    // having an index, get the item at this position
    databaseItem *itemFromIndex(const QModelIndex &index) const;

    databaseItem *rootItem;
};

#endif // DATABASEMODEL_H







