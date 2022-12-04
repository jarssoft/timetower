#ifndef DATETIME_H
#define DATETIME_H

#include <ctime>
#include <time.h>       /* time_t, struct tm, time, localtime, asctime */
#include <cstring>
#include <string>

//inline static int make_int(const char *p, int size);

void adjust_timezone(int zone);

tm convert_to_sec_since1970(const char *szYYYYMMDDHHMMSS);

// TVTablen viikko on yksikkö joka laskee viikot alkaen vuoden 2018
// ensimmäiseltä viikolta. Ensimmäinen viikko alkoi 1.1.2018, seuraava
// 8.1.2018, ja niin edespäin hamaan tulevaisuuteen saakka.

// Muunnettessa vuosikohteisiksi viikkonumeroiksi on luvusta vain
// vähennettävä vakio.

int tm_to_weeknumber(const std::tm& tm);

//weeknumber to tm
//vuosi on normaali vuosi välillä 2018-2019
//viikko on suomalaisen standardin mukainen viikko
//viikonpäivä on 0-6, jossa 0 on maanantai ja 6 sunnuntai
tm weeknumber_to_tm(int year, int week, int weekday);

std::string format_date(tm timeinfo);

//get tv-style time, example: 21.55
std::string format_time(const tm timeinfo);

std::string format_date_xmltv(const tm timeinfo);

std::string format_date_ical(const tm timeinfo);

std::string format_time_of_year(const tm timeinfo);

int weekNumberInISO8601(const tm timeinfo);

bool isRedDay(const tm timeinfo);

bool kesaAika(tm &aika);

#endif // DATETIME_H
