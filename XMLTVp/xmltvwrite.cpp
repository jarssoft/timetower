#include "xmltvwrite.h"

xmltvp::xmltvwrite::xmltvwrite() : xmltvp::xmltvwrite::xmltvread()
{
    root = document.createElement(qtv);
    document.appendChild(root);
    assert(root.tagName() == qtv);
}

int xmltvp::xmltvwrite::size() const
{
    return root.elementsByTagName(qprogramme).size();
}

void xmltvp::xmltvwrite::addProgramme(xmltvp::xmltvprogramme prog)
{
    qSetGlobalQHashSeed(0); // set a fixed hash value
    const int s=size();

    QDomElement programme = document.createElement(qprogramme);

    programme.setAttribute("start", prog.start);
    if(prog.stop!=""){
        programme.setAttribute("stop", prog.stop);
    }
    programme.setAttribute("channel", prog.channel);

    QDomElement title = document.createElement("title");
    title.setAttribute(QString("lang"), "fi");
    title.appendChild(document.createTextNode(prog.title));
    programme.appendChild(title);

    if(prog.desc!=""){
        QDomElement desc = document.createElement("desc");
        desc.setAttribute(QString("lang"), "fi");
        desc.appendChild(document.createTextNode(prog.desc));
        programme.appendChild(desc);
    }


    for(QString c : prog.categories) {
        QDomElement category = document.createElement("category");
        category.setAttribute(QString("lang"), "fi");
        category.appendChild(document.createTextNode(c));
        programme.appendChild(category);
    }


    if(prog.url!=""){
        QDomElement url = document.createElement("url");
        url.appendChild(document.createTextNode(prog.url));
        programme.appendChild(url);
    }

    root.appendChild(programme);
    nodes = root.elementsByTagName(qprogramme);

    qSetGlobalQHashSeed(-1);
    assert(size()==s+1);
}

void xmltvp::xmltvwrite::addProgrammeVector(std::vector<xmltvp::xmltvprogramme> vector)
{
    for(unsigned int iv=0; iv<vector.size(); iv++){
        xmltvp::xmltvprogramme programme = vector.at(iv);
        this->addProgramme(programme);
    }
}

void xmltvp::xmltvwrite::save(QFile& file) const
{

    //assert(document.setContent(&file));
    assert(file.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream stream(&file);
    stream << document.toString(2);
    file.close();
    qDebug() << "Writing is done";

    //nodes = root.elementsByTagName(qprogramme);
}

QString xmltvp::xmltvwrite::toString() const
{
    return document.toString(2);
}
