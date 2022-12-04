#ifndef SKVERKKO_H
#define SKVERKKO_H

#include <vector>
#include <suhde.h>
#include <neliosuhde.h>
#include <vector3d.h>

class NSVerkko : public Suhde2D
{
public:
    NSVerkko(Neliosuhde nelio);
    NSVerkko(std::vector<std::vector<Neliosuhde> > nelio);

    //yrittää lisätä verkkoon neliöitä
    void laajenna(const std::vector<Neliosuhde> &neliot);

    int getVaakaTyyppi() const;

    int getPystyTyyppi() const;

    //palauttaa esimerkin joltain olevassa olevalta riviltä
    int getRivilta(int i) const;

    //palauttaa esimerkin joltain olevassa olevalta sarakkeelta
    int getSarakkeelta(int i) const;

    int getP(const int x, const int y) const;

    int getWidth() const;

    int getHeight() const;

    int countP() const;

    //palauttaa true, jos tämä neliöverkko on osa argumentin verkkoa
    bool onOsa(const NSVerkko &verkko);

    NelioTyyppi getNelioTyyppi() const;

private:
    std::vector <std::vector <Neliosuhde> > suhteet;
    int vaakatyyppi;
    int pystytyyppi;

    //laajentaa rivin niin pitkälle kuin mahdollista
    void laajennaRivi(const std::vector<Neliosuhde> &lisattavat);

    NelioTyyppi uusiRivi(const std::vector<Neliosuhde> &lisattavat);
    void uusiRiviToteuta(NelioTyyppi nt);
    NelioTyyppi uusiSarake(const std::vector<Neliosuhde> &lisattavat);
    void uusiSarakeToteuta(NelioTyyppi nt);

    bool contains(int p) const;

    std::vector<Neliosuhde> uusirivi;
    std::vector<Neliosuhde> uusisarake;
};

#endif // SKVERKKO_H
