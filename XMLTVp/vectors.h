#ifndef VECTORS_H
#define VECTORS_H

#include <vector>
#include "compare.h"

namespace xmltvp {

//int erot(const xmltvprogramme p1, const xmltvprogramme p2);

//etsii vektorista ohjelman sen alkamisajan ja nimen mukaan
int findaikatitle(const std::vector<xmltvprogramme> &vector, const QString &startime, const QString &title, const bool exact=true);

//etsii peräkkäiset lähetykset, ja palauttaa taulukon joka viittaa seuraavaan.
//Jos seuraavaa ei ole, solun arvo on -1.
std::vector<int> jatkumo(const std::vector<xmltvprogramme> &vector, const bool exact=true);

std::vector<int> matches(const xmltvread &read, const std::vector<xmltvprogramme> &vector, const bool exact=true);

std::vector<int> nonMatches(const xmltvread &read, const std::vector<int> &found);

unsigned int matchesCount(const std::vector<int> &found);

bool allEquals(const xmltvread &read, const std::vector<xmltvprogramme> &vector);

void replaceURL(const xmltvread &read, std::vector<xmltvprogramme> &vector, const std::vector<int> &found);

}

#endif // VECTORS_H
