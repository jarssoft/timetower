#ifndef PARSEWIKIPEDIA_H
#define PARSEWIKIPEDIA_H

#include <libxml/parser.h>
#include <string>
#include "programnames.h"

namespace programp {

typedef void (*nodewithname)(std::string, xmlNode *);

std::string get(const xmlChar *element);

bool nextProgram();

bool initPrograms(const char* xmlfile_in);

void closePrograms();

}

#endif // PARSEWIKIPEDIA_H
