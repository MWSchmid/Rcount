#include <QtGui>

#include "databaseitem.h"
#include "databasereader.h"
#include "databasewriter.h"
#include "database.h"
#include "databasemodel.h"

databaseModel::databaseModel(database *data, QObject *parent) : QAbstractItemModel(parent)
{
    // the root item shall hold all the headers necessary
    this->rootItem = data->rootItem;


}

databaseModel::~databaseModel()
{
    delete this->rootItem;
}




//! some reimplemented virtual functions


//
QModelIndex databaseModel::index(int row, int column, const QModelIndex &parentIndex) const
{
    if ((parentIndex.isValid()) && (parentIndex.column() != 0)) {
        return( QModelIndex() );
    }
    else {
        databaseItem *parentItem = itemFromIndex(parentIndex);
        databaseItem *childItem = parentItem->child(row);
        if (childItem) {
            return( createIndex(row, column, childItem) );
        }
        else {
            return( QModelIndex() );
        }
    }
}

//
QModelIndex databaseModel::parent(const QModelIndex &childIndex) const
{
    if (!childIndex.isValid()) {
        return(QModelIndex());
    }
    else {
        databaseItem *childItem = itemFromIndex(childIndex);
        databaseItem *parentItem = childItem->parent();

        if (parentItem == this->rootItem) {
            return( QModelIndex() );
        }
        else {
            return( createIndex(parentItem->childNumber(), 0, parentItem) );
        }
    }
}

// return data from an entry -> this is more important in the future
QVariant databaseModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return(QVariant());
    }
    else {
        databaseItem *item = itemFromIndex(index);
        return( item->data(index.column()) );
    }
}

// access the header data -> very basic at the moment - more important in the future
QVariant databaseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return(this->rootItem->data(section));
    }
    else {
        return(QVariant());
    }
}




//! other functions


// an index to use via Chrom: Pos -> needs  to be a persistent model index!
bool databaseModel::createCPindex()
{
    return(false); //! TODO: implementation
}



// having an index, get the item at this position
databaseItem *databaseModel::itemFromIndex(const QModelIndex &index) const
{
    if (index.isValid()) {
        return(static_cast<databaseItem *>(index.internalPointer()));
    } else {
        return(this->rootItem);
    }
}






