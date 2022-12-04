#include "names.h"

namespace xmltvp {

   const xmlChar *tv = xmlCharStrdup("tv");
   const QString qtv = "tv";
   const xmlChar *programme = xmlCharStrdup("programme");
   const QString qprogramme = "programme";

   const xmlChar *title = xmlCharStrdup("title");
   const xmlChar *sub_title = xmlCharStrdup("sub-title");
   const xmlChar *desc = xmlCharStrdup("desc");
   const xmlChar *credits = xmlCharStrdup("credits");
   const xmlChar *date = xmlCharStrdup("date");

   const xmlChar *category = xmlCharStrdup("category");
   const xmlChar *keyword = xmlCharStrdup("keyword");
   const xmlChar *language = xmlCharStrdup("language");
   const xmlChar *orig_language = xmlCharStrdup("orig-language");

   const xmlChar *lenght = xmlCharStrdup("lenght");
   const xmlChar *icon = xmlCharStrdup("icon");
   const xmlChar *url = xmlCharStrdup("url");
   const xmlChar *country = xmlCharStrdup("country");
   const xmlChar *episode_num = xmlCharStrdup("episode-num");

   const xmlChar *video = xmlCharStrdup("video");
   const xmlChar *audio = xmlCharStrdup("audio");
   const xmlChar *previously_shown = xmlCharStrdup("previously-shown");
   const xmlChar *premiere = xmlCharStrdup("premiere");

   const xmlChar *last_change = xmlCharStrdup("las-change");
   const xmlChar *isnew = xmlCharStrdup("new");
   const xmlChar *subtitles = xmlCharStrdup("subtitles");
   const xmlChar *rating = xmlCharStrdup("rating");
   const xmlChar *star_rating = xmlCharStrdup("star-rating");
   const xmlChar *review = xmlCharStrdup("review");
   const xmlChar *advertisement = xmlCharStrdup("advertisement");

   const xmlChar *lang = xmlCharStrdup("lang");
   const xmlChar *en = xmlCharStrdup("en");
   const xmlChar *fi = xmlCharStrdup("fi");
   const xmlChar *sv = xmlCharStrdup("sv");

   const xmlChar *start = xmlCharStrdup("start");
   const xmlChar *stop = xmlCharStrdup("stop");
   const xmlChar *channel = xmlCharStrdup("channel");

   //episode and its system
   const xmlChar *episodesystem = xmlCharStrdup("system");
   const xmlChar *xmltv_ns = xmlCharStrdup("xmltv_ns");

   const xmlChar *director = xmlCharStrdup("director");

   const std::vector<const xmlChar *> p_elements {
            title, sub_title, desc, credits, date,
            category, keyword, language, orig_language,
            lenght, icon, url, country, episode_num,
            video, audio, previously_shown, premiere,
            last_change, isnew, subtitles, rating,
            star_rating, review};

}
