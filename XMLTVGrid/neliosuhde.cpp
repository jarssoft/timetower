#include "neliosuhde.h"
#include <iostream>
#include <assert.h>

Neliosuhde::Neliosuhde(Suhde *x1, Suhde *y1, Suhde *y2, Suhde *x2) : x1(x1), y1(y1), y2(y2), x2(x2) {}

long NelioTyyppi::rank()
{
    //return getOnlyVaaka() * getOnlyPysty();
    return ((long)tyyppix) * tyyppiy;
}

int NelioTyyppi::getOnlyVaaka()
{
    return tyyppix & (~tyyppiy);
}

int NelioTyyppi::getOnlyPysty()
{
    return tyyppiy & (~tyyppix);
}



bool isGreater(NelioTyyppi nt1, NelioTyyppi nt2)
{
    return nt1.rank() > nt2.rank();
}

int Neliosuhde::getVaakaTyyppi() const
{
    return x1->tyyppi & x2->tyyppi;
}

int Neliosuhde::getPystyTyyppi() const
{
    return y1->tyyppi & y2->tyyppi;
}

NelioTyyppi Neliosuhde::getNelioTyyppi() const
{
    return {getVaakaTyyppi(), getPystyTyyppi()};
}

int Neliosuhde::getRivilta(int i) const
{
    assert(i>=0 && i<=1);

    if(i==0){
        return x1->p1;
    }
    if(i==1){
        return x1->p2;
    }
}

int Neliosuhde::getSarakkeelta(int i) const
{
    assert(i>=0 && i<=1);

    if(i==0){
        return y1->p1;
    }
    if(i==1){
        return y1->p2;
    }
}

int Neliosuhde::getP(int x, int y) const
{
    assert(x>=0 && y<=1);
    assert(y>=0 && y<=1);

    if(x==0){
        if(y==0){
            return y1->p1;
        }else{
            return y1->p2;
        }
    }else{
        if(y==0){
            return y2->p1;
        }else{
            return y2->p2;
        }
    }
}

int Neliosuhde::getWidth() const
{
    return 2;
}

int Neliosuhde::getHeight() const
{
    return 2;
}

int Neliosuhde::countP() const
{
    return 4;
}


bool Neliosuhde::operator ==(const Neliosuhde &b) const
{
    return x1==b.x1 && y1==b.y1 && x2==b.x2 && y2==b.y2;
}

bool Neliosuhde::fitToRight(const Neliosuhde &b) const
{
    return((y2->p1 == b.y1->p1) && (y2->p2 == b.y1->p2));
}

bool Neliosuhde::fitToBottom(const Neliosuhde &b) const
{
    return(x2->p1 == b.x1->p1 && x2->p2 == b.x1->p2);
}

//const int MINIMITYYPPI = 50000; //nopeuttaa, ja toimii salkkareissa
const int MINIMITYYPPI = 5000; //toimii kuutioisssa
//const int MINIMITYYPPI = 0;

void Neliosuhde::etsiNeliot(std::vector<Suhde> &suhteet, std::vector<Neliosuhde> &neliot){

    const int koko=suhteet.size();
    //

    for(int x1=0;x1<koko;x1++){
        Suhde* sx1=&suhteet.at(x1);

        for(int y1=x1+1;y1<koko;y1++){
            Suhde* sy1=&suhteet.at(y1);

            for(int y2=y1+1;y2<koko;y2++){
                Suhde* sy2=&suhteet.at(y2);

                if(sx1->p1 == sy1->p1 && sx1->p2 == sy2->p1){

                    for(int x2=y2+1;x2<koko;x2++){
                        Suhde* sx2=&suhteet.at(x2);

                        if(sy1->p2 == sx2->p1 && sy2->p2 == sx2->p2
                                && voiYhdistaa(sx1->tyyppi, sy1->tyyppi, sx2->tyyppi, sy2->tyyppi)
                                ){

                            Neliosuhde nelio(sx1, sy1, sy2, sx2);

                            if(nelio.getPystyTyyppi() && nelio.getVaakaTyyppi() //&& (nelio.getPystyTyyppi() ^ nelio.getVaakaTyyppi())){
                            //if(nelio.getPystyTyyppi() * nelio.getVaakaTyyppi() > 500000000 //&& (nelio.getPystyTyyppi() ^ nelio.getVaakaTyyppi())){
                                    && (nelio.getPystyTyyppi() != nelio.getVaakaTyyppi())
                                    && (nelio.getPystyTyyppi() + nelio.getVaakaTyyppi())>MINIMITYYPPI){
                                std::cerr << "neliö (" << neliot.size() << ") " << sx1->toString() <<
                                             " --- " << sy1->toString() <<
                                             " --- " << sy2->toString() <<
                                             " --- " << sx2->toString() << std::endl;

                                std::cerr << "  vaaka: " << tyyppi_to_string(nelio.getVaakaTyyppi()) << std::endl;
                                std::cerr << "  pysty: " << tyyppi_to_string(nelio.getPystyTyyppi()) << std::endl;

                                //printTable(nelio);

                                neliot.push_back(nelio);
                                if(neliot.size()>8000){
                                    return;
                                }
                            }
                        }
                        //std::cerr << "    suhteet: " << x <<" "<< y <<" "<< z <<" "<< v <<" "<< std::endl;
                    }
                }
            }
        }
    }
}

Suhde *Neliosuhde::getX1() const
{
    return x1;
}

Suhde *Neliosuhde::getX2() const
{
    return x2;
}

Suhde *Neliosuhde::getY1() const
{
    return y1;
}

Suhde *Neliosuhde::getY2() const
{
    return y2;
}



