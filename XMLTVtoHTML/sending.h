#ifndef SENDING_H
#define SENDING_H

#include <string>
#include <assert.h>
#include <vector>
#include "setup.h"
#include "../Rutiinit/stringrutins.h"

typedef int Tick;

struct sending_t{

    int programId;          //index of program-vector
    int episodeId;          //index of episode-vector
    int count=0;

    tm start;
    tm end;

    //päivittäinen lähetysjatkumo
    //int isDaily=NODAILY;
    int prevDaySending = -1;
    int nextDaySending = -1;

    int sendingGroupId=-1;

    //onko naapurissa samaa aikaa?
    bool sametime = false;

    bool operator ==(const sending_t& c);

    //kesto minuutteina
    unsigned int kesto() const;
};

struct program_t{

    std::string title;
    std::string categories;
    std::string director;
    std::string desc;       //common description of program

    int year=0;

    bool isVisible=true;

    int count=0;

    std::vector<std::string> urls;

    int keskipituus=0;

    //Lisää pituuden säilyttäen keskiarvon
    void lisaaPituus(int pituus){
        double kerroin=1.0;
        assert(count);
        if(count>0){
            kerroin=kerroin/count;
        }
        keskipituus=pituus*kerroin + keskipituus*(1.0-kerroin);
    }

    int getKeskipituus(){
        return keskipituus/60;
    }

    std::string getDesc(){
        return (count>2 ? desc : "");
    }

    bool belongsToCategory(std::string category) const{
        return (categories+" ").find(category+" ") != std::string::npos;
    }

    std::vector<std::string> allCategories() const {

        std::vector<std::string> cats;
        split(categories, ' ', cats);

        std::vector<std::string> pal;
        for(std::string cat: cats){
            if(cat.length()>0){
                pal.push_back(cat);
            }
        }

        return pal;
    }

    bool operator ==(const program_t& c);
};

struct episode_t{

    int programId;

    //Episodename.
    //Only one phrase with one or a few words.
    //in XMLTV 'sub-title'-tag.
    std::string subtitle;    

    int number=0;       //episode number in season

    std::string desc;

    std::string url;    //episode url

    int firstshow=-1;   // address of sendingtable

    int season=0;
    int episodeTotal=0;

    bool hasMultipleShows=false;

    int count=0;

    bool operator ==(const episode_t& c)
    {
        assert(programId);
        assert(c.programId);
        //auto other = dynamic_cast<const Circle*>(&c);
        //return other != 0 and /* check equality */;
        return programId==c.programId && subtitle==c.subtitle && number==c.number && desc==c.desc;
        //return alanimi==c.alanimi && nro==c.nro;
    }

    //true, jos sarja alkaa
    bool isFirst() const{
        return number==1;
    }
};

//region on sending table
struct sendingshape_t{
    unsigned int d;
    unsigned int t;
    unsigned int colspan;
    unsigned int rowspan;
    unsigned int colkesto;
    unsigned int rowkesto;
    unsigned int raja;

    unsigned int totaltime(){
        return TICKSINDAY * d + t;
        //return TICKSINDAY * colkesto + rowkesto;
    }

    bool isCorrect(){
        return rowkesto<1000000;
    }
};

#endif // SENDING_H
