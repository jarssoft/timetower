#ifndef GRABBER_H
#define GRABBER_H

#include <QHash>
#include <QRegularExpression>
#include <vector>
#include "programme.h"

namespace xmltvp {

struct daygrabformat {

    //luettelo oppaan käyttämistä formaateista sitä vastaava
    QHash<QString, QString> channels;

    //luettelo oppaan käyttämistä kategorioista ja sitä vastaava
    //tuntemattomasta ilmoitetaan.
    //varoitetaan jos eri kategoriat muunnetaan samaan kategoriaan
    QHash<QString, QString> categories;

    //kanava alkaa
    //puuttuvista tai tuntemattomista kanavista ilmoitetaan
    QRegularExpression channelbeginsformat;

    //kanava loppuu. tätä voidaan käyttää kanavajärjestyksen varmistamiseen
    //(jos ei anneta, kanava loppuu kun toinen alkaa)
    QRegularExpression channelendsformat;

    //samoin ohjelmat kanavien sisällä
    //jos kanavalla ei ole ohjelmia, ilmoitetaan
    QRegularExpression programbeginsformat;
    QRegularExpression programendsformat;

    //ohjelman tiedot
    //title ja start edellytetään
    QRegularExpression titleformat;

    //start ajat pitää olla järjestyksessä kanavan sisällä
    QRegularExpression startformat;
    QRegularExpression stopformat;
    QRegularExpression descformat;
    QRegularExpression urlformat;

    //ajan formaatti Qt:n datetime-formaatin avulla
    //voi olla date tai time. toisen voi jättää tyhjäksi.
    QString timeformat;
    QString datetimeformat;

    //url-formaatti Qt:n datetime-formaatin avulla (täytyy sisältää päivä)
    QString guideurlformat;
    QString localdirectory;
};

/** Kraappaa ohjelmatiedot HTML-tiedostosta. Tarkoitettu käytettäväksi pääasiassa deklaratiivisesti,
 *  mutta metodeita voidaan tarvittessa kuormittaa täsmällisen parsimisen aikaansaamiseksi.*/
class grabber
{
public:
    grabber(daygrabformat format);

    std::vector<xmltvprogramme> grab(const QDate &dt);

private:
    //oppaan domain jota tarvitaan jos url:ssa annetaan linkki juureen
    QString domain() const;

};

}

#endif // GRABBER_H
