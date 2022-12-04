#ifndef MANIPULATE_H
#define MANIPULATE_H

#include <libxml/parser.h>
#include <string>
#include <vector>
#include "../XMLTVp/access.h"
#include "descparser.h"
#include "../Rutiinit/stringrutins.h"

void initManipulate(int timeoffset, char *channel, char *startdate, bool autodst);

void print(std::string title, xmlNode *node);

//palauttaa true, jos poistetaan
bool manipulate(xmlNode *node);

void initTrans();

#endif // MANIPULATE_H
