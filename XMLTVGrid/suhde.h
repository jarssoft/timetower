#ifndef SUHDE_H
#define SUHDE_H

#include <string>
#include "ohjelmapaikka.h"
#include <vector>

#define SAMA_ESITYS 1
#define SAMA_KUVAUS 2
#define SAMA_JAKSO  4
#define PER_JAKSO   8

#define SAMA_EDELT  16
#define SAMA_SEURA  32
#define PER_NIMI    64
#define PER_OHJ     128

#define PER_VIIKKO  256
#define PER_ARKI    512
#define PER_PAIVA   1024
#define SAMA_VIIKKO 2048
#define SAMA_PAIVA  4096
#define SAMA_VP     8192

#define SAMA_AIKA_N 16384
#define SAMA_AIKA   32768



std::string tyyppi_to_string(int tyyppi);

struct Ohjelma {
    Ohjelmapaikka paikka;
    tm aika;    
    int episode;
    int esityskerta;
    std::string kuvaus;
};

class Suhde
{
public:
    Suhde(const int p1, const int p2, int const tyyppi);

    int p1;
    int p2;
    int tyyppi;

    bool operator ==(const Suhde &b) const;

    std::string toString();
};

//tyypit voi yhdistää, jos molemmissa ulottuvuuksissa on yksilöllisiä tyyppibittejä
bool voiYhdistaa(const int x1, const int y1, const int x2, const int y2);

int teetyyppi(const Ohjelma ox, const Ohjelma oy, const bool perakkaiset, const bool nimiperakkaiset);

#endif // SUHDE_H
