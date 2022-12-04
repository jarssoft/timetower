
#include "xmltvread.h"
#include <assert.h>


unsigned int xmltvp::xmltvread::size() const
{
    return nodes.size();
}

xmltvp::xmltvprogramme xmltvp::xmltvread::at(int i) const
{
    assert(i>=0);
    assert(i<nodes.size());

    const QDomElement programme = nodes.at(i).toElement();
    xmltvprogramme prog(programme.attribute("channel"), "Ohjelma",
                        programme.attribute("start"), programme.attribute("stop"));

    const QDomNodeList pnodes = nodes.at(i).childNodes();

    for (int x = 0; x < pnodes.count(); x++)
    {
        QDomElement node = pnodes.at(x).toElement();
        if(node.tagName()=="title"){
            prog.title=node.text();
        }
        if(node.tagName()=="desc"){
            prog.desc=node.text();
        }

        if(node.tagName()=="category"){
            prog.categories.push_back(node.text());
        }

        if(node.tagName()=="url"){
            prog.url=node.text();
        }
    }

    assert(prog.title!="");

    return prog;
}

std::vector<xmltvp::xmltvprogramme> xmltvp::xmltvread::toProgrammeVector() const
{
    std::vector<xmltvp::xmltvprogramme> programmes;

    for (unsigned int i = 0; i < this->size(); i++){
        programmes.push_back(this->at(i));
    }

    return programmes;

}

void xmltvp::xmltvread::load(QFile &file)
{
    assert(file.exists() == true);
    assert(file.open(QIODevice::ReadOnly | QIODevice::Text) == true);
    assert(document.setContent(&file) == true);

    QDomElement root = document.firstChildElement();
    assert(root.tagName() == qtv);

    nodes = root.elementsByTagName(qprogramme);
}
