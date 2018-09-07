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

#ifndef PROCESSMULTIREADSVIEW_H
#define PROCESSMULTIREADSVIEW_H

#include <QWidget>

namespace Ui {
    class processMultireadsView;
}

class processMultireadsView : public QWidget
{
    Q_OBJECT

public:
    explicit processMultireadsView(QWidget *parent = 0);
    ~processMultireadsView();
    QSize sizeHint() const;

private:
    Ui::processMultireadsView *ui;
    QString readsInfile;
    QString readsOutfile;
    QString doWeight;
    QString allocationDistance;

public slots:
    void hideButtonBox();
    void showButtonBox();
    void removeFromQueue(QString workString);

private slots:
    void searchReadsInfile();
    void searchReadsOutfile();
    void sendProcessReads();
    void sendClose();

signals:
    void processReads(QString fileNames);
    void closeAll();

};

#endif // PROCESSMULTIREADSVIEW_H


