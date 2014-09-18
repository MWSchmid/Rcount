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

#include "analysisprogressview.h"
#include "processmultireadsview.h"
#include "processmultireads.h"

processMultireads::processMultireads(QWidget *parent) :
    QWidget(parent),
    progressView(new analysisProgressView(this)),
    multireadsView(new processMultireadsView(this))
{
    // set a window title
    setWindowTitle(tr("Rcount-multireads"));

    // initialize all the other widgets
    // get and set the minimal sizes
    int minWidth = multireadsView->sizeHint().width();
    int minHeight = multireadsView->sizeHint().height()+progressView->sizeHint().height()*1.5;
    this->setMinimumHeight(minHeight);
    this->setMinimumWidth(minWidth);

    // layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(multireadsView);
    layout->addWidget(progressView);
    setLayout(layout);

    // hide the progressView
    //! progressView->hide();

    // connections
    connect(&processor, SIGNAL(processStatus(QString)), progressView, SLOT(updateStatus(QString)));
    connect(&processor, SIGNAL(processProgress(int)), progressView, SLOT(updateProgress(int)));
    connect(&processor, SIGNAL(errorMessage(QString)), progressView, SLOT(updateStatus(QString)));
    //! connect(&processor, SIGNAL(idleAgain()), this, SLOT(hideProgress()));
    connect(&processor, SIGNAL(workFinished(QString)), multireadsView, SLOT(removeFromQueue(QString)));
    connect(progressView, SIGNAL(analysisCanceled()), &processor, SLOT(cancelProcessing()));
    connect(multireadsView, SIGNAL(closeAll()), &processor, SLOT(cancelProcessing()));
    connect(multireadsView, SIGNAL(closeAll()), this, SLOT(close()));
    connect(multireadsView, SIGNAL(processReads(QString)), this, SLOT(runProcessor(QString)));

}

processMultireads::~processMultireads()
{
    // stop the processor and wait for it
    processor.addWork("STOPTHREAD");
    processor.wait();
}

void processMultireads::runProcessor(QString toProcess) {
    if (progressView->isHidden()) { progressView->show(); }
    processor.addWork(toProcess);
}

void processMultireads::hideProgress() {
    if (progressView->isVisible()) { progressView->hide(); }
}
/*
void processMultireads::runProcessor(QString toProcess) {
    if (progressView->isHidden()) { progressView->show(); }
    multireadsView->hideButtonBox();//! NOTE THAT THIS IS ACTUALLY NOT NECESSARY - IT SHOULD IN PRINCIPLE ADD NEW WORK TO THE LIST
    processor.addWork(toProcess);
}

void processMultireads::hideProgress() {
    if (progressView->isVisible()) { progressView->hide(); }
    multireadsView->showButtonBox();
}
*/
