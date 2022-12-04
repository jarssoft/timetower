/**
 * section: Tree
 * synopsis: Navigates a tree to print element names
 * purpose: Parse a file to a tree, use xmlDocGetRootElement() to
 *          get the root element, then walk the document and print
 *          all the element name in document order.
 * usage: tree1 filename_or_URL
 * test: tree1 test2.xml > tree1.tmp && diff tree1.tmp $(srcdir)/tree1.res
 * author: Dodji Seketeli
 * copy: see Copyright for the status of this software.
 */
#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/parser.h>
#include <argp.h>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include "xmltv.h"
#include "timetable.h"
#include "../Rutiinit/datetime.h"
#include "../Rutiinit/stringrutins.h"
#include "../XMLTVp/access.h"

#define TV              "tv"
#define PROGRAMME       "programme"

//program start and end
//const xmlChar *startkey = xmlCharStrdup("start");
//const xmlChar *stopkey = xmlCharStrdup("stop");

//programs channel
//const xmlChar *channelkey = xmlCharStrdup("channel");

//program title and description
const xmlChar *lang = xmlCharStrdup("lang");
#define TITLE           "title"
#define DESC            "desc"
#define SUBTITLE        "sub-title"
#define URL             "url"

//episode and its system
const xmlChar *episodesystemkey = xmlCharStrdup("system");
#define EPISODENUM      "episode-num"
#define XMLTVNS         "xmltv_ns"

//date and credits
#define DATE            "date"
#define CREDITS         "credits"

//program categoty
#define CATEGORY        "category"

/*
 TODO-tags:
 - previously-shown
 - country
 - premiere
 */

char* channel=(char*)"";
char* language=(char*)"fi";

WeekTable wt;

std::string title;
std::string subtitle;
std::string desc;
std::string categories;
std::string director;
std::string url;

int vuosi, kausi, jakso, yhteensa;

/*
 *To compile this file using gcc you can type
 *gcc `xml2-config --cflags --libs` -o xmlexample libxml2-example.c
 */

void setChannel(char* achannel)
{
    channel=achannel;
    wt.setChannelName(achannel);
}

char* getChannel()
{
    return channel;
}

void setLanguage(char* alanguage)
{
    language=alanguage;
}

void importXML(const char* xmlfile)
{
    xmltvp::parseXMLTV(xmlfile, nullptr, importProgram);
}

void exportHTML(std::ostream& out)
{
    wt.getHTML(out);
}

void exportSVG(std::ostream& out)
{
    wt.getSVG(out);
}

void setWeek(int weeknum)
{
    wt.setWeek(weeknum);
}

void setWeekday(int weekdaynum)
{
    wt.setWeekDay(weekdaynum);
}

bool importProgram(xmlNode *node)
{
    //assert(alkaa.tm_year>100);

    std::string title = xmltvp::getTitle(node);
    std::string categories = xmltvp::getCategories(node);
    std::string subtitle = xmltvp::getSubtitle(node);
    std::string desc = xmltvp::getDesc(node);
    std::vector<std::string> urls = xmltvp::getURL(node);
    int vuosi = -1;
    std::string date = xmltvp::getDate(node);
    if(!date.empty()){
        vuosi=string_to_int(date);
    }

    xmltvp::episode_ns episode = xmltvp::getEpisodenum(node);

    wt.addProgram(xmltvp::getStart(node), xmltvp::getStop(node), title, categories,
                      vuosi, episode.season, episode.episode, episode.total_episodes, subtitle, desc, urls);
}
