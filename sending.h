#ifndef SENDING_H
#define SENDING_H

#include <string>
#include <assert.h>
#include "setup.h"

typedef int Tick;

typedef struct {

    int programId;          //index of program-vector
    int episodeId;          //index of episode-vector
    int count=0;

} sending_t;

struct program_t{

    std::string title;
    std::string categories;
    std::string director;

    int year=0;
    int season=0;
    int episodeTotal=0;

    int count=0;

    std::string url;

    bool operator ==(const program_t& c);
};

struct episode_t{

    int programId;

    //Episodename.
    //Only one phrase with one or a few words.
    //in XMLTV 'sub-title'-tag.
    std::string subtitle;

    int number=0;       //episode number in season

    int firstshow=-1;   // address of sendingtable

    bool hasMultipleShows=false;

    int count=0;

    bool operator ==(const episode_t& c)
    {
        assert(programId);
        assert(c.programId);
        //auto other = dynamic_cast<const Circle*>(&c);
        //return other != 0 and /* check equality */;
        return programId==c.programId && subtitle==c.subtitle && number==c.number;
        //return alanimi==c.alanimi && nro==c.nro;
    }
};

struct sendingshape_t{
    int d;
    int t;
    int colspan;
    int rowspan;

    int totaltime(){
        return TICKSINDAY * d + t;
    }
};

#endif // SENDING_H
