#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include <QString>
#include <QTime>

/*
Sääntö ohjelmakartan ryhdistämiseksi.
Säännöllä voidaan uudelleen nimetä tai värittää
vakiintuneita ohjelmaryhmiä tai -paikkoja.
*/
class filter
{
public:
    filter();
private:
    QString groupname;
    std::vector<QString> nimet;
    bool exactnames;
    std::vector<QString> kategoriat;
    QTime before;
    QTime after;
};

#endif // FILTER_H
