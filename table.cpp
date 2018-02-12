#include <algorithm>
#include <string>
#include "table.h"
#include "datetime.h"
#include "colors.h"
#include "stringrutins.h"
#include "svg_writer.h"

program_t none{"None", "None", "", -1, -1};
program_t varied{"Varied", "Varied", "", -1, -1};
program_t levelling_up{"Levelling up", "LevellingUp", "", -1, -1};
program_t levelling_down{"Levelling down", "LevellingDown", "", -1, -1};

episode_t nonejakso{-1, "", -1};

#define NONE            0
#define VARIEND         1
#define LEVELLING_UP    2
#define LEVELLING_DOWN  3

using namespace std;

//converts tm-structure to index of sendingtable
static void tm_to_tick(tm Tm, int week, int &weekday, Tick &tick){

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

WeekTable::WeekTable()
{
    //clear sendingtable
    for(int d=0;d<DAYSINWEEK;d++){
        for(int t=0;t<TICKSINDAY;t++){
            this->sendingtable[d][t]=NONE;
            this->sendingcount[d][t]=0;
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

    monday_has_set=false;
}

void WeekTable::setWeek(int weeknro)
{
    this->weeknro = weeknro;
}

void WeekTable::setChannelName(string name_of_channel)
{
    this->name_of_channel = name_of_channel;
}

void WeekTable::addProgram(const tm start, const tm end, std::string title, std::string categories, std::string director, int year, int season, int episode, int episodetotal, std::string subtitle, std::string url)
{
    program_t prog;
    prog.title=title;
    prog.categories=categories;
    //ohj.director=director;
    prog.year=year;
    prog.season=season;
    prog.episodeTotal=episodetotal;
    prog.url=url;

    int prog_id = addUnique(prog, programs);

    episode_t epis;
    epis.number=episode;
    epis.subtitle=subtitle;
    epis.firstshow=-1;

    //if week is not set, using week on first program
    if(this->weeknro==-1)
        this->weeknro=week_number(start);

    Tick starttick;
    Tick endtick;
    int startday;
    int endday;

    tm_to_tick(start, this->weeknro, startday, starttick);
    tm_to_tick(end, this->weeknro, endday, endtick);

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

            if(startday==0 && !monday_has_set){
                monday=start;
                monday_has_set=true;
            }

            //assert(!(endday == 0 && startday == -1));
            //assert(!(startday == DAYSINWEEK-1 && endday == DAYSINWEEK));

        }
    }

    assert(starttick>=0);
    assert(endtick<TICKSINDAY);

    addProgram(startday, starttick, endday, endtick, prog_id, epis);
}

void WeekTable::addProgram(const int startday, const int starttick, const int endday, const int endtick, int prog_id, episode_t epis)
{
    assert(startday>=-1);
    assert(startday<DAYSINWEEK+1);
    assert(starttick<TICKSINDAY);
    assert(endtick<=TICKSINDAY);
    assert(endtick>=0);
    assert(starttick>=0);

    epis.programId=prog_id;

    int jid = addUnique(epis, episodes);

    int kokonaisaika=startday*TICKSINDAY+starttick;

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
    if(endday<0 || startday>DAYSINWEEK-1)
        return;

    sending_t lah;
    lah.programId = prog_id;
    lah.episodeId = jid;
    sendings.push_back(lah);
    int lid=sendings.size()-1;

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

                //look-up
                int naapuri[3][4];
                for(int dl=-1;dl<2;dl++){
                    for(int tl=-1;tl<3;tl++){
                        int id = 0;
                        if(d+dl>-1 && d+dl<DAYSINWEEK && t+tl>-1 && t+tl<TICKSINDAY){
                            id = sendings.at(this->sendingtable[d+dl][t+tl]).programId;
                            //fprintf(stderr,"asetettiin id %d.\n",tl);
                        }
                        naapuri[dl+1][tl+1]=id;
                    }
                }

                int tama=naapuri[1][1];


                if(naapuri[1][2]){
                    if(naapuri[0][1]
                            && naapuri[0][1]!=tama
                            && naapuri[0][2]==tama
                            && naapuri[1][2]==tama
                            && naapuri[2][1]!=tama
                            ){
                        this->sendingtable[d][t]=LEVELLING_DOWN;
                        //fprintf(stderr,"yhdistettiin kohdassa %d - %d\n", d, t);
                    }
                    if(naapuri[2][1]
                            && naapuri[2][1]!=tama
                            && naapuri[2][2]==tama
                            && naapuri[1][2]==tama
                            && naapuri[0][1]!=tama
                            ){
                        this->sendingtable[d][t]=LEVELLING_DOWN;
                        //fprintf(stderr,"yhdistettiin kohdassa %d - %d\n", d, t);
                    }
                }

                if(naapuri[1][0] && naapuri[1][2]!=tama){
                    if(naapuri[0][1]
                            && naapuri[0][1]!=tama
                            && naapuri[0][0]==tama
                            && naapuri[1][0]==tama
                            ){
                        this->sendingtable[d][t]=LEVELLING_UP;
                        fprintf(stderr,"yhdistettiin kohdassa %d - %d\n", d, t);
                    }
                    if(naapuri[2][1]
                            && naapuri[2][1]!=tama
                            && naapuri[2][0]==tama
                            && naapuri[1][0]==tama
                            ){
                        this->sendingtable[d][t]=LEVELLING_UP;
                        fprintf(stderr,"yhdistettiin kohdassa %d - %d\n", d, t);
                    }
                }
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

sending_t WeekTable::sendingByCoords(int d, Tick t)
{
    return sendings.at(sendingtable[d][t]);
}

void WeekTable::getHTML(std::ostream& out)
{    
    Writer writer(out);
    Table_writer *tablewriter;
    tablewriter = new Html_table_writer(writer, true);
    getHTMLTable(*tablewriter);
    tablewriter->close();
}

void WeekTable::getSVG(std::ostream& out)
{
    Writer writer(out);
    Table_writer *tablewriter;
    tablewriter = new Svg_writer(writer);
    getHTMLTable(*tablewriter);
    tablewriter->close();
}

bool WeekTable::canJoint(sending_t &sending1, sending_t &sending2){

    bool canjoin = (sending1.programId == sending2.programId);

    //olympialaisissa täytyy olla sama subtitle
    if(canjoin){
        std::string title = programs.at(sending1.programId).title;
        if(title=="Korean olympialaiset" || title=="Talviolympialaiset 2018"){
            //canjoin=(episodes.at(sending1.programId).subtitle == episodes.at(sending2.programId).subtitle);
            canjoin=(episodes.at(sending1.episodeId).subtitle == episodes.at(sending2.episodeId).subtitle);
        }
    }

    return canjoin;
}

void WeekTable::getHTMLTable(Table_writer &tablewriter){

    int rowspans[DAYSINWEEK] = {0};
    int namehashs[DAYSINWEEK] = {0};

    for(Tick t=0;t<TICKSINDAY;t++){

        tablewriter.start_row(t);

        for(int d=0;d<DAYSINWEEK;d++){

            //joins row until program ends
            if(rowspans[d]>0){

                rowspans[d]--;

            }else{

                sending_t sending = sendingByCoords(d, t);
                const int programId = sending.programId;
                std::vector<episode_t> jointEpisodes;

                addUnique(episodes.at(sending.episodeId), jointEpisodes);

                assert(programId>=0);
                assert(programId<programs.size());

                int rowspan=0;

                //try to join next ticks
                for(int i=1;i<TICKSINDAY-t;i++){
                    sending_t thisSending = sendingByCoords(d, t+i);
                    if((JOIN_SIMILAR_PROGRAMS && canJoint(sending, thisSending)) ||
                            sendingtable[d][t+i]==sendingtable[d][t]){
                        rowspan++;
                        addUnique(episodes.at(thisSending.episodeId), jointEpisodes);
                    }else
                        break;
                }

                if(rowspan>0){
                    rowspans[d]=rowspan;
                }

                //try to join same program in next days
                int colspan=0;
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
                        }

                        if(continues){
                            sending_t sending = sendingByCoords(d+dl, t);
                            addUnique(episodes.at(sending.episodeId), jointEpisodes);
                            colspan++;
                            rowspans[d+dl]=rowspan;
                        }
                    }
                }

                program_t program = this->programs[programId];

                //if title is same than before, remove title
                int hash = std::hash<std::string>()(program.title);
                if(hash == namehashs[d]){
                    program.title = "";
                }else{
                    namehashs[d] = hash;
                }

                sendingshape_t shape={d, t, colspan, rowspan};
                tablewriter.addCell(program, jointEpisodes, shape);

                if(colspan>0){
                    d=d+colspan;
                }
            }
        }
        tablewriter.end_row();
    }
}

int mainppp(int argc, char **argv)
{
    struct tm Tm1 = convert_to_sec_since1970("20171212173020");
    struct tm Tm2 = convert_to_sec_since1970("20171212180020");

    struct tm Tm1h = convert_to_sec_since1970("20171213173020");
    struct tm Tm2h = convert_to_sec_since1970("20171213180020");

    WeekTable vl;
    vl.addProgram(Tm1, Tm2, "Uutiset", "", "", 0, 0, 0, 0, "", "");
    vl.addProgram(Tm1h, Tm2h, "Uutiset", "", "", 0, 0, 0, 0, "", "");
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
