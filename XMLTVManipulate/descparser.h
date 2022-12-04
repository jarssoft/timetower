#ifndef DESCPARSER_H
#define DESCPARSER_H

#include <regex>

//Kuvauksessa esiteltyjä ominaisuuksia
typedef struct {
    int jakso=-1;
    int jaksoja=-1;
    int kausi=-1;
    int vuosi=-1;
    std::string ohjaaja;
    std::string tarkennin;
} ohjelmaprops;

ohjelmaprops parse_description(std::string &phrase);

std::string getOlympicLinks(std::string subtitle);

std::string removeSign(std::string s);

#endif // DESCPARSER_H
