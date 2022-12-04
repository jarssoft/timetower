#ifndef KSTASO_H
#define KSTASO_H

#include <vector>
#include <kuutiosuhde.h>
#include <vector3d.h>

class KSTaso
{

public:

    KSTaso(vector2d<Kuutiosuhde*> pisteet, int suunta);

    KSTaso(int x, int y, int suunta);

    void setKuutio(int x, int y, Kuutiosuhde *kuutio);

    Kuutiosuhde* getKuutio(int x, int y);

    int getSizeX();

    int getSizeY();

    int getSuunta();

    int countValues();

    double getValuePortion();

    vector2d<Kuutiosuhde*>& getPisteet();

    KuutioTyyppi getTyyppi() const;
    void setTyyppi(const KuutioTyyppi &value);

    vector2d<Neliosuhde> getNelioverkko(bool index);

private:
    int suunta;
    std::vector<std::vector<Kuutiosuhde*>> pisteet;

    KuutioTyyppi tyyppi;

};

#endif // KSTASO_H
