#include "html_table_writer.h"
#include "colors.h"
#include "stringrutins.h"

const char *short_day_names[] = {
    "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
};

Html_table_writer::Html_table_writer(Writer &awriter, bool onlytable): writer(awriter)
{
    if(!onlytable){
        writer.openElt("html");
        generateHeader();
        writer.openElt("h1").content(generateName().c_str()).closeElt();
    }

    writer.openElt("table");

    writer.openElt("colgroup");
    writer.openElt("col").closeElt();
    for(int d=0;d<DAYSINWEEK;d++){
        writer.openElt("col").attr("class", "DayColumn").closeElt();
    }
    writer.closeElt();

    generateDayHeader();
}

void Html_table_writer::start_row(int tick)
{
    writer.openElt("tr");
    generateTimeHeader(tick);
}

void Html_table_writer::end_row()
{
    writer.closeElt();
}

void Html_table_writer::addCell(program_t program, std::vector<episode_t> jointEpisodes, sendingshape_t shape)
{
    //assert(((DAYSINWEEK-1) + (TICKSINDAY-1)) > totaltime);

    std::string str_subtitle="";
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

    //decide proper size for program label
    std::string sizeclass="ProgramTitle";
    bool showepisode=false;
    bool showsubtitle=false;
    {
        int minutes = (shape.rowspan+1)*(60/TICKSINHOUR);

        //cut too long words
        if(minutes<10 && shape.colspan<1)
            if(program.title.length()>15)
                program.title=program.title.substr(0,13)+"...";

        int wordlenght = longest_word(program.title);

        if(minutes<15)
            sizeclass+=" SingleWide";
        //if(minutes<45)
        //    sizeclass+=" SingleWide";

        if(program.title=="Korean olympialaiset" || program.title==""){
            showsubtitle=true;
            showepisode=true;
        }

        if(minutes>10){
            if(str_subtitle.length()<minutes*(shape.colspan+1)){
                showsubtitle=true;
            }
            showepisode=true;
        }

        /*
        if(minutes>40)
            showepisode=true;

        if(minutes>80)
            showsubtitle=true;
        */

        //movies
        if(minutes>60 && wordlenght<13)
            sizeclass+=" DoubleWide";

        //daily series
        if(minutes>20 && shape.colspan>1)
            sizeclass+=" DoubleWide";

        if(minutes>55 && shape.colspan>1)
            sizeclass+=" QuadWide";
    }

    std::string str_episode="";
    {
        if(program.season>0)
            str_episode+="S"+std::to_string(program.season);
        else{
            str_episode+=program.director;
            if(program.year!=-1){
                if(str_episode.length()>4)
                    str_episode+=", ";
                str_episode+=std::to_string(program.year);
            }
        }
    }



    std::string str_show="";
    Tick firstShown=-1;

    if(jointEpisodes.size()>0 && jointEpisodes.at(0).number>0){

        //jos on yhdistetty useita jaksoja
        if(jointEpisodes.size()>1){
            if(jointEpisodes.at(jointEpisodes.size()-1).number>0)
                str_episode+="E"+std::to_string(jointEpisodes.at(0).number)+"-"+
                        std::to_string(jointEpisodes.at(jointEpisodes.size()-1).number);
        }else{
            str_episode+="E"+std::to_string(jointEpisodes.at(0).number);


            if(jointEpisodes.at(0).hasMultipleShows){
                if(jointEpisodes.at(0).firstshow < shape.totaltime()){
                    firstShown = (jointEpisodes.at(0).firstshow % TICKSINDAY)/TICKSINHOUR+FIRSTHOUR;
                    str_show = std::string("(R) ");

                    //assert(((DAYSINWEEK-1) + (TICKSINDAY-1)) > jointEpisodes.at(0).firstshow);

                    if(((DAYSINWEEK-1) * (TICKSINDAY-1)) > jointEpisodes.at(0).firstshow){

                        if(jointEpisodes.at(0).firstshow>0){
                            str_show += short_day_names[jointEpisodes.at(0).firstshow / TICKSINDAY];
                        }else{
                            str_show += std::string("Last week");
                        }
                    }

                }else{
                    str_show = std::string("(N)");
                    firstShown=0;
                }
            }
        }
    }

    if(str_episode!="")
        str_episode=std::string(" " + str_episode);

    /////


    writer.openElt("td");

    if(shape.colspan>0){
        writer.attr("colspan", std::to_string(shape.colspan+1).c_str());
    }

    if(shape.rowspan>0){
        writer.attr("rowspan", std::to_string(shape.rowspan+1).c_str());
    }

    if(CATCOLORS){
        writer.attr("class", (program.categories).c_str());
    }else{
        if(program.count>0){
            int hue = std::hash<std::string>()(program.title) % 360;
            hsv vari = {hue, 0.2, 0.98};
            rgb varirgb=hsv2rgb(vari);
            writer.attr("style", std::string("background-color: #") + rgb2Hex(varirgb) + ";");
        }
    }

    //program title
    if(program.url.length()>0){
        writer.openElt("a");
        writer.attr("href", program.url);
    }
    writer.openElt("b");
    writer.attr("class", sizeclass.c_str());
    writer.content((program.title).c_str());
    writer.closeElt();
    if(program.url.length()>0)
        writer.closeElt();

    //episode and references its other shows
    if(showepisode){
        writer.openElt("span").attr("class", "ProgramSeason");
        writer.content(str_episode.c_str());
        if(firstShown!=-1){
            if(firstShown>0){
                writer.openElt("a");
                writer.attr("href", std::string("#") + std::to_string(firstShown));
            }
            writer.content(str_show.c_str());
            if(firstShown>0)
                writer.closeElt();
        }
        writer.closeElt();

        if(showsubtitle){
            writer.openElt("span").attr("class", "ProgramSubtitle");
            writer.content(str_subtitle.c_str());
            writer.closeElt();
        }
    }

    writer.closeElt();
}

void Html_table_writer::generateHeader()
{
    writer.openElt("head");
    writer.openElt("meta").attr("charset", "utf-8").closeElt();
    writer.openElt("link").attr("rel", "stylesheet").attr("type", "text/css").attr("href", "../style.css").closeElt();
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

void Html_table_writer::generateDayHeader()
{
    const char *day_names[] = {
        "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
    };

    writer.openElt("tr");
    writer.openElt("th").content("klo").closeElt();
    for(int i=0;i<7;i++)
        writer.openElt("th").content(day_names[i]).closeElt();
    writer.closeElt();
}

void Html_table_writer::generateTimeHeader(int t)
{

    if(t % (TICKSINHOUR/4)==0 || TICKSINHOUR==1){
    //if((t % TICKSINHOUR)==0){

        writer.openElt("th")
                .attr("rowspan", std::to_string(TICKSINHOUR/4).c_str())
                //.attr("rowspan", std::to_string(2).c_str())
                .attr("class", "TimeColumn");

        //if on the hour
        if(t % (TICKSINHOUR)==0){

            int hour=t/TICKSINHOUR+FIRSTHOUR;
            if(hour>23)
                hour-=24;

            writer.openElt("a");
            writer.attr("name", std::to_string(hour).c_str());
            writer.closeElt();

            writer.content(std::to_string(hour).c_str());
        }
        writer.closeElt();
    }
}

void Html_table_writer::close()
{
    writer.closeAll();
}
