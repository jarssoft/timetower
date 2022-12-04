#ifndef WEEKLYRULE_H
#define WEEKLYRULE_H

#include <ctime>
#include <time.h>       /* time_t, struct tm, time, localtime, asctime */
#include <libxml/parser.h>
#include <string>
#include <vector>
#include <ohjelmapaikka.h>
#include <suhde.h>
#include "neliosuhde.h"
#include "suhde2d.h"
#include "kuutiosuhde.h"
#include <ksverkko.h>
#include <nsverkko.h>
#include <sverkko.h>

class WeeklyRule
{
public:
    WeeklyRule();

    void addProgram(tm tm, std::string kuvaus, std::string edeltaja, std::string seuraaja, bool instantly, int episode);
    void nextProgram(std::string nimi);

    void etsiSuhteet();

    void etsiVerkot();
    //void laajennaYksi(int i);

    //palauttaa mahdollisimman lyhyen säännön luonnollisen kielen tekstinä
    std::string getRule() const;

    //palauttaa iCal-tiedostona
    //std::string getICAL() const;

    int getProgramCount() const;

    //palauttaa niiden ohjelmien määrän, joiden alkamisaika voidaan
    //esittää kohtuullisen pituisella (yhden rivin) merkkijonolla
    int getPeitto() const;



private:

    void printTable(const Suhde2D &nelio, bool printHeader = true);
    void printTableHeader(const Suhde2D &suhdetaulu, const int pystytyyppi);
    void printYksittain();

    bool etsiKuutioverkot();
    void etsiNelioverkot();

    std::vector<bool> merkitty;

    std::vector<tm> tms;
    std::vector<Ohjelmapaikka> paikat;
    std::vector<int> episodes;
    std::vector<int> esityskerta;
    std::vector<std::string> kuvaus;

    std::vector<Suhde> suhteet;

    std::vector<Neliosuhde> neliot;
    std::vector<Kuutiosuhde> kuutiot;
    std::string rule;

    int peitto;

    /*
     * keskinäisten etäisyyksien ulottuvuudet
     * - viikkoaika
     * - päiväaika
     * - edeltävä ja seuraava ohjelma
     * - peräkkäisyys tai väli
     * - esityskerta
     * - jakso
    */
    void printKuutioTable(KSVerkko &verkko);

    void merkitse(KSVerkko &verkko);
    void merkitse(NSVerkko verkko);
    void merkitse(SVerkko &verkko);
};

#endif // WEEKLYRULE_H
