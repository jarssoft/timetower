#include "nsverkko.h"
#include <iostream>
#include <assert.h>

//#define MINIMIRANK 0.666
#define MINIMIRANK 0.4
//#define MINIMIRANK 0.1
//#define MINIMIRANK 0.145



NSVerkko::NSVerkko(Neliosuhde nelio)
{
    this->vaakatyyppi = nelio.getVaakaTyyppi();
    this->pystytyyppi = nelio.getPystyTyyppi();

    assert(nelio.getVaakaTyyppi());
    assert(nelio.getPystyTyyppi());

    std::vector<Neliosuhde> rivi;
    rivi.push_back(nelio);
    suhteet.push_back(rivi);
}

NSVerkko::NSVerkko(std::vector<std::vector<Neliosuhde>> nelio)
{
    suhteet=nelio;
    this->vaakatyyppi = nelio.at(0).at(0).getVaakaTyyppi();
    this->pystytyyppi = nelio.at(0).at(0).getPystyTyyppi();

    for(int jx=0;jx<nelio.at(0).size();jx++){
        for(int jy=0;jy<nelio.size();jy++){
            this->vaakatyyppi &= nelio.at(jy).at(jx).getVaakaTyyppi();
            this->pystytyyppi &= nelio.at(jy).at(jx).getPystyTyyppi();
        }
    }
}

int NSVerkko::getVaakaTyyppi() const
{
    return vaakatyyppi;
}

int NSVerkko::getPystyTyyppi() const
{
    return pystytyyppi;
}

NelioTyyppi NSVerkko::getNelioTyyppi() const
{
    return {getVaakaTyyppi(), getPystyTyyppi()};
}

int NSVerkko::getRivilta(int i) const
{
    return getP(i, 0);
}

int NSVerkko::getSarakkeelta(int i) const
{
    return getP(0, i);
}

int NSVerkko::getP(const int x, const int y) const
{
    assert(x>=0 && x<getWidth());
    assert(y>=0 && y<getHeight());

    bool viimy = y < (this->getHeight()-1);
    bool viimx = x < (this->getWidth()-1);

    std::vector<Neliosuhde> rivi = (viimy ? suhteet.at(y) : suhteet.back());
    Neliosuhde suhde = (viimx ? rivi.at(x) : rivi.back());

    int ix=(viimx ? 0 : 1);
    int iy=(viimy ? 0 : 1);

    return suhde.getP(ix, iy);
}

int NSVerkko::getWidth() const
{
    return suhteet.at(0).size()+1;
}

int NSVerkko::getHeight() const
{
    return suhteet.size()+1;
}

int NSVerkko::countP() const
{
    return getHeight() * getWidth();
}

bool NSVerkko::contains(int p) const
{
    bool loytyi=false;
    for(int jx=0;jx<this->getWidth();jx++){
        for(int jy=0;jy<this->getHeight();jy++){
            if(this->getP(jx, jy) == p){
                loytyi=true;
            }
        }
    }
    return loytyi;
}

bool NSVerkko::onOsa(const NSVerkko &verkko)
{
    bool loytyikoko=true;
    for(int ix=0;ix<this->suhteet.size();ix++){
        for(int iy=0;iy<this->suhteet.back().size();iy++){
            bool loytyi=false;
            for(int jx=0;jx<verkko.suhteet.size();jx++){
                for(int jy=0;jy<verkko.suhteet.back().size();jy++){
                    if(this->suhteet.at(ix).at(iy) == verkko.suhteet.at(jx).at(jy)){
                        loytyi=true;
                    }
                }
            }

            if(!loytyi){
                loytyikoko=false;
            }

        }
    }

    if(loytyikoko){
        return true;
    }

    //jos ei tarjoa tarkempaa tyyppiä
    if(!(this->getVaakaTyyppi() & (~(verkko.getVaakaTyyppi()))) && !(this->getPystyTyyppi() & (~(verkko.getPystyTyyppi())))){

        loytyikoko=true;

        for(int ix=0;ix<this->getWidth();ix++){
            for(int iy=0;iy<this->getHeight();iy++){
                if(!verkko.contains(this->getP(ix, iy))){
                    loytyikoko=false;
                }
            }
        }
    }

    return loytyikoko;
}

void NSVerkko::laajenna(const std::vector<Neliosuhde> &neliot)
{
    //laajennaRivi(neliot);
   // uusiSarake(neliot);

    for(int i=0;i<50;i++){
        NelioTyyppi ntrivi = uusiRivi(neliot);
        NelioTyyppi ntsarake = uusiSarake(neliot);
        //std::cerr << "vaaka: " << ntrivi.rank() << " pysty:" <<  ntsarake.rank() << std::endl;
        //std::cerr << "vaaka: " << tyyppi_to_string(ntrivi.rank() << " pysty:" <<  ntsarake.rank() << std::endl;
        //if(!(ntrivi.tyyppix && ntrivi.tyyppiy) && !(ntsarake.tyyppix && ntsarake.tyyppiy)){
        //    break;
        //}
        //if(ntrivi.rank() > ntsarake.rank()){
        //if(getPystyTyyppi() > getVaakaTyyppi() && (ntrivi.getOnlyVaaka() && ntrivi.getOnlyPysty())){    //suositaan ulottuvuutta, joka on vaikeammin jatkettavissa

        //const int minimi=250019200;
        //const long minimi=117997568; //aikaisemmin
        const int minimi=0;

        if(ntrivi.rank() > ntsarake.rank() && ntrivi.rank()>minimi){    //suositaan ulottuvuutta, joka on vaikeammin jatkettavissa
            uusiRiviToteuta(ntrivi);
        }else{
            if(ntsarake.rank()>minimi){
                uusiSarakeToteuta(ntsarake);
            }else{
                break;
            }
        }
    }
}

NelioTyyppi NSVerkko::uusiRivi(const std::vector<Neliosuhde> &lisattavat)
{
    //std::vector<Neliosuhde> uusirivi;
    uusirivi={};
    uusirivi.reserve(suhteet.back().size());
    NelioTyyppi pal={0,0};

    for(int ix=0;ix<suhteet.back().size();ix++){

        int sopivin=0;
        long paras_rank=this->getNelioTyyppi().rank() * MINIMIRANK;
        const Neliosuhde end=suhteet.back().at(ix);

        /*
        std::cerr << std::endl;
        std::cerr << "*   vaaka:" << tyyppi_to_string((getVaakaTyyppi() & getVaakaTyyppi())) << std::endl;
        std::cerr << "    pysty:" << tyyppi_to_string((getPystyTyyppi() & getPystyTyyppi())) << std::endl;
        */

        for(int i=0;i<lisattavat.size();i++){

            const Neliosuhde lisattava = lisattavat.at(i);

            //sovitetaan uutta naapureihin (ylös ja vasemmalle)
            if(end.fitToBottom(lisattava) && (uusirivi.size()==0 || uusirivi.back().fitToRight(lisattava))){

                //std::cerr << "löydettiin jatke " << i << " - " << ix << std::endl;

                const long tama_rank=
                        //(getNelioTyyppi().getOnlyPysty() & lisattava.getNelioTyyppi().getOnlyPysty()) *
                        //(getNelioTyyppi().getOnlyVaaka() & lisattava.getNelioTyyppi().getOnlyVaaka());

                        (long)(getPystyTyyppi() & lisattava.getPystyTyyppi()) *
                        (long)(getVaakaTyyppi() & lisattava.getVaakaTyyppi());

                /*
                std::cerr << ix << " rank " << i << ": " << tama_rank << std::endl;
                std::cerr << "    vaaka:" << tyyppi_to_string((getVaakaTyyppi() & lisattava.getVaakaTyyppi())) << std::endl;
                std::cerr << "    pysty:" << tyyppi_to_string((getPystyTyyppi() & lisattava.getPystyTyyppi())) << std::endl;
                */

                if(tama_rank>paras_rank){

                    paras_rank=tama_rank;
                    sopivin=i;
                }

                //this->vaakatyyppi = end.getVaakaTyyppi() & lisattava.getVaakaTyyppi();
                //this->pystytyyppi = end.getPystyTyyppi() & lisattava.getPystyTyyppi();


            }
        }

        if(sopivin){
            //std::cerr << "sopivin on " << sopivin << std::endl;

            uusirivi.push_back(lisattavat.at(sopivin));

        }else{
            //std::cerr << "ei sopivaa, x: " << ix << std::endl;
            return pal;
        }

    }

    /*
    for(int i=0;i<uusirivi.size();i++){
        this->vaakatyyppi = getVaakaTyyppi() & uusirivi.at(i).getVaakaTyyppi();
        this->pystytyyppi = getPystyTyyppi() & uusirivi.at(i).getPystyTyyppi();
    }
    */

    if(uusirivi.size() == suhteet.back().size()){
        pal={getVaakaTyyppi(), getPystyTyyppi()};
        for(int i=0;i<uusirivi.size();i++){
            pal.tyyppix &= uusirivi.at(i).getVaakaTyyppi();
            pal.tyyppiy &= uusirivi.at(i).getPystyTyyppi();
        }
    }

    return pal;
}

void NSVerkko::uusiRiviToteuta(NelioTyyppi nt)
{
    assert(uusirivi.size() == suhteet.back().size());
    assert(getVaakaTyyppi() & nt.tyyppix);
    assert(getPystyTyyppi() & nt.tyyppiy);

    //std::cerr << "löydettiin uusi rivi"  << std::endl;
    suhteet.push_back(uusirivi);

    this->vaakatyyppi = getVaakaTyyppi() & nt.tyyppix;
    this->pystytyyppi = getPystyTyyppi() & nt.tyyppiy;
}

NelioTyyppi NSVerkko::uusiSarake(const std::vector<Neliosuhde> &lisattavat)
{
    //std::vector<Neliosuhde> uusisarake;
    uusisarake={};
    uusisarake.reserve(suhteet.size());
    NelioTyyppi pal={0,0};

    for(int iy=0;iy<suhteet.size();iy++){

        int sopivin=0;
        long paras_rank=this->getNelioTyyppi().rank() * MINIMIRANK;
        const Neliosuhde end=suhteet.at(iy).back();

        for(int i=0;i<lisattavat.size();i++){

            const Neliosuhde lisattava=lisattavat.at(i);

            if(end.fitToRight(lisattava) &&
                    (uusisarake.size()==0 || uusisarake.back().fitToBottom(lisattava)) &&
                    !(this->contains(lisattava.getP(1, 0))) &
                    !(this->contains(lisattava.getP(1, 0)))
                    ){

                //std::cerr << "löydettiin jatke " << i << " - " << ix << std::endl;

                const long tama_rank =
                        //(end.getNelioTyyppi().getOnlyPysty() & lisattava.getNelioTyyppi().getOnlyPysty()) *
                        //(end.getNelioTyyppi().getOnlyVaaka() & lisattava.getNelioTyyppi().getOnlyVaaka());
                        (long)(getPystyTyyppi() & lisattava.getPystyTyyppi()) *
                        (long)(getVaakaTyyppi() & lisattava.getVaakaTyyppi());

                if(tama_rank>paras_rank){
                    paras_rank=tama_rank;
                    sopivin=i;
                }

                //this->vaakatyyppi = end.getVaakaTyyppi() & lisattava.getVaakaTyyppi();
                //this->pystytyyppi = end.getPystyTyyppi() & lisattava.getPystyTyyppi();

            }
        }

        if(sopivin){
            //std::cerr << "sopivin on " << sopivin << std::endl;

            //std::cerr << "uusiSarake: ";

            uusisarake.push_back(lisattavat.at(sopivin));
            //this->vaakatyyppi = getVaakaTyyppi() & lisattavat.at(sopivin).getVaakaTyyppi();
            //this->pystytyyppi = getPystyTyyppi() & lisattavat.at(sopivin).getPystyTyyppi();



        }else{
            //std::cerr << "ei sopivaa, x: " << ix << std::endl;
            return pal;
        }

    }

    if(uusisarake.size() == suhteet.size()){
        pal = {getVaakaTyyppi(), getPystyTyyppi()};
        for(int i=0;i<uusisarake.size();i++){
            pal.tyyppix &= uusisarake.at(i).getVaakaTyyppi();
            pal.tyyppiy &= uusisarake.at(i).getPystyTyyppi();
        }
    }

    return pal;
}

void NSVerkko::uusiSarakeToteuta(NelioTyyppi nt)
{
    assert(uusisarake.size() == suhteet.size());
    assert(getVaakaTyyppi() & nt.tyyppix);
    assert(getPystyTyyppi() & nt.tyyppiy);

    //std::cerr << "löydettiin uusi rivi"  << std::endl;
    for(int i=0;i<suhteet.size();i++){
        suhteet.at(i).push_back(uusisarake.at(i));
    }

    this->vaakatyyppi = getVaakaTyyppi() & nt.tyyppix;
    this->pystytyyppi = getPystyTyyppi() & nt.tyyppiy;
}

