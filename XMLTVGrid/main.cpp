#include <iostream>
#include "../XMLTVp/access.h"
#include "../XMLWriter/xml_writer.hpp"
#include "weeklyrule.h"
#include <vector>
#include <algorithm>
#include <ohjelmanimi.h>
#include <chrono>
#include "../XMLTVtoHTML/datetime.h"

#define MAXPROGRAMS 2000

using namespace std;

WeeklyRule wr;

std::string entinen="";
bool lastAdded=false;
std::string nimi="";
//std::vector<std::string> names = {"Taikuuden tuolla puolen"};//, "Kymmenen Uutiset","Peter Nyman","Kauniit ja rohkeat","Huomenta Suomi","Myytinmurtajat"};
//std::vector<std::string> names = {"Rimakauhua ja rakkautta"};
//std::vector<std::string> names = {"Putous"};
//std::vector<std::string> names = {"Pitääkö olla huolissaan?"};
//std::vector<std::string> names = {"Huomenta Suomi"};
//std::vector<std::string> names = {"Sääntö-Suomi"};
//std::vector<std::string> names = {"Vanhan talon uusi elämä"};
//std::vector<std::string> names = {"Kotoisa"};
//std::vector<std::string> names = {"Kokkisota"};
//std::vector<std::string> names = {"Kauneuden hinta"};
//std::vector<std::string> names = {"Tulosruutu"};
//std::vector<std::string> names = {"Amazing Race"};
//std::vector<std::string> names = {"Meidän tarina"};
//std::vector<std::string> names = {"Päivän sää"};
//std::vector<std::string> names = {"Ostoskanava Tvins.com"};
//std::vector<std::string> names = {"60 päivää kiven sisässä"};
//std::vector<std::string> names = {"Duudsonit"};
//std::vector<std::string> names = {"Ostoskanava Tvins.com", "Ensitreffit alttarilla", "Meidän tarina"};
//std::vector<std::string> names = {"Salatut elämät"};

///std::vector<std::string> names = {"A-studio", "A-studio: Talk"};
//std::vector<std::string> names = {"Murhia ja kantrimusiikkia"};

//std::vector<Ohjelmanimi> names = {Ohjelmanimi("Kauniit ja rohkeat", "")};

//std::vector<std::string> names;
//std::vector<std::string> urls;
std::vector<Ohjelmanimi> names;

int totalpeitto=0;
int totalprogram=0;

std::string chan = "mtv3.fi";

//std::vector<std::string> names;

void addProgram(std::string title, xmlNode *node)
{
   // if(title=="Uuno Turhapuro menettää muistinsa"){
    //if(title=="Myytinmurtajat"){ //OK


    //if(title=="Peter Nyman"){ //OK
    //if(title=="Pitääkö olla huolissaan?"){
    //if(title=="Putous"){

    //if(title=="Puoli seitsemän"){

    //if(title=="Mitä tänään syötäisiin?"){ //OK
    //if(title=="Kauniit ja rohkeat"){ //OK
    //if(title=="Emmerdale"){ //OK
    //if(title=="Huomenta Suomi"){ //OK

    //if(title=="Tulosruutu"){ //OK
    //if(title=="Kymmenen Uutiset"){ //OK
    //if(title=="Salatut elämät"){

    //if(title=="A-studio"){

    if(lastAdded){
        wr.nextProgram(title);
    }

    if(title==nimi){

        tm alkaa = xmltvp::getStart(node);

        //std::cerr << xmltvp::getDesc(node) << std::endl;

        int episode = xmltvp::getEpisodenum(node).episode;

        if(episode==-1){
            //episode = (std::hash<std::string>()(xmltvp::getDesc(node)) % 900) + 99;
        }

        wr.addProgram(alkaa, xmltvp::getDesc(node),  entinen, "", lastAdded, episode);

        lastAdded=true;
    }else{
        lastAdded=false;
    }

    entinen=title;
}

void addProgramName(std::string title, xmlNode *node)
{
    if(names.size()<MAXPROGRAMS){
        Ohjelmanimi uusi(title, xmltvp::getURL(node));
        if (std::find(names.begin(), names.end(), uusi) == names.end()){
            if(title.length()<50){
                //names.push_back(title);
                //urls.push_back(xmltvp::getURL(node));
                names.push_back(uusi);
            }
        }
    }
}

void recognize()
{
    wr.etsiSuhteet();
    //wr.etsiNeliot();

    wr.etsiVerkot();
    //wr.laajennaYksi(0);
}

int tunnista(xmltvp::nodewithname manfun)
{
    bool arki=true;
    bool vl=true;

    char *out = "/home/jari/works/ohjelmat/Qt/XMLTVManipulate/out.xml";

    std::string dir = "/home/jari/works/lataukset/xmltv/manipuloidut/";

    //std::string chan = "tv1.yle.fi";
    //std::string chan = "sub.fi";
    //std::string chan = "nelonen.fi";


    /*
    if(arki){
        xmltvp::manipulateXML((dir+chan+"_2018-02-05.xml").c_str(), out, manfun);
        xmltvp::manipulateXML((dir+chan+"_2018-02-06.xml").c_str(), out, manfun);
        xmltvp::manipulateXML((dir+chan+"_2018-02-07.xml").c_str(), out, manfun);
        xmltvp::manipulateXML((dir+chan+"_2018-02-08.xml").c_str(), out, manfun);
        xmltvp::manipulateXML((dir+chan+"_2018-02-09.xml").c_str(), out, manfun);
    }

    if(vl){
        xmltvp::manipulateXML((dir+chan+"_2018-02-10.xml").c_str(), out, manfun);
        xmltvp::manipulateXML((dir+chan+"_2018-02-11.xml").c_str(), out, manfun);
    }

    if(arki){
        xmltvp::manipulateXML((dir+chan+"_2018-02-12.xml").c_str(), out, manfun);
        xmltvp::manipulateXML((dir+chan+"_2018-02-13.xml").c_str(), out, manfun);
        xmltvp::manipulateXML((dir+chan+"_2018-02-14.xml").c_str(), out, manfun);
        xmltvp::manipulateXML((dir+chan+"_2018-02-15.xml").c_str(), out, manfun);
        xmltvp::manipulateXML((dir+chan+"_2018-02-16.xml").c_str(), out, manfun);
    }

    if(vl){
        xmltvp::manipulateXML((dir+chan+"_2018-02-17.xml").c_str(), out, manfun);
        xmltvp::manipulateXML((dir+chan+"_2018-02-18.xml").c_str(), out, manfun);
    }

    if(arki){
        xmltvp::manipulateXML((dir+chan+"_2018-02-19.xml").c_str(), out, manfun);
        xmltvp::manipulateXML((dir+chan+"_2018-02-20.xml").c_str(), out, manfun);
        xmltvp::manipulateXML((dir+chan+"_2018-02-21.xml").c_str(), out, manfun);
        xmltvp::manipulateXML((dir+chan+"_2018-02-22.xml").c_str(), out, manfun);
        xmltvp::manipulateXML((dir+chan+"_2018-02-23.xml").c_str(), out, manfun);
    }

    if(vl){
        xmltvp::manipulateXML((dir+chan+"_2018-02-24.xml").c_str(), out, manfun);
        xmltvp::manipulateXML((dir+chan+"_2018-02-25.xml").c_str(), out, manfun);
    }
    */

    time_t now = time(0);
    tm date = *(gmtime(&now));

    //etsitään edeltävä maanantai
    while(date.tm_wday!=1){
        date.tm_mday-=1;
        mktime(&date);
    }

    for(int i=0;i<7*3;i++){
        std::string filename = dir+chan+"_"+format_date(date)+".xml";
        std::cerr << filename << std::endl;
        xmltvp::manipulateXML(filename.c_str(), out, manfun);
        date.tm_mday+=1;
        mktime(&date);
    }
}

int main(int argc, char* argv[])
{
    chan = argv[1];

    if(names.size()==0){
        tunnista(addProgramName);
    }

    sort(names.begin(), names.end());

    std::vector<std::string> rules;
    std::vector<double> peitot;

    for(int i=0;i<names.size();i++){
        std::cerr << names.at(i).getNimi() << std::endl;

        wr=WeeklyRule();
        lastAdded=false;
        nimi=names.at(i).getNimi();
        tunnista(addProgram);
        recognize();

        {
            double peitto = wr.getPeitto();
            std::string rule = wr.getRule();
            if(rule==""){
                peitto=0;
            }
            totalpeitto+=peitto;
            //peitot.push_back(peitto/((double)(wr.getProgramCount())));
            peitot.push_back(wr.getProgramCount());
            totalprogram+=wr.getProgramCount();
            //peitot.push_back(peitto);
            rules.push_back(rule);
        }
    }

    Writer writer(std::cout, true);

    /*
    writer.openElt("meta").attr("charset", "UTF-8").closeElt();
    writer.openElt("table");

    writer.openElt("th");
        writer.content("Ohjelman nimi");
    writer.closeElt();
    writer.openElt("th");
        writer.content("Esitysaika");
    writer.closeElt();
    writer.openElt("th");
        writer.content("Yhteensä");
    writer.closeElt();

    for(int i=0;i<names.size();i++){
        //std::cout << names.at(i) << ": "<< rules.at(i) << " (" << peitot.at(i) << ")" << std::endl;
            writer.openElt("tr");
                writer.openElt("td");
                if(names.at(i).getUrl()!=""){
                    writer.openElt("a");
                    writer.attr("href", names.at(i).getUrl().c_str());
                }
                    writer.content(names.at(i).getNimi().c_str());
                if(names.at(i).getUrl()!=""){
                    writer.closeElt();
                }
                writer.closeElt();
                writer.openElt("td");
                if(rules.at(i).size()<120){
                    writer.content(rules.at(i).c_str());
                }else{
                    writer.content("Useita epäsäännöllisiä lähetyksiä");
                }
                writer.closeElt();
                writer.openElt("td");
                //if(peitot.at(i)<1.0){
                    writer.content(std::to_string((unsigned int)peitot.at(i)).c_str());
                //}
                writer.closeElt();
            writer.closeElt();
    }
    writer.closeElt();
    writer.closeAll();
    */

    writer.openElt("programs");

    for(int i=0;i<names.size();i++){
        //std::cout << names.at(i) << ": "<< rules.at(i) << " (" << peitot.at(i) << ")" << std::endl;
        writer.openElt("program");

            writer.openElt("title");
            writer.content(names.at(i).getNimi().c_str());
            writer.closeElt();

            writer.openElt("sendingtimes");
            writer.content(rules.at(i).c_str());
            writer.closeElt();

            writer.openElt("count");
            writer.content(std::to_string((unsigned int)peitot.at(i)).c_str());
            writer.closeElt();

        writer.closeElt();
    }
    writer.closeElt();
    writer.closeAll();

    std::cerr << ((float)totalpeitto)/totalprogram<<std::endl;

    return 0;
}
