#include <algorithm>
#include <string>
#include "timetable.h"
#include "../Rutiinit/datetime.h"
#include "../Rutiinit/colors.h"
#include "../Rutiinit/stringrutins.h"
#include "writer_svg.h"
#include <sendinggroup.h>
#include <math.h>

program_t none{"", "None", "", "", -1, static_cast<bool>(-1), false};
program_t varied{"", "Varied", "", "", -1, static_cast<bool>(-1), false};
program_t levelling_up{"", "LevellingUp", "", "", -1, static_cast<bool>(-1), false};
program_t levelling_down{"", "LevellingDown", "", "", -1, static_cast<bool>(-1), false};

episode_t nonejakso{-1, "", -1};

#define NONE            0
#define VARIEND         1
#define LEVELLING_UP    2
#define LEVELLING_DOWN  3

using namespace std;

//converts tm-structure to index of sendingtable for one week
static void tm_to_tick(tm Tm, int &weekday, int &week, Tick &tick, bool roundup){

    //fprintf(stderr,"tm_to_tick: PVM %d.%d.%d .\n", Tm.tm_mday, Tm.tm_mon, Tm.tm_year);

    //starting day in HOUROFDAYCHANGE
    int hour = Tm.tm_hour;
    if (hour<HOUROFDAYCHANGE){
        hour+=24;
        Tm.tm_mday -= 1;
        mktime(&Tm);
    }

    //rounding up
    /*
    if(roundup){
        int min = Tm.tm_min;
        if (min>0){
            Tm.tm_hour += 1;
            mktime(&Tm);
        }
    }*/


    //weekday number in ISO 8601 (monday is first day of the week)
    weekday = weekNumberInISO8601(Tm);

    week=tm_to_weeknumber(Tm);

    //assert(weekday<DAYSINWEEK);

    double floattick = (hour - FIRSTHOUR) * TICKSINHOUR + (Tm.tm_min + (Tm.tm_sec/60.0)) * TICKSLENGHT;

    if(roundup){
        tick=ceil(floattick);
    }else{
        tick=floattick;
    }
    if(tick>=TICKSINDAY){
        tick=TICKSINDAY-1;
    }

    //fprintf(stderr,"tm_to_tick: Viikko %d vp %d .\n", week, weekday);

}

/*
//converts tm-structure to index of sendingtable
static void tm_to_weekdaytick(tm Tm, int weekday, int startweek, int &week, Tick &tick){

    assert(weekday>=0 && (weekday<=0);

    int week = startweek - week_number(Tm);


    weekday = Tm.tm_wday-1;
    if(weekday<0)
        weekday=6;

    int hour = Tm.tm_hour;
    if (hour<HOUROFDAYCHANGE){
        hour+=24;
        weekday--;
        if(weekday<0)
            weekday=6;
        Tm.tm_mday -= 1;
        mktime(&Tm);
    }

    if (week_number(Tm) < week)
        weekday=-1;

    if (week_number(Tm) > week)
        weekday=DAYSINWEEK;

    //assert(weekday<DAYSINWEEK);

    tick = (hour - FIRSTHOUR) * TICKSINHOUR + Tm.tm_min * TICKSLENGHT;
}
*/


/* Muuttaa viikonpäiväksi (weekday) -1 tai DAYSINWEEK,
 * jos viikko (week) ei ole kyseiseltä viikolta (tableweek) */
static void tm_to_week_tick(int week, const int tableweek, int &weekday){
    if (week < tableweek){
        weekday=-1;
    }

    if (week > tableweek){
        weekday=DAYSINWEEK;
    }
}

static void tm_to_weekday_tick(int week, const int tableweek, int tableweekday, int &weekday, Tick &tick){

    int ero = weekday - tableweekday;
    if(ero>5){
        ero=ero-7;
        week+=1;
    }
    if(ero<-5){
        ero=ero+7;
        week-=1;
    }

    if(abs(ero)>1){
        weekday=-1;
    }else{
        if(ero==1){
            //tick=TICKSINDAY-1;
            tick=0;
            week+=1;
        }
        if(ero==-1){
            tick=0;
        }

        weekday=week+3-tableweek;

        if(weekday<0){
            weekday=-1;
        }
        if(weekday>5){
            weekday=DAYSINWEEK;
        }
    }

}
WeekTable::WeekTable()
{
    //clear sendingtable
    for(int d=0;d<DAYSINWEEK;d++){
        for(int t=0;t<TICKSINDAY;t++){
            this->sendingtable[d][t]=NONE;
            this->sendingtimes[t]=t;
        }
    }

    //init build-in names
    this->programs.push_back(none);
    this->programs.push_back(varied);
    this->programs.push_back(levelling_up);
    this->programs.push_back(levelling_down);

    this->episodes.push_back(nonejakso);

    for(int i=0;i<4;i++){
        sending_t lah{i, 0, 0};
        this->sendings.push_back(lah);
    }

    this->weeknro = -1;
    this->weekdaynro = -1;

    monday_has_set=false;
}

void WeekTable::setWeek(int tableweek)
{
    this->weeknro = tableweek;

    //päätellään vuosi automaattisesti
    time_t now = time(nullptr);
    struct tm today = *localtime(&now);
    int tableyear=today.tm_year+1900;
    if(today.tm_mon<5 && tableweek>25){
        tableyear-=1;
    }
    if(today.tm_mon>5 && tableweek<25){
        tableyear+=1;
    }

    this->monday = weeknumber_to_tm(tableyear, tableweek, 0);

    //fprintf(stderr, "Asetetaan viikko %d.\n", weeknro);
    //fprintf(stderr, "Asetetaan vuosi %d.\n", yearnow);
    //fprintf(stderr, "Päivä: %d.\n", this->monday.tm_mday);
    //fprintf(stderr, "Kuukausi: %d.\n", this->monday.tm_mon);
    //fprintf(stderr, "Vuosi: %d.\n", this->monday.tm_year);
    //fprintf(stderr, "tableyear %d.\n", tableyear);

    assert(this->monday.tm_year>2019-1900 || this->monday.tm_year<2025-1900);
    //assert(this->monday.tm_mon>=5);
    //assert(this->monday.tm_mon<=12);
    assert(this->monday.tm_wday==1);
    this->monday_has_set = true;
}

void WeekTable::setWeekDay(int weekdaynro)
{
    this->weekdaynro = weekdaynro;
}

void WeekTable::setChannelName(string name_of_channel)
{
    this->name_of_channel = name_of_channel;
}

void WeekTable::addProgram(const tm start, const tm end, std::string title,
                           std::string categories, int year,
                           int season, int episode, int episodetotal, std::string subtitle,
                           std::string desc, std::vector<std::string> urls)
{

    assert(start.tm_year > 99);

    assert(title.size()>0);

    sending_t lah;
    lah.start=start;
    lah.end=end;


    program_t prog;
    prog.title=title;

    prog.categories=categories;
    prog.year=year;

    prog.urls=urls;


    //prog.desc="";

    int prog_id = addUnique(prog, programs);
    assert(prog_id);
    lah.programId = prog_id;


    program_t* progvanha = &(programs.at(prog_id));

    if(progvanha->urls.size()<urls.size()){
        progvanha->urls=urls;
    }

    if(progvanha->categories.size()<categories.size()){
        progvanha->categories=categories;
    }
    //progvanha->desc="aaaa";

    eraseSubStr(desc, " Uusinta.");

    /*
    if(progvanha->desc!=""){
        if(progvanha->desc=="-" || progvanha->desc==last_sentence(desc)){
            progvanha->desc = last_sentence(desc);
        }else{
            progvanha->desc="";
        }
    }
    */

    tm start_m = start;
    tm end_m = end;
    progvanha->lisaaPituus(difftime(mktime(&end_m), mktime(&start_m)));
    //progvanha->lisaaPituus(180*60);

    episode_t epis;
    epis.number=episode;
    epis.season=season;
    epis.episodeTotal=episodetotal;
    epis.subtitle=subtitle;
    epis.desc=desc;
    epis.firstshow=-1;
    //epis.url=url;


    //if week is not set, using week on first program
    /*
    if(this->weeknro==-1)
        this->weeknro=week_number(start);
        */

    //assert((this->weekdaynro==-1) != (this->weeknro==-1));

    Tick starttick;
    Tick endtick;
    int startday;
    int endday;
    int startweek;
    int endweek;

    tm_to_tick(start, startday, startweek, starttick, false);
    tm_to_tick(end, endday, endweek, endtick, true);

    //fprintf(stderr,"Viikko %d .\n", startweek);

    if(this->weekdaynro==-1){

        assert(start.tm_year>99);

        tm_to_week_tick(startweek, this->weeknro, startday);
        tm_to_week_tick(endweek, this->weeknro, endday);

        if(start.tm_hour>6){
            if(startday==0){
                //this->monday = start;
                //monday_has_set = true;
            }
            if(startday>0 && startday<5){
                //this->monday = start;
                //this->monday.tm_mday -= startday;
                //mktime(&(this->monday));
                //monday_has_set = true;
            }
        }
    }else{
        assert(false);
        tm_to_weekday_tick(startweek, this->weeknro, this->weekdaynro, startday, starttick);
        tm_to_weekday_tick(endweek, this->weeknro, this->weekdaynro, endday, endtick);
    }

/*
    if(startday >= DAYSINWEEK){

        startday=DAYSINWEEK;
        endday=DAYSINWEEK;
        starttick=FIRSTHOUR;
        endtick=FIRSTHOUR;

    }else{

        if(endday <= -1){

            startday=-1;
            endday=-1;
            starttick=FIRSTHOUR;
            endtick=FIRSTHOUR;

        }else{

            //assert(!(week_number(start) < this->weeknro && week_number(end) == this->weeknro));
            //assert(!(week_number(start) == this->weeknro && week_number(end) > this->weeknro));

            //*
            if(startday==0 && !monday_has_set){
                monday=start;
                monday_has_set=true;
            }

            //assert(!(endday == 0 && startday == -1));
            //assert(!(startday == DAYSINWEEK-1 && endday == DAYSINWEEK));

        }
    }
//*/

    addProgram(startday, starttick, endday, endtick, prog_id, epis, lah);

}

void WeekTable::addProgram(const int startday, const int starttick, const int endday, const int endtick, int prog_id, episode_t epis, sending_t lah)
{
    // Luo instanssit jaksosta ja lähetyksestä.
    // Lisää viittauksen lähetystaulukkoon.

    assert(startday>=-1);
    assert(startday<DAYSINWEEK+1);
    assert(starttick<TICKSINDAY);
    assert(endtick<TICKSINDAY);
    assert(endtick>=0);
    assert(starttick>=0);

    epis.programId=prog_id;

    const int jid = addUnique(epis, episodes);
    const int kokonaisaika=startday*TICKSINDAY+starttick;

    epis = episodes.at(jid);

    //if(kokonaisaika<0){
        //fprintf(stderr, "kokonaisaika: %d\n", kokonaisaika);
        //fprintf(stderr, "ensimmainenesitys: %d\n", jaksot.at(jid).ensimmainenesitys);
        //fprintf(stderr, "useitaEsityksia: %d\n", jaksot.at(jid).useitaEsityksia);
    //}

    if(epis.firstshow==-1){
        //jaks.useitaEsityksia=false;
        episodes[jid].firstshow=kokonaisaika;
    }else{
        if(kokonaisaika>epis.firstshow){
            episodes[jid].hasMultipleShows=true;
        }
        if(kokonaisaika<epis.firstshow){
            episodes[jid].firstshow=kokonaisaika;
            episodes[jid].hasMultipleShows=true;
        }
    }

    //if(kokonaisaika<0){
        //fprintf(stderr,"ensimmainenesitys: %d\n", jaksot.at(jid).ensimmainenesitys);
        //fprintf(stderr,"useitaEsityksia: %d\n\n", jaksot.at(jid).useitaEsityksia);
    //}


    //printf("lisätään päivään %d tikkeihin %d - %d\n", viikonpaiva, alkaatick, loppuutick);
    //printf("ohjelma %s lisätty id:llä %d\n", nimi.c_str(), id);

    //assert(alkaatick<loppuutick);
    assert(prog_id>-1);

    //programs not hit in this week
    if(endday<0 || startday>DAYSINWEEK-1){
        return;
    }


    lah.episodeId = jid;
    sendings.push_back(lah);
    const int lid=sendings.size()-1;

    //fprintf(stderr,"Ohjelma: %s\n", jaks.alanimi.c_str());

    assert(starttick>=0);
    assert(endtick>=0);

    //assert(!(startday==0 && starttick==0 && endday>-1 && endtick>0));

    if(startday==endday){
        for(int t=starttick; t<endtick; t++){
            assert(startday>-1 && startday < DAYSINWEEK);
            assert(t>-1 && t<TICKSINDAY);
            assert(lid>-1);
            this->sendingtable[startday][t] = lid;
        }
    }

    //assert(!(endday == 0 && endtick > 1 && startday == -1));

    if(startday == (endday - 1)){

        if(startday > -1){
            for(int t=starttick; t<TICKSINDAY; t++){
                assert(startday>-1 && startday < DAYSINWEEK);
                assert(t>-1 && t<TICKSINDAY);
                assert(lid>-1);
                this->sendingtable[startday][t] = lid;
            }
        }

        if(endday < DAYSINWEEK){
            for(int t=0; t<endtick; t++){
                this->sendingtable[endday][t] = lid;
            }
        }

    }
}

template <class T> static int addUnique(T ohj, std::vector<T> &v)
{
    ptrdiff_t pos = find(v.begin(), v.end(), ohj) - v.begin();
    if(pos >= v.size()) {
        ohj.count++;
        v.push_back(ohj);
        return v.size()-1;
    }else{
        v[pos].count++;
        return pos;
    }
}

void WeekTable::level(){
    for(int d=0;d<DAYSINWEEK;d++){

        for(int t=1;t<TICKSINDAY-1;t++){
            if(this->sendingtable[d][t]!=this->sendingtable[d][t-1] ||          //ohjelma alkaa
                    this->sendingtable[d][t] != this->sendingtable[d][t+1]){    //ohjelma loppuu

                //make look-up for neighbours
                int near[3][4];
                for(int dl=-1;dl<2;dl++){
                    for(int tl=-1;tl<3;tl++){
                        int id = 0;
                        if(d+dl>-1 && d+dl<DAYSINWEEK && t+tl>-1 && t+tl<TICKSINDAY){
                            id = sendings.at(this->sendingtable[d+dl][t+tl]).programId;
                            //fprintf(stderr,"asetettiin id %d.\n",tl);
                        }
                        near[dl+1][tl+1]=id;
                    }
                }

                int tama=near[1][1];

                if(near[0][1] && near[2][1]
                        && near[0][1]!=tama && near[2][1]!=tama){

                    if(near[1][2]){
                        if(        near[0][2]==tama
                                && near[1][2]==tama
                                && near[2][2]==tama
                                ){
                            if(near[0][1]==near[2][1])
                                this->sendingtable[d][t]=this->sendingtable[d-1][t];
                            else
                                this->sendingtable[d][t]=LEVELLING_DOWN;
                            //fprintf(stderr,"yhdistettiin alas kohdassa %d - %d\n", d, t);
                        }
                    }

                    if(near[1][0]){
                        if(        near[0][0] == tama
                                && near[1][0] == tama
                                && near[2][0] == tama
                                ){
                            if(near[0][1]==near[2][1])
                                this->sendingtable[d][t]=this->sendingtable[d-1][t];
                            else
                                this->sendingtable[d][t]=LEVELLING_UP;
                            //this->sendingtable[d][t]=LEVELLING_UP;
                            //fprintf(stderr,"yhdistettiin ylös kohdassa %d - %d\n", d, t);
                        }
                        if(        near[1][2] && near[0][1] != near[0][2] && near[0][1] != near[2][2]
                                && near[0][1] == near[1][0]
                                && near[1][0] == near[2][1]
                                ){
                            if(tama == near[1][2])
                                this->sendingtable[d][t]=this->sendingtable[d][t-1];

                            //this->sendingtable[d][t]=LEVELLING_UP;
                            //fprintf(stderr,"yhdistettiin ylös kohdassa %d - %d\n", d, t);
                        }
                    }
                }

                /*
                if(near[1][2]){
                    if(near[0][1]
                            && near[0][1]!=tama
                            && near[0][2]==tama
                            && near[1][2]==tama
                            && near[2][1]!=tama
                            ){
                        this->sendingtable[d][t]=LEVELLING_DOWN;
                        //fprintf(stderr,"yhdistettiin kohdassa %d - %d\n", d, t);
                    }
                    if(near[2][1]
                            && near[2][1]!=tama
                            && near[2][2]==tama
                            && near[1][2]==tama
                            && near[0][1]!=tama
                            ){
                        this->sendingtable[d][t]=LEVELLING_DOWN;
                        //fprintf(stderr,"yhdistettiin kohdassa %d - %d\n", d, t);
                    }
                }

                if(near[1][0] && near[1][2]!=tama){
                    if(near[0][1]
                            && near[0][1]!=tama
                            && near[0][0]==tama
                            && near[1][0]==tama
                            ){
                        this->sendingtable[d][t]=LEVELLING_UP;
                        fprintf(stderr,"yhdistettiin kohdassa %d - %d\n", d, t);
                    }
                    if(near[2][1]
                            && near[2][1]!=tama
                            && near[2][0]==tama
                            && near[1][0]==tama
                            ){
                        this->sendingtable[d][t]=LEVELLING_UP;
                        fprintf(stderr,"yhdistettiin kohdassa %d - %d\n", d, t);
                    }
                }
                */
            }
        }
    }
}

void WeekTable::level2(){
    for(Tick t=1;t<TICKSINDAY-1;t++){
        int alut=0;
        for(int d=0;d<DAYSINWEEK;d++){
            if(this->sendingtable[d][t]!=this->sendingtable[d][t-1]){
                alut++;
            }
        }

        if(t%TICKSINHOUR==0)
            std::cout << std::endl;

        std::cout << t << ": " << alut << std::endl;
    }
}

void WeekTable::snapToGrid()
{
    const int ASKEL=8;

    for(int d=0;d<DAYSINWEEK;d++){
        int sama=0;
        for(Tick t=1;t<TICKSINDAY-1;t++){
            if(this->sendingtable[d][t] == this->sendingtable[d][t-1]){
                sama++;
            }else{
                //syötetään sama aiemmasta tikki-kohdasta lähtien
                if(sama > (t % ASKEL)){
                    for(int tback = 0; tback < (t % ASKEL); tback++){
                        this->sendingtable[d][t-tback] = this->sendingtable[d][t];
                    }
                }
                sama=0;
            }
        }
    }
}

void WeekTable::snapToBefore(){

    for(Tick t=1;t<TICKSINDAY-1;t++){
        //jos rivillä on vähintään yksi sauma (ohjelma alkaa ja loppuu), asetetaan (sauma==true)
        bool sauma=false;
        for(int d=0;d<DAYSINWEEK;d++){
            if(this->sendingtable[d][t]!=this->sendingtable[d][t-1]){
                sauma=true;
                break;
            }
        }
        if(sauma){
            bool canSnap=true;
            for(int d=0;d<DAYSINWEEK;d++){
                //jos ohjelma vaihtuu alemmalla rivillä
                if(this->sendingtable[d][t]!=this->sendingtable[d][t+1]){
                    //eikä tällä rivillä
                    if(this->sendingtable[d][t]!=this->sendingtable[d][t-1]){
                        canSnap=false;
                    }
                }
            }
            if(canSnap){
                for(int d=0;d<DAYSINWEEK;d++){
                    //nostetaan, jos vaihtuu seuraavalla
                    if(this->sendingtable[d][t]!=this->sendingtable[d][t+1]){
                        this->sendingtable[d][t]=this->sendingtable[d][t+1];
                    }
                }
            }
        }
    }
}

void WeekTable::snapToBeforeMore()
{
    for(Tick t=1;t<TICKSINDAY-2;t++){
        //jos rivillä on vähintään yksi sauma (ohjelma alkaa ja loppuu), asetetaan (sauma==true)
        bool sauma=false;
        for(int d=0;d<DAYSINWEEK;d++){
            if(this->sendingtable[d][t] != this->sendingtable[d][t-1]){
                sauma=true;
                break;
            }
        }
        if(sauma){
            bool canSnap=true;
            for(int d=0;d<DAYSINWEEK;d++){
                //jos ohjelma vaihtuu jonakin päivänä 2 riviä alempana
                if(this->sendingtable[d][t+1]!=this->sendingtable[d][t+2]){
                    //ja tällä tai seuraavalla rivillä, asetetaan (canSnap=false)
                    if(this->sendingtable[d][t]==this->sendingtable[d][t-1] && this->sendingtable[d][t]==this->sendingtable[d][t+1]){
                        //canSnap=true;
                    }else{
                        canSnap=false;
                    }
                }
            }
            if(canSnap){
                for(int d=0;d<DAYSINWEEK;d++){
                    //nostetaan, jos vaihtuu seuraavalla ja jos samannimisiä ohjelmia nykyisellä rivillä
                    if(this->sendingtable[d][t+1]!=this->sendingtable[d][t+2]){
                        const int prog_id = sendings.at(this->sendingtable[d][t+2]).programId;
                        bool samannimisia=false;
                        for(int d2=0;d2<DAYSINWEEK;d2++){
                            if(d != d2 && sendings.at(this->sendingtable[d2][t]).programId == prog_id){
                                samannimisia=true;
                            }
                        }
                        if (samannimisia){
                            this->sendingtable[d][t]=this->sendingtable[d][t+2];
                            this->sendingtable[d][t+1]=this->sendingtable[d][t+2];
                        }
                    }
                }
            }
        }
    }
}

void WeekTable::findDaily()
{
    const int SARJATOLERANSSI = 3 * TICKSINHOUR;
    //const int TOLERANSSI = 1 * TICKSINHOUR;

    //etsitään lähetysten alkamiset
    for(Tick t=1;t<TICKSINDAY-1;t++){
        for(int d=0;d<DAYSINWEEK;d++){
            if(this->sendingtable[d][t]!=this->sendingtable[d][t-1]){
                int parast2=-1;
                int pienin_ero=1000;

                sending_t sending1 = sendings.at(this->sendingtable[d][t]);

                //etsiään seuraavasta päivästä
                for(int t2=std::max(0,t-SARJATOLERANSSI); t2<std::min(TICKSINDAY,t+SARJATOLERANSSI); t2++){

                    sending_t sending2 = sendings.at(this->sendingtable[d+1][t2]);

                    if(canJoint(sending1, sending2)){
                        int ero=std::abs(t-t2);

                        if(sending1.episodeId >0 && sending2.episodeId >0){
                            if(episodes.at(sending1.episodeId).number != -1 && episodes.at(sending2.episodeId).number != -1){
                                if(episodes.at(sending1.episodeId).number + 1 == episodes.at(sending2.episodeId).number){
                                    ero=ero-1000;
                                }else{
                                    ero=ero+2000;
                                }
                            }
                        }
                        if(ero<pienin_ero){
                            pienin_ero=ero;
                            parast2=t2;
                        }
                    }
                }

                //yhdistetään
                if(parast2!=-1 && pienin_ero<1000){
                    sendings.at(this->sendingtable[d][t]).nextDaySending = this->sendingtable[d+1][parast2];
                    sendings.at(this->sendingtable[d+1][parast2]).prevDaySending = this->sendingtable[d][t];
                }

                //jatkuuko sauma seuraavassa lähetyksessä?
                if(this->sendingtable[d+1][t]!=this->sendingtable[d+1][t-1]){
                    if(!canJoint(sendings.at(this->sendingtable[d][t]), sendings.at(this->sendingtable[d+1][t]))){
                        //if(sendings.at(this->sendingtable[d][t]).start.tm_min == sendings.at(this->sendingtable[d+1][t]).start.tm_min){
                        //if(abs(sendings.at(this->sendingtable[d][t]).start.tm_min - sendings.at(this->sendingtable[d+1][t]).start.tm_min)<2){
                            sendings.at(this->sendingtable[d][t]).sametime = true;
                            sendings.at(this->sendingtable[d+1][t]).sametime = true;
                        //}
                    }
                }
            }
        }
    }

    /*
    for(Tick t=1;t<TICKSINDAY-1;t++){
        for(int d=0;d<DAYSINWEEK;d++){
            if(this->sendingtable[d][t]!=this->sendingtable[d][t-1]){

                //lisätään peräkkäisiltä päiviltä eriaikaan tulevat
                std::vector<episode_t> jointEpisodesDaily;
                std::vector<sending_t> jointSendingsDaily;

                sending_t sending = sendings.at(this->sendingtable[d][t]);

                if(sending.nextDaySending != -1){

                    jointEpisodesDaily.push_back(episodes.at(sending.episodeId));
                    jointSendingsDaily.push_back(sending);
                    sendings.at(this->sendingtable[d][t]).sendingGroupId = sendinggroups.size();

                    sending_t nextsending = sendings.at(sending.nextDaySending);

                    jointEpisodesDaily.push_back(episodes.at(nextsending.episodeId));
                    jointSendingsDaily.push_back(nextsending);
                    sendings.at(sending.nextDaySending).sendingGroupId = sendinggroups.size();

                    sendingGroup sg(jointSendingsDaily, programs.at(sending.programId), jointEpisodesDaily);
                    sendinggroups.push_(sg);
                }
            }
        }
    }
    */
}

void WeekTable::levelDaily()
{
    //etsitään lähetysten alkamiset
    for(Tick t=1;t<TICKSINDAY-1;t++){
        for(int d=0;d<DAYSINWEEK;d++){
            if(this->sendingtable[d][t]!=this->sendingtable[d][t-1]){
                if(sendings.at(this->sendingtable[d][t]).prevDaySending == -1){
                    if(sendings.at(this->sendingtable[d][t]).nextDaySending != -1){
                        //Päivittäisen sarjan ensimmäinen lähetys.
                        //Käydään läpi kaikki sarjan lähetykset.
                        //Tasataan, jos ero on tarpeeksi pieni,
                        //eikä tuhota muita ohjelmia kokonaan
                        //fprintf(stderr, "kokonaisaika: %d\n", kokonaisaika);
                        int sending_id = this->sendingtable[d][t];
                        int count=1;
                        while(sending_id != -1){
                            sending_id = sendings.at(sending_id).nextDaySending;
                            count++;
                        }

                        /*
                        std::cerr << programs.at(sendings.at(this->sendingtable[d][t]).programId).title <<
                                  " t:" << t <<
                                  " d:" <<  d <<
                                  " c:"<< count <<  std::endl;
                                  */
                    }
                }
            }
        }
    }
}

int WeekTable::findMorrowline()
{
    FIRSTHOUR=10;
    LASTHOUR=20;
    HOUROFDAYCHANGE=10;

    return 0;
    for(Tick t=1;t<TICKSINDAY-1;t++){
        bool onRaja=true;
        for(int d=0;d<DAYSINWEEK;d++){
            onRaja == onRaja || (this->sendingtable[d][t]!=0);
        }
        if(onRaja){
            //return t;
            LASTHOUR = t;
        }
    }

}

bool WeekTable::isEmptyTick(Tick t)
{
    bool onTyhja=true;
    for(int d=0;d<DAYSINWEEK;d++){
        onTyhja = onTyhja && (this->sendingtable[d][t]==0);
    }
    return onTyhja;
}

bool WeekTable::hasProgramChange(Tick t)
{
    bool muutoksia=false;
    for(int d=0;d<DAYSINWEEK;d++){
        if (sendingtable[d][t] != sendingtable[d][t-1]){
            muutoksia=true;
        }
    }
    return muutoksia;
}

void WeekTable::trim()
{
    //luodataan tyhjä alku
    Tick t;
    for(t=1;t<TICKSINDAY-1;t++){
        if(!isEmptyTick(t)){
            break;
        }
    }

    Tick t2;
    for(t2=1;t2<TICKSINDAY-1;t2++){
        if(!isEmptyTick(TICKSINDAY-t2)){
            break;
        }
    }

    assert(t>0);
    //assert(t<8*TICKSINHOUR);
    //int trimhour=3;
    int trimhour=t/TICKSINHOUR;
    int trimhour2=t2/TICKSINHOUR;

    FIRSTHOUR += trimhour;
    //HOUROFDAYCHANGE+=start/TICKSINHOUR;
    LASTHOUR -= trimhour * TICKSINHOUR;
    LASTHOUR -= trimhour2 * TICKSINHOUR;

    TICKSINDAY -= trimhour * TICKSINHOUR;
    TICKSINDAY -= trimhour2 * TICKSINHOUR;

    //siirretään alkuun
    for(t=1;t<TICKSINDAY;t++){
        for(int d=0;d<DAYSINWEEK;d++){
           this->sendingtable[d][t]=this->sendingtable[d][t+(trimhour * TICKSINHOUR)];
        }
    }

}

void WeekTable::removeEmptyTicks()
{
    Tick t1=1;

    for(Tick t2=2;t2<TICKSINDAY;t2++){
        //if((t+(FIRSTHOUR*TICKSINHOUR)) % (TICKSINHOUR*3)==0){
        if(hasProgramChange(t2) || ((t2+(FIRSTHOUR*TICKSINHOUR)) % (TICKSINHOUR*1)==0)){
            //siirretään rivi ylös
            for(int d=0;d<DAYSINWEEK;d++){
                this->sendingtimes[t1]=this->sendingtimes[t2];
                this->sendingtable[d][t1]=this->sendingtable[d][t2];
            }
            t1++;
        }
    }

    TICKSINDAY=t1;
}

int WeekTable::countCategory(string category) const
{
    int count=0;
    for(int si=0; si<sendings.size(); si++){
        if(programs.at(sendings.at(si).programId).belongsToCategory(category)){
            //count++;
            count+=sendings.at(si).kesto();
        }
    }
    return count;
}

// Function to convert a map<key,value> to a multimap<value,key>
multimap<CategoryCount, string> invert(map<string, CategoryCount> & mymap)
{
    multimap<CategoryCount, string> multiMap;

    map<string, CategoryCount> :: iterator it;
    for (it=mymap.begin(); it!=mymap.end(); it++)
    {
        multiMap.insert(make_pair(it->second, it->first));
    }

    return multiMap;
}

void WeekTable::countCategories()
{
    void split(const std::string& s, char c, std::vector<std::string>& v);
    std::string text;
    program_t lastProgram;

    int count=0;

    for(Tick t=1;t<TICKSINDAY-1;t++){
        for(int d=0;d<DAYSINWEEK;d++){
            if(this->sendingtable[d][t]!=this->sendingtable[d][t-1]){

                std::vector<std::string> cats =
                        programs.at(sendings.at(this->sendingtable[d][t-1]).programId).allCategories();

                sort( cats.begin(), cats.end() );
                cats.erase( unique( cats.begin(), cats.end() ), cats.end() );


                for(std::string cat : cats){
                    if(cat.length()>1){
                        //assert(count<12*60);
                        categories[cat].minutes += (count * (60/TICKSINHOUR));
                        categories[cat].count++;
                        if(lastProgram.belongsToCategory(cat)){
                            categories[cat].sequence++;
                        }
                    }
                }
                count=0;

                lastProgram = programs.at(sendings.at(this->sendingtable[d][t-1]).programId);

            }
            count++;
        }
    }

    /*
    // invert mymap using the invert function created above
    multimap<CategoryCount, string> newmap = invert(categories);

    // print the multimap
    //cerr << "The map, sorted by value is: " << endl;
    multimap<CategoryCount, string> :: reverse_iterator iter;
    for (iter=newmap.rbegin(); iter!=newmap.rend(); iter++)
    {
        // printing the second value first because the
        // order of (key,value) is reversed in the multimap
        //cerr << iter->second << ": " << iter->first/60 << " h." << endl;

        text+=iter->second+" ";
        if(text.length()>50){
            break;
        }

        cerr << iter->second << " " << iter->first.count << " " << iter->first.sequence << " " << (int)(iter->first.perakkain()*100) << endl;

    }
    //cerr << text << endl;
    */

    //return "";//std::string::(categories.size());
}

void WeekTable::listCategoryValues()
{
    std::map<std::string, CategoryCount>::iterator it = categories.begin();

    //std::cerr << "-------------------------------------------" << endl;

    // Iterate over the map using Iterator till end.
    while (it != categories.end())
    {
        // Accessing KEY from element pointed by it.
        std::string word = it->first;

        // Accessing VALUE from element pointed by it.
        int count = it->second.count;
        int minutes = it->second.minutes;
        int sequence = it->second.sequence;

        std::cerr << (double)count / sendings.size() << "    " <<  it->second.perakkaisyys() << "    #" << word.substr(0,5) <<  std::endl;

        // Increment the Iterator to point to next entry
        it++;
    }
}


sending_t WeekTable::sendingByCoords(int d, Tick t)
{
    return sendings.at(sendingtable[d][t]);
}

void WeekTable::getHTML(std::ostream& out)
{    

    int tabletype=WEEKTABLE;
    if(this->weekdaynro != -1){
        tabletype=WEEKDAYTABLE;
    }

    Writer writer(out);
    Table_writer *tablewriter;
    assert(monday_has_set);
    assert(this->monday.tm_year >= 2019-1900);
    tablewriter = new Html_table_writer(writer, true, tabletype, this->weeknro, this->monday);
    getTable(*tablewriter);
    tablewriter->close();
}

void WeekTable::getSVG(std::ostream& out)
{
   Writer writer(out);

    Table_writer *tablewriter;

    tablewriter = new Svg_writer(writer);

    getTable(*tablewriter);

    tablewriter->close();
}

bool WeekTable::canJoint(sending_t &sending1, sending_t &sending2){

    bool canjoin = (sending1.programId == sending2.programId);

    program_t program = programs.at(sending1.programId);
    //olympialaisissa täytyy olla sama subtitle
    if(canjoin){
        std::string title = program.title;
        if(title=="Korean olympialaiset" || title=="Talviolympialaiset 2018" || title=="Salpausselän kisat"){
            //canjoin=(episodes.at(sending1.programId).subtitle == episodes.at(sending2.programId).subtitle);
            canjoin=(episodes.at(sending1.episodeId).subtitle == episodes.at(sending2.episodeId).subtitle);
        }
        //urheiluohjelmia ei kannata liittää, koska ovat usein epäsäännöllisiä
        //paitsi urheilu-uutiset

        if((program.belongsToCategory("sports") || program.belongsToCategory("Sports")  || program.belongsToCategory("Sport")  || program.belongsToCategory("puluur") || program.belongsToCategory("Urheilu")) &&
                (!(program.belongsToCategory("News") || program.belongsToCategory("Urheilu-uutislähetys")) &&
                  (sending1.episodeId != sending2.episodeId))){
            canjoin=false;
        }

        //elokuvia ei liitetä, koska ovat yksittäisiä lähetyksiä
        if(program.belongsToCategory("movie")){
            canjoin=false;
        }
    }

    return canjoin;
}


/*
 * plot 'kategoriat.csv'   using 1:2:(sprintf("(%d)", $3)) with labels point  pt 7 offset char 1,1 notitle , x*0.5+0.56, x*0.4+0.7, x*0.65+0.4
 * plot 'kategoriat.csv'   using 1:2:(sprintf("(%d)", $3)) with labels point  pt 7 offset char 1,1 notitle , x+0.5, x*0.5+0.75
 * plot 'kategoriat.csv'   using 1:2:(sprintf("(%d)", $3)) with labels point  pt 7 offset char 1,1 notitle ,   -1.5*(x-1)**2+1.5,   -3*(x-1)**2+3
 * plot 'tv2.csv'   using 1:2:(sprintf("(%d)", $3)) with labels point  pt 7 offset char 1,1 notitle ,   -1.5*(x-1)**2+1.5,   -3*(x-1)**2+3
 * plot 'kategoriat.csv',   -1.15*(x-1)**2+1.15,   -2*(x-1)**2+2
 * plot 'kategoriat.csv',   -0.4*(x-1)**2+1,   -0.6*(x-1)**2+0.9

 * 0.3, 0.5
 * 0.5, 0.75 - 0.87
 * 0.8, 0.955
 * 1.0, 1.0
 *
 * y = x
 * 1 = y1+x
 * 0 = y0+x
 * 0.05 = 0.3
 *
 * y1^2 + x1 +z−1 =y0^2 + x0 + z
 * x = 1 − y
 * x - 1 = -y
 * -x + 1 = y
 ****************************
 * y-1 = 2(x-1)**2
 * y-1 = 2*2*(x-1)*(x-1)
 * y-1 = 4*(x**2-1-x-x)
 * y-1 = 4*(x**2-x2-1)
 * y-1 = 4x**2-8x-4
 * y = 4x**2-8x-3

*/

double yhdistysfunktio(double x){
    //return x*0.4+0.7;
    //return x*0.5+0.75;
    //return -3*pow(x-1,2)+3;
    return -2*pow(x-1,2)+2;
    //return -0.4*pow(x-1,2)+1;
}

double katkofunktio(double x){
    //return x*0.65+0.4;
    //return x+0.5;
    //return -1.5*pow(x-1,2)+1.5;
    return -1.15*pow(x-1,2)+1.15;
    //return -0.6*pow(x-1,2)+0.9;
}

unsigned int liitos(double tiheys, double jonoosuus)
{

    assert(tiheys>=0);
    assert(tiheys<=1);
    assert(jonoosuus>=0);
    //assert(jonoosuus<=1); //oli pakko poistaa, koska yle teema keskeytyi
    //assert(jonoosuus>=tiheys);

    assert(yhdistysfunktio(0.0) >= katkofunktio(0.0));
    assert(yhdistysfunktio(0.5) >= katkofunktio(0.5));
    assert(yhdistysfunktio(1.0) >= katkofunktio(1.0) || yhdistysfunktio(1.0)>1.0);
    assert(katkofunktio(1.0) > 0);
    //assert(yhdistysfunktio(0.0) < 1);

    assert(0.825 < yhdistysfunktio(0.487805));

    if(jonoosuus > katkofunktio(tiheys)){
        if(jonoosuus > yhdistysfunktio(tiheys)){
            return 0;
        }else{
            return 1;
        }
    }else{
        return 2;
    }

}

double todnakjonossa(const double taytto)
{
    return taytto*taytto;
}

double todennaikoisyys(double taytto, double jonoosuus)
{
    if(jonoosuus < todnakjonossa(taytto)){
        //aliedustus
    }
    if(jonoosuus > todnakjonossa(taytto)){
        //yliedustus
    }
    if(jonoosuus == todnakjonossa(taytto)){
        //odotettu edustus
    }
}

double jonoon(const int tyhjia){
    assert(tyhjia>=1);
    return 1.0 / tyhjia;
}

double erilleen(const int tyhjia){
    assert(tyhjia>=1);
    //return tyhjia / (tyhjia-1.0);
    return (tyhjia-1.0)/tyhjia;
}

double WeekTable::todennaikoisyys(const int avaruus, const  int joukko, const  int jonossa)
{

    assert(joukko<=avaruus);
    assert(jonossa<joukko);
    //assert(fabs(jonoon(4)*erilleen(3) - jonoon(3)*erilleen(4))<0.03);

    double todnak=1;
    int tyhjia = avaruus;

    //std::cerr << avaruus << ", " << joukko << ", " << jonossa << ", " << std::endl;
    //std::cerr << joukko / avaruus << ", " << jonossa / joukko << ", " << std::endl;

    //sijoitetaan ensimmäinen, joka ei voi mennä jatkeeksi

    for(int lahetys=1; lahetys<joukko; lahetys++){

        tyhjia--;

        if(lahetys >= joukko-jonossa){
            todnak *= jonoon(tyhjia);
        }else{
            todnak *= erilleen(tyhjia);
        }

    }



    /*

    if(joukko==1){
        assert(jonossa==0);
    }

    ////

    if(joukko==2 && jonossa==0){
        //return 1/((avaruus-1)/avaruus);
        return (avaruus-1)/(avaruus-2);
    }

    if(joukko==2 && jonossa==1){
        return 1/(avaruus-1);
    }

    ////

    if(joukko==3 && jonossa==0){
        return avaruus/(avaruus-1) * avaruus/(avaruus-2);
    }

    if(joukko==3 && jonossa==1){
        return 1/avaruus * avaruus/(avaruus-2);
    }

    if(joukko==3 && jonossa==2){
        return 1/avaruus * 2/avaruus;
    }

    /////

    if(joukko==4 && jonossa==0){
        return avaruus/(avaruus-1) * avaruus/(avaruus-2) * avaruus/(avaruus-3);
    }

    if(joukko==4 && jonossa==1){
        return 1/avaruus * avaruus/(avaruus-2) * avaruus/(avaruus-3);
    }

    if(joukko==4 && jonossa==2){
        return 1/avaruus * 2/avaruus * avaruus/(avaruus-3);
    }

    if(joukko==4 && jonossa==3){
        return 1/avaruus * 2/avaruus * 3/avaruus;
    }

    //1, 2, 6, 24, 120, 720
    if(joukko==6 && jonossa==5){
        return 120 / pow(avaruus,5);
    }

    if(joukko==7 && jonossa==6){
        return 720 / pow(avaruus,6);
    }

    ///

    if(joukko==avaruus){
        assert(jonossa==joukko-1);
    }

*/



    //304, 216, 180
    //304, 216/2, 180
    //tiheys = 216 / 304 = 0,71052632
    //odotus =

    /*
    assert(avaruus >= joukko && joukko > jonossa);



    double todnak=1;

    const double jonot = joukko - jonossa;


    for(int viereinen=0; viereinen<jonot; viereinen++){

        const double vapaana = avaruus - viereinen*2;

        todnak *= vapaana / (avaruus - viereinen);

    }

    for(int viereinen=0; viereinen<jonossa; viereinen++){

        const double vapaana = avaruus - jonot*2 - viereinen;

        //todnak *= jonot/vapaana;

    }

    assert(todnak<=1);
    assert(todnak>=0);*/

    //std::cerr << todnak << std::endl;

    return todnak;
}



unsigned int WeekTable::perakkaisyysEdustus(std::string cat) const
{


    //assert(todennaikoisyys(5, 2, 1) == 0.25);
    //assert(todennaikoisyys(5, 2, 0) == 1/0.75);
    //assert(todennaikoisyys(5, 3, 1) );

    if(cat=="None" || cat=="Varied" || cat=="LevellingUp" || cat=="LevellinDown" || cat=="full-row"){
        return 2;
    }

    /*

    //if(categories.find(cat) == categories.end()){
    //    cerr << "'" << cat << "'" << endl;
    //}

    assert(categories.find(cat) != categories.end());
    assert(categories.at(cat).perakkaisyys()<categories.at(cat).count);

    //todennakoisyys, jolla kategorian sattumavarainen esiintymä on jatkoa saman kategorian edustajalle
    //jos ripoteltaisiin sattumanvaraisesti
    double odotus =  pow(categories.at(cat).count / ((double)sendings.size()), 2);

    assert(odotus<=1);
    assert(odotus>=0);


    //peräkkäisten määrä verrattuna todennäköisyyteen
    //yliedustus, jos > 1, muuten aliedustus
    return categories.at(cat).perakkaisyys() / odotus;
    */

    //std::cerr << "#" << cat << std::endl;


    //return todennaikoisyys(sendings.size(), categories.at(cat).count, categories.at(cat).sequence);



    return liitos((double)categories.at(cat).count / sendings.size(), categories.at(cat).perakkaisyys());
}

void WeekTable::getTable(Table_writer &tablewriter){

    //level();



    if(!getMakeSVG()){


        for(int t=0;t<2;t++){
            snapToBefore();
        }


        snapToBeforeMore();
    }else{

    }
    countCategories();


    findDaily();
    //findDaily();
    //levelDaily();
    if(!getMakeSVG()){
        trim();
        removeEmptyTicks();
    }


/*
    int c = countCategory("Draama");
    int d = countCategory("Tieteet");
    fprintf(stderr,"Draama %d, Tieteet %d.\n", c, d);
    */





    //findMorrowline();

    int rowspans[DAYSINWEEK]  = {0};
    int namehashs[DAYSINWEEK] = {0};



    for(Tick t=0;t<TICKSINDAY;t++){

        //if(!isEmptyTick(t)){
        //if(!kaikkijatkuu || !isEmptyTick(t)){

        //if(hasProgramChange(t)){
        if(true){

            tablewriter.start_row();

            if((this->sendingtimes[t]+(FIRSTHOUR*TICKSINHOUR)) % (TICKSINHOUR)==0){
                //luodataan saman tunnin sisällä olevan
                Tick tspan=0;
                while(((this->sendingtimes[t+tspan+1])+(FIRSTHOUR*TICKSINHOUR)) % (TICKSINHOUR)!=0){
                    tspan++;
                }
                tablewriter.addTimeHeader(this->sendingtimes[t], tspan);
            }

            for(unsigned int d=0; d<DAYSINWEEK; d++){

                //joins row until program ends
                if(rowspans[d]>0){

                    rowspans[d]--;

                }else{

                    sending_t sending = sendingByCoords(d, t);
                    const int programId = sending.programId;
                    std::vector<episode_t> jointEpisodes;
                    std::vector<sending_t> jointSendings;

                    //Jos uusi lähetys, lisätään se, sekä episodi
                    const int jssize = jointSendings.size();
                    if(jssize==addUnique(sending, jointSendings)){
                        jointEpisodes.push_back(episodes.at(sending.episodeId));
                    }

                    assert(programId>=0);
                    assert(programId<programs.size());

                    unsigned int rowspan=0;
                    int realrowspan=0; //span, johon lasketaan vain ne rivit, joilla ei ole vaihtuvia lähetyksiä

                    //try to join next ticks
                    for(int i=1;i<TICKSINDAY-t;i++){
                        sending_t thisSending = sendingByCoords(d, t+i);
                        if((JOIN_SIMILAR_PROGRAMS && canJoint(sending, thisSending)) ||
                                sendingtable[d][t+i]==sendingtable[d][t]){
                            rowspan++;

                            //onko rivillä ohjelmien alkamisia tai loppumisia
                            if(hasProgramChange(t+i)){
                                realrowspan++;
                            }

                            //if(jointEpisodes.size()==addUnique(episodes.at(thisSending.episodeId), jointEpisodes)){
                            //    jointSendings.push_back(thisSending);
                            //}

                            const int jssize = jointSendings.size();
                            if(jssize==addUnique(thisSending, jointSendings)){
                                //addUnique(episodes.at(thisSending.episodeId), jointEpisodes);
                                jointEpisodes.push_back(episodes.at(thisSending.episodeId));
                            }
                        }else{
                            break;
                        }
                    }

                    if(rowspan>0){
                        rowspans[d]=rowspan;
                    }

                    //try to join same program in next days
                    unsigned int colspan=0;
                    bool continues=true;

                    if(JOIN_SIMILAR_PROGRAMS){
                        for(int dl=1;dl<DAYSINWEEK-d && continues;dl++){
                            assert(d+dl<DAYSINWEEK);
                            assert(dl>0);

                            //if program on the next day is already joined
                            if(rowspans[d+dl]>0)
                                break;

                            for(int i=0;i<rowspan+1;i++){
                                sending_t thissending = sendingByCoords(d+dl, t+i);
                                continues=continues && canJoint(sending, thissending);
                            }

                            //programs must also ends concurrently
                            if(t+rowspan+1<TICKSINDAY){
                                sending_t thissending = sendingByCoords(d+dl, t+rowspan+1);
                                continues=continues && !canJoint(sending, thissending);
                                //continues=continues && (sendingtable[d+dl][t+rowspan+1] != sendingtable[d+dl][t]);
                            }

                            if(continues){

                                for(int i=0;i<rowspan+1;i++){

                                    sending_t sending = sendingByCoords(d+dl, t+i);
                                    //addUnique(episodes.at(sending.episodeId), jointEpisodes);
                                    //if(jointEpisodes.size()==addUnique(episodes.at(thisSending.episodeId), jointEpisodes)){
                                    //    jointSendings.push_back(sending);
                                    //}

                                    const int jssize = jointSendings.size();
                                    if(jssize==addUnique(sending, jointSendings)){
                                        //addUnique(episodes.at(sending.episodeId), jointEpisodes);
                                        jointEpisodes.push_back(episodes.at(sending.episodeId));
                                    }

                                }

                                colspan++;
                                rowspans[d+dl] = rowspan;
                            }
                        }
                    }

                    program_t program = this->programs[programId];

                    //if title is same than before, remove title
                    int hash = std::hash<std::string>()(program.title);
                    if(hash == namehashs[d]){
                        //program.title = "";
                    }else{
                        namehashs[d] = hash;
                    }

                    assert(TICKSINDAY<=TICKSINHOUR*HOURSINDAY);

                    for(int st=0;st<TICKSINDAY-1;st++){
                        assert(this->sendingtimes[st] < this->sendingtimes[st+1]);
                    }

                    sendingGroup sg(jointSendings, program, jointEpisodes);
                    //sendingshape_t shape={d, this->sendingtimes[t], colspan, rowspan};

                    int lopput = t+rowspan+1;
                    if(lopput==TICKSINDAY){
                        lopput=TICKSINDAY-1;
                    }
                    assert(lopput<TICKSINDAY);
                    assert(lopput>=t);

                    int rowkesto = ((int)this->sendingtimes[lopput]) - ((int)this->sendingtimes[t]);

                    assert(lopput<=TICKSINDAY);
                    assert(rowkesto>=0);

                    /*
                    if(rowkesto<0){
                        rowkesto += TICKSINDAY;
                        assert(lopput<=t || (t+rowspan+1)>t);
                    }*/



                    sendingshape_t shape={
                                d, this->sendingtimes[t],
                                colspan, rowspan,
                                colspan+1, static_cast<unsigned int>(rowkesto), false
                                };

                    //selvitetään, voisiko ohjelma olla jatkumoa edellisestä

                    shape.raja = 2;/*
                            (  (lastPrograms[shape.d].title=="Yle Uutiset 20.30" && program.title == "Urheiluruutu")
                            || (lastPrograms[shape.d].title=="Urheiluruutu" && program.title == "A-studio")
                            //|| (lastPrograms[shape.d].belongsToCategory("Lapset") && program.belongsToCategory("Lapset"))
                            //|| (lastPrograms[shape.d].belongsToCategory("lastenohjelmat") && program.belongsToCategory("lastenohjelmat"))
                            //|| (lastPrograms[shape.d].belongsToCategory("Luonto,ympäristö") && program.belongsToCategory("Luonto,ympäristö"))
                            //|| (lastPrograms[shape.d].belongsToCategory("Tieteet") && program.belongsToCategory("Tieteet"))
                            //|| (lastPrograms[shape.d].belongsToCategory("Historia") && program.belongsToCategory("Historia"))
                            //|| (lastPrograms[shape.d].belongsToCategory("Talous") && program.belongsToCategory("Talous"))
                            //|| (lastPrograms[shape.d].belongsToCategory("Filosofia,elämänkatsomus") && program.belongsToCategory("Filosofia,elämänkatsomus"))
                            //|| (lastPrograms[shape.d].title=="Kymmenen uutiset" && program.title == "Päivän sää")
                            //|| (lastPrograms[shape.d].title=="Seitsemän uutiset" && program.title == "Päivän sää")
                            || (lastPrograms[shape.d].title=="Päivän sää" && program.title == "Tulosruutu")
                            || (lastPrograms[shape.d].title==program.title)
                            ? 0 : 1);
                            */

                    //if(shape.colspan < 1){

                        const program_t& lastprogramFrom = lastPrograms[shape.d];
                        const program_t& lastprogramTo = lastPrograms[shape.d+shape.colspan];

                        //n*(n-1)*(n-2)
                        //2: 2/n
                        //3: 3/n
                        //4: 4/n

                        for(std::string cat: program.allCategories()){

                            if(cat.length()>2){

                                const unsigned int perakkaisyysedustus = perakkaisyysEdustus(cat);

                                //categories[cat].perakkain()>=0.4
                                /*
                                if(tiheys < 0.1 && lastprogram.belongsToCategory(cat)){
                                    shape.raja = 0;
                                }
                                if(tiheys < 0.5 && categories[cat].perakkain()>0.6 && lastprogram.belongsToCategory(cat)){
                                    shape.raja = 0;
                 k               }
                                */
                                //if(categories[cat].getRyhmaPituus() >= 2 && lastprogram.belongsToCategory(cat)){

                                //37 - täsmälliset kategoriat
                                //10 - subin ja maikkarin lastenohjelmat, ruotsinkieliset, enemmistö, mutta ei tv2 lastenohjelmia. ehkä joitain ylimääräisiä.
                                //7  - isot alueet, tv2-lastenohjelmat

                                /*
                                if(perakkaisyysedustus < 1.0e-100 && lastprogram.belongsToCategory(cat)){
                                    shape.raja = 0;
                                }
                                */

                                if(perakkaisyysedustus < shape.raja
                                        && lastprogramFrom.belongsToCategory(cat)
                                        && (shape.colspan == 0 || lastprogramTo.belongsToCategory(cat))){

                                    shape.raja = perakkaisyysedustus;

                                }

                                //categories[cat].perakkain()<=0.35
                                //if(shape.raja && tiheys > 0.65 && !lastprogram.belongsToCategory(cat)){
                                    //shape.raja = 2;
                                //}
                            }
                        }
                        /*
                        for(std::string cat: lastprogram.allCategories()){

                            if(cat.length()>2){

                                unsigned int perakkaisyysedustus = perakkaisyysEdustus(cat);


                                //double tiheys = categories[cat].count/lahcount;
                                //categories[cat].perakkain()<=0.35
                                //if(shape.raja && tiheys > 0.65 && !program.belongsToCategory(cat)){
                                    //shape.raja = 2;
                                //}

                                /
                                if(perakkaisyysedustus < 1.0e-50 && program.belongsToCategory(cat) && shape.raja==2){
                                    shape.raja = 1;
                                }
                                /

                                if(perakkaisyysedustus < shape.raja && program.belongsToCategory(cat)){
                                    shape.raja = perakkaisyysedustus;
                                }
                            }
                        }*/
                    //}else{
                    //    shape.raja=1;
                    //}

                    for(int di=0;di<=shape.colspan;di++){
                        lastPrograms[shape.d+di] = program;
                    }

                    assert(shape.isCorrect());
                    //assert(((t+rowspan+1) % (TICKSINHOUR*HOURSINDAY)) < TICKSINHOUR*HOURSINDAY);

                    {
                        // Kootaan listaan peräkkäisiltä päiviltä vähän eri aikaan tulevat
                        // sendingGroup-luokkaan
                        // käyttämällä apuna prevDaySending ja nextDaySending muuttujia.
                        // tämä pitäisi siirtää
                        std::vector<episode_t> jointEpisodesDaily = jointEpisodes;
                        std::vector<sending_t> jointSendingsDaily = jointSendings;

                        /*

                        {
                            int count = 0;

                            while(jointSendingsDaily.back().nextDaySending != -1 && count < 6){
                                int nextDaySending = jointSendingsDaily.back().nextDaySending;
                                count++;
                                if(nextDaySending!=-1){
                                    int episodeId = sendings.at(nextDaySending).episodeId;
                                    if(episodeId!=-1 && episodeId < episodes.size()){
                                        jointSendingsDaily.push_back(sendings.at(nextDaySending));
                                        jointEpisodesDaily.push_back(episodes.at(episodeId));
                                    }else{
                                        break;
                                    }
                                }
                            }
                        }

                        {
                            int count = 0;

                            while(jointSendingsDaily.front().prevDaySending != -1 && count < 6){
                                int prevDaySending = jointSendingsDaily.front().prevDaySending;
                                count++;
                                if(prevDaySending!=-1){
                                    int episodeId = sendings.at(prevDaySending).episodeId;
                                    if(episodeId!=-1 && episodeId < episodes.size()){
                                        jointSendingsDaily.insert(jointSendingsDaily.begin(), sendings.at(prevDaySending));
                                        jointEpisodesDaily.insert(jointEpisodesDaily.begin(), episodes.at(episodeId));
                                    }else{
                                        break;
                                    }
                                }
                            }
                        }
                        */

                        sendingGroup sgdaily(jointSendingsDaily, program, jointEpisodesDaily);




                        tablewriter.addCell(sg, shape, sgdaily);
                    }

                    if(colspan>0){
                        d=d+colspan;
                    }
                }
            }
            tablewriter.end_row();
        }else{

            for(int d=0;d<DAYSINWEEK;d++){
                if(rowspans[d]>0){
                    rowspans[d]=rowspans[d]-1;
                }
            }

        }
    }
}

void WeekTable::getList(Table_writer &tablewriter){

}

int main_sfgg(int argc, char **argv)
{
    /*
    struct tm Tm1 = convert_to_sec_since1970("20171212173020");
    struct tm Tm2 = convert_to_sec_since1970("20171212180020");

    struct tm Tm1h = convert_to_sec_since1970("20171213173020");
    struct tm Tm2h = convert_to_sec_since1970("20171213180020");
    */

    struct tm Tm1 = convert_to_sec_since1970("20210125173020");
    struct tm Tm2 = convert_to_sec_since1970("20210125173520");

    struct tm Tm1h = convert_to_sec_since1970("20210125173520");
    struct tm Tm2h = convert_to_sec_since1970("20210125175520");



    WeekTable vl;
    vl.setWeek(4);
    vl.addProgram(Tm1,  Tm2,  "Uutiset", "", 0, 0, 0, 0, "", "", std::vector<std::string>{});
    vl.addProgram(Tm1h, Tm2h, "Saa", "", 0, 0, 0, 0, "", "", std::vector<std::string>{});
    vl.getHTML(std::cout);

    //vl.setOhjelma(2,1,"Uutiset2");
    //vl.setOhjelma(3,1,"Uutiset");
    /*

    //struct tm * timeinfo;

    struct tm Tm = ConvertToSecSince1970("20171212173020");
    printf("%s\n", asctime(&Tm));
    //timeinfo = localtime ( &rawtime );

    struct tm start;
    start.tm_year=2017-1900;
    start.tm_mon=12-1;
    start.tm_mday=12;
    start.tm_hour=17;
    start.tm_min=10;
    start.tm_sec=10;
    mktime(&start);

    printf("%s\n", asctime(&start));
    printf("%d\n", start.tm_wday);*/
}
