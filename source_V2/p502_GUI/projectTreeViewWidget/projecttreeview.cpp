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

#include <iostream>

#include "projecttreeview.h"
#include "ui_projecttreeview.h"
//#include "../projectTreeViewWidget-build-desktop/ui_projecttreeview.h"

#include "projecttreeviewitem.h"
#include "projecttreeviewmodel.h"

projectTreeView::projectTreeView(QWidget *parent) :
    QWidget(parent),
    model(new projectTreeViewModel(this)),
    ui(new Ui::projectTreeView)
{
    ui->setupUi(this);

    // set the model for the view
    ui->treeView->setModel(model);
    for (int column = 0; column < model->columnCount(); ++column) { ui->treeView->resizeColumnToContents(column); }

    // connect the dataChanged signal?
    connect(ui->newPushButton, SIGNAL(clicked()), this, SIGNAL(newClicked()));
    connect(ui->openPushButton, SIGNAL(clicked()), this, SIGNAL(openClicked()));
    connect(ui->runPushButton, SIGNAL(clicked()), this, SLOT(runClicked()));
    connect(ui->runAllPushButton, SIGNAL(clicked()), this, SLOT(runAllClicked()));
    connect(ui->treeView, SIGNAL(activated(QModelIndex)), this, SLOT(switchedProject(QModelIndex)));
    connect(ui->closePushButton, SIGNAL(clicked()), this, SLOT(closeCurrentProject()));
    connect(ui->treeView, SIGNAL(expanded(QModelIndex)), this, SLOT(resizeColumns()));
    connect(ui->treeView, SIGNAL(collapsed(QModelIndex)), this, SLOT(resizeColumns()));
}

projectTreeView::~projectTreeView()
{
    //delete model; should be automatically deleted as this is the parent of it
    delete ui;
}

QSize projectTreeView::sizeHint() const
{
    return(ui->groupBox->sizeHint());
}

void projectTreeView::resizeColumns()
{
    for (int column = 0; column < model->columnCount(); ++column) { ui->treeView->resizeColumnToContents(column); }
}

void projectTreeView::closeCurrentProject()
{
    // get the index of the active item
    QModelIndex curIndex = ui->treeView->currentIndex();
    // get the countfile
    QString countFile = model->getCountFile(curIndex);
    // get the counts file name
    model->removeProject(curIndex);
    // update the view
    //ui->treeView->setModel(model);
    resizeColumns();
    // EMIT THE COUNT FILE
    emit closedProject(countFile);
}

void projectTreeView::addProject(QString fileName)
{
    model->addProject(fileName);
    // update the view
    //ui->treeView->setModel(model);
    resizeColumns();
}

void projectTreeView::addStats(QString statsString)
{
    QStringList lines = statsString.split("|");
    QString projectName = lines.takeFirst();
    model->addStats(projectName, lines);
    // update the view
    //ui->treeView->setModel(model);
    resizeColumns();
}

void projectTreeView::runClicked()
{
    // get the index of the active item
    QModelIndex curIndex = ui->treeView->currentIndex();
    // get the countfile
    QString projectFile = model->getFileName(curIndex);
    emit runClickedFile(projectFile);
}

void projectTreeView::runAllClicked()
{
    QStringList fileNames = model->getAllFileNames();
    foreach(QString fileName, fileNames) {
        emit runClickedFile(fileName);
    }
}

void projectTreeView::switchedProject(QModelIndex index)
{
    // get the countfile
    QString countFile = model->getCountFile(index);
    // EMIT THE COUNT FILE
    emit currentProject(countFile);
}

//! todo: when selection changes, emit the current count file



