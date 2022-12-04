#ifndef XMLTVWRITE_H
#define XMLTVWRITE_H

#include "names.h"
#include "programme.h"
#include <QtXml>
#include <QFile>
#include "xmltvread.h"

namespace xmltvp {

class xmltvwrite : public xmltvread
{
public:

    xmltvwrite();
    void addProgramme(xmltvprogramme prog);
    void addProgrammeVector(std::vector<xmltvprogramme>);
    int size() const;
    void save(QFile& file) const;
    QString toString() const;

private:

    QDomElement root;
};

}

#endif // XMLTVWRITE_H

