#include <assert.h>
#include "kstaso.h"

KSTaso::KSTaso(vector2d<Kuutiosuhde *> pisteet, int suunta)
{
    this->pisteet=pisteet;
    this->suunta=suunta;
}

KSTaso::KSTaso(int x, int y, int suunta) : suunta(suunta)
{
    //alustetaan x,y kokoinen taulu
    if(y>0 && x>0){
        pisteet = vector2d<Kuutiosuhde*>(x, std::vector<Kuutiosuhde*>(y, 0));
    }

    assert(this->getSizeX()==x);
    assert(this->getSizeY()==y);
}

void KSTaso::setKuutio(int x, int y, Kuutiosuhde *kuutio)
{
    assert(x<this->getSizeX());
    assert(y<this->getSizeY());
    //pisteet.at(y).insert(pisteet.at(y).begin()+x, kuutio);
    pisteet.at(x).at(y)=kuutio;
}

Kuutiosuhde *KSTaso::getKuutio(int x, int y)
{
    assert(x<this->getSizeX());
    assert(y<this->getSizeY());
    return pisteet.at(x).at(y);
}

int KSTaso::getSizeX()
{
    return pisteet.size();
}

int KSTaso::getSizeY()
{
    if(getSizeX()>0){
        return pisteet.at(0).size();
    }else{
        return 0;
    }
}

int KSTaso::getSuunta()
{
    return suunta;
}

int KSTaso::countValues()
{
    int values=0;
    for(int iy=0; iy<this->getSizeY(); iy++){
        for(int ix=0; ix<this->getSizeX(); ix++){
            if(this->pisteet.at(ix).at(iy)){
                values++;
            }
        }
    }
    return values;
}

double KSTaso::getValuePortion()
{
    return (double)countValues() / (getSizeX() * getSizeY());
}

vector2d<Kuutiosuhde *> &KSTaso::getPisteet()
{
    return this->pisteet;
}

KuutioTyyppi KSTaso::getTyyppi() const
{
    return tyyppi;
}

void KSTaso::setTyyppi(const KuutioTyyppi &value)
{
    tyyppi = value;
}

vector2d<Neliosuhde> KSTaso::getNelioverkko(bool behind)
{
    Neliosuhde tyhja(0, 0, 0, 0);

    std::vector<std::vector<Neliosuhde>> nelioverkko;
    for(int iy=0; iy<this->getSizeY(); iy++){
        std::vector<Neliosuhde> rivi;
        for(int ix=0; ix<this->getSizeX(); ix++){
            if(behind){
                //rivi.push_back(*(getKuutio(ix, iy)->behind));
                //rivi.push_back(*(getKuutio(ix, iy)->right));
                if(getKuutio(ix, iy)){
                    rivi.push_back(*(getKuutio(ix, iy)->bottom));
                }else{
                    rivi.push_back(tyhja);
                }
            }else{
                //rivi.push_back(*(getKuutio(ix, iy)->front));
                //rivi.push_back(*(getKuutio(ix, iy)->left));
                if(getKuutio(ix, iy)){
                    rivi.push_back(*(getKuutio(ix, iy)->top));
                }else{
                    rivi.push_back(tyhja);
                }
            }
        }
        nelioverkko.push_back(rivi);
    }
    return nelioverkko;
}
