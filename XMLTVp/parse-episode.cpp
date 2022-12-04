#include "parse-episode.h"
#include "../Rutiinit/stringrutins.h"
#include <regex>

enum {NONE, JAKSO, JAKSOJA, KAUSI};

namespace xmltvp {

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
std::vector<jaksopattern> jv = {jp1, jp2, jp3, jp4, jp5, jp6, jp7, jp8, jp9};

episode_ns parse_episode(std::string &phrase){

    xmltvp::episode_ns op = {-1, -1, -1};

    std::smatch matches;
    //bool found=false;

    for(std::vector<jaksopattern>::iterator it = jv.begin(); it != jv.end(); ++it) {
        if(std::regex_search(phrase, matches, (*it).regex)) {
            //std::cout << "Match found\n";
            //std::cout << phrase << "\n";

            //std::string nimet[]={"jakso", "jaksoja", "kausi"};

            for (size_t i = 1; i < matches.size(); ++i) {
                //std::cout << i << ": '" << matches[i].str() << "'\n";
                //std::cout << nimet[(*it).groups[i-1]] << ": '" << matches[i].str() << "'\n";

                int jaksokey = (*it).groups[i-1];
                int jaksovalue = string_to_int(matches[i].str());

                if(jaksokey==JAKSO){
                    op.episode=jaksovalue;
                }

                if(jaksokey==JAKSOJA){
                    op.total_episodes=jaksovalue;
                }

                if(jaksokey==KAUSI){
                    op.season=jaksovalue;
                }

            }

            //std::cout << phrase << "\n";
            phrase = std::regex_replace(phrase, (*it).regex, "");
            //std::cout << phrase << "\n\n";

            //found=true;
            //return;

        } else {
            //std::cout << "Match not found\n";
        }
    }

    return op;

}
}
