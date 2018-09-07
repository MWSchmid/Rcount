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

#ifndef PROCESSMULTIREADS_H
#define PROCESSMULTIREADS_H

#include <QWidget>
#include "multireadsprocessor.h"

class analysisProgressView;
class processMultireadsView;

class processMultireads : public QWidget
{
    Q_OBJECT
    
public:
    processMultireads(QWidget *parent = 0);
    ~processMultireads();

private:
    analysisProgressView* progressView;
    processMultireadsView* multireadsView;
    multireadsProcessor processor;

private slots:
    void runProcessor(QString toProcess);
    void hideProgress();

};

#endif // PROCESSMULTIREADS_H
