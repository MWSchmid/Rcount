#include <QtCore>

#include "fastahandler.h"

fastaHandler::fastaHandler()
{
    // does not do anything
}

bool fastaHandler::readSequences(const QString &fileName)
{
    bool rval = false;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        rval = false;
    }
    else {
        QTextStream in(&file);
        in.setCodec("UTF-8");

        QString curline;
        QString header;
        QString seq;
        bool firstline = true;

        while(!in.atEnd()) {
            curline = in.readLine();
            if (curline.at(0) == '>') {
                if (firstline) {
                    firstline = false;
                }
                else {
                    this->sequences.insert(header, seq);
                }
                curline.remove(">");
                header = curline.trimmed();
                header.replace(QRegExp("\\s"), "_"); //! NEW 23. FEB 2012
                seq.clear();
            }
            else {
                seq += curline.trimmed();
            }
        }
        this->sequences.insert(header, seq);

        file.close();
        if (file.error() != QFile::NoError) {
            std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                      << ": " << qPrintable(file.errorString())
                      << std::endl;
            rval = false;
        }
        else { rval = true; }

    }

    return(rval);
}

bool fastaHandler::writeSequences(const QString &fileName)
{
    bool rval = false;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        rval = false;
    }
    else {
        QTextStream out(&file);
        out.setCodec("UTF-8");

        // File writing
        QMap<QString,QString>::const_iterator iter = this->sequences.constBegin();
        while (iter != this->sequences.constEnd()) {
            out << '>' << iter.key() << '\n';
            //out << iter.value() << endl; //! or split the long strings
            QString seq = QString(iter.value());
            int i = 120;
            while ( i < seq.count() ) {
                out << seq.mid((i-120),120) << '\n';
                i += 120;
            }
            out << seq.mid((i-120), seq.count()) << '\n';
            ++iter;
        }
        out.flush();

        file.close();
        if (file.error() != QFile::NoError) {
            std::cerr << "Error: Cannot write file " << qPrintable(fileName)
                      << ": " << qPrintable(file.errorString())
                      << std::endl;
            rval = false;
        }
        else { rval = true; }
    }
    return(rval);
}





