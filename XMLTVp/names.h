#ifndef NAMES_H
#define NAMES_H

#include <libxml/parser.h>
#include <vector>
#include <QString>

// http://github.com/AlekSi/xmltv/blob/master/xmltv.dtd
namespace xmltvp {

    extern const QString qtv;
    extern const xmlChar *tv;
    extern const QString qprogramme;
    extern const xmlChar *programme;

    extern const xmlChar *title;
    extern const xmlChar *sub_title;
    extern const xmlChar *desc;
    extern const xmlChar *credits;
    extern const xmlChar *date;

    extern const xmlChar *category;
    extern const xmlChar *keyword;
    extern const xmlChar *language;
    extern const xmlChar *orig_language;

    extern const xmlChar *lenght;
    extern const xmlChar *icon;
    extern const xmlChar *url;
    extern const xmlChar *country;
    extern const xmlChar *episode_num;

    extern const xmlChar *video;
    extern const xmlChar *audio;
    extern const xmlChar *previously_shown;
    extern const xmlChar *premiere;

    extern const xmlChar *last_change;
    extern const xmlChar *isnew;
    extern const xmlChar *subtitles;
    extern const xmlChar *rating;
    extern const xmlChar *star_rating;
    extern const xmlChar *review;
    extern const xmlChar *advertisement;


    extern const xmlChar *lang;
    extern const xmlChar *en;
    extern const xmlChar *fi;
    extern const xmlChar *sv;

    extern const xmlChar *start;
    extern const xmlChar *stop;
    extern const xmlChar *channel;

    extern const xmlChar *episodesystem;
    extern const xmlChar *xmltv_ns;

    extern const xmlChar *director;
}

#endif // NAMES_H
