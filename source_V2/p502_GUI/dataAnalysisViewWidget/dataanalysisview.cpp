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
#include <iostream>

#include "../../p502_SOURCE/dataAnalysis/dataanalysis.h"

#include "dataanalysisview.h"

#include "../createProjectWidget/createprojectwidget.h"
#include "../projectTreeViewWidget/projecttreeview.h"
#include "../resultTableViewWidget/resulttableview.h"
#include "../analysisProgressWidget/analysisprogressview.h"

dataAnalysisView::dataAnalysisView(QWidget *parent)
    : QWidget(parent),
      createProjectView(new createProjectWidget(this)),
      projectView(new projectTreeView(this)),
      resultView(new resultTableView(this)),
      progressView(new analysisProgressView(this))
{
    // set a window title
    setWindowTitle(tr("Rcount-distribute"));
    //QIcon icon;
    //icon.addFile(QString::fromUtf8(":/icons/p502icon.png"), QSize(), QIcon::Normal, QIcon::Off);
    //setWindowIcon(icon);

    // initialize all the other widgets
    // get and set the minimal sizes
    int leftSideMinimalWidth = createProjectView->sizeHint().width();
    int leftSideMinimalHeight = 2*createProjectView->sizeHint().height();
    this->setMinimumHeight(leftSideMinimalHeight);
    this->setMinimumWidth(3*leftSideMinimalWidth);

    // create a vertical layout on the left
    QVBoxLayout *leftSide = new QVBoxLayout;
    leftSide->addWidget(projectView);
    leftSide->addWidget(createProjectView);
    // create a vertical layout on the right
    QVBoxLayout *rightSide = new QVBoxLayout;
    rightSide->addWidget(resultView);
    rightSide->addWidget(progressView);
    // create a layout over all
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(leftSide, 1);
    layout->addLayout(rightSide, 2);
    // set the layout
    setLayout(layout);

    // hide the createProjectWidget and the progressView
    createProjectView->hide();
    progressView->hide();

    //! connections
    connect(projectView, SIGNAL(newClicked()), this, SLOT(createProject()));
    connect(projectView, SIGNAL(openClicked()), this, SLOT(loadProject()));
    connect(projectView, SIGNAL(currentProject(QString)), resultView, SLOT(displayCounts(QString)));
    connect(projectView, SIGNAL(closedProject(QString)), resultView, SLOT(removeCounts(QString)));
    connect(createProjectView, SIGNAL(savedNewProject(QString)), projectView, SLOT(addProject(QString)));
    connect(projectView, SIGNAL(runClickedFile(QString)), this, SLOT(runDataAnalysis(QString)));
    connect(&analyzer, SIGNAL(analysisStatus(QString)), progressView, SLOT(updateStatus(QString)));
    connect(&analyzer, SIGNAL(analysisProgress(int)), progressView, SLOT(updateProgress(int)));
    connect(&analyzer, SIGNAL(errorMessage(QString)), progressView, SLOT(updateStatus(QString)));
    connect(&analyzer, SIGNAL(everythingProcessed()), this, SLOT(hideProgress()));
    connect(&analyzer, SIGNAL(analysisCountsWritten(QString)), resultView, SLOT(updateCounts(QString)));
    connect(&analyzer, SIGNAL(writerStatsObtained(QString)), projectView, SLOT(addStats(QString)));
    connect(progressView, SIGNAL(analysisCanceled()), &analyzer, SLOT(cancelDataAnalysis()));
}

dataAnalysisView::~dataAnalysisView()
{
    // stop the analyzer and wait for it
    analyzer.addProject("STOPTHREAD");
    analyzer.wait();
    //! TODO
}

void dataAnalysisView::createProject()
{
    createProjectView->show();
}


void dataAnalysisView::loadProject()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
                                                    tr("select a project file"),
                                                    ".",
                                                    tr("p502 project files (*.xml)"));
    if (!fileNames.isEmpty()) {
        foreach (QString fileName, fileNames) {
            projectView->addProject(fileName);
        }
    }
}

void dataAnalysisView::runDataAnalysis(QString fileName)
{
    if (progressView->isHidden()) {
        progressView->show();
    }
    analyzer.addProject(fileName);
}

void dataAnalysisView::hideProgress() {
    progressView->hide();
}

