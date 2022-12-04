#include "writer_html.h"
#include "xmltv.h"
#include "../Rutiinit/colors.h"
#include "../Rutiinit/stringrutins.h"
#include "../Rutiinit/natlan.h"
#include <sstream>
#include <algorithm>

#define YLITUNTI    "Ylitunti"
#define MAXVARTTI   " Maxvartti"

#define QUADWIDE    "QuadWide"
#define HALFWIDE    "HalfWide"
#define SINGLEWIDE  "SingleWide"
#define DOUBLEWIDE  "DoubleWide"        //yli tunnin kestävät ja monipäiväiset

const char *short_day_names[] = {
    //"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
    "ma", "ti", "ke", "to", "pe", "la", "su"
};

const char *day_names[] = {
    //"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
    "maanantai", "tiistai", "keskiviikko", "torstai", "perjantai", "lauantai", "sunnuntai"
};

int ticksToHour(int ticks){    
    int hour=ticks/TICKSINHOUR+FIRSTHOUR;
    if(hour>23)
        hour-=24;
    return hour;
}

int ticksToMinute(int ticks){
    return (ticks % TICKSINHOUR) / TICKSLENGHT;
}

tm ticksToTm(int ticks){
    tm aika;
    aika.tm_hour = ticksToHour(ticks);
    aika.tm_min = ticksToMinute(ticks);
    return aika;
}

std::string vko("Vko ");

//std::string lastweek("Last week");
std::string lastweek("Viime vko");
std::string seriesbegins("(Sarja alkaa!)");
std::string seasonbegins("(Kausi alkaa!)");
std::string seriesends("(kausi loppuu)");
std::string str_firstshow("(1. es.)");
std::string str_rerun("(U)");

Html_table_writer::Html_table_writer(Writer &awriter, bool onlytable, int tabletype, int week, tm monday):
    writer(awriter),
    programmap((std::string("/home/jari/works/ohjelmat/Qt/XMLTVtoHTML/tables/ohjelmat-")+getChannel()+".xml").c_str())
{

    this->monday = monday;
    //´assert(monday.tm_mon>=5 && monday.tm_mon<=7);
    assert(this->monday.tm_year > 99);

    if(!onlytable){        
        writer.openElt("html");
        generateHeader();
        writer.openElt("h1").content(generateName().c_str()).closeElt();
    }

    writer.openElt("table");
    writer.attr("id", "tvtable");

    writer.openElt("colgroup");
    writer.openElt("col").closeElt();
    for(int d=0;d<DAYSINWEEK;d++){

        if(d<5){
            writer.openElt("col").attr("class", "DayColumn Week");
        }else{
            writer.openElt("col").attr("class", "DayColumn WeekEnd");
        }


        if(d==0){
            writer.attr("style", "border-left: 1px solid; border-left-color: gray;");
        }
        if(d==5){
            writer.attr("style", "border-left: 4px double; border-left-color: gray;");
        }
        if(d==6){
            writer.attr("style", "border-right: 1px solid; border-right-color: gray;");
        }
        writer.closeElt();
    }
    writer.closeElt();

    if(tabletype==WEEKTABLE)
        generateWeekDayHeader();
    else
        generateWeekHeader(week-3);

}

void Html_table_writer::start_row()
{
    writer.openElt("tr");
    //addTimeHeader(tick, 0);
}

void Html_table_writer::end_row()
{
    writer.closeElt();
}

std::string getTimeId(int tick){
    int d=tick / TICKSINDAY;
    int t=tick % TICKSINDAY;
    //std::string minutes(std::to_string((int)(int(t % TICKSINHOUR)/TICKSLENGHT)));
    /*
    std::string minutes(std::to_string(ticksToMinute(t)));
    if(minutes.length()==1)
        minutes="0"+minutes;
    //int hours = t / TICKSINHOUR + FIRSTHOUR;
    int hours = ticksToHour(t);
    if(hours>23){
        hours-=24;
    }
    */
    tm aika = ticksToTm(t);

    return std::string(short_day_names[d])+format_time(aika);
}

std::string Html_table_writer::getEpisodeStr(sendingGroup sg, sendingshape_t shape){

    //const std::string kausi = "S";
    //const std::string episodi = "E";

    const std::string kausi = " Kausi ";
    const std::string episodi = " Jakso ";
    const std::string episodit = " Jaksot ";

    std::string str_episode="";
    {
        if(sg.getJointEpisodes().at(0).season>0){
            str_episode+=kausi+std::to_string(sg.getJointEpisodes().at(0).season)+".";
        }else{
            /*
            str_episode+=program.director;
            if(program.year!=-1){
                if(str_episode.length()>4)
                    str_episode+=", ";
                str_episode+=std::to_string(program.year);
            }
            */
        }
    }

    std::string str_show="";
    Tick firstShown=-1;

    if(sg.getJointEpisodes().size()>0 && sg.getJointEpisodes().at(0).number>0){

        //jos on yhdistetty useita jaksoja
        if(sg.size()>1){
            if(sg.getJointEpisodes().at(sg.size()-1).number>0){
                str_episode+=episodit+std::to_string(sg.getJointEpisodes().at(0).number)+"-"+
                        std::to_string(sg.getJointEpisodes().at(sg.size()-1).number)+".";
            }

            bool isFirstShow=true;
            bool isRerun=true;

            for(unsigned int i=0;i<sg.size();i++){
                if(sg.getJointEpisodes().at(i).hasMultipleShows){
                    if(sg.getJointEpisodes().at(i).firstshow < shape.totaltime()){
                        //firstShown = (jointEpisodes.at(0).firstshow % TICKSINDAY)/TICKSINHOUR+FIRSTHOUR;
                        isFirstShow=false;

                        /*
                        //assert(((DAYSINWEEK-1) + (TICKSINDAY-1)) > jointEpisodes.at(0).firstshow);

                        if(((DAYSINWEEK-1) * (TICKSINDAY-1)) > jointEpisodes.at(0).firstshow){

                            if(jointEpisodes.at(0).firstshow>0){
                                str_show += short_day_names[jointEpisodes.at(0).firstshow / TICKSINDAY];
                            }else{
                                str_show += std::string(lastweek);
                            }
                        }
                        */

                    }else{
                        isRerun=false;
                        //firstShown=0;
                    }
                }
            }

            if(isFirstShow){
                str_show = " " + str_firstshow;
            }
            if(isRerun){
                str_show = " " + str_rerun + " ";
            }

        }else{
            str_episode+=episodi+std::to_string(sg.getJointEpisodes().at(0).number)+
                    (sg.getJointEpisodes().at(0).episodeTotal>0 ? "/"+std::to_string(sg.getJointEpisodes().at(0).episodeTotal):"")+".";


            if(sg.getJointEpisodes().at(0).hasMultipleShows){
                if(sg.getJointEpisodes().at(0).firstshow < shape.totaltime()){
                    firstShown = (sg.getJointEpisodes().at(0).firstshow);// % TICKSINDAY)/TICKSINHOUR+FIRSTHOUR;
                    str_show = std::string(" ") + str_rerun+" ";

                    /*
                    //assert(((DAYSINWEEK-1) + (TICKSINDAY-1)) > jointEpisodes.at(0).firstshow);

                    if(((DAYSINWEEK-1) * (TICKSINDAY-1)) > jointEpisodes.at(0).firstshow){

                        if(jointEpisodes.at(0).firstshow>0){
                            str_show += short_day_names[jointEpisodes.at(0).firstshow / TICKSINDAY];
                        }else{
                            str_show += std::string(lastweek);
                        }
                    }
                    */

                }else{
                    str_show = std::string(" ") + str_firstshow;
                    firstShown=0;
                }
            }
        }

        if(sg.getJointEpisodes().at(0).episodeTotal > 7){
            if(sg.getJointEpisodes().at(0).number == 1){
                if(sg.getJointEpisodes().at(0).season == 1){
                    str_episode+=" "+seriesbegins;
                }else{
                    str_episode+=" "+seasonbegins;
                }
            }
            if(sg.getJointEpisodes().at(sg.size()-1).number == sg.getJointEpisodes().at(0).episodeTotal){
                str_episode+=" "+seriesends;
            }
        }

    }

    if(str_episode!="")
        str_episode=std::string(" " + str_episode);

    return str_episode;

}

std::string Html_table_writer::formatweektime(tm start, bool space){
    return std::string(short_day_names[weekNumberInISO8601(start)])+(space?" klo ":"")+format_time(start);
}

std::string doSegment(const char *sentence)
{
  std::stringstream ss(sentence);
  std::string to;
  std::string ret="<p>";

  if (sentence != nullptr)
  {
    while(std::getline(ss,to,'\n')){
      //cout << to <<endl;
        ret+="</p><p>"+to;
    }
  }
  return ret+"</p>";

}

std::string Html_table_writer::addItem(sendingGroup sg, sendingshape_t shape, bool inshape){

    std::string pal;
    std::string sendingtimes = "";
    program_t program = sg.getProgram();

    if(programmap.mapOfProgram.find(program.title) != programmap.mapOfProgram.end()){
        programp::program valittu = programmap.mapOfProgram[program.title];
        sendingtimes = valittu.sendingtimes;
        if(sendingtimes.length()>200){
            sendingtimes="";
        }
    }

    //pal+="<div id=\""+getTimeId(shape.totaltime())+"\" class=\"overlay\">";
    pal+="<div id=\""+formatweektime(sg.getJointSendings().at(0).start, false)+"\" class=\"overlay\">";
        pal+="<div class=\"popup\">";


            /*
            pal+="<div class=\"modal-header "+program.categories+"\">";



            pal+="</div>";
            */


            pal+="<div class=\"content "+program.categories+"\">";

                pal+="<h2>"+program.title+"</h2>";

                /*pal+="<p>";*/

                if(program.getDesc().length()>2){
                    pal+="<br>";
                }

                pal+="<h3>"+getEpisodeStr(sg, shape)+"</h3>";

                for(int i=0;i<sg.getJointSendings().size();i++){
                    pal+="<h4>"+formatweektime(sg.getJointSendings().at(i).start, true)+" ";
                    std::string subtitle = sg.getJointEpisodes().at(i).subtitle;
                    pal+=subtitle;
                    pal+="</h4>";
                    std::string thisdesc = sg.getJointEpisodes().at(i).desc;
                    std::string part = thisdesc.substr(0, thisdesc.length()-program.getDesc().size());
                    pal+=doSegment(part.c_str());
                }


                //pal+=" <b>Kesto:</b> "+std::to_string(program.getKeskipituus())+" min. ";
                std::vector<std::string> tyypit;
                split(program.categories, ' ', tyypit);
                pal+="<b>Tyyppi:</b> ";
                for(unsigned int i=0; i<tyypit.size(); i++){
                    pal+=tyypit.at(i);
                    if(i<tyypit.size()-2){
                        pal+=", ";
                    }
                }

                if(!program.urls.empty()){
                    pal+=" <br><b>Linkit:</b> ";
                    for(std::string url : program.urls){
                        if(url.size()>0){
                            std::string sitename="";

                            if (url.find("fi.wikipedia.org") != std::string::npos) {
                                sitename="Wikipedia";
                            }

                            if (url.find("sv.wikipedia.org") != std::string::npos) {
                                sitename="Wikipedia (sv)";
                            }

                            if (url.find("areena.yle.fi") != std::string::npos) {
                                sitename="Yle Areena";
                            }else{
                                if (url.find("yle.fi") != std::string::npos) {
                                    sitename="Yle.fi";
                                }
                            }
                            if (url.find("ruutu.fi") != std::string::npos) {
                                sitename="Ruutu";
                            }
                            if (url.find("dplay.fi") != std::string::npos) {
                                sitename="Dplay";
                            }
                            if (url.find("svtplay.se") != std::string::npos) {
                                sitename="SVT Play";
                            }
                            if (url.find("tv4play.se") != std::string::npos) {
                                sitename="TV4 Play";
                            }
                            if (url.find("mtv.fi") != std::string::npos) {
                                sitename="mtv.fi";
                            }
                            if (url.find("facebook.com") != std::string::npos) {
                                sitename="Facebook";
                            }
                            if (url.find("twitter.com") != std::string::npos) {
                                sitename="Twitter";
                            }
                            if (url.find("thetvdb.com") != std::string::npos) {
                                sitename="TheTVDB.com";
                            }
                            if(sitename==""){
                                //sitename="Tuntematon";
                                sitename=url;
                            }

                            pal+="<b><a title=\""+url+"\" href=\""+url+"\" target=\"_blank\">"+sitename+"</a></b>, ";

                        }
                    }
                }

                /*pal+="</p>";*/
                //pal+="<i>"+sendingtimes+"</i>";






            pal+="<ul>";

            /*
            if(sg.getJointEpisodes().size()==1 && program.title.find("FIFAn jalkapallon MM 2018") != std::string::npos){

                program.url = "http://fi.wikipedia.org/wiki/Jalkapallon_maailmanmestaruuskilpailut_2018";
                pal+="<li><a href=\"https://yle.fi/urheilu/jalkapallon_mm\" target=\"_blank\">Ylen kisaopas</a></li>";

                std::string title = "";
                std::string url = "";

                if(sg.size()==1){

                    std::string desc = sg.getJointEpisodes().at(0).desc;

                    if (desc.find("A-lohko") != std::string::npos) {
                        url = "http://yle.fi/urheilu/3-10240019";
                        title = "Yle Urheilu: A-lohko on yksi jalkapallon MM-kisojen heikoimmista...";
                    }
                    if (desc.find("B-lohko") != std::string::npos) {
                        url = "https://yle.fi/urheilu/3-10241692";
                        title = "Jalkapallon MM-kisojen B-lohkoa tähdittävät suursuosikit...";
                    }
                    if (desc.find("C-lohko") != std::string::npos) {
                        url = "https://yle.fi/urheilu/3-10244167";
                        title = "C-lohkossa pannaan maaliverkot heilumaan";
                    }
                    if (desc.find("D-lohko") != std::string::npos) {
                        url = "https://yle.fi/urheilu/3-10246372";
                        title = "D-lohkossa pelaavat jalkapallon MM-kisojen mystisin mestarisuosikki, loppuun katsottu yllätyskortti ja Huuhkajien shokeeraama joukkue – Messin viimeinen ristiretki alkaa vastustajilla, jotka osaavat venyä";
                    }

                    if (desc.find("E-lohko") != std::string::npos) {
                        url = "https://yle.fi/urheilu/3-10246500";
                        title = "E-lohkossa vastassa viime kisojen yllättäjä, paikallaan polkeva keskikastilainen ja riitaisan valmentajavaihdon kokenut karsintamenestyjä";
                    }

                    if (desc.find("F-lohko") != std::string::npos) {
                        url = "https://yle.fi/urheilu/3-10246544";
                        title = "F-lohko on yksi jalkapallon MM-kisojen mehukkaimmista";
                    }
                    if (desc.find("G-lohko") != std::string::npos) {
                        url = "http://yle.fi/urheilu/3-10249380";
                        title = "Jalkapallon MM-kisojen G-lohko kiehtoo arvoituksellisuudellaan";
                    }

                    if (desc.find("H-lohko") != std::string::npos) {
                        url = "https://yle.fi/urheilu/3-10251858";
                        title = "Jalkapallon MM-kisojen tasaisin lohko pelataan kirjaimella ";
                    }

                    if(url.length()>0){
                        pal+="<li><a href=\""+url+"\" target=\"_blank\">Yle Urheilu: "+title+"</a></li>";
                    }
                }
            }
            */



            pal+="</ul></div>";

            pal+="<p><div class=\"btn-group\">";

            pal+="<a class=\"button\" href=\"#!\" title=\"Sulje tämä ikkunan\"><img class=\"icon\" src=\"../../logot/back.png\"> Takaisin</a>";

            for(std::string url : program.urls){
                if (url.find("areena.yle.fi") != std::string::npos ||
                        url.find("ruutu.fi")  != std::string::npos ||
                        url.find("dplay.fi")  != std::string::npos ||
                        url.find("alfatv.fi")  != std::string::npos ||
                        url.find("svtplay.se")  != std::string::npos ||
                        url.find("tv4play.se")  != std::string::npos ||
                        url.find("mtv.fi")    != std::string::npos) {
                    pal+=std::string("<a class=\"button\" ")
                            +"title=\"Avaa ohjelma suoratoistopalvelusta\""
                            +"href=\""+url+"\" target=\"_blank\"><img class=\"icon\" src=\"../../logot/cinema.png\"> Tallenteet</a>";
                    break;
                }
            }

            if(inshape){
                //pal+=std::string("<a class=\"button\" href=\"webcal://www.ohjelmakartta.fi/ical.php?id=201806062300")+
                pal+=std::string("<a class=\"button\" ")
                        +"title=\"Lisää ohjelman lähetysaika kalenteriisi\" "
                        +"href=\"https://www.ohjelmakartta.fi/ical.php?id=201806062300"
                        "&title="+program.title+
                        "&description="+sg.getJointEpisodes().at(0).desc+
                        "&channel="+getChannel()+
                        "&datestart="+format_date_ical(sg.getJointSendings().at(0).start)+
                        "&dateend="+format_date_ical(sg.getJointSendings().at(0).end)+
                        "&count="+std::to_string(shape.colspan+1)+"\" target=\"_blank\"><img class=\"icon\" src=\"../../logot/calendar.png\"> iCalendar</a>";
            }


            pal+="</p>";

            //pal+="<p>(Sulje tämä ikkuna painamalla selaimesi takaisin-nappia.)</p>";

            pal+="</div>";
        pal+="</div>";
    pal+=std::string("</div>")+"\n";

    return pal;
}

//viimeinen aika
//std::string aika_last_std="";


//viimeinen sarake, jossa aika on ilmoitettu
int lastcolumn=-1;

int lastrow=-1;

void Html_table_writer::addCell(sendingGroup sg, sendingshape_t shape, sendingGroup sgdaily)
{

    //assert(((DAYSINWEEK-1) + (TICKSINDAY-1)) > totaltime);

    program_t program = sg.getProgram();
    std::string vakiintunut = "";
    std::string title = program.title;

    list+=addItem(sgdaily, shape, sg.size() == sgdaily.size());

    std::string aika_std;
    if(sg.size()>0){
        /*
        aika_std = format_time(sg.getJointSendings().at(0).start);
        if(aika_std==aika_last_std && lastcolumn==shape.d-1){
            aika_std="";
            lastcolumn=shape.d+shape.colspan;
        }else{
            aika_last_std=aika_std;
            lastcolumn=shape.d+shape.colspan;            
        }
        */
        //if(lastrow!=shape.t || lastcolumn!=shape.d-1){ // Merkitse ajat vain johonkin ohjelmiin
            if(sg.getJointSendings().at(0).sametime){
                //aika_std += "~";
            }
            //aika_std += format_time(sg.getJointSendings().at(0).start);

            //tm aika = ticksToTm(shape.t);
            tm aika = sg.getJointSendings().at(0).start;

            //aika.tm_hour = ticksToHour(shape.t);
            //aika.tm_min = ticksToMinute(shape.t);
            //aika.tm_min = shape.t;
            //aika_std += format_time(sg.getJointSendings().at(0).start);
            aika_std += format_time(aika);

            lastrow=shape.t;
        //}
        lastcolumn=shape.d+shape.colspan;
    }

    std::string maara_std;
    //jos lähetyksiä peräkkäin, samana päivänä
    /*
    if(sg.size() > shape.colspan+1){
        maara_std = "(x"+std::to_string(sg.getJointSendings().size())+")";
    }
    */

    std::string str_summary;
    bool saannollinen=false;


    /*
    if(sg.size()==1){
        //str_subtitle=first_sentence(jointEpisodes.at(0).desc);
        str_subtitle=sg.getJointEpisodes().at(0).subtitle;
    }
    */

    /*
    for(int i=0;i<sg.size();i++){
        if(sg.getJointEpisodes().at(i).subtitle.length()>0){
            str_subtitle+=sg.getJointEpisodes().at(i).subtitle+" - ";
        }
    }
    */

    /*
    //ohjelman yleiskuvaus
    if(sg.getProgram().getDesc().length()<40 || shape.colspan>0){
        str_desc = sg.getProgram().getDesc();
    }
    */


    /*
    {
        std::string last="";
        for(int i=0;i<jointEpisodes.size();i++){
            if(jointEpisodes.at(i).subtitle.length()>0 && jointEpisodes.at(i).subtitle != last){
                if(last.length()>0)
                    str_subtitle+=" | ";
                str_subtitle+=jointEpisodes.at(i).subtitle;
                last=jointEpisodes.at(i).subtitle;
            }
        }
    }
    */

    std::string str_filmfacts="";

    //decide proper size for program label
    std::string sizeclass="";
    bool showepisode=false;
    bool showsubtitle=false;
    bool showtime=true;

    const unsigned int minutes = shape.rowkesto * (60/TICKSINHOUR);// * (shape.colspan+1) / 2;

    //vikonloppuun lisätään vähän enemmän tilaa
    const int left = std::max((int)shape.d, 5);
    const int right = std::min(((int)shape.d) + ((int)shape.colspan), 6);
    const unsigned int vllisa = std::max(right - left + 1, 0);

    //const double char_per_minute = (ticksToHour(shape.t) < 16 ? CHARPERMINUTE : CHARPERMINUTE_EVENING);
    //const double char_per_minute = (ticksToHour(shape.t + shape.rowkesto * 0.5) < 16 ? CHARPERMINUTE : CHARPERMINUTE_EVENING);
    const int avghour = ticksToHour(shape.t + shape.rowkesto * 0.5);
    const double char_per_minute = (avghour >= 6 ? ((avghour <= 16 || avghour >= 23) ? CHARPERMINUTE : CHARPERMINUTE_EVENING) : CHARPERMINUTE_NIGHT)*1.05;

    const unsigned int totalsize = shape.rowkesto * (60/TICKSINHOUR) * (shape.colkesto + vllisa * 0.23) * char_per_minute;
    const int rivileveys = shape.colkesto*30;
    //const int titletila = ((title.size()+7) / rivileveys + 1) * rivileveys;
    const int titletila = ((title.size()+7) / rivileveys + 1) * rivileveys;
    int sizeleft = totalsize - titletila;

    std::string desc = sg.getJointEpisodes().at(0).desc;
    NatLan nsfirstdesc(desc);



    if(true){

        //int minutes = program.getKeskipituus();


        //assert(minutes<=60*24);

        //cut too long words
        //if(minutes<10 && shape.colspan<1)
            //if(program.title.length()>15)
            //    program.title=program.title.substr(0,13)+"...";

        int wordlenght = 5; //longest_word(program.title);

        /*
        if(minutes<10){
            sizeclass+=" ";
            sizeclass+=HALFWIDE;
        }else{
            if((minutes<15 || wordlenght>12) && shape.colspan<1){
                sizeclass+=" ";
                if(wordlenght>16){
                    sizeclass+=SINGLEWIDE;
                }else{
                    sizeclass+=HALFWIDE;
                }
            }
        }
        */

        //if(minutes<45)
        //    sizeclass+=" SingleWide";

        /*
        if(program.title=="Korean olympialaiset" || program.title==""){
            showsubtitle=true;
            showepisode=true;
        }

        if(program.title=="Yleisurheilun EM"){
            showsubtitle=true;
            showepisode=true;
        }
        */


        //jos kesto väh. 10 minuuttia eikä yöllä
        if(minutes>10 && (ticksToHour(shape.t)<=1 || ticksToHour(shape.t)>=8)){ //jos ei yö

            showepisode=true;

            /*
            if(((str_subtitle+str_desc).length()+20)<minutes*(shape.colspan+1)){
                showsubtitle=true;
            }
            */
            if(minutes>=20){
                //showsubtitle=true;
            }
            if(ticksToHour(shape.t)>=20 && ticksToHour(shape.t)<=21){
                showsubtitle=true;
            }
            if(shape.d > 4 && ticksToHour(shape.t)>=17 && ticksToHour(shape.t)<=22){
                showsubtitle=true;
            }


        }

        /*
        if(longest_word(str_subtitle+" "+str_desc)>20 && shape.colspan<1){
            //showsubtitle=false;
            //str_desc="";
        }
        */
        /*
        if(minutes>40)
            showepisode=true;

        if(minutes>80)
            showsubtitle=true;
        */


        //movies
        if(false && program.getKeskipituus()>75 && sg.getJointEpisodes().size()==1){

            sizeclass+=" ";
            sizeclass+=YLITUNTI;

            if(program.year>0){
                //str_year = " (" + std::to_string(program.year) + ")";
                str_filmfacts = std::to_string(program.year);
            }

            for(unsigned long ph = 0; ph < nsfirstdesc.size(); ph++){

                const std::string phrase = nsfirstdesc.at(ph);

                if(str_filmfacts.length()==4 && phrase.find(str_filmfacts) != std::string::npos){

                    str_filmfacts = phrase;
                    nsfirstdesc.remove(ph);
                    break;

                }else if((phrase.find("(") != std::string::npos
                          || phrase.find("Ohjaaja") != std::string::npos
                          || phrase.find("Ohjaus") != std::string::npos
                          || phrase.find("Yhdysvallat") != std::string::npos
                          || phrase.find("USA") != std::string::npos
                          || phrase.find("Suomi") != std::string::npos
                          || phrase.find("Ruotsi") != std::string::npos
                         )&&(phrase.find(" 19") != std::string::npos
                          || phrase.find(" 20") != std::string::npos
                         )){

                    str_filmfacts = phrase;
                    nsfirstdesc.remove(ph);
                    break;

                }
            }
        }

        if(!vakiintunut.empty() && lastTitles[shape.d]==vakiintunut){
            sizeclass+=" ";
            //sizeclass+=MAXVARTTI;
        }

        //muodostetaan yhteenveto
        if(sg.size()>1){

            for(int i=1; i<sg.getJointEpisodes().size(); i++){
                NatLan other(sg.getJointEpisodes().at(i).desc);
                //desc=commonPhrases(desc, sg.getJointEpisodes().at(i).desc);
                nsfirstdesc = nsfirstdesc.commonPhrases(other);
            }

            str_summary = nsfirstdesc.toString();
        }

        //onko päivittäin samanlaisena toistuva taulukko?
        if(shape.colkesto > 1 && sg.size() > shape.colkesto && (sg.size() % shape.colkesto) == 0){
            unsigned long korkeus = sg.getJointEpisodes().size()/shape.colkesto;
            for(unsigned int i=0; i<korkeus; i++){
                assert(shape.colkesto-1 > 0);
                assert(korkeus>0);
                for(unsigned int d=1; d<shape.colkesto; d++){
                    if(sg.getJointSendings().at(i).start.tm_hour != sg.getJointSendings().at(i+korkeus*d).start.tm_hour){
                        goto eisaannollinen;
                    }
                    if(sg.getJointSendings().at(i).start.tm_min != sg.getJointSendings().at(i+korkeus*d).start.tm_min){
                        goto eisaannollinen;
                    }
                    if(sg.getJointSendings().at(i).episodeId != sg.getJointSendings().at(i+korkeus*d).episodeId){
                        goto eisaannollinen;
                    }
               }

            }
            saannollinen=true;
        }
        eisaannollinen:

        if(minutes>=25 && desc.size()>3){

            //0.1 == 10 minuuttia saa yhden rivin

            float riviaMinuutti = 0.02f;
            /*
            if(shape.d > 4){
                riviaMinuutti=0.04;
            }

            if(ticksToHour(shape.t)>=17 && ticksToHour(shape.t)<=22){
                riviaMinuutti=0.06;
            }

            if(ticksToHour(shape.t)>=19 && ticksToHour(shape.t)<=21){
                riviaMinuutti=0.08;
            }
            */
            //if((shape.d < 5 && ticksToHour(shape.t)>=20 && ticksToHour(shape.t)<=21) ||
            //        ( shape.d > 4 && ticksToHour(shape.t)>=17 && ticksToHour(shape.t)<=22)){

                //str_desc = first_sentence(sg.getJointEpisodes().at(0).desc);

            int sublenght = sg.getJointEpisodes().at(0).subtitle.length();
            assert(sublenght>=0);
            int titlelenght = program.title.size()*(15.0/30.0);//*(30.0/20.0);
            assert(titlelenght>=0);

            int prosleveys;
            if(shape.d < 5){
                prosleveys=13;
            }else{
                prosleveys=17;
            }

            //unsigned int maxlen = 100000;//prosleveys*2.3*minutes*riviaMinuutti*sg.getJointEpisodes().size()-sublenght-titlelenght;


            //str_desc = shorten_text(desc, sizeleft);

                //str_desc = sg.getJointEpisodes().at(0).desc;


            //if(minutes>70){
            //    str_desc = first_sentence(sg.getJointEpisodes().at(0).desc);
                //str_desc = sg.getJointEpisodes().at(0).desc;
            //}
        }

        //movies
        if(program.getKeskipituus()<20){
            //sizeclass+=MAXVARTTI;
        }

        if(DEBUGLABELS && shape.rowkesto * (60/TICKSINHOUR) < 20){
            sizeclass+=MAXVARTTI;
        }

        if(ticksToHour(shape.t)<6){
            //sizeclass+=MAXVARTTI;
        }

        /*
        //daily series
        if(minutes>20 && shape.colspan>1){
            sizeclass+=" ";
            sizeclass+=DOUBLEWIDE;
        }

        if(minutes>55 && shape.colspan>1){
            sizeclass+=" ";
            sizeclass+=QUADWIDE;
        }
        */
    }

    if(program.categories == "None"){
        showtime=false;
    }



    /*
    std::string sarjaalkaa="";
    if(sg.getJointEpisodes().at(0).number==1 /*&& (
                sg.getJointEpisodes().at(0).episodeTotal==0
                || sg.getJointEpisodes().at(0).episodeTotal>2
                || desc.find("SARJA ALKAA!") != std::string::npos
                || desc.find("UUSI SARJA!") != std::string::npos) /){
        sarjaalkaa="sarjaalkaa ";
    }
    */

    if(title == "Jalkapallon naisten MM" && sg.getJointEpisodes().size()==1){
        vakiintunut = title;
        title = sg.getJointEpisodes().at(0).subtitle;
        eraseSubStr(title, "FIFAn jalkapallon naisten MM:");
        eraseSubStr(title, "FIFAn jalkapallon naisten MM");
        sizeclass=YLITUNTI;
    }

    if(title == "SM-viikko" && sg.getJointEpisodes().size()==1){
        vakiintunut = title;
        title = sg.getJointEpisodes().at(0).subtitle;
        eraseSubStr(title, "SM-viikko:");
        sizeclass=YLITUNTI;
    }

    if(title == "Yle Live" && sg.getJointEpisodes().size()==1){
        vakiintunut = title;
        title = sg.getJointEpisodes().at(0).subtitle;
        sizeclass=YLITUNTI;
    }

    if(title == "Disney esittää:" && sg.getJointEpisodes().size()==1){
        vakiintunut = title;
        title = sg.getJointEpisodes().at(0).subtitle;
        sizeclass=YLITUNTI;
    }

    /*
    if((title == "Yle Uutiset selkosuomeksi"
        || title == "Yle Oddasat"
        || title == "Novosti Yle"
        || title == "Yle Uutiset viittomakielellä"
        || title == "Yle Uutiset"
        || title == "Yle Uutiset Alueeltasi"
        ) //&& sg.getJointEpisodes().size()==1
            ){
        //vakiintunut = title;
        title="Uutisohjelmia";
        //title = sg.getJointEpisodes().at(0).subtitle;
        //sizeclass=YLITUNTI;
    }
    */


    if(program.belongsToCategory("Ruotsinkieliset")){
        //vakiintunut = "Yle Fem:";
    }
 /*
    if(program.belongsToCategory("Pohjoismaisetohjelmat") && program.belongsToCategory("Asia")){
        vakiintunut = "Pohjoismaiset";
    }


    if(program.belongsToCategory("Lastenohjelmat")
            || program.belongsToCategory("Kids")
            || program.belongsToCategory("lastenohjelmat")){
        vakiintunut = "Lastenohjelmia:";
    }*/

    /*
    if(title.find("007") != std::string::npos || title.find("Casino Royale") != std::string::npos){
        vakiintunut = "James Bond:";
    }
    */

    if(title.find("Subleffa:") != std::string::npos || title.find("#Subleffa:") != std::string::npos){
        vakiintunut = "Subleffa:";
        title=after(title,": ");
    }

    if(title.find("Pekingin olympiakisat, ") != std::string::npos){
        vakiintunut = "Pekingin olym.";
        title=after(title,", ");
    }

    if(title.find("Tokion olympialaiset: ") != std::string::npos){
        vakiintunut = "Tokion olympialaiset:";
        title=after(title,": ");
    }

    if(title.find("Tulossa:") != std::string::npos){
        vakiintunut = "Tulossa...";
        title="";//after(title,": ");
    }

    if(title.find("EM-viikko München 2022, ") != std::string::npos || title.find("EM-viikko 2022 München, ") != std::string::npos){
        vakiintunut = "EM-viikko:";
        title=after(title, ", ");
    }

    std::vector<std::string> parts;
    split(title.c_str(), ':', parts);
    if(parts.size()>=2 && parts.size()<=3){
        if(parts.at(0)=="Arkisto"
                //|| parts.at(0)=="#Subleffa"
                //|| parts.at(0)=="Disney esittää"
                //|| parts.at(0)=="Disneyn esikoulu"
                || parts.at(0)=="Avara luonto"
                || parts.at(0)=="Prisma"
                || parts.at(0)=="Ulkolinja"
                || parts.at(0)=="Historia"
                || parts.at(0)=="Dokumenttiprojekti"
                || parts.at(0)=="Yle Live"
                || parts.at(0)=="FIFAn jalkapallon naisten MM"
                || parts.at(0)=="Kino"
                || parts.at(0)=="Kino Klassikko"
                || parts.at(0)=="Uusi Kino"
                || parts.at(0)=="Dok"
                || parts.at(0)=="Liv D"
                || parts.at(0)=="AVA rikosputki"
                || parts.at(0)=="Dok"
                || parts.at(0)=="SM-viikko 2020"
                || parts.at(0)=="SM-viikko 2021"

                || parts.at(0)=="Kiveen hakatut"
                || parts.at(0)=="Radioteatteri esittää"
                || parts.at(0)=="Elokuva"

                || parts.at(0)=="Olympialaiset"

                //|| parts.at(0)=="LIVE"
                ){
            vakiintunut=parts.at(0)+":";
            if(parts.size()==2){
                title=parts.at(1);
            }else{
                title=parts.at(1) + ": " + parts.at(2);
            }
        }
    }

    if(vakiintunut=="" && ((program.belongsToCategory("elokuvat")) || program.belongsToCategory("movie") || program.belongsToCategory("(Elokuvateatteri)elokuva"))){
        vakiintunut = "elokuva:";
        if(program.belongsToCategory("Lapset")){
            vakiintunut = "lastenelokuva:";
        }
        if(program.belongsToCategory("kotimaiset") || program.belongsToCategory("Kotimaisetelokuvat")){
            vakiintunut = "kotimainen elokuva:";
        }
        /*if(program.belongsToCategory("Ulkomaisetelokuvat") || program.belongsToCategory("ulkomaiset")){
            vakiintunut = "ulkom. elokuva:";
        }*/
    }



    /**************************************************************************************************/

    writer.openElt("td");

    //writer.attr("id", getTimeId(shape.totaltime()).c_str());

    if(shape.colspan>0){
        writer.attr("colspan", std::to_string(shape.colspan+1).c_str());
    }

    if(shape.rowspan>0){
        writer.attr("rowspan", std::to_string(shape.rowspan+1).c_str());
    }

    if(CATCOLORS){
        //writer.attr("class", (program.categories+(shape.t % (TICKSINHOUR)==0 ? " hour" : "")).c_str());
        writer.attr("class",                
                ("programTd "+
                 program.categories+
                (shape.colspan>0 ? " colspan" : "")+
                ((lastTitles[shape.d]!=vakiintunut) ? " vakalkaa" : "")+
                ((vakiintunut!="" && (lastTitles[shape.d]==vakiintunut)) ? " jatkuu" : "")+
                //(lastTitles[shape.d]=="Yle Uutiset 20.30" && title == "Urheiluruutu" ? " jatkuu" : "")
                (shape.raja==0 ? " jatkuu" : "")+
                (shape.raja==2 ? " katko" : "")+
                ((vakiintunut!="") ? " vak" : "")+
                ((shape.colspan + shape.d * DAYSINWEEK)==lastSpans[shape.d] ? " secondspan" : " firstspan")+                
                ((sg.getJointSendings().at(0).sametime || sg.getJointSendings().back().sametime)  ? " sametime"  : "")+
                ((sg.getJointSendings().at(0).prevDaySending)  ? " dailyleft"  : "")+
                ((sg.getJointSendings().at(sg.size()-1).nextDaySending) ? " dailyright" : "")
                ).c_str());
    }else{
        if(program.count>0){
            int hue = std::hash<std::string>()(program.title) % 360;
            hsv vari = {static_cast<double>(hue), 0.2, 0.98};
            rgb varirgb=hsv2rgb(vari);
            writer.attr("style", std::string("background-color: #") + rgb2Hex(varirgb) + ";");
        }
    }

    if(program.isVisible){




        //linkki alkaa
        writer.openElt("a");
        //writer.attr("target", "_blank");
        //writer.attr("href", std::string("#") + getTimeId(shape.totaltime()));
        writer.attr("href", std::string("#") + formatweektime(sgdaily.getJointSendings().at(0).start, false));


        if(DEBUGLABELS){
            writer.attr("title", (std::to_string(sizeleft)+"/"+std::to_string(totalsize)).c_str());
            //writer.attr("title", (std::to_string(ticksToHour(shape.t + shape.rowkesto * 0.5))).c_str());
            //writer.attr("title", (std::to_string(shape.colkesto + vllisa * 0.23)).c_str());


            /*
            writer.openElt("i");
            //writer.direct(std::to_string(showepisode).c_str());
            writer.direct(std::to_string(totalsize).c_str());
            writer.direct("|");
            writer.direct(std::to_string(sizeleft).c_str());
            //writer.direct(std::to_string(vllisa).c_str());
            writer.closeElt();
            */
        }

        writer.openElt("div");
        writer.attr("class", "Program");


        {

            writer.openElt("span");
            writer.attr("class", "ProgramTitle");

            //program title
            //if(program.url.length()>0){

            //}

            //finnishHyphenation(str_desc, "&shy;").c_str()
            //writer.content((aika_std + " "+program.title + " " + maara_std).c_str());


            if((aika_std.length()>0 && showtime)){
                writer.openElt("span")
                        .attr("class", "ProgTime")
                        .content(aika_std.c_str())
                        .closeElt();

            }else{
                //aika_last_std="";
                //writer.openElt("span").attr("class", "ProgTimeSmall").content(">").closeElt();
            }

            if(!vakiintunut.empty()){
                if(program.getKeskipituus()<=75){
                    //sizeclass=MAXVARTTI;
                }
                if (lastTitles[shape.d]!=vakiintunut){
                    writer.openElt("span").attr("class", std::string("ProgTitle vakiintunut"));
                    //writer.direct((" "+finnishHyphenation(program.title, "&shy;") + " " + maara_std).c_str());
                    writer.direct((" "+finnishHyphenation(vakiintunut, "&shy;")).c_str());
                    writer.closeElt();

                    writer.closeElt();
                    writer.openElt("span");
                    writer.attr("class", "ProgramTitle");
                    sizeleft-=rivileveys;
                }
            }


            //if(program.url.length()>0){

            //}




            //if(lastTitles[shape.d]!=program.title){
                writer.openElt("span").attr("class", std::string("ProgTitle ")+(vakiintunut.empty()?"":" varsinainentitle")+sizeclass.c_str());
                writer.direct((" "+finnishHyphenation(title, "&shy;") + " " + maara_std).c_str());
                writer.closeElt("span");
            //}


            writer.closeElt("span");
        }


        //lastTitles[shape.d]=vakiintunut;

        for(int di=0;di<=shape.colspan;di++){
            lastSpans[shape.d+di]=shape.colspan+shape.d*DAYSINWEEK;
            lastTitles[shape.d+di]=title;
            //lastPrograms[shape.d+di] = program;
        }


        //episode and references its other shows
        //if(str_subtitle.size()>3 || str_desc.size()>3 ||
        //        (sg.getJointEpisodes().size()>0 && sg.getJointEpisodes().at(0).subtitle.length()>3) ||
         //       str_year.size()>3){

            //if(showepisode){

                /*
                writer.openElt("span").attr("class", "ProgramSeason");
                writer.content(str_episode.c_str());
                //if(firstShown!=-1){
                    writer.content(str_show.c_str());

                //}
                writer.closeElt();
                */

                //if(showsubtitle){
                    writer.openElt("div").attr("class", "ProgDesc");

                    /*
                    if(jointEpisodes.size()==1 && jointEpisodes.at(0).url.length()>0){
                        writer.openElt("a");
                        writer.attr("href", jointEpisodes.at(0).url);
                    }

                    writer.content(str_subtitle.c_str());

                    if(jointEpisodes.size()==1 && jointEpisodes.at(0).url.length()>0){
                        writer.closeElt();
                    }
                    */

                    //if(jointEpisodes.size()==1){

                        // Yhteenveto jaksoista

                        writer.content(" ");

                        if(str_summary.size()>0 && sizeleft>0 && sg.size()>1){
                            std::string shorted = shorten_text(str_summary, sizeleft);
                            if(shorted.length()>10){
                                writer.openElt("span").attr("class", "Summary");
                                writer.direct(finnishHyphenation(" " + shorted, "&shy;").c_str());
                                writer.closeElt("span");
                                sizeleft-=shorted.length();
                            }
                        }

                        const int sizeleft_beforeepisodes = sizeleft;

                        writer.content(" ");


                        std::string lastSubtitle="";
                        std::string lastDesc="";

                        //for(int i=0;i<sg.getJointSendings().size();i++){
                            //pal+="<p>"+std::string("<b>")+formatweektime(sg.getJointSendings().at(i).start, true)+" ";

                        int wday=-1;

                        //päälläkäisten episodien määrä ohjelmalaatikon listassa
                        const unsigned long korkeus = (saannollinen ? sg.getJointEpisodes().size()/shape.colkesto : sg.size());

                        //if(sizeleft> 0 && sizeleft > 10 * shape.colkesto){
                        if(sizeleft> 0 && sizeleft > 35 * korkeus){


                            writer.openElt("div");
                                if(korkeus>1){
                                if(shape.colkesto>3){
                                    writer.attr("class", "sublist newspaper");
                                }else{
                                    writer.attr("class", "sublist");
                                }
                            }

                            for(unsigned int i=0;i<korkeus;i++){

                                int tila = sizeleft_beforeepisodes / korkeus;
                                const std::string subtitle = shorten_text(sg.getJointEpisodes().at(i).subtitle, tila);

                                NatLan descphrases(sg.getJointEpisodes().at(i).desc);
                                if(i==0){
                                    descphrases = nsfirstdesc;
                                }

                                std::string desc="";
                                for(unsigned long ph=0; ph<descphrases.size(); ph++){
                                    const std::string phrase = descphrases.at(ph);

                                    if(phrase.find("Kausi ") == std::string::npos &&
                                            phrase.find("Jakso ") == std::string::npos &&
                                            phrase.find("/") == std::string::npos &&
                                            str_summary.find(phrase) == std::string::npos){
                                        desc+=phrase+" ";
                                    }
                                }

                                bool uusisub=true;
                                bool uusidesc=true;

                                if(lastSubtitle == subtitle || subtitle.length()<3){
                                    uusisub = false;
                                }
                                if(lastDesc == desc || desc.length()<3 || desc.find("Kausi ") != std::string::npos){
                                    uusidesc = false;
                                }

                                lastSubtitle = subtitle;
                                lastDesc = desc;

                                //int tila = ((int)(sg.getJointSendings().at(i).kesto())) - subtitle.length();

                                //int tila = sizeleft * ((int)(sg.getJointSendings().at(i).kesto())*char_per_minute) / ((double)sizeleft_beforeepisodes);





                                if(sg.getJointSendings().at(i).start.tm_wday != wday || shape.colspan==0 || saannollinen){
                                    wday = sg.getJointSendings().at(i).start.tm_wday;

                                    writer.openElt("div").attr("class", "ProgSubTitleDay");

                                    if((uusisub) || (tila>0 && shorten_text(desc, tila).size() > 3)){
                                        if(shape.colspan>0 && !saannollinen){
                                            writer.openElt("span").attr("class", "ProgSubTitleDayTitle").
                                                    content(short_day_names[weekNumberInISO8601(sg.getJointSendings().at(i).start)]).content(":").closeElt("span");
                                        }else{
                                            if(sg.size()>1){
                                                writer.openElt("span").attr("class", "ProgSubTitleTimeTitle").
                                                        content(format_time(sg.getJointSendings().at(i).start).c_str()).content(":").closeElt("span");
                                            }
                                        }
                                    }

                                }


                                if(uusisub || uusidesc){

                                    if(uusisub){

                                        std::string sarjaalkaa="";
                                        if(sg.getJointEpisodes().at(i).isFirst() /*&& (
                                                    sg.getJointEpisodes().at(0).episodeTotal==0
                                                    || sg.getJointEpisodes().at(0).episodeTotal>2
                                                    || desc.find("SARJA ALKAA!") != std::string::npos
                                                    || desc.find("UUSI SARJA!") != std::string::npos) */){
                                            sarjaalkaa="sarjaalkaa ";
                                        }

                                        writer.openElt("span").attr("class", sarjaalkaa+" ProgSubTitle");
                                        writer.content((subtitle+" ").c_str());
                                        writer.closeElt("span");
                                        tila-=subtitle.length();
                                    }

                                    if(uusidesc){

                                        if(tila>0){
                                            std::string str_desc = shorten_text(desc, tila);
                                            //std::string str_desc = desc;

                                            writer.openElt("span").attr("class", "ProgSubTitleMore");
                                            writer.content(" ");
                                            //writer.content(str_desc.c_str());

                                            writer.direct(finnishHyphenation(" " + str_desc,"&shy;").c_str());
                                            writer.closeElt("span");

                                            tila-=str_desc.length();
                                        }

                                    }

                                    //sizeleft -= 5 * shape.colkesto;
                                }

                                if(DEBUGLABELS){
                                    writer.openElt("i");
                                    //writer.direct(std::to_string(tila).c_str());
                                    //writer.direct(std::to_string(descphrases.size()).c_str());
                                    //writer.direct(desc.c_str());
                                    writer.closeElt();
                                }

                                if(i+1 >= sg.size() || sg.getJointSendings().at(i+1).start.tm_wday != wday || shape.colspan==0 || saannollinen){
                                    writer.closeElt("div");
                                }

                            }

                            writer.closeElt("div");
                        }else{
                            if(sizeleft > 35){
                                writer.openElt("div");
                                if(sg.getJointEpisodes().at(0).isFirst()){
                                    writer.attr("class", "sarjaalkaa");
                                }

                                writer.content(getEpisodeStr(sg, shape).c_str());
                                writer.closeElt("div");
                            }
                        }




                        //writer.direct(finnishHyphenation(str_subtitle + " " +str_year+" "+str_desc, "&shy;").c_str());
                        //writer.direct(finnishHyphenation(str_subtitle + " " +str_desc + " " +sizeclass+ " " + program.categories, "&shy;").c_str());
                    //}

                    //tulostataan julkaisuvuosi (elokuville)
                    if(str_filmfacts.length()>0){
                        writer.openElt("div");
                        writer.attr("class", "filmfacts");
                        writer.direct(str_filmfacts.c_str());
                        writer.closeElt("div");
                    }

                    writer.closeElt();
                //}


            //}

        //}





        //linkki loppuu
        writer.closeElt("div");
        writer.closeElt("a");



    }
    writer.closeElt("td");
}

void Html_table_writer::generateHeader()
{
    writer.openElt("head");
    writer.openElt("meta").attr("charset", "utf-8").closeElt();
    writer.openElt("link").attr("rel", "stylesheet").attr("type", "text/css").attr("href", "../style.css").closeElt();
    writer.openElt("link").attr("rel", "stylesheet").attr("type", "text/css").attr("href", "../popup.css").closeElt();
    writer.openElt("title").content(generateName().c_str()).closeElt();
    writer.closeElt();
}

std::string Html_table_writer::generateName()
{
    /*
    tm sunday = monday;
    sunday.tm_mday += 6;
    mktime(&sunday);

    //return std::string("Kanavakartta ")+name_of_channel+" "+format_date(monday)+"-"+format_date(sunday);

    return std::string("Ohjelmakartta ")+name_of_channel+" viikolle "+std::to_string(weeknro)+"/"+std::to_string(monday.tm_year+1900);
    */
    return "-";
}

void Html_table_writer::generateWeekDayHeader()
{
    assert(this->monday.tm_year > 99);

    writer.openElt("tr");

    //tyhjä neliö
    writer.openElt("th").content("").closeElt();

    tm weekday = monday;
    //weekday.tm_hour -= 24;
    weekday.tm_hour=0;
    weekday.tm_min=0;
    mktime(&weekday);

    //fprintf(stderr,"Maanantai: %d .\n", this->monday.tm_mday);

    for(int i=0;i<7;i++){
        //writer.openElt("th").openElt("a").attr("href", day_names[i]).content(day_names[i]).closeElt().closeElt();
        //writer.openElt("th").content((std::string(short_day_names[i])+std::string(" ")+format_time_of_year(weekday)).c_str()).closeElt();
        //writer.openElt("th").content((std::string(day_names[i])+std::string(" ")+std::to_string(monday.tm_year)).c_str()).closeElt();
        writer.openElt("th");

        if(isRedDay(weekday)){
            writer.attr("class", "redday");
        }

        writer.content(
                ( //std::string(short_day_names[i])+std::string(" ")+
                 std::to_string(weekday.tm_mday)+"."+
                 std::to_string(weekday.tm_mon+1)+".")
                .c_str()).closeElt();
        weekday.tm_hour += 24;
        mktime(&weekday); /* normalize result */
    }
    writer.closeElt();
}

void Html_table_writer::generateWeekHeader(int startweek)
{
    std::string vuosi("2018-");

    writer.openElt("tr");

    writer.openElt("th").content("").closeElt("th"); //vasemmen yläkulman tyhjä

    for(int i=0;i<7;i++){
        //writer.openElt("th").openElt("a").attr("href", day_names[i]).content(day_names[i]).closeElt().closeElt();
        if(i==3){
            writer.openElt("th");
            writer.openElt("a").attr("href", (std::string(".")).c_str());
        }else{
            writer.openElt("td");
            writer.openElt("a").attr("href", (vuosi+std::to_string(startweek)).c_str());
        }

        writer.content((vko+std::to_string(startweek++)).c_str()).closeElt();
        writer.closeElt();
    }


    writer.closeElt("tr");
}

void Html_table_writer::addTimeHeader(const int t, const int span)
{
    //if(t % (TICKSINHOUR/1)==0 || TICKSINHOUR==1){
    //if((t % TICKSINHOUR)==0){

        writer.openElt("th")
                //.attr("rowspan", std::to_string(TICKSINHOUR/1).c_str())
                //.attr("rowspan", std::to_string(2).c_str())
                .attr("rowspan", std::to_string(span+1).c_str())
                ;

        //if on the hour
        if((t+(FIRSTHOUR*TICKSINHOUR)) % (TICKSINHOUR*3)==0){

            int hour = ticksToHour(t);

            writer.attr("id", std::to_string(hour).c_str());
            writer.attr("class", "TimeColumn hour3");

            writer.openElt("a");
            writer.attr("name", std::to_string(hour).c_str());
            writer.closeElt("a");

            writer.content(std::to_string(hour).c_str());

        }else{

            if((t+(FIRSTHOUR*TICKSINHOUR)) % (TICKSINHOUR)==0){
                writer.attr("class", "TimeColumn hour1");
            }else{
                writer.attr("class", "TimeColumn");
            }

        }

        writer.closeElt("th");
    //}
}

void Html_table_writer::close()
{    
    writer.closeElt();
    writer.direct(list.c_str());
    writer.closeAll();
}
