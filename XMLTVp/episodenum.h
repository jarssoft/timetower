#ifndef EPISODENUM_H
#define EPISODENUM_H

#include <string>

//format "2 . 9 . 0/1"

namespace xmltvp {

struct episode_ns {
    int season ;
    int episode;
    int total_episodes;

    bool isEmpty() const{
        return season==-1 && episode==-1 && total_episodes==-1;
    }

    bool isValid() const{
        return season>-2 && total_episodes>-2 && episode>-2;
    }

    //episode_ns(): season(-1), episode(-1), total_episodes(-1) {}

};

episode_ns parse_xmltv_ns(std::string ns);

std::string format_xmltv_ns(const episode_ns episode);

}

#endif // EPISODENUM_H
