#ifndef COMPARE_H
#define COMPARE_H

#include "programme.h"
#include "xmltvread.h"

namespace xmltvp {

//lähetykset ovat samoja
bool equals(const xmltvprogramme &p1, const xmltvprogramme &p2, const bool exact = true);

//ohjelman tulevat eri päivinä, mutta samaan aikaan päivästä
bool samaaika(const xmltvprogramme &p1, const xmltvprogramme &p2, const bool exact = true);

//palauttaa true, jos ohjelma alkaa ajalla start
bool aika(const QString &start, const xmltvprogramme &p2, const bool exact = true);

//palauttaa true, jos ohjelma alkaa ajalla start ja sen nimi on title
bool aikatitle(const xmltvprogramme &p2, const QString &start, const QString &title, const bool exact = true);

//ohjelman tulevat eri viikkoina, mutta samaan aikaan viikosta
bool samaviikkoaika(const xmltvprogramme &p1, const xmltvprogramme &p2, const bool exact = true);
bool viikkoaika(const QDateTime &dt, const xmltvprogramme &p2, const bool exact = true);

//p2 tulee välittömästi p1:n jälkeen
bool perakkain(const xmltvprogramme &p1, const xmltvprogramme &p2, const bool exact = true);

}

#endif // COMPARE_H
