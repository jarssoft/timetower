#include "suhde.h"
#include "../XMLTVtoHTML/datetime.h"

Suhde::Suhde(const int x, const int y, const int tyyppi)
{
    this->p1=x;
    this->p2=y;
    this->tyyppi=tyyppi;
}

bool Suhde::operator ==(const Suhde &b) const
{
    return p1==b.p1 && p2==b.p2 && tyyppi==b.tyyppi;
}

std::string Suhde::toString()
{
    return std::to_string(p1)+"-"+std::to_string(p2)+" "+std::to_string(tyyppi);
}

std::string tyyppi_to_string(int tyyppi)
{
    std::string pal;
    std::string sep=", ";

    if(tyyppi & SAMA_ESITYS){
        pal+="eri jakson yhtämones esityskerta"+sep;
    }
    if(tyyppi & SAMA_KUVAUS){
        //pal+="sama jakson peräkkäinen esityskerta"+sep;
        pal+="sama kuvaus"+sep;
    }
    if(tyyppi & SAMA_JAKSO){
        pal+="sama jakso"+sep;
    }
    if(tyyppi & PER_JAKSO){
        pal+="peräkkäinen jakso"+sep;
    }

    if(tyyppi & SAMA_EDELT){
        pal+="sama edeltävä ohjelma"+sep;
    }
    if(tyyppi & SAMA_SEURA){
        pal+="sama seuraava ohjelma"+sep;
    }
    if(tyyppi & PER_NIMI){
        pal+="peräkkäinen ohjelma tällä nimellä"+sep;
    }
    if(tyyppi & PER_OHJ){
        pal+="peräkkäinen ohjelma"+sep;
    }

    if(tyyppi & PER_ARKI){
        pal+="peräkkäisinä arkipäivinä"+sep;
    }
    if(tyyppi & PER_PAIVA){
        pal+="peräkkäisinä päivinä"+sep;
    }
    if(tyyppi & PER_VIIKKO){
        pal+="peräkkäisenä viikkona"+sep;
    }
    if(tyyppi & SAMA_PAIVA){
        pal+="samana päivänä"+sep;
    }
    if(tyyppi & SAMA_VIIKKO){
        pal+="samana viikkona"+sep;
    }
    if(tyyppi & SAMA_VP){
        pal+="sama viikonpäivä"+sep;
    }

    if(tyyppi & SAMA_AIKA_N){
        pal+="suunnilleen sama kellonaika"+sep;
    }

    if(tyyppi & SAMA_AIKA){
        pal+="sama aika päivästä"+sep;
    }

    return pal;
}

bool voiYhdistaa(const int x1, const int y1, const int x2, const int y2){
    const int samatbitit = (x1 & x2) & (y1 & y2);
    return (samatbitit != (x1 & x2)) && (samatbitit != (y1 & y2));
}

int dmin(const tm tm){
    return (tm.tm_hour * 60) + tm.tm_min;
}

int wmin(const tm tm){
    return dmin(tm) + tm.tm_wday * 24 * 60;
}

int calcwmin(int day, int hour, int minute){
    return minute + (hour * 60) + (day * 24 * 60);
}

bool isArki(tm aika){
    return (aika.tm_wday>0 && aika.tm_wday<6);
}

int teetyyppi(const Ohjelma ox, const Ohjelma oy, const bool perakkaiset, const bool nimiperakkaiset){

    //oy on aina myöhempi kuin ox

    int tyyppi=0;

    if(oy.episode!=-1){

        if(ox.episode == oy.episode && oy.episode!=-1){
            //tyyppi = tyyppi | SAMA_ESITYS;
        }

        bool samajakso = (ox.episode == oy.episode);

        /*
        if(abs(ox.esityskerta - oy.esityskerta)==1 && samajakso){
            tyyppi = tyyppi | PER_ESITYS;
        }*/


        if(samajakso){
            //tyyppi = tyyppi | SAMA_JAKSO;
        }

        if(abs(ox.episode - oy.episode)==1){
            //tyyppi = tyyppi | PER_JAKSO;
        }

    }

    if(ox.kuvaus.size() == oy.kuvaus.size()){
        //tyyppi = tyyppi | SAMA_KUVAUS;
    }


    if(ox.paikka.getEdeltava() == oy.paikka.getEdeltava()){
        tyyppi = tyyppi | SAMA_EDELT;
    }

    if(ox.paikka.getSeuraava() == oy.paikka.getSeuraava()){
        tyyppi = tyyppi | SAMA_SEURA;
    }

    if(nimiperakkaiset){
        tyyppi = tyyppi | PER_NIMI;
    }

    if(perakkaiset){
        tyyppi = tyyppi | PER_OHJ;
    }


    if(isArki(ox.aika) && isArki(oy.aika) && (abs(oy.aika.tm_yday - ox.aika.tm_yday)==1 || ((abs(oy.aika.tm_yday - ox.aika.tm_yday)==3 && oy.aika.tm_wday == 1)))){
        tyyppi = tyyppi | PER_ARKI;
    }

    if(abs(ox.aika.tm_yday - oy.aika.tm_yday)==1){
        tyyppi = tyyppi | PER_PAIVA;
    }

    if(abs(week_number(ox.aika) - week_number(oy.aika)) == 1){
        tyyppi = tyyppi | PER_VIIKKO;
    }

    if(ox.aika.tm_yday == oy.aika.tm_yday){
        tyyppi = tyyppi | SAMA_PAIVA;
    }

    if(week_number(ox.aika) == week_number(oy.aika)){
        tyyppi = tyyppi | SAMA_VIIKKO;
    }

    if(ox.aika.tm_wday == oy.aika.tm_wday){
        tyyppi = tyyppi | SAMA_VP;
    }

    tm oxaika=ox.aika;
    tm oyaika=oy.aika;

    time_t epoch1 = mktime(&oxaika);
    time_t epoch2 = mktime(&oyaika);
    int ero_m = abs(epoch1-epoch2) / 60;

    //if(abs(ero - 60*60*24) < 20*60){
    const int paiva_m = 60*24;
    int jakojaannos = abs(ero_m % paiva_m);
    //const int toleranssi_m = 96;
    const int toleranssi_m = 121;

    if(jakojaannos < toleranssi_m || jakojaannos > (paiva_m-toleranssi_m)){
        tyyppi = tyyppi | SAMA_AIKA_N;
    }

    if(jakojaannos == 0){
        tyyppi = tyyppi | SAMA_AIKA;
    }

    //suodatetaan tyyppi
    //tyyppi = tyyppi & (PER_ESITYS | SAMA_JAKSO | PER_JAKSO | PER_VIIKKO);
    //tyyppi = tyyppi & (PER_PAIVA | PER_VIIKKO | PER_JAKSO);

    return tyyppi;

}
