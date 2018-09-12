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

#include "analysisprogressview.h"
#include "ui_analysisprogressview.h"
//#include "../analysisProgressWidget-build-desktop/ui_analysisprogressview.h"

//#include "printTimeAndMem.h" //! MEM USAGE

analysisProgressView::analysisProgressView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::analysisProgressView)
{
    ui->setupUi(this);
    progress = 0;
    status = "idle";
    ui->label->setText(status);
    ui->progressBar->setValue(progress);
    //ui->pushButton->hide(); //! CANCEL DOES NOT WORK AT THE MOMENT - IT DOES NOW
    connect(ui->pushButton, SIGNAL(clicked()), this, SIGNAL(analysisCanceled()));
}

analysisProgressView::~analysisProgressView()
{
    delete ui;
}

QSize analysisProgressView::sizeHint() const
{
    return(ui->groupBox->sizeHint());
}

void analysisProgressView::updateProgress(int progressIN)
{
    progress = progressIN;
    ui->progressBar->setValue(progress);
    //print_time_and_memory(); //! MEM USAGE
}

void analysisProgressView::updateStatus(QString statusIN)
{
    status = statusIN;
    ui->label->setText(status);
}

