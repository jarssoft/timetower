#ifndef KUUTIOSUHDE_H
#define KUUTIOSUHDE_H

#include <neliosuhde.h>
#include <iostream>

#define SUUNTAX  0
#define SUUNTAY  1
#define SUUNTAZ  2
#define VASTAAN  3

struct KuutioTyyppi{
    int tyyppix;
    int tyyppiy;
    int tyyppiz;

    long rank() const;

    KuutioTyyppi yhdista(const KuutioTyyppi& tyyppi) const;

    std::string toString() const;

    //palauttaa ulottuvuuden yksilöllisen tyypin
    //int getOnlyX();
    //int getOnlyY();
    //int getOnlyZ();
};

class Kuutiosuhde
{

public:

    Kuutiosuhde(Neliosuhde *top, Neliosuhde *bottom, Neliosuhde *left, Neliosuhde *right, Neliosuhde *front, Neliosuhde *behind);

    int getPystyTyyppi() const;
    int getVaakaTyyppi() const;    
    int getSyvyysTyyppi() const;

    KuutioTyyppi getKuutioTyyppi() const;

    //palauttaa
    //int getP(int x, int y, int z) const;

    //int getWidth() const;
    //int getHeight() const;
    //int getDepth() const;

    //int countP() const;

    bool sovita(const Kuutiosuhde *kuutio, int suunta) const;

    static void etsiKuutiot(std::vector<Neliosuhde> &neliot, std::vector<Kuutiosuhde> &kuutiot);

    Neliosuhde *front;
    Neliosuhde *behind;
    Neliosuhde *top;
    Neliosuhde *bottom;
    Neliosuhde *left;
    Neliosuhde *right;

    std::vector<int> getAllP();

};

#endif // KUUTIOSUHDE_H
