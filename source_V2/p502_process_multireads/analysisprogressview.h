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

#ifndef ANALYSISPROGRESSVIEW_H
#define ANALYSISPROGRESSVIEW_H

#include <QWidget>

namespace Ui {
    class analysisProgressView;
}

class analysisProgressView : public QWidget
{
    Q_OBJECT

public:
    explicit analysisProgressView(QWidget *parent = 0);
    ~analysisProgressView();
    QSize sizeHint() const;

private:
    Ui::analysisProgressView *ui;
    int progress;
    QString status;

public slots:
    void updateProgress(int progressIN);
    void updateStatus(QString statusIN);

signals:
    void analysisCanceled();
};




#endif // ANALYSISPROGRESSVIEW_H
