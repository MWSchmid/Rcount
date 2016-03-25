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

#include <QtWidgets>
#include <QtXml>

#include <iostream>

#include "createprojectwidget.h"
#include "ui_createprojectwidget.h"
//#include "../createProjectWidget-build-desktop/ui_createprojectwidget.h"

createProjectWidget::createProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::createProjectWidget)
{
    ui->setupUi(this);

    // initial parameters and hiding of the Annotation Outfile
    ui->subRegionGroupBox->hide();
    ui->biasParameterGroupBox->hide();
    ui->annotationOutfileLabel->hide();
    ui->annotationOutfileLineEdit->hide();
    ui->findAnnotationOutfilePushButton->hide();
    // hide the subregion and settings tab completely
    ui->regionTab->setEnabled(false);
    ui->settingsTab->setEnabled(false);

    // connect signals and slots
    connect(ui->findAnnotationInfilePushButton, SIGNAL(clicked()), this, SLOT(searchAnnotationInfile()));
    connect(ui->findReadsInfilePushButton, SIGNAL(clicked()), this, SLOT(searchReadsInfile()));
    connect(ui->findAnnotationOutfilePushButton, SIGNAL(clicked()), this, SLOT(searchAnnotationOutfile()));
    connect(ui->findReadsOutfilePushButton, SIGNAL(clicked()), this, SLOT(searchReadsOutfile()));
    connect(ui->findCountsOutfilePushButton, SIGNAL(clicked()), this, SLOT(searchCountsOutfile()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveProject()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(hide()));
}

createProjectWidget::~createProjectWidget()
{
    delete ui;
}

//! file searcher slots

void createProjectWidget::searchAnnotationInfile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("select an annotation file"),
                                                    ".",
                                                    tr("Rcount-XML files (*.xml)"));
    if (!fileName.isEmpty()) { ui->annotationInfileLineEdit->setText(fileName); }
}

void createProjectWidget::searchReadsInfile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("select an alignment file"),
                                                    ".",
                                                    tr("compressed SAM files (*.bam)"));
    if (!fileName.isEmpty()) { ui->readsInfileLineEdit->setText(fileName); }
}

void createProjectWidget::searchAnnotationOutfile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("select an annotation file"),
                                                    ".",
                                                    tr("Rcount-XML files (*.xml)"));
    if (!fileName.isEmpty()) { ui->annotationOutfileLineEdit->setText(fileName); }
}

void createProjectWidget::searchReadsOutfile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("select an alignment file"),
                                                    ".",
                                                    tr("compressed SAM files (*.bam)"));
    if (!fileName.isEmpty()) { ui->readsOutfileLineEdit->setText(fileName); }
}

void createProjectWidget::searchCountsOutfile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("select a count file"),
                                                    ".",
                                                    tr("text files (*.txt)"));
    if (!fileName.isEmpty()) { ui->countsOutfileLineEdit->setText(fileName); }
}

//! accessors

QString createProjectWidget::getAnnotationInfile()
{
    QString fileName = ui->annotationInfileLineEdit->text();
    return(fileName);
}

QString createProjectWidget::getReadsInfile()
{
    QString fileName = ui->readsInfileLineEdit->text();
    return(fileName);
}

QString createProjectWidget::getAnnotationOutfile()
{
    QString fileName = ui->annotationOutfileLineEdit->text();
    return(fileName);
}

QString createProjectWidget::getReadsOutfile()
{
    QString fileName = ui->readsOutfileLineEdit->text();
    return(fileName);
}

QString createProjectWidget::getCountsOutfile()
{
    QString fileName = ui->countsOutfileLineEdit->text();
    return(fileName);
}

int createProjectWidget::getMinReads()
{
    int rval = ui->minReadsSpinBox->text().toInt();
    return(rval);
}

int createProjectWidget::getMaxDist()
{
    int rval = ui->maxDistSpinBox->text().toInt();
    return(rval);
}

int createProjectWidget::getMinBelowMaxDist()
{
    int rval = 0;
    if (ui->biasCheckBox->isChecked()) { rval = ui->minBelowMaxDistSpinBox->text().toInt(); }
    return(rval);
}

bool createProjectWidget::getStranded()
{
    bool rval = ui->strandedCheckBox->isChecked();
    return(rval);
}

bool createProjectWidget::getAntisense()
{
    bool rval = ui->antisenseCheckBox->isChecked();
    return(rval);
}

bool createProjectWidget::getMulti()
{
    bool rval = ui->multiCheckBox->isChecked();
    return(rval);
}

bool createProjectWidget::getRegion()
{
    bool rval = ui->regionCheckBox->isChecked();
    return(rval);
}

QString createProjectWidget::getRegionStartName()
{
    QString fileName = ui->regionStartNameLineEdit->text();
    return(fileName);
}

int createProjectWidget::getRegionStart()
{
    int rval = ui->regionStartSpinBox->text().toInt();
    return(rval);
}

QString createProjectWidget::getRegionEndName()
{
    QString fileName = ui->regionEndNameLineEdit->text();
    return(fileName);
}

int createProjectWidget::getRegionEnd()
{
    int rval = ui->regionEndSpinBox->text().toInt();
    return(rval);
}

int createProjectWidget::getIndexStepSize()
{
    int rval = ui->indexStepSizeSpinBox->text().toInt();
    return(rval);
}

int createProjectWidget::getBufferSizeBAM()
{
    int rval = ui->bufferSizeBamSpinBox->text().toInt();
    return(rval);
}

int createProjectWidget::getBufferSizeMAP()
{
    int rval = ui->bufferSizeMapSpinBox->text().toInt();
    return(rval);
}

int createProjectWidget::getBufferSizeOUT()
{
    int rval = ui->bufferSizeOutSpinBox->text().toInt();
    return(rval);
}

QSize createProjectWidget::sizeHint() const
{
    return(ui->groupBox->sizeHint());
}

//! the file saver slot
void createProjectWidget::saveProject()
{
    // open a saveFile dialog
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("select a project file"),
                                                    ".",
                                                    tr("p502 project files (*.xml)"));
    if (!fileName.isEmpty()) {
        // get the project name (equals to the filename minus the ending)
        QStringList fields = QFileInfo(fileName).fileName().split('.');
        QString projectName = fields.at(0);

        // an XML stream writer
        QXmlStreamWriter writer;
        // try to open the file and check for errors
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                      << ": " << qPrintable(file.errorString())
                      << std::endl;
        }
        else {
            writer.setDevice(&file);
            writer.setAutoFormatting(true);

            // write header
            writer.writeStartDocument();
            writer.writeStartElement("p502project");
            writer.writeAttribute("version", "1.0");

            // write elements
            writer.writeStartElement("name");
            writer.writeAttribute("entry", projectName);
            writer.writeStartElement("files");
            writer.writeAttribute("entry", "");
            {
                writer.writeStartElement("dataBaseInfile");
                writer.writeAttribute("entry", this->getAnnotationInfile());
                writer.writeEndElement();
                writer.writeStartElement("dataBaseOutfile");
                writer.writeAttribute("entry", this->getAnnotationOutfile());
                writer.writeEndElement();
                writer.writeStartElement("readsInfile");
                writer.writeAttribute("entry", this->getReadsInfile());
                writer.writeEndElement();
                writer.writeStartElement("readsOutfile");
                writer.writeAttribute("entry", this->getReadsOutfile());
                writer.writeEndElement();
                writer.writeStartElement("countTableFile");
                writer.writeAttribute("entry", this->getCountsOutfile());
                writer.writeEndElement();
            }
            writer.writeEndElement();
            writer.writeStartElement("parameters");
            writer.writeAttribute("entry", "");
            {
                writer.writeStartElement("multi");
                if (this->getMulti()) { writer.writeAttribute("entry", QString("true")); }
                else { writer.writeAttribute("entry", QString("false")); }
                writer.writeEndElement();
                writer.writeStartElement("stranded");
                if (this->getStranded()) {
                    if (this->getAntisense()) { writer.writeAttribute("entry", QString("antisense")); }
                    else { writer.writeAttribute("entry", QString("sense")); }
                }
                else { writer.writeAttribute("entry", QString("false")); }
                writer.writeEndElement();
                writer.writeStartElement("minReads");
                writer.writeAttribute("entry", QString::number(this->getMinReads()));
                writer.writeEndElement();
                writer.writeStartElement("maxDist");
                writer.writeAttribute("entry", QString::number(this->getMaxDist()));
                writer.writeEndElement();
                writer.writeStartElement("minBelowMaxDist");
                writer.writeAttribute("entry", QString::number(this->getMinBelowMaxDist()));
                writer.writeEndElement();
            }
            writer.writeEndElement();
            writer.writeStartElement("region");
            writer.writeAttribute("entry", "");
            {
                writer.writeStartElement("useRegion");
                if (this->getRegion()) { writer.writeAttribute("entry", QString("true")); }
                else { writer.writeAttribute("entry", QString("false")); }
                writer.writeEndElement();
                writer.writeStartElement("regionStartName");
                writer.writeAttribute("entry", this->getRegionStartName());
                writer.writeEndElement();
                writer.writeStartElement("regionStart");
                writer.writeAttribute("entry", QString::number(this->getRegionStart()));
                writer.writeEndElement();
                writer.writeStartElement("regionEndName");
                writer.writeAttribute("entry", this->getRegionEndName());
                writer.writeEndElement();
                writer.writeStartElement("regionEnd");
                writer.writeAttribute("entry", QString::number(this->getRegionEnd()));
                writer.writeEndElement();
            }
            writer.writeEndElement();
            writer.writeStartElement("settings");
            writer.writeAttribute("entry", "");
            {
                writer.writeStartElement("indexStepSize");
                writer.writeAttribute("entry", QString::number(this->getIndexStepSize()));
                writer.writeEndElement();
                writer.writeStartElement("bufferSizeBAM");
                writer.writeAttribute("entry", QString::number(this->getBufferSizeBAM()));
                writer.writeEndElement();
                writer.writeStartElement("bufferSizeMAP");
                writer.writeAttribute("entry", QString::number(this->getBufferSizeMAP()));
                writer.writeEndElement();
                writer.writeStartElement("bufferSizeOUT");
                writer.writeAttribute("entry", QString::number(this->getBufferSizeOUT()));
                writer.writeEndElement();
            }
            writer.writeEndElement();
            writer.writeEndElement();

            // end the document
            writer.writeEndElement();
            writer.writeEndDocument();

            // close the file and check for errors
            file.close();
            if (file.error() != QFile::NoError) {
                std::cerr << "Error: Cannot write file " << qPrintable(fileName)
                          << ": " << qPrintable(file.errorString())
                          << std::endl;
            }
        }
    }
    //! emit a signal that the file was written
    emit this->savedNewProject(fileName);
    //! close the application (hm - check if this does not create problems with the emitted signal)
    //! together with the other stuff, we hide it - then we also keep it if one creates several of them
    this->hide();
}




