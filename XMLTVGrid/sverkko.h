#ifndef SVERKKO_H
#define SVERKKO_H

#include <vector>
#include <suhde.h>

class SVerkko
{
public:
    SVerkko();
    void laajenna(std::vector<Suhde> uudet);
    int size() const;

    int getTyyppi() const;

    int getP(int i) const;

private:
    std::vector<Suhde> suhteet;
    int tyyppi;
};

#endif // SVERKKO_H
