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

#ifndef FORMATWIZARD_H
#include "formatwizard.h"
#endif

#ifndef SIMPLE_DATA_COLLECTION_H
#include "simple_data_collection.h"
#endif

#ifndef SIMPLE_TREE_MODEL_H
#include "simple_tree_model.h"
#endif

//! [0]
formatwizard::formatwizard(QWidget *parent)
    : QWizard(parent)
{
    addPage(new InfilePage);
    addPage(new StructurePage);
    addPage(new OutfilePage);
    addPage(new MessagePage);

    setWindowTitle(tr("Rcount-format"));

    last_id = 0;

    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(do_sth()));
    // set connection to the treeviewpage
    connect(this, SIGNAL(enableTreeView(QString)), this->page(1), SLOT(create_tree(QString)));
    // set connection to the treeviewpage - react on an updated structure
    connect(this, SIGNAL(enableOutfile()), this->page(1), SLOT(update_structure()));
    connect(this->page(1), SIGNAL(structure_modified(QString)), this, SLOT(check_updated_priorities(QString)));
}

void formatwizard::accept(){
    //QMessageBox::warning(0, QObject::tr("Format Wizard"),
      //QObject::tr("testmessage:\n%1").arg(field("lineEditInfile").toString()));
    QDialog::accept();
}

//! TODO TODO: enable back and forth
void formatwizard::do_sth()
{
    bool rval = false;
    QString errMess;
    int cur_id = currentId();
    if (cur_id > last_id) {
        if (last_id == 0) {
            errMess = "could not load the annotation";
            QString filename = field("lineEditInfile").toString();
            QString filetype;
            uint three_prime = field("lineEditFlankSize3").toUInt();
            uint five_prime = field("lineEditFlankSize5").toUInt();
            bool useZeroBased = field("checkBoxOffset").toBool();
            bool useEnsemblLoader = field("checkBoxEnsembl").toBool();
            if ( filename.contains("bed", Qt::CaseInsensitive) ) { filetype = "BED"; useZeroBased = false; } // it is per default zero-based
            else if ( filename.contains("gtf", Qt::CaseInsensitive) ) { filetype = "GTF"; }
            else if ( filename.contains("gff", Qt::CaseInsensitive) ) { filetype = "GFF3"; }
            else { QMessageBox::warning(0, QObject::tr("Format Wizard"), QObject::tr("this should not happen!")); }
            mydata.set_zeroBased(useZeroBased);
            mydata.set_filename(filename);
            mydata.set_filetype(filetype);
            mydata.set_3_prime(three_prime);
            mydata.set_5_prime(five_prime);
            mydata.set_ensembl_loader(useEnsemblLoader);
            QApplication::setOverrideCursor(Qt::WaitCursor);
            rval = mydata.load_data();
            QApplication::restoreOverrideCursor();
            emit enableTreeView(mydata.get_structure());
        } else if (last_id == 1) {
            emit enableOutfile(); //! THIS NEEDS TO BE HERE BEFORE THE mydata.update_priorities
            errMess = "could not update priorities";
            QApplication::setOverrideCursor(Qt::WaitCursor);
            rval = mydata.update_priorities();
            QApplication::restoreOverrideCursor();
        } else if (last_id == 2) {
            errMess = "could not write the outfile";
            QString outfilename = field("lineEditOutfile").toString();
            QApplication::setOverrideCursor(Qt::WaitCursor);
            rval = mydata.write_xml_V2(outfilename);
            QApplication::restoreOverrideCursor();
        } else if (last_id == 3) {
            errMess = "could not exit - this is a very strange error";
            rval = true;
        } else {
            errMess = "this page does not exist";
            rval = false;
        }
        if (!rval) {
            QMessageBox::warning(0, QObject::tr("Format Wizard"), errMess);
        }
    }
    last_id = currentId();
}

void formatwizard::check_updated_priorities(const QString &str){
    bool rval;
    rval = mydata.update_treestructure(str);
}

//! [0]



//! [1]
InfilePage::InfilePage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Select Infile"));

    // initialize all the members
    // a validator and the infile searches
    flankValidator = new QRegExpValidator(QRegExp("\\d+"), this);
    labelLineEdit = new QLabel(tr("Select an annotation file:"));
    lineEdit = new QLineEdit;
    labelLineEdit->setBuddy(lineEdit);
    pushButton = new QPushButton(tr("&search file"));
    // the comment for flanking regions
    labelComment = new QLabel(tr("Optional: Add flanking regions by specifying the size in bp:"));
    labelComment->setWordWrap(true);
    // the 3 prime flanking regions
    labelLineEditFlank3 = new QLabel(tr("3-prime flanking regions:"));
    lineEditFlank3 = new QLineEdit;
    lineEditFlank3->setValidator(flankValidator);
    labelLineEditFlank3->setBuddy(lineEditFlank3);
    lineEditFlank3->setText("0");
    // the 5 prime flanking regions
    labelLineEditFlank5 = new QLabel(tr("5-prime flanking regions:"));
    lineEditFlank5 = new QLineEdit;
    labelLineEditFlank5->setBuddy(lineEditFlank5);
    lineEditFlank5->setValidator(flankValidator);
    lineEditFlank5->setText("0");
    // the comment for the 0-1 based
    labelOffset = new QLabel(tr("Uncheck the box if the annotation is already 0-based (see User Guide):"));
    labelOffset->setWordWrap(true);
    checkBoxOffset = new QCheckBox;
    labelOffset->setBuddy(checkBoxOffset);
    checkBoxOffset->setChecked(true);
    // the use of ensembl types or generic types
    labelEnsembl = new QLabel(tr("Check the box if the GFF/GTF contains ENSEMBL feature types (see User Guide):"));
    labelEnsembl->setWordWrap(true);
    checkBoxEnsembl = new QCheckBox;
    labelEnsembl->setBuddy(checkBoxEnsembl);
    //labelRadioButtons = new QLabel(tr("Select the format:"));
    //radioButtonBED = new QRadioButton(tr("BED"));
    //radioButtonGTF = new QRadioButton(tr("GTF"));
    //radioButtonGFF3 = new QRadioButton(tr("GFF3"));
    //radioButtonGroup = new QButtonGroup;
    //radioButtonGroup->addButton(radioButtonBED);
    //radioButtonGroup->addButton(radioButtonGTF);
    //radioButtonGroup->addButton(radioButtonGFF3);
    //radioButtonGroup->setExclusive(true);

    // set page internal connections
    connect(pushButton, SIGNAL(clicked()), this, SLOT(search_infile()));

    // register fields
    registerField("lineEditInfile*", lineEdit);
    registerField("lineEditFlankSize3", lineEditFlank3);
    registerField("lineEditFlankSize5", lineEditFlank5);
    registerField("checkBoxOffset", checkBoxOffset);
    registerField("checkBoxEnsembl", checkBoxEnsembl);

    // set layout
    QGridLayout *layout = new QGridLayout;
    layout->setColumnMinimumWidth(0,30);
    layout->setColumnMinimumWidth(1,10);
    layout->addWidget(labelLineEdit, 0, 0, 1, 2);
    layout->addWidget(lineEdit, 1, 0);
    layout->addWidget(pushButton, 1, 1);
    layout->addWidget(labelComment, 2, 0, 2, 2);
    layout->addWidget(labelLineEditFlank3, 4, 0);
    layout->addWidget(lineEditFlank3, 4, 1);
    layout->addWidget(labelLineEditFlank5, 5, 0);
    layout->addWidget(lineEditFlank5, 5, 1);
    layout->addWidget(labelOffset, 6, 0, 2, 2);
    layout->addWidget(checkBoxOffset, 8, 0);
    layout->addWidget(labelEnsembl, 9, 0, 2, 2);
    layout->addWidget(checkBoxEnsembl, 11, 0);
    //layout->addWidget(labelRadioButtons, 2, 0, 1, 2);
    //layout->addWidget(radioButtonBED, 3, 0, 1, 2);
    //layout->addWidget(radioButtonGTF, 4, 0, 1, 2);
    //layout->addWidget(radioButtonGFF3, 5, 0, 1, 2);
    setLayout(layout);
}

void InfilePage::search_infile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("select the annotation file"),
                                                    ".",
                                                    tr("GFF3 files (*.gff);;GTF files (*.gtf);;BED files (*.bed)"));
    if (!fileName.isEmpty()) { lineEdit->setText(fileName); }
}

//! [1]



//! [2]
StructurePage::StructurePage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Annotation Structure"));

    // initialize all the members
    labelTreeView = new QLabel(tr("The file implies this structure within the annotation"));
    //pushButton = new QPushButton(tr("&save changes"));
    treeView = new QTreeView;
    labelComment = new QLabel(tr("The structure was inferred using the parent-to-child relations given in the file. You should now edit the priority values. For each node level, the mapping will prefer nodes with the highest priority. Values should be integers with 1 for the highest priority. Setting a priority of 0 will remove the feature completely. NOTE: splice is equal to, and will be saved as 'intron'."));
    labelComment->setWordWrap(true);
    headers << tr("genomic feature (occurences)") << tr("priority");
    treestructure = "";

    // set page internal connections
    //connect(pushButton, SIGNAL(clicked()), this, SLOT(update_structure()));

    // set layout
    QGridLayout *layout = new QGridLayout;
    layout->setColumnMinimumWidth(0,30);
    layout->setColumnMinimumWidth(1,10);
    layout->addWidget(labelTreeView, 0, 0);
    //layout->addWidget(pushButton, 0, 1);
    layout->addWidget(treeView, 1, 0, 1, 2);
    layout->addWidget(labelComment, 2, 0, 1, 2);
    setLayout(layout);
}

//void StructurePage::initializePage() { }

void StructurePage::create_tree(const QString &str)
{
    treestructure = str;
    //QMessageBox::warning(0, QObject::tr("Structure Page"), treestructure);
    if (treestructure.length() > 10) {
        simple_tree_model *model = new simple_tree_model(headers, treestructure);
        treeView->setModel(model);
        for (int column = 0; column < model->columnCount(); ++column) { treeView->resizeColumnToContents(column); }
    } else {
        QMessageBox::warning(0, QObject::tr("Structure Page"), QObject::tr("the structure is empty"));
    }
}

void StructurePage::update_structure()
{
    simple_tree_model *model = (simple_tree_model*)(treeView->model());
    treestructure = model->save_treestructure();
    //QMessageBox::warning(0, QObject::tr("Structure Page"), treestructure);
    emit structure_modified(treestructure);
}
//! [2]


//! [3]
OutfilePage::OutfilePage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Select Outfile"));

    // initialize all the members
    labelLineEdit = new QLabel(tr("select a file to store the reformatted annotation:"));
    lineEdit = new QLineEdit;
    labelLineEdit->setBuddy(lineEdit);
    pushButton = new QPushButton(tr("&search file"));

    // set page internal connections
    connect(pushButton, SIGNAL(clicked()), this, SLOT(search_outfile()));

    // register fields
    registerField("lineEditOutfile*", lineEdit);

    // set layout
    QGridLayout *layout = new QGridLayout;
    layout->setColumnMinimumWidth(0,30);
    layout->setColumnMinimumWidth(1,10);
    layout->addWidget(labelLineEdit, 0, 0, 1, 2);
    layout->addWidget(lineEdit, 1, 0);
    layout->addWidget(pushButton, 1, 1);
    setLayout(layout);
}

//void OutfilePage::initializePage(){}

void OutfilePage::search_outfile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("save the new annotation file"),
                                                    ".",
                                                    tr("Rcount-XML files (*.xml)"));
    if (!fileName.isEmpty()) { lineEdit->setText(fileName); }
}

//! [3]



//! [4]
MessagePage::MessagePage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Messages"));

    // initialize all the members
    labelPlainTextEdit = new QLabel(tr("messages during processing:"));
    plainTextEdit = new QPlainTextEdit(tr("There are no messages to display.\nReport errors to:\nschmid.m@access.uzh.ch"));
    labelPlainTextEdit->setBuddy(plainTextEdit);

    // set page internal connections

    // register fields
    registerField("plainTextEditMessages", plainTextEdit);

    // set layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(labelPlainTextEdit);
    layout->addWidget(plainTextEdit);
    setLayout(layout);
}

//void MessagePage::initializePage(){}
//! [4]

