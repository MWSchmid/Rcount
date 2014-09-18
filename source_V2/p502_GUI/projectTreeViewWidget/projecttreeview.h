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

#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QWidget>
#include <QModelIndex>

//! all the pushButtons should be linked externally
//! the newPushButton shall be externally linked to the createProjectWidget
//! the addProject slot shall be externally linked to the okPushButton of the createProjectWidget
//! the runPushButton shall be externally linked to...

class projectTreeViewItem;
class projectTreeViewModel;

namespace Ui {
    class projectTreeView;
}

class projectTreeView : public QWidget
{
    Q_OBJECT

public:
    explicit projectTreeView(QWidget *parent = 0);
    ~projectTreeView();
    QSize sizeHint() const;

private:
    Ui::projectTreeView *ui;
    projectTreeViewModel *model;

private slots:
    void switchedProject(QModelIndex index);
    void resizeColumns();
    void runClicked();
    void runAllClicked();

public slots:
    void closeCurrentProject();
    void addProject(QString fileName);
    void addStats(QString statsString); // the statsString comes from the QtBamStats in the bamhelpers.h file - the projectName is added in the dataAnalysis slot

signals:
    void newClicked();
    void openClicked();
    void runClickedFile(QString);
    void closedProject(QString); // the count file name
    void currentProject(QString); // the count file name
};

#endif // PROJECTTREEVIEW_H
