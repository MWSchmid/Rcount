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

#ifndef DATAANALYSISVIEW_H
#define DATAANALYSISVIEW_H

#include <QWidget>

#include "../../p502_SOURCE/dataAnalysis/dataanalysis.h"

class createProjectWidget;
class projectTreeView;
class resultTableView;
class analysisProgressView;

class dataAnalysisView : public QWidget
{
    Q_OBJECT

public:
    dataAnalysisView(QWidget *parent = 0);
    ~dataAnalysisView();

private:
    createProjectWidget* createProjectView;
    projectTreeView* projectView;
    resultTableView* resultView;
    analysisProgressView* progressView;
    dataAnalysis analyzer;

private slots:
    void createProject();
    void loadProject();
    void runDataAnalysis(QString fileName);
    void hideProgress();
};

#endif // DATAANALYSISVIEW_H
