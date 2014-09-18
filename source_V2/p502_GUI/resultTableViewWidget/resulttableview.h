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

#ifndef RESULTTABLEVIEW_H
#define RESULTTABLEVIEW_H

#include <QWidget>

#include "resulttableviewmodel.h"

//! add and remove Counts slots shall be connected externally

namespace Ui {
    class resultTableView;
}

class resultTableView : public QWidget
{
    Q_OBJECT

public:
    explicit resultTableView(QWidget *parent = 0);
    ~resultTableView();
    QSize sizeHint() const;

private:
    Ui::resultTableView *ui;
    QString curTable;
    QMap<QString, resultTableViewModel*> projectToModel; // well, in fact it is countsOutfile to the index of the list where the pointer to the tableModel is stored
private slots:
    void findRow();
public slots:
    // add and display are very similar. Display will only obdate the view if the new table is not already shown, update will display only if the new table is already shown
    void displayCounts(QString fileName);
    void updateCounts(QString fileName);
    void removeCounts(QString fileName);
};

#endif // RESULTTABLEVIEW_H
