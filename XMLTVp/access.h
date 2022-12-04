#ifndef PARSENAME_H
#define PARSENAME_H

#include <libxml/parser.h>
#include <string>
#include "episodenum.h"
#include "names.h"
#include <vector>

namespace xmltvp {

//palauttaa true, jos poistetaan
typedef bool (*nodewithname)(xmlNode *);

//parsii xmltv-tiedoston
void parseXMLTV(const char* xmlfile_in, const char* xmlfile_out, nodewithname manipulatefunction);

void addCategory(xmlNode *node, const char* category, const xmlChar *lang=xmltvp::en);

//palauttaa kategoriat yhtenä merkkijonona erotettuna väliviivalla
std::string getCategories (xmlNode *a_node);

void addSubtitle(xmlNode *node, const char* subtitle);

void addTitle(xmlNode *node, const char* title);

void addDesc(xmlNode *node, const char* desc);

void addURL(xmlNode *node, const char* url);

void addDate(xmlNode *node, const char* url);

void clearURL(xmlNode *node);

void clearDate(xmlNode *node);

void clearTitle(xmlNode *node);

void clearCategory (xmlNode *node);

void clearSubTitle (xmlNode *node);

void clearDesc (xmlNode *node);

tm getStart(xmlNode *node);

std::string getStartStr(xmlNode *node);

void setStart(xmlNode *node, tm time);

std::string getTitle(xmlNode * a_node, const xmlChar *language = nullptr);

std::string getDesc(xmlNode * a_node, const xmlChar* language = nullptr);

std::string getSubtitle(xmlNode * a_node);

std::vector<std::string> getURL(xmlNode * a_node);

std::string getDate(xmlNode * a_node);

std::string getLanguage(xmlNode * a_node);

std::string getCountry(xmlNode * a_node);

std::string getChannel(xmlNode *node);

void setChannel(xmlNode *node, std::string channelstring);

tm getStop(xmlNode *node);

void setStop(xmlNode *node, tm time);

xmltvp::episode_ns getEpisodenum(xmlNode * a_node);

void remove(xmlNode * a_node);

void setEpisodenum(xmlNode * a_node, const episode_ns episode);

}

#endif // PARSENAME_H
