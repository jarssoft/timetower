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
#include "../Rutiinit/datetime.h"
#include "xmltv.h"
#include "../Rutiinit/stringrutins.h"
#include "table.h"

#define TV              "tv"
#define PROGRAMME       "programme"

//program start and end
const xmlChar *startkey = xmlCharStrdup("start");
const xmlChar *stopkey = xmlCharStrdup("stop");

//programs channel
const xmlChar *channelkey = xmlCharStrdup("channel");

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

char* channel="";
char* language="fi";

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



static void parse_credits(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if(strcmp((char *)(cur_node->name), "director") == 0){
                director = std::string(((char *)xmlNodeGetContent(cur_node)));
                //fprintf(stderr,"O: %s\n", director.c_str());
                return;
            }
        }
    }
}

//convert string type of xmltv_ns to sets of integer (season, episode, total episodes)
//xmltv-format is used on xmltv, example: "0 . 9/10"
static void parse_xmltv_ns(std::string ns, int &season, int &episode, int &total_episodes){

    std::vector<std::string> parts;
    split(ns.c_str(), '.', parts);

    season = string_to_int(parts.at(0));

    if (season!=-1) {
        season=season+1;
    }

    if(parts.size()>1){
        std::vector<std::string> parts2;
        split(parts.at(1).c_str(), '/', parts2);
        if(parts2.size()>0){
            episode = string_to_int(parts2.at(0));
            if(parts.size()>1){
                total_episodes = string_to_int(parts2.at(1));
            }
            if (episode!=-1) {
                episode=episode+1;
            }
        }else{
            //otetaan suoraan vektorista, jos ei kauttaviivaa
            episode = string_to_int(parts.at(1));
            if (episode!=-1) {
                episode=episode+1;
            }
        }
    }
}

static void parse_programme(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;

    categories="";
    title = "";
    subtitle = "";
    desc="";
    url="";
    director="";

    jakso=0;
    yhteensa=0;
    vuosi=-1;
    kausi=-1;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {

        if (cur_node->type == XML_ELEMENT_NODE) {

            char* nodename  = (char *)(cur_node->name);

            if(strcmp(nodename, TITLE) == 0){

                if(strcmp((char *)xmlGetProp(cur_node, lang), language) == 0 || title.length()==0){

                    //title = removeSign(std::string((char *)xmlNodeGetContent(cur_node)));
                    title = std::string((char *)xmlNodeGetContent(cur_node));

                    std::vector<std::string> parts;
                    split(title.c_str(), ':', parts);
                    if(parts.size()>1){
                        title=parts.at(0);
                        subtitle=parts.at(1);
                    }
                    if(parts.size()>2){
                        desc=parts.at(2)+". ";
                    }

                    //ohjelmapaikka (Elävä arkisto, Subleffa)
                    //ohjelmatyyppi (Elokuva)
                    //ohjelma (MOT)
                    if(title=="#Subleffa" || title=="Kino Suomi" || title=="Kino Klassikko"){
                        title=subtitle;
                        subtitle="";
                        categories+=" movie Movies";
                    }
                    if(title=="LIVE"){
                        title=subtitle;
                        subtitle="";
                    }
                    if(title=="Dok" || title=="Tiededokumentti" || title=="Historia"){
                        title=subtitle;
                        subtitle="";
                        categories+=" Documentary";
                    }

                    if(subtitle.length()>0){
                        //title = removeSign(std::string((char *)xmlNodeGetContent(cur_node)));
                        title = std::string((char *)xmlNodeGetContent(cur_node));
                        subtitle="";
                    }

                }
            }
            if((strcmp(nodename, SUBTITLE) == 0) && subtitle==""){
                //printf("%s", xmlNodeGetContent(cur_node));
               // break;
                if(strcmp((char *)xmlGetProp(cur_node, lang), language) == 0 ){
                    std::string description((char *)xmlNodeGetContent(cur_node));
                    subtitle = description;
                }
            }

            if(strcmp(nodename, DESC) == 0){

                if(strcmp((char *)xmlGetProp(cur_node, lang), language) == 0 ){

                    std::string description((char *)xmlNodeGetContent(cur_node));

                    /*
                    ohjelmaprops op = parse_description(description);
                    jakso=op.jakso;
                    vuosi=op.vuosi;
                    yhteensa=op.jaksoja;
                    kausi=op.kausi;
                    director=op.ohjaaja;
                    */

                    /*
                    if(desc.length()>0 && op.tarkennin.length()>0){
                        desc+=". ";
                    }*/

                    //desc+=op.tarkennin;
                    desc=description;

                    if(subtitle==""){
                        subtitle = first_sentence(desc);
                        if(subtitle.length()<40){
                            desc=desc.substr(subtitle.length());
                        }else{
                            subtitle="";
                        }
                    }
                }
            }

            if(strcmp(nodename, CATEGORY) == 0){
                std::string category = std::string((char *)xmlNodeGetContent(cur_node));
                eraseSubStr(category, " / ");
                eraseSubStr(category, " / ");
                eraseSubStr(category, " / ");
                eraseSubStr(category, " ");
                eraseSubStr(category, " ");
                eraseSubStr(category, " ");

                categories+=category+" ";

                if(category=="movie"){
                    subtitle="";
                }
            }

            if(strcmp(nodename, DATE) == 0){
                std::string date((char *)xmlNodeGetContent(cur_node));
                vuosi = string_to_int(date);
            }

            if(strcmp(nodename, URL) == 0 && url==""){
                url = std::string((char *)xmlNodeGetContent(cur_node));
            }

            if(strcmp(nodename, CREDITS) == 0){
                parse_credits(cur_node->children);
            }

            if(strcmp(nodename, EPISODENUM) == 0){
                if(strcmp((char *)xmlGetProp(cur_node, episodesystemkey), XMLTVNS) == 0 ){
                    std::string episode((char *)xmlNodeGetContent(cur_node));
                    //parse_xmltv_ns(episode, kausi, jakso, yhteensa);
                }
            }
        }
    }
}

static void parse_tv(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {

        if(cur_node->type == XML_ELEMENT_NODE) {
            //printf("node type: Element, name: %s\n", cur_node->name);
            if(strcmp((char *)(cur_node->name), PROGRAMME) == 0){

                if(strcmp(channel, "")==0){
                    channel = (char *)xmlGetProp(cur_node, channelkey);
                    wt.setChannelName(channel);
                }

                if(strcmp(channel, "")==0 || strcmp((char *)xmlGetProp(cur_node, channelkey), channel) == 0 ){

                    struct tm alkaa;
                    memset(&alkaa, 0, sizeof(struct tm));
                    alkaa = convert_to_sec_since1970((char *)xmlGetProp(cur_node, startkey));
                    struct tm loppuu;
                    memset(&loppuu, 0, sizeof(struct tm));
                    loppuu= convert_to_sec_since1970((char *)xmlGetProp(cur_node, stopkey));

                    parse_programme(cur_node->children);

                    if(title=="Talviolympialaiset 2018"){
                        categories+=" sports olympic";
                        //url=getOlympicLinks(subtitle+desc);
                    }

                    if(title=="Korean olympialaiset"){
                        categories+=" olympic";
                        //url=getOlympicLinks(subtitle+desc);
                    }

                    if(title=="Korean paralympialaiset"){
                        categories+=" olympic";
                    }

                    if(title=="Uutisikkuna" || title=="Astral-TV" || title=="Textnytt" || title=="Yök­las­si­nen" ){
                        categories+=" Night";
                    }

                    if(categories==""){
                        categories="NoneSeries";
                    }

                    /*
                    url="";
                    if(title=="Uutisvuoto"){
                        url="../ohjelmat/uutisvuoto";
                    }
                    */

                    //if(title=="Uutisvuoto"){

                    //if(title.size()>0){
                        assert(alkaa.tm_year>100);
                            wt.addProgram(alkaa, loppuu, title, categories, director,
                                              vuosi, kausi, jakso, yhteensa, subtitle, desc, url);
                        //}
                    //}

                }
            }
        }
    }
}

/**
 * print_element_names:
 * @a_node: the initial xml node to consider.
 *
 * Prints the names of the all the xml elements
 * that are siblings or children of a given xml node.
 */
static void parse_root(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {

            //printf("node type: Element, name: %s\n", cur_node->name);

            if(strcmp((char *)(cur_node->name), TV) == 0){
                parse_tv(cur_node->children);
            }

        }
    }
}

/**
 * Simple example to parse a file called "file.xml",
 * walk down the DOM, and print the name of the
 * xml elements nodes.
 */
int mainsdgg(int argc, char **argv)
{
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    if (argc != 3)
        return(1);

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    channel=argv[2];

    /*parse the file and get the DOM */
    doc = xmlReadFile(argv[1], NULL, 0);

    if (doc == NULL) {
        printf("error: could not parse file %s\n", argv[1]);
    }

    /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

    parse_root(root_element);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    //std::ostream& myfile = std::cout;
    std::ofstream myfile;
    myfile.open("/home/jari-intel/works/ohjelmat/Qt/XMLTVtoHTML/table.html");
    wt.getHTML(myfile);
    myfile.flush();
    myfile.close();

    //printf("Yhteensä %d aikaa.\n", sorter.count());

    return 0;
}

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
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    //fprintf(stderr,"Adding %s...\n", xmlfile);

    /*parse the file and get the DOM */
    doc = xmlReadFile(xmlfile, NULL, 0);

    if (doc == NULL) {
        fprintf(stderr, "error: could not parse file %s\n", xmlfile);
    }

    /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

    parse_root(root_element);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();
}

void exportHTML(std::ostream& out){
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

