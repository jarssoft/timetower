#ifndef VIIKKOLISTA_H
#define VIIKKOLISTA_H

#include <setup.h>
#include <vector>
#include <string.h>
#include <iostream>
#include <assert.h>
#include "xml_writer.hpp"
#include "sending.h"
#include "writer_html.h"

//add unique object to vector and return index where object is saved.
//counts number of objects.
template <class T> static int addUnique(T ohj, std::vector<T> &vektori);
using namespace std;

class CategoryCount
{
public:
    unsigned int count=0;
    unsigned int minutes=0;
    unsigned int sequence=1;

    bool operator < (const CategoryCount& rhs) const {
      return minutes < rhs.minutes;
    }
    bool operator > (const CategoryCount& rhs) const {
      return minutes > rhs.minutes;
    }

    unsigned int getCount() const{
        return count;
    }

    unsigned int getMinutes() const{
        return minutes;
    }

    unsigned int getSequence() const{
        return sequence;
    }

    //kuinka suuri osa lähetyksistä on peräkkäin
    //peräkkäisten osuus kaikista
    double perakkaisyys() const {
        //assert(sequence <= count);
        if(!(sequence <= count)){
            cerr << "Varoitus: perakkaisyys() sequence <= count" << endl;
        }
        return (double)sequence/count;
    }



    //kuinka monta ryhmää muodostuu
    unsigned int getRyhmaCount() const {
        return count - sequence;
    }

    //kuinka monta lähetystä kuuluu ryhmään keskimäärin
    double getRyhmaPituus() const {
        return (double)count / getRyhmaCount();
    }
};

//weekly table for one tv-channel
class WeekTable
{

public:

    WeekTable();

    void setWeek(int week);

    void setWeekDay(int weekday);

    void setChannelName(std::string name_of_channel);

    //add program to table
    void addProgram(const tm start, const tm end, std::string name, std::string categories,
                    int year, int season, int episode, int episodetotal, std::string subtitle, std::string desc, std::vector<std::string> urls);

    //generate table to HTML
    void getHTML(std::ostream& out);

    void getSVG(std::ostream& out);

    void getTable(Table_writer &writer);

    void getList(Table_writer &writer);

private:

    //try to level programs on slight different time
    void level();

    void level2();

    /* yrittää pyöristää ohjelmien alkamisajat. */
    void snapToGrid();

    //yrittää tasata ohjelmat aikaisempaan tickiin, jos aikaisemmassa on ohjelmia
    //ja jos lähetykselle on (yhden tickin verran) varaa.
    //tämä on hyvin toimiva, koska se ei muuta ohjelmien ajallista järjestystä
    //vaikka ajaisi useammin. Testeissä ei mitään eroa 2 ja 20 suorituskerran välillä.
    void snapToBefore();

    //sama, mutta tasoittaa myös sitä seuraavien rivien ohjelmat, mikäli ovat
    //samannimisiä.
    void snapToBeforeMore();

    /* etsii päivittäin toistuvat ohjelmat */
    void findDaily();

    /* pyrkii tasoittamaan päivittäin toistuvat ohjelmat, jotka on etsitty
        findDaily() -funktiolla. */
    void levelDaily();

    /* Etsii ajan, jolloin yön ohjelmat loppuvat ja aamun ohjelmat alkavat. */
    int findMorrowline();

    bool isEmptyTick(Tick t);

    bool hasProgramChange(Tick t);

    /* Poistaa tyhjän alun ja lopun. */
    void trim();

    /* Poistaa tyhjät rivit. */
    void removeEmptyTicks();

    //laskee, kuinka monta kertaa kategoriaa käyttävä lähetys esiintyy soluissa peräkkäin
    int neighbourCountY(std::string category);

    //laskee, kuinka monta kertaa kategoriaa käyttävä lähetys esiintyy soluissa
    //samaan aikaan peräkkäisinä päivinä niin, että vähintään 50% vasemman lähetyksen
    //soluista saa naapurikseen oikealta saman kategorian lähetyksen
    int neighbourCountX(std::string category);

    //palauttaa määrän, kuinka monta lähetystä kuuluu kyseiseen kategoriaan
    int countCategory(std::string category) const;

    //laskee kategorioiden määrän keston ja peräkkäisyyden, ja tallentaa sen categories-map-olioon
    //tietoa käytetään ohjelmien väliviivojen paksuuksien päättelemiseen
    void countCategories();

    //Palauttaa luvun, kuinka tehokkaasti kategoria jakaa alueita
    //Paras kategoria muodostaa yhtenäisiä alueita, vaaka- tai pystytasossa tai
    //molemmissa. Alue ei kuitenkaan täytä kaikkea, vaan optimaalisesti 50% kartasta.
    int categoryValue(std::string category);

    //listaa kaikkien kategorioiden arvot
    void listCategoryValues();

    //Laskee kategorian yleisyyden eri aikana Tickien tarkkuudella
    //esim.
    //aika, uutiset, elokuvat, viihde, lastenohjelmat
    //18:00, 3, 0, 1, 3
    //21:00, 3, 1, 2, 0
    void countCategoriesByTime();

    void addProgram(const int startday, const int starttick, const int endday, const int endtick, int prog_id, episode_t epis, sending_t lah);

    bool canJoint(sending_t &sending1, sending_t &sending2);

    int sendingtable[DAYSINWEEK+2][TICKSINHOUR*HOURSINDAY];
    unsigned int sendingtimes[TICKSINHOUR*HOURSINDAY];

    sending_t sendingByCoords(int x, int y);

    std::vector<sending_t> sendings;
    std::vector<program_t> programs;
    std::vector<episode_t> episodes;
    std::vector<sendingGroup> sendinggroups;

    std::string name_of_channel;
    int weeknro;
    int weekdaynro;
    tm monday;
    bool monday_has_set;

    static double todennaikoisyys(const int avaruus, const int joukko, const int jonossa);

    //montako perekkäisyyttä kategorialla on suhteessa odotettuun, jos ohjelmat ripoteltaisiin satunnaisessa järjestyksessä.
    unsigned int perakkaisyysEdustus(std::string cat) const;

    map<string, CategoryCount> categories;

    //ajan pienin mahdollinen sijainti y-koordinaatissa
    //int minimum_y[TICKSINHOUR*HOURSINDAY];

    program_t lastPrograms[7];
};



#endif // VIIKKOLISTA_H
