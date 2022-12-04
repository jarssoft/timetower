#include "manipulate.h"
#include <iostream>
#include <assert.h>
#include "translate.h"
#include <vector>
#include "../XMLTVp/episodenum.h"
#include "../XMLTVp/parse-episode.h"
#include "../XMLTVp/parse-specs.h"
#include "../Rutiinit/natlan.h"
#include <stdio.h>      /* printf */
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */

std::vector<std::string> titles;

void print (std::string title, xmlNode *node) {
    std::cout << title << std::endl;
}

Translate trans;

void initTrans(){

    trans.loadFromCVS("/home/jari/works/ohjelmat/Qt/XMLTV/XMLTVManipulate/omat.txt");

    trans.loadFromDplayCVS("/home/jari/media/lataukset/xmltv/csv/mtv.csv");
    trans.loadFromDplayCVS("/home/jari/media/lataukset/xmltv/csv/ruutu.csv");
    trans.loadFromDplayCVS("/home/jari/media/lataukset/xmltv/csv/ruutu2.csv");
    trans.loadFromDplayCVS("/home/jari/media/lataukset/xmltv/csv/dplay.csv");
    trans.loadFromDplayCVS("/home/jari/media/lataukset/xmltv/csv/alfatv.csv");

    trans.loadFromPetscanCVS("/home/jari/media/lataukset/xmltv/csv/2020-04-14/Televisio-ohjelmat.csv", "fi");
    trans.loadFromPetscanCVS("/home/jari/media/lataukset/xmltv/csv/2020-04-14/Radio-ohjelmat.csv", "fi");
    trans.loadFromPetscanCVS("/home/jari/media/lataukset/xmltv/csv/2020-04-14/Elokuvat.csv", "fi");
    trans.loadFromPetscanCVS("/home/jari/media/lataukset/xmltv/csv/2020-04-14/Film.csv", "sv");
    trans.loadFromPetscanCVS("/home/jari/media/lataukset/xmltv/csv/2020-04-14/TV-program.csv", "sv");

    /*
    trans.loadFromMediawiki("/home/jari/media/lataukset/wikipedia/Suomalaiset_komediasarjat.xml");
    trans.loadFromMediawiki("/home/jari/media/lataukset/wikipedia/1990-luvun_suomalaiset_sketsisarjat.xml");
    trans.loadFromMediawiki("/home/jari/media/lataukset/wikipedia/2000-vuosikymmenen_suomalaiset_sketsisarjat.xml");
    trans.loadFromMediawiki("/home/jari/media/lataukset/wikipedia/2010-luvun_suomalaiset_sketsisarjat.xml");
    trans.loadFromMediawiki("/home/jari/media/lataukset/wikipedia/Suomalaiset_dokumenttiohjelmat.xml");
    trans.loadFromMediawiki("/home/jari/media/lataukset/wikipedia/Suomalaiset_draamasarjat.xml");
    trans.loadFromMediawiki("/home/jari/media/lataukset/wikipedia/Suomalaiset_tositelevisiosarjat.xml");
    trans.loadFromMediawiki("/home/jari/media/lataukset/wikipedia/Suomalaiset_televisiosarjat.xml");

    trans.loadFromMediawiki("/home/jari/media/lataukset/wikipedia/Yhdysvaltalaiset_televisiosarjat.xml");
    trans.loadFromMediawiki("/home/jari/media/lataukset/wikipedia/Yhdysvaltalaiset_komediasarjat.xml");
    trans.loadFromMediawiki("/home/jari/media/lataukset/wikipedia/Yhdysvaltalaiset_draamasarjat.xml");

    trans.loadFromMediawiki("/home/jari/media/lataukset/wikipedia/Ruotsalaiset_televisiosarjat.xml");
    trans.loadFromMediawiki("/home/jari/media/lataukset/wikipedia/Ruotsalaiset_draamasarjat.xml");

    trans.loadFromMediawiki("/home/jari/media/lataukset/wikipedia/1990-luvun_televisiosarjat.xml");
    trans.loadFromMediawiki("/home/jari/media/lataukset/wikipedia/2000-vuosikymmenen_televisiosarjat.xml");
    trans.loadFromMediawiki("/home/jari/media/lataukset/wikipedia/2010-luvun_televisiosarjat.xml");
    */

}

void initLinks(){

    std::string url, category;
    if(trans.getProgram("Napakymppi", url, category)){
        std::cout << url << std::endl;
        std::cout << category << std::endl;
    }
}

int timeoffset=0;
std::string channel="";
std::string startdate="";
bool autodst=false;

void initManipulate(int timeoffset_a, char* channel_a, char* startdate_a, bool autodst_a){
    timeoffset=timeoffset_a;
    channel=channel_a;
    startdate=startdate_a;
    autodst=autodst_a;
}

unsigned int points(std::string phrase)
{
    if(phrase.size()>80){
        return 0;
    }
    return (phrase.find(" 19") != -1 || phrase.find(" 20") != -1 ? 1000-phrase.size() : 0 );
}


bool manipulate(xmlNode *node) {

    std::string title = xmltvp::getTitle(node, xmltvp::fi);
    if(title.empty()){
        title = xmltvp::getTitle(node, xmltvp::sv);
    }
    std::string orgtitle = title;

    if(title.size() != removeSign(title).size()){
        title=removeSign(title);
    }

    if(title.size()==0){
        std::cerr << "Varoitus! Ohjelma ilman nimeä. Ohitetaan." << std::endl;
        return true;
    }

    std::string desc = xmltvp::getDesc(node, xmltvp::fi);
    if(desc.empty()){
        desc = xmltvp::getDesc(node, xmltvp::sv);
    }

    std::string subtitle = xmltvp::getSubtitle(node);

    //parse episode number
    xmltvp::episode_ns episode = xmltvp::getEpisodenum(node);
    if(episode.isEmpty()){
        //std::cout << desc << std::endl;
        episode = xmltvp::parse_episode(desc);

        if(episode.isEmpty()){
            episode = xmltvp::parse_episode(subtitle);
        }

        if(!episode.isEmpty()){
            xmltvp::setEpisodenum(node, episode);
            //std::cout << xmltvp::format_xmltv_ns(episode) << std::endl<< std::endl;
        }
    }

    xmltvp::tuotanto_ns tuotanto = xmltvp::parse_tuotanto(desc);
    if(tuotanto.isEmpty()){
        tuotanto = xmltvp::parse_tuotanto(subtitle);
    }
    if(!tuotanto.isEmpty()){
        //std::cout << tuotanto.year << std::endl;
        xmltvp::clearDate(node);
        xmltvp::addDate(node, tuotanto.year.c_str());
    }

    std::string firstpart = title;
    std::vector<std::string> parts;
    split(title.c_str(), ':', parts);

    //std::cerr << "title" << std::endl;
    /*
    if(firstpart=="Yle Uutiset Uusimaa"
            || firstpart=="Yle Uutiset Kaakkois-Suomi"
            || firstpart=="Yle Uutiset Lounais-Suomi"
            || firstpart=="Yle Uutiset Häme"
            || firstpart=="Yle Uutiset Pirkanmaa"
            || firstpart=="Yle Uutiset Keski-Suomi ja Etelä-Savo"
            || firstpart=="Yle Uutiset Itä-Suomi"
            || firstpart=="Yle Uutiset Pohjanmaa"
            || firstpart=="Yle Uutiset Pohjois-Suomi"
            || firstpart=="Yle Uutiset Lappi"){

        title="Alueuutiset";
        desc = subtitle;
        //subtitle="";
        xmltvp::addCategory(node, "Alueuutiset");
    }
    */

    /*
    if(firstpart=="Yle Uutiset selkosuomeksi"
            || firstpart=="Yle Oddasat"
            || firstpart=="Novosti Yle"
            || firstpart=="Yle Uutiset viittomakielellä"
            || firstpart=="Yle Uutiset"
            || firstpart=="Yle Uutiset Alueeltasi"
            ){
        title="Uutisohjelmia";
        desc = subtitle;
        //subtitle="";
    }
    */

    double diff;
    {
        tm alkaa = xmltvp::getStop(node);
        tm loppuu = xmltvp::getStop(node);
        diff = difftime(mktime(&loppuu), mktime(&alkaa)) / 60;
    }

    /*
    if(        firstpart== "Hirviökoulu"
            || firstpart== "Postimies Pate, erikoislähettipalvelu"
            || firstpart== "Arvaa kuinka paljon sinua rakastan"
            || firstpart== "Pikku Kakkonen: Juontopalat"
            || firstpart== "Vilkki Ankka"
            || firstpart== "Kesäleiri"
            || firstpart== "Belle ja Sebastian"
            || firstpart== "Unna Junná"
            || firstpart== "Pikku perunat"

            || firstpart== "Hilla ja avaruuden Eetu"
            || firstpart== "Kuiske"
            || firstpart== "Pikku Pörriäinen"
            || firstpart== "Simo"
            || firstpart== "Ritari Mikke"
            || firstpart== "Askarrellaan"
            || firstpart== "Ella, Oskari ja Huu"
            || firstpart== "Nimipäiväonnittelu"
            || firstpart== "Urheiluhullut"
            || firstpart== "Lassie"

            || firstpart== "Ludovic"
            || firstpart== "Poppelikumpu"
            || firstpart== "Tete ja Mama"
            || firstpart== "Herää pahvi"
            || firstpart== "Kaisa ja Olli"
            || firstpart== "Mauno"
            || firstpart== "Tiedonjyvä"
            || firstpart== "Aappo ja Tööt"
            || firstpart== "Otso ja sopulit"
            || firstpart== "Näin meillä"

            || firstpart== "Astroset"
            || firstpart== "Albi Lumiukko"
            || firstpart== "Sasu"
            || firstpart== "Pusse"
            || firstpart== "Mimosa ja Lennu"
            || firstpart== "Gudrun, viikinkiprinsessa"
            || firstpart== "Pertti ja Purtti"
            || firstpart== "Patu-koira"

            || firstpart== "Kim ja Kai"
            || firstpart== "Ruskea pikkukarhu"
            || firstpart== "Killi ja Kiki"
            || firstpart== "Kallen keittiö"
            || firstpart== "Pikku Kakkosen posti"
            || firstpart== "Tilda ja hänen ystävänsä"
            || firstpart== "Uula ja Kuu"
            || firstpart== "Jetron matkaan"
            || firstpart== "Ville ja Lousku vauhdissa"
            || firstpart== "Villit futarit"

            || firstpart== "Kuu ja minä"
            || firstpart== "Nipa ja Pete"
            || firstpart== "Kiri ja Lolo"
            || firstpart== "Franklin ja ystävät"
            || firstpart== "Kaarle"
            || firstpart== "Puluboin ja Ponin Juujuutuubi"
            || firstpart== "Robottijunat"
            || firstpart== "Kiri ja Lolo "

            || firstpart=="Jääkarhu Otto"
            || firstpart== "Hertan maailma"
            || firstpart== "Musarullaa!"
            || firstpart== "Petran poppoo"
            || firstpart== "Nelli ja Iiro"
            || firstpart== "Safari"
            || firstpart== "Oktonautit"
            || firstpart== "Lelumesta"

            || firstpart== "Heimo ja Hemmo"
            || firstpart== "Athleticus"
            || firstpart== "Kati ja Töppö: Töpön mahtava pääsiäinen"
            || firstpart== "Kallen keittiö "
            || firstpart== "Laura"
            || firstpart== "Nukketeatteri Sampon lauluja"
            || firstpart== "Herra Mäyränen ja rouva Repolainen"
            || firstpart== "Pikku Kakkosen talo"
            || firstpart== "Unelmajuttu"

            || firstpart== "Nallen laulut"
            || firstpart== "Pipsa Possu"
            || firstpart== "Mumenvággi"
            ){

        desc = subtitle;
        subtitle=title;
        title="Lastenohjelmia";
    }
    */

    if(parts.size()>1){
        firstpart = parts.at(0);
        //std::cout << firstpart << std::endl;

        /*
        if(firstpart=="Talviolympialaiset 2018" || firstpart=="Korean olympialaiset" || firstpart=="Korean paralympialaiset"){
            xmltvp::addCategory(node, "olympic");
            std::string secondpart = parts.at(1);

            std::string url=getOlympicLinks(secondpart);
            if(url.length()>0){
                xmltvp::addURL(node, url.c_str());
            }
        }*/



        if(firstpart=="Katastrofileffa" || firstpart=="Hirviöleffa" || firstpart=="#Subleffa" || firstpart=="Elokuva"
                || firstpart=="Kino Suomi" || firstpart=="Kino Klassikko" || firstpart=="Kotikatsomo"){
            xmltvp::addCategory(node, "Movies");

            //std::string takaosa=parts.at(1);
            //title=takaosa.substr(1, takaosa.length());

        }
        if(firstpart=="LIVE"){
            //title=parts.at(1);
            xmltvp::addCategory(node, "Suora");
        }
        if(firstpart=="Tokion olympialaiset" || firstpart=="Tennistä"){
            //title=parts.at(1);
            xmltvp::addCategory(node, "Sport");
        }

        if(firstpart=="Dok" || firstpart=="Tiededokumentti" || firstpart=="Historia"){
            xmltvp::addCategory(node, "Documentary");
            //title=parts.at(1);
        }
    }


    if(title!=orgtitle){
        xmltvp::clearTitle(node);
        xmltvp::addTitle(node, title.c_str());
    }


    std::string country = xmltvp::getCountry(node);
    std::string language = xmltvp::getLanguage(node);

    if(language=="fi"){
        xmltvp::addCategory(node, "Suomenkieliset", xmltvp::fi);
    }else if(language=="sv" || language=="no" ||
                language=="da" || language=="is"){
        xmltvp::addCategory(node, "Ruotsinkieliset", xmltvp::fi);
    }else if(language=="en"){
        xmltvp::addCategory(node, "Englanninkieliset", xmltvp::fi);
    }else{
        //xmltvp::addCategory(node, "Muunkieliset", xmltvp::fi);
    }


    if(country=="Suomi"){
        xmltvp::addCategory(node, "Kotimaisetohjelmat", xmltvp::fi);
    }

    if(country=="Ruotsi" || country=="Norja" ||
            country=="Tanska" || country=="Islanti"){
        xmltvp::addCategory(node, "Pohjoismaisetohjelmat", xmltvp::fi);
    }



    //change time zone
    tm alkaa;
    memset(&alkaa, 0, sizeof(struct tm));
    alkaa = xmltvp::getStart(node);
    //alkaa.tm_hour += timeoffset + (autodst && kesaAika(alkaa) ? 1 : 0);

    timegm(&alkaa);
    xmltvp::setStart(node, alkaa);

    tm loppuu;
    memset(&loppuu, 0, sizeof(struct tm));
    loppuu = xmltvp::getStop(node);
    //loppuu.tm_hour += timeoffset + (autodst && kesaAika(loppuu) ? 1 : 0);

    timegm(&loppuu);
    xmltvp::setStop(node, loppuu);

    std::string kanava = xmltvp::getChannel(node);

    //shorten too long programmes



    //alfatv:n kategoriatiedot virheellisiä (News kaikissa)
    if(kanava=="alfatv.fi"){
        xmltvp::clearCategory(node);
    }

    //if(title=="Uutisikkuna" || title=="Astral-TV" || title=="Textnytt" ||
    //        title.compare("Yök­las­si­nen") == 0 || title=="Yön A­VA" ){
    if(title.find("Uutisikkuna") != std::string::npos
            || title.find("Textnytt") != std::string::npos
            || title.find("Astral-TV") != std::string::npos
            || title.c_str()[0] == 'Y'
            || title.find("n A­VA") != std::string::npos
            || title.find("Nattklassiskt") != std::string::npos
            ){

        xmltvp::addCategory(node, "Night");

    }else{
        //ylen kanavilla ei virheellisiä loppumisaikoja
        if(kanava!="tv2.yle.fi" && kanava!="tv1.yle.fi"){

            //std::cout << "ei yö-ohjelma"<< std::endl;

            unsigned int diffhour = (loppuu.tm_yday - alkaa.tm_yday)*24 +
                    (loppuu.tm_hour - alkaa.tm_hour);

            //if(diffhour>5 || (diffhour>2 && alkaa.tm_hour<6) || (diffhour>1 && alkaa.tm_hour>2 && alkaa.tm_hour<5)){
            //if(diffhour>6 || (diffhour>2 && (alkaa.tm_hour+diffhour/2)<6) || (diffhour>1 && alkaa.tm_hour>2 && alkaa.tm_hour<5)){
            if(diffhour>6){
                std::cout << "liian pitkä ohjelma: " << title << " (" << kanava << ")" << std::endl;
                loppuu=alkaa;
                loppuu.tm_hour += 1;
                timegm(&loppuu);
                xmltvp::setStop(node, loppuu);
            }
        }
    }

    //remove and add url
    std::string wurl, category;
    trans.newSearch();

    while(trans.getProgram(firstpart, wurl, category)){

        //jos ruutu-linkki, poistetaan aiemmat, jotta saadaan virallinen linkki ykköseksi
        /*
        if (wurl.find("ruutu.fi") != std::string::npos ||
                wurl.find("areena.yle.fi") != std::string::npos ||
                wurl.find("mtv.fi") != std::string::npos) {
            xmltvp::clearURL(node);
        }*/

        xmltvp::addURL(node, wurl.c_str());

        if(!category.empty()){
            xmltvp::addCategory(node, category.c_str(), xmltvp::fi);
        }

    }


    /*
    if(kanava=="33.dvb.guide"){
        xmltvp::setChannel(node, "tv2.yle.fi");
    }*/

    //vaihtaa sub_title -esityksen desciin
    //koska joskus virheellisesti laitetaan desc subtitleen

    xmltvp::clearSubTitle(node);

    if(desc.empty()){
        xmltvp::clearDesc(node);
        desc = subtitle;//xmltvp::getSubtitle(node);
        if(desc.size()>40){

            std::string subtitle = "";//bestPhrase(desc, &points);
            if(subtitle.length()==0){
                NatLan natlan(desc);
                //subtitle = first_sentence(desc);
                if(natlan.size()>0){
                    subtitle = natlan.at(0);
                }
            }
            if(subtitle.find("alkaa uusintana") != std::string::npos ||
                    subtitle.find(" 19") != std::string::npos ||
                    subtitle.find(" 20") != std::string::npos ||
                    subtitle.find("Kausi") != std::string::npos ||
                    subtitle.find("Jakso") != std::string::npos){
                subtitle = "";
            }

            if(subtitle.length()<60){
                //xmltvp::addDesc(node, desc.substr(subtitle.length()).c_str());
                eraseSubStr(desc, subtitle);
                xmltvp::addDesc(node, desc.c_str());
                xmltvp::addSubtitle(node, subtitle.c_str());
            }else{
                xmltvp::addDesc(node, desc.c_str());
            }
        }else{
            xmltvp::addSubtitle(node, subtitle.c_str());
        }
    }else{
        //std::string subtitle = xmltvp::getSubtitle(node);
        if(subtitle.empty()){

            std::string subtitle = "";//bestPhrase(desc, &points);
            if(subtitle.length()==0){
                //subtitle = first_sentence(desc);
                NatLan natlan(desc);
                //subtitle = first_sentence(desc);
                if(natlan.size()>0){
                    subtitle = natlan.at(0);
                }
            }
            if(subtitle.find("alkaa uusintana") != std::string::npos ||
                    subtitle.find(" 19") != std::string::npos ||
                    subtitle.find(" 20") != std::string::npos ||
                    subtitle.find("Kausi") != std::string::npos ||
                    subtitle.find("Jakso") != std::string::npos){
                subtitle = "";
            }

            if(subtitle.length()>1 && subtitle.length()<60){
                xmltvp::addSubtitle(node, subtitle.c_str());
                xmltvp::clearDesc(node);
                //xmltvp::addDesc(node, desc.substr(subtitle.length()).c_str());
                eraseSubStr(desc, subtitle);
                xmltvp::addDesc(node, desc.c_str());
            }
        }
        if(!subtitle.empty()){
            xmltvp::addSubtitle(node, subtitle.c_str());
        }
    }


    if(title.find("Yle Uutiset") != std::string::npos
            || title.find("Merisää") != std::string::npos
            || title.find("Yle Nyheter") != std::string::npos
            || title.find("Väder- och sjörapport") != std::string::npos
            || title.find("Merisää") != std::string::npos
            || title.find("Merisää") != std::string::npos
            || title.find("Merisää") != std::string::npos){
        xmltvp::addCategory(node, "News");
    }

    //Nelonen, Jim, Liv, Hero
    //muodossa "Kanadalainen realitysarja."

    if(desc.find("Kotimainen") != std::string::npos){
        xmltvp::addCategory(node, "Suomenkieliset", xmltvp::fi);
    }
    if(desc.find("Ruotsalainen") != std::string::npos){
        xmltvp::addCategory(node, "Ruotsinkieliset", xmltvp::fi);
    }
    if(desc.find("Amerikkalainen") != std::string::npos){
        xmltvp::addCategory(node, "Englanninkieliset", xmltvp::fi);
    }
    if(desc.find("Kanadalainen") != std::string::npos || desc.find("Kanadalais-amerikkalainen") != std::string::npos){
        xmltvp::addCategory(node, "Englanninkieliset", xmltvp::fi);
    }
    if(desc.find("Englantilainen") != std::string::npos || desc.find("Brittiläinen") != std::string::npos){
        xmltvp::addCategory(node, "Englanninkieliset", xmltvp::fi);
    }
    if(desc.find("Australialainen") != std::string::npos){
        xmltvp::addCategory(node, "Englanninkieliset", xmltvp::fi);
    }
    if(desc.find("Saksalainen") != std::string::npos){
        xmltvp::addCategory(node, "Muunkieliset", xmltvp::fi);
    }

    if(desc.find("telenovela") != std::string::npos){
        xmltvp::addCategory(node, "Draama", xmltvp::fi);
    }

    if(desc.find("draamasarja") != std::string::npos || desc.find("jännitysdraama") != std::string::npos){
        xmltvp::addCategory(node, "Draama", xmltvp::fi);
    }

    if(desc.find("rikossarja") != std::string::npos || desc.find("rikosdraama") != std::string::npos){
        xmltvp::addCategory(node, "Draama", xmltvp::fi);
    }

    if(desc.find("komediasarja") != std::string::npos || desc.find("toimintasarja") != std::string::npos){
        xmltvp::addCategory(node, "Draama", xmltvp::fi);
    }

    if(desc.find("keskusteluohjelma") != std::string::npos){
        xmltvp::addCategory(node, "Keskustelu", xmltvp::fi);
    }

    if(desc.find("musiikkiohjelma") != std::string::npos){
        xmltvp::addCategory(node, "Musiikki", xmltvp::fi);
    }

    if(desc.find("viihdeohjelma") != std::string::npos){
        xmltvp::addCategory(node, "Viihde", xmltvp::fi);
    }

    if(desc.find("dokumentaarinen sarja") != std::string::npos){
        xmltvp::addCategory(node, "dokumentti", xmltvp::fi);
    }

    if(desc.find("lifestyleohjelma") != std::string::npos){
        xmltvp::addCategory(node, "Lifestyle", xmltvp::fi);
    }

    if(desc.find("realitysarja") != std::string::npos || desc.find("realityohjelma") != std::string::npos || desc.find("ruokareality") != std::string::npos){
        xmltvp::addCategory(node, "Reality", xmltvp::fi);
    }

    if(desc.find("scifi-sarja") != std::string::npos || desc.find("tieteissarja") != std::string::npos){
        xmltvp::addCategory(node, "Draama", xmltvp::fi);
    }

    //TV5, Kutonen, TLC, Frii
    //muodossa "(Celebrity Motor Homes, viihde, USA, 2015)"

    if(desc.find(" lifestyle, ") != std::string::npos){
        xmltvp::addCategory(node, "lifestyle", xmltvp::fi);
    }

    if(desc.find(" reality, ") != std::string::npos){
        xmltvp::addCategory(node, "reality", xmltvp::fi);
    }

    if(desc.find(" tosi-tv, ") != std::string::npos){
        xmltvp::addCategory(node, "tosi-tv", xmltvp::fi);
    }

    if(desc.find("dokumentti, ") != std::string::npos || desc.find("dokumenttisarja, ") != std::string::npos){
        xmltvp::addCategory(node, "dokumentti", xmltvp::fi);
    }

    if(desc.find(", perhesarja, ") != std::string::npos || desc.find(", rikostarina, ") != std::string::npos){
        xmltvp::addCategory(node, "dokumentti", xmltvp::fi);
    }

    if(desc.find(" viihde, ") != std::string::npos){
        xmltvp::addCategory(node, "viihde", xmltvp::fi);
    }

    if(desc.find(" rikosdraama, ") != std::string::npos){
        xmltvp::addCategory(node, "draama", xmltvp::fi);
    }

    // sesonki:

    /*
    if((title.find("joulu") != std::string::npos
                || title.find("Joulu") != std::string::npos
                || desc.find("joulu") != std::string::npos
                || desc.find("Joulu") != std::string::npos
                || desc.find("Christmas") != std::string::npos
                || desc.find("Tiernapojat") != std::string::npos)
            && (alkaa.tm_mon == 11)
            && (title!="Uutisaamu" && title!="Emmerdale" && title!="Muumilaakson tarinoita" && title!="Simpsonit" && title!="Suomen huutokauppakeisari" && title!="Family Guy" && title!="Miehen puolikkaat" && title!="Frendit" && title!="Ensisilmäyksellä" && title!="Ykkösaamu")){
        xmltvp::addCategory(node, "joulu", xmltvp::fi);
    }*/

    if((title+subtitle+desc).find("Venäjä") != std::string::npos || (title+subtitle+desc).find("Ukraina") != std::string::npos
            || (title+subtitle+desc).find("Putin") != std::string::npos || (title+subtitle+desc).find("Nato") != std::string::npos
            || (title+subtitle+desc).find("eparatisti") != std::string::npos || (title+subtitle+desc).find("akote") != std::string::npos
            || (title+subtitle+desc).find("eparatismi") != std::string::npos || (title+subtitle+desc).find("akotte") != std::string::npos
            || (title+subtitle+desc).find("invaasio") != std::string::npos || (title+subtitle+desc).find("emilitarisoi") != std::string::npos){
        xmltvp::addCategory(node, "ukraina", xmltvp::fi);
    }

    /*
    if((title+desc).find("korona") != std::string::npos || desc.find("Korona") != std::string::npos
            || desc.find("covid") != std::string::npos || desc.find("pandemia") != std::string::npos){
        xmltvp::addCategory(node, "covid", xmltvp::fi);
    }

    if((title+desc).find("lympia") != std::string::npos && title!="Simpsonit" && title!="Ylen aamu"  && title!="Puoli seitsemän" && title!="Viiden jälkeen"){
        //xmltvp::addCategory(node, "Sport");
        xmltvp::addCategory(node, "olympialaiset", xmltvp::fi);
    }
    */
    if(title.find("rheilustudio") != std::string::npos || title.find("Jääkiekkostudio") != std::string::npos || title.find(". kisapäivä") != std::string::npos){
        xmltvp::addCategory(node, "urheilumakasiini", xmltvp::fi);
    }

    /*
    if(special==1){
        return alkaa.tm_mday!=10 || kanava!="tv2.yle.fi";
    }

    if(special==2){
        return alkaa.tm_mday != 9 || kanava != "fem.yle.fi";
    }*/

    //poistetaanko elementti? (jos parametrin määrätty toisiksi)
    return (startdate!="" && (xmltvp::getStartStr(node).find(startdate) != 0)) ||
            (channel!="" && kanava != channel);
}
