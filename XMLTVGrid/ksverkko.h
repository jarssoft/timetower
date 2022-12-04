#ifndef KSVERKKO_H
#define KSVERKKO_H

#include <vector>
#include <neliosuhde.h>
#include <kuutiosuhde.h>
#include <kstaso.h>
#include <vector3d.h>
#include <kstaso.h>

class KSVerkko
{
public:

    KSVerkko(Kuutiosuhde *kuutio);

    //yrittää lisätä verkkoon neliöitä
    bool laajenna(std::vector<Kuutiosuhde> *kuutiot);

    int getSizeX() const;
    int getSizeY() const;
    int getSizeZ() const;
    int getSizeShape() const;

    KuutioTyyppi getTyyppi() const;
    void setTyyppi(const KuutioTyyppi &value);

    //irrottaa tason
    KSTaso getTaso(int suunta, int index);
    KSTaso getTasoFromBottom();
    KSTaso getTasoFromRight();
    KSTaso getTasoFromBehind();

private:

    void addTasoToBottom(KSTaso taso);
    void addTasoToRight(KSTaso taso);
    void addTasoToBehind(KSTaso taso);

    void addRowToBottom();
    void addRowToRight();
    void addRowToBehind();

    Kuutiosuhde* getKuutio(int x, int y, int z);

    std::vector<std::vector<std::vector<Kuutiosuhde*>>> suhteet;

    /*
    Kuutiosuhde* top;
    Kuutiosuhde* bottom;
    Kuutiosuhde* left;
    Kuutiosuhde* right;
    Kuutiosuhde* front;
    Kuutiosuhde* behind;
    */

    //int tyypit[3];
    KuutioTyyppi tyyppi;

};

#endif // KSVERKKO_H
