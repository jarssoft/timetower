#ifndef NELIOSUHDE_H
#define NELIOSUHDE_H

#include <suhde.h>
#include "suhde2d.h"
#include <vector>


struct NelioTyyppi{
    int tyyppix;
    int tyyppiy;

    long rank();

    //palauttaa ulottuvuuden yksilöllisen tyypin
    int getOnlyVaaka();
    int getOnlyPysty();
};


/* yksinkertaistettu, yksisilmäinen verkko */

class Neliosuhde : public Suhde2D
{
public:
    Neliosuhde(Suhde* x1, Suhde* y1, Suhde* y2, Suhde* x2);

    int getVaakaTyyppi() const;

    int getPystyTyyppi() const;

    NelioTyyppi getNelioTyyppi() const;

    //palauttaa esimerkin joltain olevassa olevalta riviltä
    int getRivilta(int i) const;

    //palauttaa esimerkin joltain olevassa olevalta sarakkeelta
    int getSarakkeelta(int i) const;

    int getP(int x, int y) const;

    int getWidth() const;

    int getHeight() const;

    int countP() const;

    bool operator ==(const Neliosuhde &b) const;

    bool fitToRight(const Neliosuhde &b) const;

    bool fitToBottom(const Neliosuhde &b) const;

    static void etsiNeliot(std::vector<Suhde> &suhteet, std::vector<Neliosuhde> &neliot);

    Suhde *getX1() const;

    Suhde *getX2() const;

    Suhde *getY1() const;

    Suhde *getY2() const;

    std::vector<int> getAllP();

private:

    Suhde* x1;
    Suhde* x2;
    Suhde* y1;
    Suhde* y2;

};

bool isGreater(NelioTyyppi nt1, NelioTyyppi nt2);

#endif // NELIOSUHDE_H
