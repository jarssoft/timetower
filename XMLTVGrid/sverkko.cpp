#include "sverkko.h"
#include <assert.h>

SVerkko::SVerkko(){}

void SVerkko::laajenna(std::vector<Suhde> uudet)
{
    //aloitaan ensimmäisestä parhaasta suhteesta
    Suhde parassuhde=uudet.at(0);
    {
        for(int i=0; i<uudet.size(); i++){
            if(uudet.at(i).tyyppi > parassuhde.tyyppi){
                //parassuhde=uudet.at(i);
            }
        }
    }
    suhteet.push_back(parassuhde);
    tyyppi=parassuhde.tyyppi;

    assert(!((tyyppi & SAMA_PAIVA) && (tyyppi & PER_PAIVA)));

    for(int i=0; i<uudet.size(); i++){

        Suhde uusi = uudet.at(i);

        //if(suhteet.back().p2 == uusi.p1 && (suhteet.back().tyyppi & tyyppi)){
        //if((suhteet.back().p2 == uusi.p1) && ((uusi.tyyppi & tyyppi) >= SAMA_AIKA+SAMA_VP)){
        //if((((uusi.tyyppi & tyyppi) & SAMA_AIKA) && ((uusi.tyyppi & tyyppi) & SAMA_VP))){
        //if((uusi.tyyppi & tyyppi) > tyyppi*0.875){
        if((uusi.tyyppi & tyyppi) > tyyppi*0.5){
            if((suhteet.back().p2 == uusi.p1)){
                suhteet.push_back(uusi);
                tyyppi&=uusi.tyyppi;
            }
            if((suhteet.back().p1 == uusi.p2)){
                suhteet.insert(suhteet.begin(), uusi);
                tyyppi&=uusi.tyyppi;
            }
        }

    }


}

int SVerkko::size() const
{
    if(suhteet.size()){
        return suhteet.size()+1;
    }else{
        return 0;
    }
}

int SVerkko::getTyyppi() const
{
    return tyyppi;
}

int SVerkko::getP(int i) const
{
    assert(i<this->size());
    if(i==0){
        return suhteet.at(0).p1;
    }else{
        return suhteet.at(i-1).p2;
    }
}
