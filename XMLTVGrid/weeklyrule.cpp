#include "weeklyrule.h"
#include "../XMLTVp/access.h"
#include <iostream>
#include <iomanip>
#include <sstream>



#include <chrono>  // for high_resolution_clock
#include <assert.h>
#include "../XMLTVtoHTML/datetime.h"

const char *wdna[7] = {
    "sunnuntaina", "maanantaina", "tiistaina", "keskiviikkona",
    "torstaina", "perjantaina", "lauantaina",
    };

const char *wdsin[7] = {
    "sunnuntaisin", "maanantaisin", "tiistaisin", "keskiviikkoisin",
    "torstaisin", "perjantaisin", "lauantaisin",
    };

const char *wdsta[7] = {
    "sunnuntaista", "maanantaista", "tiistaista", "keskiviikosta",
    "torstaista", "perjantaista", "lauantaista",
    };

const char *wdhin[7] = {
    "sunnuntaihin", "maanantaihin", "tiistaihin", "keskiviikkoon",
    "torstaihin", "perjantaihin", "lauantaihin",
    };


WeeklyRule::WeeklyRule():peitto(0) {}

std::string fixedlenght(std::string str, int length){
    return (str+"                                          ").substr(0,length-1)+" ";
}

void WeeklyRule::addProgram(tm tm, std::string akuvaus, std::string edeltaja, std::string seuraaja, bool instantly, int episode)
{
    std::cerr << paikat.size() << std::endl;
    std::cerr << std::put_time(&tm, "%a %d.%m. klo %H.%M") << std::endl;
    std::cerr << akuvaus << " (" << akuvaus.size() << ")" << std::endl;
    std::cerr << episode << std::endl;

    merkitty.push_back(false);

    tms.push_back(tm);
    int edeltajaId=-1;
    //if(instantly)
        edeltajaId=paikat.size()-1;
    paikat.push_back(Ohjelmapaikka("MTV3", 0, edeltaja, seuraaja, !instantly));
    episodes.push_back(episode);
    this->kuvaus.push_back(akuvaus);

    int esityskerrat=0;
    if(episode!=-1){
        for(int i=0;i<episodes.size(); i++){
            if(episodes.at(i) == episode){
                esityskerrat++;
                /*
                if(esityskerrat>5 && episode>99){
                    laskeEsitykset=false;
                }*/
            }
        }
    }
    esityskerta.push_back(esityskerrat);
    std::cerr << "esityskerta : " << esityskerrat << std::endl;

    assert(tms.size() == paikat.size());
    assert(episodes.size() == paikat.size());
    assert(esityskerta.size() == paikat.size());


    //std::cerr << tm.tm_hour << std::endl;
}

void WeeklyRule::nextProgram(std::string nimi)
{
    assert(paikat.size()>0);
    paikat.back().setSeuraava(nimi);
}

void WeeklyRule::etsiSuhteet()
{
    const int koko=tms.size();

    suhteet.clear();

    for(int x=0;x<koko;x++){

        if(!merkitty.at(x)){
            int yhdistetty = 0;
            Ohjelma ox = {paikat.at(x), tms.at(x), episodes.at(x), esityskerta.at(x), kuvaus.at(x)};

            for(int y=x+1;y<koko;y++){

                if(!merkitty.at(y)){

                    Ohjelma oy = {paikat.at(y), tms.at(y), episodes.at(y), esityskerta.at(y), kuvaus.at(y)};

                    bool nimiperakkaiset = (y==x+1);
                    bool perakkaiset = (!(paikat.at(y).isHyppy()));

                    int tyyppi=teetyyppi(ox, oy, perakkaiset && nimiperakkaiset, nimiperakkaiset);

                    if(tyyppi){
                    //if(tyyppi & (~yhdistetty)){ //sama yhdistetään uudelleen vain, jos uusia bittejä


                        std::cerr << fixedlenght(std::to_string(x) + " vs " + std::to_string(y)+":", 12);
                        //std::cerr << "    viikkoaika: " << calcwmin(5, 22, 00) << std::endl;
                        std::cerr << tyyppi_to_string(tyyppi) << std::endl;

                        Suhde suhde(x, y, tyyppi);
                        //Suhde suhde(x, y, tyyppi & (~yhdistetty));
                        suhteet.push_back(suhde);
                    }

                    yhdistetty = yhdistetty | tyyppi;
                }
            }
        }
    }

    std::cerr << "    suhteet: " << suhteet.size() << std::endl;


    //etsiNelio();
}


/*
void WeeklyRule::laajennaYksi(int i){
    NSVerkko skverkko(neliot.at(i));;
    skverkko.laajenna(neliot);
    std::cerr << "Laajennetiin nelio " << "laajuuteen " << skverkko.countP() << ":" << std::endl;
    printTable(skverkko);
}*/

void WeeklyRule::printKuutioTable(KSVerkko& verkko){

    for(int i=0; i<verkko.getSizeY(); i++){
        KSTaso taso = verkko.getTaso(SUUNTAY, i);
        if(i==0){
            printTable(NSVerkko(taso.getNelioverkko(false)), true);
        }
        printTable(NSVerkko(taso.getNelioverkko(true)), false);
    }
}

//listaa kaikki pisteet kuutioverkossa
void WeeklyRule::merkitse(KSVerkko& verkko){

    for(int i=0; i<verkko.getSizeY(); i++){
        KSTaso taso = verkko.getTaso(SUUNTAY, i);
        if(i==0){
            merkitse(NSVerkko(taso.getNelioverkko(false)));
        }
        merkitse(NSVerkko(taso.getNelioverkko(true)));
    }

}

//listaa kaikki pisteet neliöverkossa
void WeeklyRule::merkitse(NSVerkko verkko){

    for(int y=0;y<verkko.getHeight();y++){
        for(int x=0;x<verkko.getWidth();x++){
            merkitty.at(verkko.getP(x,y))=true;
        }
    }
}

//listaa kaikki pisteet neliöverkossa
void WeeklyRule::merkitse(SVerkko& verkko){

    for(int i=0;i<verkko.size();i++){
        merkitty.at(verkko.getP(i))=true;
        std::cerr << "merkitään: " << verkko.getP(i) << std::endl;
    }


}

bool WeeklyRule::etsiKuutioverkot(){

    bool hyvaksytaan=false;

    /*
    for(int i = 0; i < kuutiot.size();i++){
        std::cerr << "kuutio: " << i << std::endl;
        printTable(*(kuutiot.at(i).front));
        printTable(*(kuutiot.at(i).behind));
        std::cerr << tyyppi_to_string(kuutiot.at(i).getSyvyysTyyppi()) << std::endl << std::endl;
    }//*/

    Kuutiosuhde* paraskuutio=&(kuutiot.at(0));
    /*
    for(int i=1;i<kuutiot.size();i++){
        if(kuutiot.at(i).getKuutioTyyppi().rank() > paraskuutio->getKuutioTyyppi().rank()){
            paraskuutio=&(kuutiot.at(i));
        }
    }*/

    KSVerkko verkko(paraskuutio);

    //std::cerr << "laajennettu kokoon " << verkko.getSizeShape() << std::endl;

    for(int t=0;t<8;t++){
        if(!verkko.laajenna(&kuutiot)){
            break;
        };
    }

     /*
    verkko.laajenna(&kuutiot);

    verkko.laajenna(&kuutiot);
    verkko.laajenna(&kuutiot);
    */

    std::cerr << "laajennettu kokoon " << verkko.getSizeShape() << std::endl;

    KuutioTyyppi kt = verkko.getTyyppi();
    std::cerr << tyyppi_to_string(kt.tyyppix) << std::endl;
    std::cerr << tyyppi_to_string(kt.tyyppiy) << std::endl;
    std::cerr << tyyppi_to_string(kt.tyyppiz) << std::endl;

    if(kt.tyyppiy & PER_VIIKKO && verkko.getSizeY()==2){
        if(kt.tyyppiz & SAMA_VIIKKO){
            if(kt.tyyppix & PER_PAIVA && verkko.getSizeX()==1){

                KSTaso taso = verkko.getTaso(SUUNTAY, 0);
                NSVerkko nelioverkko(taso.getNelioverkko(false));

                for(int i=0; i<nelioverkko.getHeight(); i++){
                    rule += wdsin[this->tms.at(nelioverkko.getP(0, i)).tm_wday];
                    rule += ", ";
                }

                std::stringstream ss;
                ss << std::put_time(&(this->tms.at(nelioverkko.getP(0,0))), " klo %H.%M,");
                ss << std::put_time(&(this->tms.at(nelioverkko.getP(1,0))), " ja seuraavana päivänä klo %H.%M,");
                rule += ss.str();

                hyvaksytaan=true;
            }else if(kt.tyyppiz & PER_PAIVA && verkko.getSizeZ()==6){
                //rule += "joka päivä";
            }else if(kt.tyyppiz & PER_ARKI && verkko.getSizeZ()==4){
                //rule += "joka arkipäivä";
            }
        }
        if((kt.tyyppix & SAMA_PAIVA) && (kt.tyyppiy & SAMA_AIKA_N) && (kt.tyyppiz & SAMA_AIKA_N)){
            //rule+=  ", " + std::to_string(verkko.getSizeX()+1) + " esitystä päivässä";
            KSTaso taso = verkko.getTaso(SUUNTAY, 0);
            NSVerkko nelioverkko(taso.getNelioverkko(false));

            if(kt.tyyppiz & SAMA_VIIKKO){
                if(kt.tyyppiz & PER_PAIVA || kt.tyyppiz & PER_ARKI){
                    if(kt.tyyppiz & PER_PAIVA && verkko.getSizeZ()==6){
                        rule += "joka päivä";
                    }else if(kt.tyyppiz & PER_ARKI && verkko.getSizeZ()==4){
                        rule += "joka arkipäivä";
                    }else{
                        for(int i=0; i<nelioverkko.getHeight(); i++){
                            rule += wdsin[this->tms.at(nelioverkko.getP(0, i)).tm_wday];
                            rule += ", ";
                            //std::stringstream ss;
                            //ss << std::put_time(&(this->tms.at(nelioverkko.getP(i,0))), " klo %H.%M,");
                            //rule += ss.str();
                        }
                    }
                }
            }

            this->peitto = verkko.getSizeShape();
            for(int i=0; i<nelioverkko.getWidth(); i++){
                std::stringstream ss;
                ss << std::put_time(&(this->tms.at(nelioverkko.getP(i,0))), " klo %H.%M,");
                rule += ss.str();
            }

            if(!((kt.tyyppiy & SAMA_AIKA) && (kt.tyyppiz & SAMA_AIKA))){
                rule+=" (ajat vaihtelevat vähän)";
            }

            hyvaksytaan=true;
        }
    }

    //std::cerr << rule << " s\n";

    //int values=verkko.getSizeShape();

    printKuutioTable(verkko);
    //exit(0);
//return false;
    if(hyvaksytaan){
        merkitse(verkko);
    }

    return hyvaksytaan;

    //std::cerr << "Elapsed time: " << elapsed.count() << " s\n";
    //std::cerr << "Peitto: " << values << "/" << this->paikat.size() << " s\n";

}

void WeeklyRule::etsiNelioverkot(){

}

void WeeklyRule::etsiVerkot()
{
    Neliosuhde::etsiNeliot(this->suhteet, this->neliot);
    Kuutiosuhde::etsiKuutiot(this->neliot, this->kuutiot);

    //auto start = std::chrono::high_resolution_clock::now();

    //auto finish = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double> elapsed = finish - start;

    bool hyvaksytaan=false;

    if(kuutiot.size()>0){
        hyvaksytaan=this->etsiKuutioverkot();
        //etsitään suhteet uudelleen
        if(hyvaksytaan){
            suhteet.clear();
            neliot.clear();
            this->etsiSuhteet();
            Neliosuhde::etsiNeliot(this->suhteet, this->neliot);
        }
    }

    hyvaksytaan=true;

    for(int ti = 0; ti < 4 && hyvaksytaan ; ti++){

         hyvaksytaan=false;

         neliot.clear();
         this->etsiSuhteet();
         Neliosuhde::etsiNeliot(this->suhteet, this->neliot);

        if(neliot.size()>0){

            std::cerr << "Ei kuutioita, etsitään neliöitä" << std::endl;

            //int suurin =0;
            std::vector<NSVerkko> verkot;

            for(int i = 0; i < 1;i++){

                //printTable(neliot.at(i));

                NSVerkko skverkko(neliot.at(i));

                skverkko.laajenna(neliot);
                //printTable(skverkko);

                if(skverkko.countP()>2){ // && (skverkko.getVaakaTyyppi()+skverkko.getPystyTyyppi())*skverkko.countP()>1300000){
                    verkot.push_back(skverkko);
                    //suurin=skverkko.countP();
                    //suurin = (skverkko.getVaakaTyyppi() + skverkko.getPystyTyyppi()) * skverkko.countP();
                }
            }

            for(int i = 0; i < verkot.size();i++){
                bool loytyi = false;
                //for(int j = i+1; j < verkot.size();j++){

                for(int j = 0; j < verkot.size();j++){
                    if(i!=j){
                        if(verkot.at(i).onOsa(verkot.at(j))){
                            loytyi=true;
                        }
                    }
                }
                if(!loytyi){
                    if(rule.length()>0){
                        rule+= ", ";
                    }

                    std::cerr << std::endl;
                    std::cerr << "Laajennetiin nelio " << "laajuuteen " << verkot.at(i).countP() << ":" << std::endl;

                    const NelioTyyppi nt = verkot.at(i).getNelioTyyppi();

                    //if(nt.tyyppiy & PER_VIIKKO && verkot.at(i).getHeight()==3){ //jos toistuu joka viikko
                    //if(nt.tyyppiy & PER_VIIKKO && verkot.at(i).getHeight()>=2){ //jos toistuu vähintään kahtena viikkona
                    if(!(nt.tyyppiy & SAMA_VIIKKO) && verkot.at(i).getHeight()>=2){ //jos toistuu vähintään kahtena viikkona

                        if(verkot.at(i).getHeight()==2){
                            std::stringstream ss;
                            ss << std::put_time(&(this->tms.at(verkot.at(i).getP(0,0))), "viikoilla %W ");
                            ss << std::put_time(&(this->tms.at(verkot.at(i).getP(0,1))), "ja %W ");
                            rule+=ss.str();
                        }

                        if(((nt.tyyppiy & SAMA_AIKA_N) && (nt.tyyppiy & SAMA_VP)) && (nt.tyyppix & SAMA_VIIKKO)){

                            std::cerr << "nt.tyyppiy:" << tyyppi_to_string(nt.tyyppiy) << std::endl;
                            std::cerr << "nt.tyyppiy & SAMA_AIKA:" << (nt.tyyppiy & (SAMA_AIKA | SAMA_VP)) << std::endl;


                            if(nt.tyyppix & PER_PAIVA){
                                if(verkot.at(i).getWidth()==7){
                                    rule+="joka päivä";
                                }
                                if(verkot.at(i).getWidth()==2){
                                    rule += wdsin[this->tms.at(verkot.at(i).getP(0,0)).tm_wday];
                                    std::stringstream ss;
                                    ss << std::put_time(&(this->tms.at(verkot.at(i).getP(0,0))), " klo %H.%M ");
                                    //ss << " ja sitä seuraavana päivänä ";
                                    ss << " ja ";
                                    ss << wdsin[this->tms.at(verkot.at(i).getP(0,0)).tm_wday];
                                    ss << std::put_time(&(this->tms.at(verkot.at(i).getP(1,0))), " klo %H.%M ");
                                    rule += ss.str() + " ";
                                    hyvaksytaan=true;
                                }
                            }else{
                                if(nt.tyyppix & PER_ARKI && verkot.at(i).getWidth()==5){
                                    rule+="joka arkipäivä";
                                }else{
                                    if(nt.tyyppix & PER_OHJ){
                                        rule += wdsin[this->tms.at(verkot.at(i).getP(0,0)).tm_wday];
                                        std::stringstream ss;
                                        ss << std::put_time(&(this->tms.at(verkot.at(i).getP(0,0))), " klo %H.%M");
                                        rule += ss.str() + " ";
                                        rule += std::to_string(verkot.at(i).getWidth())+" jaksoa peräkkäin, ";
                                        hyvaksytaan=true;
                                    }else{
                                        for(int p = 0 ; p < verkot.at(i).getWidth();p++){
                                            rule += wdsin[this->tms.at(verkot.at(i).getP(p,0)).tm_wday];

                                            if(!(nt.tyyppix & SAMA_AIKA)){
                                                std::stringstream ss;
                                                ss << std::put_time(&(this->tms.at(verkot.at(i).getP(p,0))), " klo %H.%M");
                                                rule += ss.str();
                                                hyvaksytaan=true;
                                            }
                                            rule += ", ";
                                        }
                                    }
                                }
                            }


                            if(nt.tyyppix & SAMA_AIKA){
                                std::stringstream ss;
                                ss << std::put_time(&(this->tms.at(verkot.at(i).getP(0,0))), " klo %H.%M");
                                rule += ss.str();
                                hyvaksytaan=true;
                            }
                            if(!(nt.tyyppiy & SAMA_AIKA)){
                                rule+=" (ajat vaihtelevat vähän)";
                            }
                        }
                    }
                    if(!(nt.tyyppiy & SAMA_PAIVA) && nt.tyyppiy & SAMA_AIKA){
                        if(nt.tyyppix & SAMA_PAIVA){
                            for(int p = 0 ; p < verkot.at(i).getHeight();p++){
                                std::stringstream ss;
                                ss << wdna[this->tms.at(verkot.at(i).getP(0, p)).tm_wday];
                                ss << std::put_time(&(this->tms.at(verkot.at(i).getP(0, p))), " %e.%m. ");
                                rule+=ss.str();
                            }
                            for(int t = 0 ; t < verkot.at(i).getWidth();t++){
                                std::stringstream ss;
                                ss << std::put_time(&(this->tms.at(verkot.at(i).getP(t,0))), " klo %H.%M");
                                rule += ss.str() + " ";
                            }
                            hyvaksytaan=true;
                        }

                    }

                    printTable(verkot.at(i));
                    std::cerr << "Peitto: " << verkot.at(i).countP() << "/" << this->paikat.size() << std::endl;
                    this->peitto=verkot.at(i).countP();
                }

                if(hyvaksytaan){
                    merkitse(verkot.at(i));
                }
            }

        }
    }


    hyvaksytaan=true;

    for(int ti = 0; ti < 4 && hyvaksytaan ; ti++){


        hyvaksytaan=false;

        //etsitään suhteet uudelleen
        this->etsiSuhteet();

        if(!suhteet.empty()){

            //listataan jäljellä olevat (ehkä turhaa)
            /*
            std::vector<int> jaljella;
            for(int i=0; i<merkitty.size();i++){
                if(!merkitty.at(i)){
                    jaljella.push_back(i);
                }
            }*/
            //std::cerr << "Jäljellä: " << jaljella.size() << "/" << paikat.size() << std::endl;

            //luodaan verkko
            //rule="";
            SVerkko sv;
            sv.laajenna(this->suhteet);
            this->peitto=sv.size();
            //std::cerr << "Peitto: " << sv.size() << "/" << jaljella.size() << std::endl;
            std::cerr << "Tyyppi: " << tyyppi_to_string(sv.getTyyppi()) << std::endl;
            std::cerr << "--------------------------------------------" << std::endl;

            //luodaan sääntö
            //if(jaljella.size() > 1 && sv.size() == jaljella.size()){
            if(sv.size() > 1){

                 if(rule.length()>0){
                    rule+= ", ";
                 }

                 if((sv.getTyyppi() & SAMA_VP) && (sv.getTyyppi() & SAMA_AIKA_N)){
                     std::stringstream ss;

                     if(sv.size()==2 && !(sv.getTyyppi() & SAMA_VIIKKO)){
                         std::stringstream ss;
                         ss << std::put_time(&(this->tms.at(sv.getP(0))), " viikoilla %W ");
                         ss << std::put_time(&(this->tms.at(sv.getP(1))), "ja %W ");
                         rule+=ss.str();
                     }

                     rule+= wdsin[(this->tms.at(sv.getP(0)).tm_wday)];
                     if(!(sv.getTyyppi() & SAMA_AIKA)){
                         rule+= " noin ";
                     }
                     ss << std::put_time(&(this->tms.at(sv.getP(0))), " klo %H.%M");
                     rule+=ss.str();
                     hyvaksytaan=true;

                 } else if(sv.getTyyppi() & SAMA_AIKA_N){

                         if(paikat.size() > 2 && (sv.getTyyppi() & PER_ARKI || sv.getTyyppi() & PER_PAIVA)){
                             std::stringstream ss;
                             if(sv.getTyyppi() & PER_PAIVA){
                                ss << "peräkkäisinä päivinä ";
                             }else{
                                ss << "peräkkäisinä arkipäivinä ";
                             }
                             ss << wdsta[this->tms.at(sv.getP(0)).tm_wday];
                             ss << std::put_time(&(this->tms.at(sv.getP(0))), " %e.%m. - ");
                             ss << wdhin[this->tms.at(sv.getP(sv.size()-1)).tm_wday];
                             ss << std::put_time(&(this->tms.at(sv.getP(sv.size()-1))), " %e.%m., ");
                             rule+=ss.str();
                         }else{
                             for(int i=0;i<sv.size();i++){
                                 rule+= wdna[(this->tms.at(sv.getP(i)).tm_wday)];
                                 std::stringstream ss;
                                 ss << std::put_time(&(this->tms.at(sv.getP(i))), " %e.%m., ");
                                 rule+=ss.str();
                             }
                         }

                         std::stringstream ss;
                         ss << std::put_time(&(this->tms.at(sv.getP(0))), " klo %H.%M, ");
                         rule += ss.str();
                         hyvaksytaan=true;

                 } else if(sv.getTyyppi() & (SAMA_VIIKKO)){
                     std::stringstream ss;
                     ss << std::put_time(&(this->tms.at(sv.getP(0))), "viikolla %W ");
                     rule += ss.str();
                     hyvaksytaan=true;

                     if(sv.getTyyppi() & (SAMA_AIKA)){

                         for(int i=0;i<sv.size();i++){
                             rule += wdna[(this->tms.at(sv.getP(i)).tm_wday)] + std::string(", ");
                         }

                         std::stringstream ss;
                         ss << std::put_time(&(this->tms.at(sv.getP(0))), " klo %H.%M, ");
                         rule += ss.str();

                     }else{

                         for(int i=0;i<sv.size();i++){
                             rule += wdna[(this->tms.at(sv.getP(i)).tm_wday)];
                             std::stringstream ss;
                             ss << std::put_time(&(this->tms.at(sv.getP(i))), " klo %H.%M, ");
                             rule += ss.str();
                         }
                     }
                 }

                 if(hyvaksytaan==true){
                    merkitse(sv);
                 }
            }
        }
    }

    printYksittain();

    //if(this->paikat.size() < 4){

    //}else{
        //rule+=std::to_string(this->paikat.size())+" lähetystä";
    //}
    if(this->paikat.size()==1){
        this->peitto=1.0;
    }

    //std::cerr << "tarkistus: " << tyyppi_to_string(neliot.at(16).getVaakaTyyppi()) << std::endl;



    std::string lisarule="";
    for(int i=0;i<paikat.size();i++){
        if(!merkitty.at(i)){
            lisarule+= wdna[(this->tms.at(i).tm_wday)];
            std::stringstream ss;
            ss << std::put_time(&(this->tms.at(i)), " %e.%m. klo %H.%M, ");
            lisarule+=ss.str();
        }
    }

    //täydennetään ne ohjelmat, joita ei tunnistettu toistuvaksi
    if(rule.length()>0 && lisarule.length()>0){
        rule+= " sekä ";
    }

    rule+=lisarule;

}

void WeeklyRule::printYksittain()
{
    //ei säännönmukaisuutta, pelkkä lista
    for(int i=0;i<paikat.size();i++){

        std::cerr << std::endl;

        std::cerr << wdna[(this->tms.at(i).tm_wday)]
                << std::put_time(&(this->tms.at(i)), " %d.%m. klo %H.%M") << std::endl;

        std::cerr << kuvaus.at(i) << std::endl;
    }
}

std::string WeeklyRule::getRule() const
{
    return rule;
}

int WeeklyRule::getProgramCount() const
{
    return this->paikat.size();
}

int WeeklyRule::getPeitto() const
{
    return peitto;
}

const int SOLULEVEYS = 25;
const int XOTSIKONLEVEYS = 15;

void WeeklyRule::printTableHeader(const Suhde2D &suhdetaulu, const int pystytyyppi){

    if((pystytyyppi & SAMA_VP)){
        std::cerr << fixedlenght("Viikonpäivä",XOTSIKONLEVEYS);
        for(int x=0;x<suhdetaulu.getWidth();x++){
            std::cerr << std::put_time(&(this->tms.at(suhdetaulu.getRivilta(x))), fixedlenght("%a", SOLULEVEYS-1).c_str());
        }
        std::cerr << std::endl;
    }

    if(pystytyyppi & (SAMA_AIKA)){
        std::cerr << fixedlenght("Klo",XOTSIKONLEVEYS);
        for(int x=0;x<suhdetaulu.getWidth();x++){
            std::cerr << std::put_time(&(this->tms.at(suhdetaulu.getRivilta(x))), fixedlenght("%H.%M", SOLULEVEYS).c_str());
        }
        std::cerr << std::endl;
    }else{
        if(pystytyyppi & SAMA_AIKA_N){
            std::cerr << fixedlenght("Klo",XOTSIKONLEVEYS);
            for(int x=0;x<suhdetaulu.getWidth();x++){
                std::cerr << std::put_time(&(this->tms.at(suhdetaulu.getRivilta(x))), fixedlenght("~%H.%M", SOLULEVEYS).c_str());
            }
            std::cerr << std::endl;
        }
    }

    if(pystytyyppi & SAMA_EDELT){
        std::cerr << fixedlenght("Edeltävä", XOTSIKONLEVEYS);
        for(int x=0;x<suhdetaulu.getWidth();x++){
            std::cerr << fixedlenght(paikat.at(suhdetaulu.getRivilta(x)).getEdeltava(), SOLULEVEYS);
        }
        std::cerr << std::endl;
    }

    if(pystytyyppi & SAMA_SEURA){
        std::cerr << fixedlenght("Seuraava", XOTSIKONLEVEYS);
        for(int x=0;x<suhdetaulu.getWidth();x++){
            std::cerr << fixedlenght(paikat.at(suhdetaulu.getRivilta(x)).getSeuraava(), SOLULEVEYS);
        }
        std::cerr << std::endl;
    }


    /*
    if((vaakatyyppi & PER_ESITYS)){
        std::cerr << "            ";
        for(int x=0;x<suhdetaulu.getWidth();x++){
            std::cerr << fixedlenght("esitys "+std::to_string(esityskerta.at(suhdetaulu.getRivilta(x))), SOLULEVEYS);
        }
        std::cerr << std::endl;
    }
    */
}

void WeeklyRule::printTable(const Suhde2D &suhdetaulu, bool printHeader)
{return;
    const int tyyppi = suhdetaulu.getPystyTyyppi() & suhdetaulu.getVaakaTyyppi();
    const int pystytyyppi = suhdetaulu.getPystyTyyppi() & (~tyyppi); //vain yksilölliset bitit
    const int vaakatyyppi = suhdetaulu.getVaakaTyyppi() & (~tyyppi);

    //yhteiset tyypit

    if(printHeader){

        {
            const int esim_index = suhdetaulu.getRivilta(0);
            if((tyyppi & SAMA_EDELT)){
                std::cerr << "Edeltäjä:    " << paikat.at(esim_index).getEdeltava() << std::endl;
            }
            if((tyyppi & SAMA_SEURA)){
                std::cerr << "Seuraava:    " << paikat.at(esim_index).getSeuraava() << std::endl;
            }

            if((tyyppi & SAMA_AIKA)){
                std::cerr << "Aika:        " << std::put_time(&(this->tms.at(esim_index)), "%H.%M") << std::endl;
            }else{
                if((tyyppi & SAMA_AIKA_N)){
                    std::cerr << "Aika:   noin " << std::put_time(&(this->tms.at(esim_index)), "%H.%M") << std::endl;
                }
            }

            if((tyyppi & SAMA_ESITYS)){
                std::cerr << "Esityskerta: " << this->esityskerta.at(esim_index) << std::endl;
            }

            if((tyyppi & SAMA_KUVAUS)){
                if(this->kuvaus.at(esim_index).size()>0){
                    std::cerr << "Kuvaus: " << this->kuvaus.at(esim_index) << std::endl;
                }else{
                    std::cerr << "Kuvaus: Ei kuvausta" << std::endl;
                }
            }
        }

        printTableHeader(suhdetaulu, pystytyyppi);
    }

    std::cerr << std::endl;

    //vaakatyypit

    for(int y=0;y<suhdetaulu.getHeight();y++){

        int index=suhdetaulu.getSarakkeelta(y);

        if(vaakatyyppi & PER_PAIVA){
            std::cerr << std::put_time(&(this->tms.at(index)), fixedlenght("%a %d.%m...  ", XOTSIKONLEVEYS-1).c_str());
        }

        if(vaakatyyppi & SAMA_PAIVA){
            std::cerr << std::put_time(&(this->tms.at(index)), fixedlenght("%a %d.%m.  ", XOTSIKONLEVEYS-1).c_str());
        }else{
            if(vaakatyyppi & SAMA_VIIKKO){
                std::cerr << fixedlenght("Viikko "+std::to_string(week_number(this->tms.at(index))), XOTSIKONLEVEYS);
            }else{
                std::cerr << fixedlenght("", XOTSIKONLEVEYS);
            //    std::cerr << std::put_time(&(this->tms.at(index)), fixedlenght("%a %d.%m.  ", XOTSIKONLEVEYS-1).c_str());
            }
        }

        if(vaakatyyppi & SAMA_JAKSO){
            std::cerr << std::endl << fixedlenght("jakso "+std::to_string(this->episodes.at(index))+".", XOTSIKONLEVEYS);
        }

        if(vaakatyyppi & SAMA_KUVAUS){
            std::cerr << fixedlenght(kuvaus.at(suhdetaulu.getP(0, y)), SOLULEVEYS * suhdetaulu.getWidth());
        }else{
            for(int x=0;x<suhdetaulu.getWidth();x++){
                //std::cerr << std::put_time(&(this->tms.at(nelio.getP(x,y))), "%a %d.%m.  ");
                //std::cerr << fixedlenght(std::to_string(suhdetaulu.getP(x,y)), SOLULEVEYS);
                std::cerr << fixedlenght(kuvaus.at(suhdetaulu.getP(x,y)), SOLULEVEYS);
                /*
                if(vaakatyyppi & SAMA_JAKSO){
                    std::cerr << std::put_time(&(this->tms.at(nelio.getP(x,y))), "%a %d.%m.  ");
                }else{
                    std::cerr << fixedlenght(std::to_string(this->episodes.at(nelio.getP(x,y))), SOLULEVEYS);
                }*/
            }
        }
        std::cerr << std::endl;
    }

    //*

    std::cerr << "peitto: " << suhdetaulu.countP() << "/" << this->paikat.size() << ", rank: " << vaakatyyppi * pystytyyppi << std::endl;

    std::cerr << "  vaaka: " << tyyppi_to_string(vaakatyyppi) << std::endl
              << "  pysty: " << tyyppi_to_string(pystytyyppi) << std::endl
              << "  yhteiset: " << tyyppi_to_string(tyyppi)   << std::endl;
              //*/
}
