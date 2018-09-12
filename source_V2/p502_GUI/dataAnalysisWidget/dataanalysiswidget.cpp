#include <QtGui>

#include "../createProjectWidget/createprojectwidget.h"

#include "settingstreeitem.h"
#include "settingstreemodel.h"

#include "dataanalysiswidget.h"
#include "ui_dataanalysiswidget.h"

dataAnalysisWidget::dataAnalysisWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dataAnalysisWidget),
    _createProjectWidget(new createProjectWidget()), // no parent - otherwise it is embedded
    _projectListModel(new QStringListModel(this))
{
    ui->setupUi(this);

    //! external connections
    connect(_createProjectWidget, SIGNAL(savedNewProject(QString)), this, SLOT(readProjectFile(QString)));

    //! connections within this widget
    // from the projects List
    connect(ui->newProjectPushButton, SIGNAL(clicked()), this, SLOT(createProject()));
    connect(ui->openProjectPushButton, SIGNAL(clicked()), this, SLOT(loadProject()));
    connect(ui->closeProjectPushButton, SIGNAL(clicked()), this, SLOT(closeProject()));
    connect(ui->projectListView, SIGNAL(activated(QModelIndex)), this, SLOT(updateOtherViews(QModelIndex)));

    // from the settings Tree

    // from the results Table

}

dataAnalysisWidget::~dataAnalysisWidget()
{
    delete ui;
    delete _createProjectWidget;
    delete _projectListModel;
    //! TODO - update?
}







//! SLOTS

void dataAnalysisWidget::createProject()
{
    if (!_createProjectWidget) {
        _createProjectWidget = new createProjectWidget();
        connect(_createProjectWidget, SIGNAL(savedNewProject(QString)), this, SLOT(readProjectFile(QString)));
    } // this is in case somebody closes the window fully
    _createProjectWidget->show();
    _createProjectWidget->raise();
    _createProjectWidget->activateWindow();
}

void dataAnalysisWidget::loadProject()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("select a project file"),
                                                    ".",
                                                    tr("p502 project files (*.xml)"));
    if (!fileName.isEmpty()) { this->readProjectFile(fileName); }
}

void dataAnalysisWidget::closeProject()
{
    // close the current settings and results views
    ui->settingsTreeView->reset();
    ui->resultsTableView->reset();

    // remove the element from the settings and results model collections
    QModelIndex curIndex = ui->projectListView->currentIndex();
    QString projectName = this->_projectListModel->data(curIndex, Qt::DisplayRole).toString();
    _projectToSettings.remove(projectName);
    _projectToResults.remove(projectName);

    // remove it from the project list
    _projectListModel->removeRows(curIndex.row(), 1);
}

void dataAnalysisWidget::updateOtherViews(QModelIndex)
{
    QModelIndex curIndex = ui->projectListView->currentIndex();
    //! TODO
}

void dataAnalysisWidget::readProjectFile(QString fileName)
{
    //! TODO
}

void dataAnalysisWidget::readCountTable(QString fileName)
{
    //! TODO
}
