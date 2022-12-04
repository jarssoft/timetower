#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "access.h"
#include "../Rutiinit/stringrutins.h"
#include "../Rutiinit/datetime.h"

namespace xmltvp {

static void remove_elements_by_name(xmlNode * a_node, const xmlChar *name)
{
    xmlNode *cur_node = nullptr;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(cur_node->name, name)){
                xmlUnlinkNode(cur_node);
            }
        }
    }
}

static bool parse_programme(xmlNode * a_node, nodewithname f)
{
    return f(a_node);
}

std::string getProp(xmlNode *a_node, const xmlChar* name)
{
    xmlNode *cur_node = nullptr;
    std::string prop="";

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(cur_node->name, name)) {
                    prop = std::string((char *)xmlNodeGetContent(cur_node));
                    break;
            }
        }
    }

    return prop;
}

std::string getNodeWithLang(xmlNode *a_node, const xmlChar* name, const xmlChar* language){

    xmlNode *cur_node = nullptr;
    std::string pal="";

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(cur_node->name, name)) {
                if (language == nullptr || !xmlStrcmp(xmlGetProp(cur_node, xmltvp::lang), language)) {
                    pal = std::string((char *)xmlNodeGetContent(cur_node));
                    break;
                }
            }
        }
    }

    return pal;
}

void addSubtitle (xmlNode *node, const char* subtitle) {
    const xmlChar *subtitlexml = xmlCharStrdup(subtitle);
    xmlNodePtr ptr = xmlNewTextChild(node->parent, NULL,  xmltvp::sub_title, subtitlexml);
    xmlSetProp(ptr, xmltvp::lang, xmltvp::fi);
}

void addTitle (xmlNode *node, const char* title) {
    const xmlChar *titlexml = xmlCharStrdup(title);
    xmlNodePtr ptr = xmlNewTextChild(node->parent, NULL,  xmltvp::title, titlexml);
    xmlSetProp(ptr, xmltvp::lang, xmltvp::fi);
}

void addDesc (xmlNode *node, const char* desc) {
    const xmlChar *descxml = xmlCharStrdup(desc);
    xmlNodePtr ptr = xmlNewTextChild(node->parent, NULL,  xmltvp::desc, descxml);
    xmlSetProp(ptr, xmltvp::lang, xmltvp::fi);
}

std::string getDesc(xmlNode * a_node, const xmlChar* language)
{
    return getNodeWithLang(a_node, xmltvp::desc, language);
}

std::string getTitle(xmlNode * a_node, const xmlChar* language)
{
    return getNodeWithLang(a_node, xmltvp::title, language);
}

std::string getSubtitle(xmlNode * a_node)
{
    return getNodeWithLang(a_node, xmltvp::sub_title, xmltvp::fi);
}

xmltvp::episode_ns getEpisodenum(xmlNode * a_node)
{

    //const xmlChar *episodesystem = xmltvp::xmltv_ns;
    xmlNode *cur_node = nullptr;
    episode_ns episodepal = {-1, -1, -1};

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {

            if (!xmlStrcmp(cur_node->name, xmltvp::episode_num)) {
                if (!xmlStrcmp(xmlGetProp(cur_node, xmltvp::episodesystem), xmltvp::xmltv_ns)) {
                    std::string ns_string = std::string((char *)xmlNodeGetContent(cur_node));
                    //std::cout << "getEpisodenum: " << ns_string << std::endl;
                    episodepal = xmltvp::parse_xmltv_ns(ns_string);
                    break;
                }
            }
        }
    }

    return episodepal;
}



std::vector<std::string> getURL(xmlNode * a_node)
{
    xmlNode *cur_node = nullptr;
    std::vector<std::string> urls;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(cur_node->name, xmltvp::url)) {
                    urls.push_back(std::string((char *)xmlNodeGetContent(cur_node)));
            }
        }
    }

    return urls;
}


void setEpisodenum(xmlNode *node, const episode_ns episode)
{
    const xmlChar *episode_ns = xmlCharStrdup(xmltvp::format_xmltv_ns(episode).c_str());
    xmlNodePtr ptr = xmlNewTextChild(node->parent, NULL,  xmltvp::episode_num, episode_ns);
    xmlSetProp(ptr, xmltvp::episodesystem, xmltvp::xmltv_ns);

}



void remove(xmlNode * a_node)
{
    if (a_node->parent->type == XML_ELEMENT_NODE) {
            //xmlUnlinkNode(a_node->parent);
    }
}

static void parse_tv(xmlNode * a_node, nodewithname f)
{
    xmlNode *cur_node = NULL;
    xmlNode *prev_node = nullptr;
    bool remove_prev=false;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {

        if(prev_node!=nullptr){
            if (prev_node->type == XML_ELEMENT_NODE && remove_prev) {
                xmlUnlinkNode(prev_node);
            }
        }
        prev_node=cur_node;

        if(cur_node->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(cur_node->name, xmltvp::programme)){
                remove_prev = parse_programme(cur_node->children, f);
            }
        }
    }
}

static void parse_root(xmlNode * a_node, nodewithname f)
{
    if (a_node->type == XML_ELEMENT_NODE) {
        if (!xmlStrcmp(a_node->name, xmltvp::tv)){
            parse_tv(a_node->children, f);
        }
    }
}

void addCategory (xmlNode *node, const char* category, const xmlChar *lang) {
    const xmlChar *nonprogram = xmlCharStrdup(category);
    xmlNodePtr ptr = xmlNewTextChild(node->parent, NULL,  xmltvp::category, nonprogram);
    xmlAttrPtr newattr = xmlNewProp (ptr, xmltvp::lang, lang);
}

std::string getCategories (xmlNode *a_node) {

    xmlNode *cur_node = nullptr;
    std::string categories="";

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(cur_node->name, xmltvp::category)) {
                    std::string category = std::string((char *)xmlNodeGetContent(cur_node));
                    for(int i=0;i<9;i++){
                        eraseSubStr(category, "/");
                        eraseSubStr(category, " ");
                    }
                    categories+=category + " ";
            }
        }
    }

    return categories;
}

void addURL (xmlNode *node, const char* url) {
    const xmlChar *nonprogram = xmlCharStrdup(url);
    xmlNodePtr ptr = xmlNewTextChild(node->parent, nullptr,  xmltvp::url, nonprogram);
}

void addDate (xmlNode *node, const char* date) {
    const xmlChar *nonprogram = xmlCharStrdup(date);
    xmlNodePtr ptr = xmlNewTextChild(node->parent, nullptr,  xmltvp::date, nonprogram);
}
void clearURL (xmlNode *node) {
    remove_elements_by_name(node, xmltvp::url);
}

void clearDate (xmlNode *node) {
    remove_elements_by_name(node, xmltvp::url);
}

void clearTitle (xmlNode *node) {
    remove_elements_by_name(node, xmltvp::title);
    remove_elements_by_name(node, xmltvp::title);
}

void clearCategory (xmlNode *node) {
    remove_elements_by_name(node, xmltvp::category);
    remove_elements_by_name(node, xmltvp::category);
}

void clearSubTitle (xmlNode *node) {
    remove_elements_by_name(node, xmltvp::sub_title);
    remove_elements_by_name(node, xmltvp::sub_title);
}

void clearDesc (xmlNode *node) {
    remove_elements_by_name(node, xmltvp::desc);
    remove_elements_by_name(node, xmltvp::desc);
}

std::string getChannel(xmlNode *node){
    return ((char *)xmlGetProp(node->parent, xmltvp::channel));
}

tm getTimeProp(xmlNode *node, const xmlChar* name){
    return convert_to_sec_since1970((char *)xmlGetProp(node->parent, name));
}

void setTimeProp(xmlNode *node, const xmlChar* name, tm time)
{
    const char *timestr = format_date_xmltv(time).c_str();
    const xmlChar *aika = xmlCharStrdup(timestr);
    xmlSetProp(node->parent, name, aika);
}

tm getStart(xmlNode *node){
    return getTimeProp(node, xmltvp::start);
}

std::string getStartStr(xmlNode *node){
    return (char *)xmlGetProp(node->parent, xmltvp::start);
}

void setStart(xmlNode *node, tm time)
{
    setTimeProp(node, xmltvp::start, time);
}

tm getStop(xmlNode *node){
   return getTimeProp(node, xmltvp::stop);
}

void setStop(xmlNode *node, tm time)
{
    setTimeProp(node, xmltvp::stop, time);
}

void parseXMLTV(const char* xmlfile_in, const char* xmlfile_out, nodewithname manipulatefunction)
{
    xmlDoc *doc = nullptr;
    xmlNode *root_element = nullptr;

    /*parse the file and get the DOM */
    doc = xmlReadFile(xmlfile_in, nullptr, 0);

    if (doc == nullptr) {
        fprintf(stderr, "error: could not parse file %s\n", xmlfile_in);
    }

    /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

    parse_root(root_element, manipulatefunction);

    if(xmlfile_out){
        xmlSaveFormatFile (xmlfile_out, doc, 1);
    }

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();
}

void setChannel(xmlNode *node, std::string channelstring)
{
    const xmlChar *channelxml = xmlCharStrdup(channelstring.c_str());
    xmlSetProp(node->parent, xmltvp::channel, channelxml);
}

std::string getDate(xmlNode *a_node)
{
    std::string date=getProp(a_node, xmltvp::date);
    return date;
}

std::string getLanguage(xmlNode *a_node)
{
    std::string language=getProp(a_node, xmltvp::language);
    return language;
}

std::string getCountry(xmlNode *a_node)
{
    return getProp(a_node, xmltvp::country);
}

}
