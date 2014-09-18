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

#include "processmultireadsview.h"
#include "ui_processmultireadsview.h"

#include <iostream>

processMultireadsView::processMultireadsView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::processMultireadsView)
{
    ui->setupUi(this);
    connect(ui->inputFind, SIGNAL(clicked()), this, SLOT(searchReadsInfile()));
    connect(ui->outputFind, SIGNAL(clicked()), this, SLOT(searchReadsOutfile()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(sendProcessReads()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(sendClose()));

}

processMultireadsView::~processMultireadsView()
{
    delete ui;
}

QSize processMultireadsView::sizeHint() const
{
    return(ui->frame->sizeHint());
}

void processMultireadsView::searchReadsInfile() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("select an alignment file"),
                                                    ".",
                                                    tr("compressed SAM files (*.bam)"));
    if (!fileName.isEmpty()) { ui->inputLineEdit->setText(fileName); }
}

void processMultireadsView::searchReadsOutfile() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("select an alignment file"),
                                                    ".",
                                                    tr("compressed SAM files (*.bam)"));
    if (!fileName.isEmpty()) { ui->outputLineEdit->setText(fileName); }

}

void processMultireadsView::hideButtonBox() {
    if (this->ui->buttonBox->isVisible()) {
        this->ui->buttonBox->hide();
    }
}

void processMultireadsView::showButtonBox() {
    if (this->ui->buttonBox->isHidden()) {
        this->ui->buttonBox->show();
    }
}

void processMultireadsView::removeFromQueue(QString workString) {
    QList<QListWidgetItem *> items = this->ui->queueListWidget->findItems(workString, Qt::MatchExactly);
    foreach (QListWidgetItem * item, items) { delete item; }
    std::cerr << QTime::currentTime().toString().toStdString() << std::endl << std::flush;
}

void processMultireadsView::sendProcessReads() {
    this->readsInfile = ui->inputLineEdit->text();
    this->readsOutfile = ui->outputLineEdit->text();
    this->doWeight = "n";
    this->allocationDistance = ui->spinBox->text();
    if (ui->checkBox->isChecked()) {this->doWeight = "y";}
    QStringList temp;
    temp << this->readsInfile << this->readsOutfile << this->doWeight << this->allocationDistance;
    QString messageSent = temp.join("|");
    this->ui->queueListWidget->addItem(messageSent);
    std::cerr << QTime::currentTime().toString().toStdString() << std::endl << std::flush;
    emit this->processReads(messageSent);
}

void processMultireadsView::sendClose() {
    emit this->closeAll();
}






