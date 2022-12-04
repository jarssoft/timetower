#ifndef PARSEWIKIPEDIA_H
#define PARSEWIKIPEDIA_H

#include <libxml/parser.h>
#include <string>
#include "wikinames.h"

namespace wikip {

typedef void (*nodewithname)(std::string, xmlNode *);

std::string currentTitle();

bool nextPage();

void initMediawiki(const char* xmlfile_in);

void closeMediawiki();

}

#endif // PARSEWIKIPEDIA_H
