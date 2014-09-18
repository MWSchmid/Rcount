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

#include "resulttableview.h"
#include "ui_resulttableview.h"
//#include "../resultTableViewWidget-build-desktop/ui_resulttableview.h"

resultTableView::resultTableView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::resultTableView)
{
    ui->setupUi(this);
    curTable = "";

    // connect the find button
    connect(ui->findPushButton, SIGNAL(clicked()), this, SLOT(findRow()));
    connect(ui->findLineEdit, SIGNAL(returnPressed()), this, SLOT(findRow()));
}

resultTableView::~resultTableView()
{
    foreach (QString project, projectToModel.keys()) {
        delete projectToModel.value(project);
    }

    delete ui;
}

QSize resultTableView::sizeHint() const
{
    return(ui->groupBox->sizeHint());
}

void resultTableView::findRow()
{
    QString rowName = ui->findLineEdit->text();
    int rowIndex = -1;
    if (projectToModel.contains(curTable)) { rowIndex = projectToModel.value(curTable)->findRow(rowName); }
    if (rowIndex >= 0) { ui->tableView->selectRow(rowIndex); }
    else { ui->findLineEdit->setText(QObject::tr("could not find: %1").arg(rowName)); }
}

void resultTableView::displayCounts(QString fileName)
{
    if ( !projectToModel.contains(fileName) ) {
        resultTableViewModel* model = new resultTableViewModel(fileName);
        projectToModel.insert(fileName, model);
    } else {
        projectToModel.value(fileName)->readCounts(fileName);
    }
    // display the table
    if (fileName != curTable) {
        ui->tableView->setModel(projectToModel.value(fileName));
        //ui->tableView->horizontalHeader()->resizeSections();
        //ui->tableView->verticalHeader()->resizeSections();
        ui->tableView->resizeColumnsToContents();
        ui->tableView->resizeRowsToContents();
    }
    curTable = fileName;
}

void resultTableView::updateCounts(QString fileName)
{
    if ( !projectToModel.contains(fileName) ) {
        resultTableViewModel* model = new resultTableViewModel(fileName);
        projectToModel.insert(fileName, model);
    } else {
        projectToModel.value(fileName)->readCounts(fileName);
    }
    // display the table
    if (fileName == curTable) {
        ui->tableView->setModel(projectToModel.value(fileName));
        //ui->tableView->horizontalHeader()->resizeSections();
        //ui->tableView->verticalHeader()->resizeSections();
        ui->tableView->resizeColumnsToContents();
        ui->tableView->resizeRowsToContents();
    }
}

void resultTableView::removeCounts(QString fileName)
{
    if (projectToModel.contains(fileName)) {
        delete projectToModel.value(fileName);
        projectToModel.remove(fileName);
    }
}
