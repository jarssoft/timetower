#ifndef ACCESS2_H
#define ACCESS2_H

#include "names.h"
#include "programme.h"
#include <QtXml>
#include <QFile>
#include <vector>

namespace xmltvp {

class xmltvread
{
public:

    unsigned int size() const;
    xmltvprogramme at(int i) const;
    std::vector<xmltvprogramme> toProgrammeVector() const;
    void load(QFile &file);

protected:

    QDomDocument document;
    QDomNodeList nodes;

};

}
#endif // ACCESS2_H
