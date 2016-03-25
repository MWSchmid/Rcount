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

#include <QtWidgets/QApplication>
#include "formatwizard.h"

int main(int argc, char *argv[])
{
    QString path = QString(argv[0]);
    QString end = path.split("/").last();
    path.remove(path.length() - end.length(), end.length());
    QDir dir(path);
    QApplication::addLibraryPath(dir.absolutePath());
    //QStringList libPaths = QApplication::libraryPaths();
    //std::cerr << libPaths.join("; ").toStdString() << std::endl << std::flush;

    QApplication a(argc, argv);
    formatwizard w;
    w.show();

    return a.exec();
}
