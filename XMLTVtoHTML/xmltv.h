#ifndef XMLTV_H
#define XMLTV_H

#include "../XMLTVGrid/gridp/programmap.h"
#include <libxml/parser.h>

//asetukset
void setWeek(int weeknum);
void setWeekday(int weekdaynum);
void setChannel(char* akanava);
char* getChannel();

//aloittaa xmltv:n tuomisen
void importXML(const char* xmlfile);

//xmltv:n käsittely
bool importProgram(xmlNode *node);

//viennit
void exportHTML(std::ostream& out);
void exportSVG(std::ostream& out);

#endif // XMLTV_H
