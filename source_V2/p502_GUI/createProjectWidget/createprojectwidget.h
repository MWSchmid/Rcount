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

#ifndef CREATEPROJECTWIDGET_H
#define CREATEPROJECTWIDGET_H

#include <QWidget>

//! this creator is in principle ready to use -> see the main.cpp for this
//! TODO: in the parent application, you may connect a slot to the savedNewProject signal

namespace Ui {
    class createProjectWidget;
}

class createProjectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit createProjectWidget(QWidget *parent = 0);
    ~createProjectWidget();

    // accessors (to fill the data analysis class)
    // from the files tab
    QString getAnnotationInfile();
    QString getReadsInfile();
    QString getAnnotationOutfile();
    QString getReadsOutfile();
    QString getCountsOutfile();
    int getMinReads();
    int getMaxDist();
    int getMinBelowMaxDist();
    bool getStranded();
    bool getMulti();
    bool getRegion();
    QString getRegionStartName();
    int getRegionStart();
    QString getRegionEndName();
    int getRegionEnd();
    int getIndexStepSize();
    int getBufferSizeBAM();
    int getBufferSizeMAP();
    int getBufferSizeOUT();
    QSize sizeHint() const;


private:
    Ui::createProjectWidget *ui;

private slots:
    void searchAnnotationInfile();
    void searchReadsInfile();
    void searchAnnotationOutfile();
    void searchReadsOutfile();
    void searchCountsOutfile();
    void saveProject();

signals:
    void savedNewProject(QString fileName);
};

#endif // CREATEPROJECTWIDGET_H
