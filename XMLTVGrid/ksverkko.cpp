#include "ksverkko.h"
#include "assert.h"
#include <algorithm>

KSVerkko::KSVerkko(Kuutiosuhde *kuutio)
{
    suhteet =vector3d<Kuutiosuhde*>{{{kuutio}}};
    setTyyppi(kuutio->getKuutioTyyppi());
}

bool KSVerkko::laajenna(std::vector<Kuutiosuhde>* kuutiot)
{
    //pysyvät

    KSTaso reuna[6] = {KSTaso(0,0,SUUNTAX), KSTaso(0,0,SUUNTAY), KSTaso(0,0,SUUNTAZ), KSTaso(0,0,SUUNTAX), KSTaso(0,0,SUUNTAY), KSTaso(0,0,SUUNTAZ)};
    KSTaso uusi[6]  = {KSTaso(0,0,SUUNTAX), KSTaso(0,0,SUUNTAY), KSTaso(0,0,SUUNTAZ), KSTaso(0,0,SUUNTAX), KSTaso(0,0,SUUNTAY), KSTaso(0,0,SUUNTAZ)};

    //Kuutiosuhde* tama[3];

    for(int d=0; d<3; d++){
        reuna[d] = getTaso(d, size3d(suhteet, d)-1);
        reuna[d+VASTAAN] = getTaso(d, 0);
    }
    for(int d=0; d<6; d++){
        reuna[d].setTyyppi(this->getTyyppi());
        uusi[d] = KSTaso(reuna[d].getSizeX(), reuna[d].getSizeY(), reuna[d].getSuunta());
        uusi[d].setTyyppi(this->getTyyppi());
    }

    //etsitään...

    std::cerr << "verrataan " << std::endl;
    std::cerr << "tyypille "<< std::endl << this->getTyyppi().toString();
    //std::cerr << "tyypille "<< std::endl << this->getKuutio(0,0,0)->getKuutioTyyppi().toString();

    const int xaxel[]={0,0,0};
    const int yaxel[]={2,0,2};

    //muuttujat, joilla etsitään paras laajentumissuunta
    int suurind=-1;
    int suurinarvo=0;

    for(int d=0; d<6; d++){

        std::cerr << "etsitään d:" << d << " " << uusi[d].getSizeX() << "x" << uusi[d].getSizeY() << std::endl;

        for(int iy=0;iy<reuna[d].getSizeY();iy++){
            for(int ix=0;ix<reuna[d].getSizeX();ix++){

                //std::cerr << "etsitään (" << ix << ", " << iy << ")" << std::endl;
                Kuutiosuhde* vanhakuutio = reuna[d].getKuutio(ix,iy);
                //assert(vanhakuutio);

                if(vanhakuutio){

                    Kuutiosuhde* paraskuutio = 0;
                    KuutioTyyppi parastyyppi={0,0,0};

                    //sovitetaan jokaista kuutiota
                    for(int i=0; i<kuutiot->size(); i++){

                        //std::cerr << "etsitään == " << i << std::endl;

                        Kuutiosuhde* uusikuutio = &(kuutiot->at(i));
                        assert(uusikuutio);

                        //uusi tyyppi syntyy yhdistämällä uusi kuutio koko verkkoon
                        KuutioTyyppi uusityyppi = uusi[d].getTyyppi().yhdista(uusikuutio->getKuutioTyyppi());

                        //reunojen pitää sopia
                        if(vanhakuutio->sovita(uusikuutio, d)){
                                //&& this->getTyyppi().yhdista(uusikuutio->getKuutioTyyppi()).rank()){
                                //&& uusikuutio->getKuutioTyyppi().rank() >= this->getTyyppi().rank()){   //edellytetään yhtä vahvaa tyyppiä
                                //&& uusikuutio->getKuutioTyyppi().rank()*2 > this->getTyyppi().rank()){   //edellytetään puoliksi niin vahvaa tyyppiä
                               // && uusikuutio->getKuutioTyyppi().rank()*1.5 > this->getTyyppi().rank()){   //erottaa salkkarien arkilähetykset vl-uusinnoista
                                //&& uusikuutio->getKuutioTyyppi().rank() == this->getTyyppi().rank()){   //erottaa salkkarien arkilähetykset vl-uusinnoista
                                //&& this->getTyyppi().yhdista(uusikuutio->getKuutioTyyppi()).rank() > this->getTyyppi().rank()){   //erottaa salkkarien arkilähetykset vl-uusinnoista

                            //if(!uusi[d].getKuutio(ix, iy)){
                            if(uusityyppi.rank() > parastyyppi.rank()){

                                paraskuutio=uusikuutio;
                                parastyyppi=uusityyppi;

                                std::cerr << "yhdistettiin kuutio " << i << " kohtaan (" << ix << ", " << iy << ")" << std::endl;
                                //std::cerr << "tyyppiin " << uusikuutio->getKuutioTyyppi().rank() << std::endl;

                                //std::cerr << uusikuutio->getKuutioTyyppi().toString();

                                //sopiiko naapureihin? ei vielä vaikuta verkon muodostumiseen.

                                /*
                                if(ix>0 && uusi[d].getKuutio(ix-1, iy)){
                                    std::cerr << "naapuri x-1" << std::endl;
                                    if(uusi[d].getKuutio(ix-1, iy)->sovita(uusikuutio,xaxel[d])){
                                        std::cerr << "sopi" << std::endl;
                                    }else{
                                        std::cerr << "ei sopinut" << std::endl;
                                    }
                                }
                                if(iy>0 && uusi[d].getKuutio(ix, iy-1)){
                                    std::cerr << "naapuri y-1" << std::endl;
                                    if(uusi[d].getKuutio(ix, iy-1)->sovita(uusikuutio,yaxel[d])){
                                        std::cerr << "sopi" << std::endl;
                                    }else{
                                        std::cerr << "ei sopinut" << std::endl;
                                    }
                                }
                                //*/



                            }
                        }
                    }

                    //if(paraskuutio && parastyyppi.rank() * 1.05 > uusi[d].getTyyppi().rank()){
                    //if(paraskuutio && parastyyppi.rank() >= this->getTyyppi().rank()){
                    if(paraskuutio && parastyyppi.rank() * 1.005 > uusi[d].getTyyppi().rank()){
                        uusi[d].setKuutio(ix, iy, paraskuutio);
                        uusi[d].setTyyppi(parastyyppi);
                    }
                }

            }
        }

        std::cerr << "löytyi" << " " << uusi[d].countValues()
                  << "/" << uusi[d].getSizeX()*uusi[d].getSizeY()
                  << " rank " << uusi[d].getTyyppi().rank() << std::endl;

        std::cerr << uusi[d].getTyyppi().toString();

        //if(uusi[d].getValuePortion()==1 && uusi[d].getTyyppi().rank()){
        //if(uusi[d].getValuePortion()==1 && uusi[d].getTyyppi().rank() >= this->getTyyppi().rank()){
        if(uusi[d].getValuePortion()==1 && uusi[d].getTyyppi().rank() * 1.005 > this->getTyyppi().rank()){
        //if(uusi[d].getValuePortion()>0.45 && uusi[d].getTyyppi().rank() * 1.005 > this->getTyyppi().rank()){
            //if(uusi[d].countValues() > suurinarvo){
            if(uusi[d].getTyyppi().rank() > suurinarvo){
                //suurinarvo=uusi[d].countValues();
                suurinarvo=uusi[d].getTyyppi().rank();
                suurind=d;
            }
        }

    }

    //laajennetaan...

    if(suurind!=-1){
        std::cerr << "laajenetaan suunnassa " << suurind << std::endl;
        if(suurind>=VASTAAN){
            insert3d(suhteet, uusi[suurind].getPisteet(), suurind-VASTAAN, 0);
        }else{
            push_back3d(suhteet, uusi[suurind].getPisteet(), suurind);
        }
        this->setTyyppi(uusi[suurind].getTyyppi());
    }

    //poistetaan lisätyt listata

    /*
    for(int iy=0;iy<uusi[suurind].getSizeY();iy++){
        for(int ix=0;ix<uusi[suurind].getSizeX();ix++){
            if(uusi[suurind].getKuutio(ix, iy)){

                Kuutiosuhde ks = *(uusi[suurind].getKuutio(ix, iy));
                //std::vector<Kuutiosuhde>::iterator position = std::find(kuutiot->begin(), kuutiot->end(), ks);
                for(int i=0;i<kuutiot->size();i++){
                    if(uusi[suurind].getKuutio(ix, iy) == &(kuutiot->at(i))){
                        std::cerr << "Löydettiin kuutio, poistettiin, size: " << kuutiot->size() << std::endl;
                        //kuutiot->erase(kuutiot->begin()+i);
                        //break;
                        //return;
                    }
                }

                //if (position != kuutiot->end()){ // == myVector.end() means the element was not found
                    //kuutiot->erase(position);
                //}
            }
        }
    }
    //*/

    std::cerr << "koko: " << size3d(suhteet, 0) << "x" << size3d(suhteet, 1) << "x" << size3d(suhteet,2) << std::endl;

    return (suurind!=-1);
}

int KSVerkko::getSizeX() const
{
    assert(size3d(this->suhteet, 0) && this->suhteet.size()<40);
    return size3d(this->suhteet, 0);
}

int KSVerkko::getSizeY() const
{
    assert(size3d(this->suhteet, 1)>0 && this->suhteet.at(0).size()<40);
    return size3d(this->suhteet, 1);
}

int KSVerkko::getSizeZ() const
{
    assert(size3d(this->suhteet, 2)>0 && this->suhteet.at(0).at(0).size()<40);
    return size3d(this->suhteet, 2);
}

int KSVerkko::getSizeShape() const
{
    return (this->getSizeX()+1) * (this->getSizeY()+1) * (this->getSizeZ()+1);
}

KSTaso KSVerkko::getTaso(int suunta, int index)
{
    return KSTaso(at3d(suhteet, suunta, index), suunta);
}

Kuutiosuhde *KSVerkko::getKuutio(int x, int y, int z)
{
    return suhteet.at(x).at(y).at(z);
}

KuutioTyyppi KSVerkko::getTyyppi() const
{
    return tyyppi;
}

void KSVerkko::setTyyppi(const KuutioTyyppi &value)
{
    tyyppi = value;
}
