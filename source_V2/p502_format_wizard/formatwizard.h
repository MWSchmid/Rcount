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

#ifndef FORMATWIZARD_H
#define FORMATWIZARD_H

#include <QWizard>

#ifndef SIMPLE_DATA_COLLECTION_H
#include "simple_data_collection.h"
#endif

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QRadioButton;
class QPushButton;
class QTreeView;
class QPlainTextEdit;
class QButtonGroup;
QT_END_NAMESPACE

//! [0]
class formatwizard : public QWizard
{
    Q_OBJECT

public:
    formatwizard(QWidget *parent = 0);
    void accept();
    simple_data_collection mydata;

private:
    int last_id;

signals:
    void enableTreeView(const QString &str);
    void enableOutfile();

private slots:
    void do_sth();
    void check_updated_priorities(const QString &str);
};
//! [0]



//! [1]
class InfilePage : public QWizardPage
{
    Q_OBJECT

public:
    InfilePage(QWidget *parent = 0);

//protected:
    //void initializePage();

private:
    QRegExpValidator *flankValidator;
    QLabel *labelLineEdit;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QLabel *labelLineEditFlank3;
    QLineEdit *lineEditFlank3;
    QPushButton *pushButtonFlank3;
    QLabel *labelLineEditFlank5;
    QLineEdit *lineEditFlank5;
    QPushButton *pushButtonFlank5;
    QLabel *labelComment;
    QLabel *labelOffset;
    QCheckBox *checkBoxOffset;
    QLabel *labelEnsembl;
    QCheckBox *checkBoxEnsembl;
    //QLabel *labelRadioButtons;
    //QRadioButton *radioButtonBED;
    //QRadioButton *radioButtonGTF;
    //QRadioButton *radioButtonGFF3;
    //QButtonGroup *radioButtonGroup;
    //QAbstractButton *selectedButton;

private slots:
    void search_infile();
};
//! [1]



//! [2]
class StructurePage : public QWizardPage
{
    Q_OBJECT

public:
    StructurePage(QWidget *parent = 0);

protected:
    //void initializePage();

private:
    QLabel *labelTreeView;
    QTreeView *treeView;
    QLabel *labelComment;
    //QPushButton *pushButton;
    QStringList headers;
    QString treestructure;

private slots:
    void update_structure();
    void create_tree(const QString &str);

signals:
    void structure_modified(const QString &str);
};
//! [2]



//! [3]
class OutfilePage : public QWizardPage
{
    Q_OBJECT

public:
    OutfilePage(QWidget *parent = 0);

//protected:
    //void initializePage();

private:
    QLabel *labelLineEdit;
    QLineEdit *lineEdit;
    QPushButton *pushButton;

private slots:
    void search_outfile();
};
//! [3]



//! [4]
class MessagePage : public QWizardPage
{
    Q_OBJECT

public:
    MessagePage(QWidget *parent = 0);

//protected:
    //void initializePage();

private:
    QLabel *labelPlainTextEdit;
    QPlainTextEdit *plainTextEdit;
};
//! [4]


#endif // FORMATWIZARD_H
