#include "descparser.h"
#include "stringrutins.h"
#include <iostream>

#define JAKSO       0
#define JAKSOJA     1
#define KAUSI       2
#define NONE       -1

struct jaksopattern{
  std::regex regex;
  int groups[3];
};

jaksopattern jp1 = {std::regex("Kausi ([[:digit:]]+), osa ([[:digit:]]+)\\."), {KAUSI, JAKSO}};
jaksopattern jp2 = {std::regex("Kausi ([[:digit:]]+), ([[:digit:]]+)/([[:digit:]]+)\\."), {KAUSI, JAKSO, JAKSOJA}};
jaksopattern jp3 = {std::regex("Kausi ([[:digit:]]+)\\."), {KAUSI}};
jaksopattern jp4 = {std::regex("Jakso ([[:digit:]]+)/([[:digit:]]+)."), {JAKSO, JAKSOJA}};
jaksopattern jp5 = {std::regex("Jakso ([[:digit:]]+)\\."), {JAKSO}};
jaksopattern jp6 = {std::regex("Osa ([[:digit:]]+)."), {JAKSO}};
jaksopattern jp7 = {std::regex("([[:digit:]]+)/([[:digit:]]+)."), {JAKSO, JAKSOJA}};
jaksopattern jp8 = {std::regex("([[:digit:]]+). jakso"), {JAKSO}};
jaksopattern jp9 = {std::regex("([[:digit:]]+). kausi"), {KAUSI}};

std::regex vuosip = std::regex(" ([0-9]{4})[\\.\\)]");
std::regex ohjaajap = std::regex("O(hjaaja|hjaus)?: ([^\\.,]+)[\\.,]");


ohjelmaprops parse_description(std::string &phrase){

    std::vector<jaksopattern> jv;
    jv.push_back(jp1);
    jv.push_back(jp2);
    jv.push_back(jp3);
    jv.push_back(jp4);
    jv.push_back(jp5);
    jv.push_back(jp6);
    jv.push_back(jp7);
    jv.push_back(jp8);
    jv.push_back(jp9);

    ohjelmaprops op;

    std::smatch matches;
    bool found=false;

    for(std::vector<jaksopattern>::iterator it = jv.begin(); it != jv.end(); ++it) {
        if(std::regex_search(phrase, matches, (*it).regex)) {
            //std::cout << "Match found\n";
            //std::cout << phrase << "\n";

            std::string nimet[]={"jakso", "jaksoja", "kausi"};

            for (size_t i = 1; i < matches.size(); ++i) {
                //std::cout << i << ": '" << matches[i].str() << "'\n";
                //std::cout << nimet[(*it).groups[i-1]] << ": '" << matches[i].str() << "'\n";

                int jaksokey = (*it).groups[i-1];
                int jaksovalue = string_to_int(matches[i].str());

                if(jaksokey==JAKSO)
                    op.jakso=jaksovalue;

                if(jaksokey==JAKSOJA)
                    op.jaksoja=jaksovalue;

                if(jaksokey==KAUSI)
                    op.kausi=jaksovalue;

            }

            //std::cout << phrase << "\n";
            phrase = std::regex_replace(phrase, (*it).regex, "");
            //std::cout << phrase << "\n\n";

            found=true;
            //return;

        } else {
            //std::cout << "Match not found\n";
        }
    }

    if(std::regex_search(phrase, matches, vuosip)) {
        //std::cout << phrase << "\n";
        //std::cout << matches[1].str() << "\n";
        op.vuosi = string_to_int(matches[1].str());
    }

    if(std::regex_search(phrase, matches, ohjaajap)) {
        //std::cout << phrase << "\n";
        //std::cout << matches[2].str() << "\n";
        //std::cout << "\n";
        op.ohjaaja=matches[2].str();
    }else{
        /*
        if (phrase.find("ohjaus") != std::string::npos
                || phrase.find("O:") != std::string::npos
                || phrase.find("Ohjaus") != std::string::npos
                || phrase.find("hjaaja:") != std::string::npos)
            std::cout << phrase << "\n";
            */
    }

    op.tarkennin = first_sentence(phrase);
    if(op.tarkennin.length()>150)
        op.tarkennin="";

    //else
    //    std::cout << op.tarkennin << std::endl;

    //if(found)
        return op;

    /*
    if (phrase.find("kausi") != std::string::npos
            || phrase.find("jakso") != std::string::npos
            || phrase.find("osa") != std::string::npos
            || phrase.find("/") != std::string::npos)
        std::cout << phrase << "\n";
    //*/


}
