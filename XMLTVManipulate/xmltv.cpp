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
#include <iostream>
#include <cstdlib>
#include "xmltv.h"
#include "names.h"
#include <episodenum.h>
#include "../XMLTVtoHTML/datetime.h"
#include "../XMLTVtoHTML/descparser.h"
#include "../XMLTVtoHTML/stringrutins.h"

const xmlChar *channel=nullptr;
const xmlChar *language=xmltvp::fi;

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
            if (!xmlStrcmp(cur_node->name, xmltvp::director)){
                director = std::string(((char *)xmlNodeGetContent(cur_node)));
                //fprintf(stderr,"O: %s\n", director.c_str());
                return;
            }
        }
    }
}



static void parse_programme(xmlNode * a_node)
{
    xmlNode *cur_node = nullptr;

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

            const xmlChar* nodename  = cur_node->name;

            if (!xmlStrcmp(nodename, xmltvp::title)){

                if (!xmlStrcmp(xmlGetProp(cur_node, xmltvp::lang), language) || title.length()==0){
                //if(strcmp((char *)xmlGetProp(cur_node, xmltvp::lang), language) == 0 || title.length()==0){

                    title = removeSign(std::string((char *)xmlNodeGetContent(cur_node)));

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
                    if(title=="#Subleffa" || title=="Kino Suomi" || title=="Kino Klassikko" || title=="Kotikatsomo"){
                        title=subtitle;
                        subtitle="";
                        categories+=" Movies";
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
                        title = removeSign(std::string((char *)xmlNodeGetContent(cur_node)));
                        subtitle="";
                    }

                }

            }

            //if(strcmp(nodename, SUBTITLE) == 0){
                //printf("%s", xmlNodeGetContent(cur_node));
               // break;
            //}

            if (!xmlStrcmp(nodename, xmltvp::desc)){

                if(!xmlStrcmp(xmlGetProp(cur_node, xmltvp::lang), language)){

                    std::string description((char *)xmlNodeGetContent(cur_node));

                    ohjelmaprops op = parse_description(description);
                    jakso=op.jakso;
                    vuosi=op.vuosi;
                    yhteensa=op.jaksoja;
                    kausi=op.kausi;
                    director=op.ohjaaja;

                    /*
                    if(desc.length()>0 && op.tarkennin.length()>0){
                        desc+=". ";
                    }*/

                    desc+=op.tarkennin;
                }
            }

            if (!xmlStrcmp(nodename, xmltvp::category)){
                categories+=std::string((char *)xmlNodeGetContent(cur_node))+" ";

                if(title=="Uutisikkuna" || title=="Astral-TV"){
                    const xmlChar *category = xmlCharStrdup("category");
                    const xmlChar *nonprogram = xmlCharStrdup("NoneSeries");
                    xmlNodePtr ptr = xmlNewTextChild(cur_node->parent, NULL,  category, nonprogram);

                    xmlAttrPtr newattr;

                    newattr = xmlNewProp (ptr, xmltvp::lang, xmltvp::en);

                    cur_node = cur_node->parent;
                    cur_node = cur_node->next;
                }
            }

            if (!xmlStrcmp(nodename, xmltvp::date)){
                std::string date((char *)xmlNodeGetContent(cur_node));
                vuosi = string_to_int(date);
            }

            if (!xmlStrcmp(nodename, xmltvp::url)){
                url = std::string((char *)xmlNodeGetContent(cur_node));
            }

            if (!xmlStrcmp(nodename, xmltvp::credits)){
                parse_credits(cur_node->children);
            }

            if (!xmlStrcmp(nodename, xmltvp::episode_num)){
                if (!xmlStrcmp(xmlGetProp(cur_node, xmltvp::episodesystem), xmltvp::xmltv_ns)){
                    std::string episode((char *)xmlNodeGetContent(cur_node));
                    parse_xmltv_ns(episode, kausi, jakso, yhteensa);
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
            //if(strcmp((char *)(cur_node->name), PROGRAMME) == 0){
            if (!xmlStrcmp(cur_node->name, xmltvp::programme)){

                if(channel==0){
                    channel = xmlGetProp(cur_node, xmltvp::channel);
                    //wt.setChannelName(channel);
                }

                if(channel==0 || !xmlStrcmp(xmlGetProp(cur_node, xmltvp::channel), channel)){

                    struct tm alkaa  = convert_to_sec_since1970((char *)xmlGetProp(cur_node, xmltvp::start));
                    struct tm loppuu = convert_to_sec_since1970((char *)xmlGetProp(cur_node, xmltvp::stop));

                    parse_programme(cur_node->children);

                    if(title=="Talviolympialaiset 2018"){
                        categories+=" sports olympic";
                        url=getOlympicLinks(subtitle+desc);
                    }

                    if(title=="Korean olympialaiset"){
                        categories+=" olympic";
                        url=getOlympicLinks(subtitle+desc);

                    }

                    if(title=="Korean paralympialaiset"){
                        categories+=" olympic";
                    }

                    if(title=="Uutisikkuna" || title=="Astral-TV"){
                        categories="NoneSeries";
                    }

                    url="";
                    if(title=="Uutisvuoto"){
                        url="../ohjelmat/uutisvuoto";
                    }

                    //if(title=="Uutisvuoto"){

                        //wt.addProgram(alkaa, loppuu, title, categories, director,
                        //                  vuosi, kausi, jakso, yhteensa, subtitle, desc, url);
                    //}

                    std::cout << title << std::endl;
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

            //if(strcmp((char *)(cur_node->name), TV) == 0){
            if (!xmlStrcmp(cur_node->name, xmltvp::tv)){
                parse_tv(cur_node->children);
            }

        }
    }
}

void manipulateXML(const char* xmlfile)
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

    xmlSaveFormatFile ("/home/jari/works/ohjelmat/Qt/XMLTVManipulate/out.xml", doc, 1);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();
}
