#include "episodenum.h"
#include <vector>
#include "../Rutiinit/stringrutins.h"
#include <sstream>
#include <iostream>
#include <assert.h>

namespace xmltvp {

//convert string type of xmltv_ns to sets of integer (season, episode, total episodes)
//xmltv-format is used on xmltv, example: "0 . 9/10"
episode_ns parse_xmltv_ns(std::string ns){

    //std::cout << "parse_xmltv_ns: " << episodepal.season << std::endl;

    int season = -1;
    int episode = -1;
    int total_episodes = -1;

    //std::cout << ns << std::endl;

    std::vector<std::string> parts;
    split(ns.c_str(), '.', parts);


    //std::cout << parts.size() << std::endl;
    if(parts.at(0).size()>0){
        season = string_to_int(parts.at(0));
    }

    if (season!=-1) {
        season=season+1;
    }

    //episode_ns episodepalzzz = {0, 0, 0};
    //return episodepalzzz;

    if(parts.size()>1){
        std::vector<std::string> parts2;
        split(parts.at(1).c_str(), '/', parts2);
        if(parts2.size()>0){
            episode = string_to_int(parts2.at(0));

            if(parts2.size()>1){
                total_episodes = string_to_int(parts2.at(1));
            }

            if (episode!=-1) {
                episode=episode+1;
            }
        }else{
            //otetaan suoraan vektorista, jos ei kauttaviivaa
            episode = string_to_int(parts.at(1));
            if (episode!=-1) {
                episode=episode+1;
            }
        }
    }

    return {season, episode, total_episodes};
}

std::string format_xmltv_ns(const episode_ns episode)
{
    assert(episode.isValid());

    std::stringstream ss;
    ss << (episode.season != -1 ? std::to_string(episode.season-1) : "");
    ss << ".";
    ss << (episode.episode != -1 ? std::to_string(episode.episode-1) : "");
    ss << (episode.total_episodes != -1 ? "/"+std::to_string(episode.total_episodes) : "");
    ss << ".";
    std::string s = ss.str();
    //std::cout << s << '\n';
    return s;
}

}
