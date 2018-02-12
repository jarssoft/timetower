#ifndef DATETIME_H
#define DATETIME_H

#include <ctime>
#include <time.h>       /* time_t, struct tm, time, localtime, asctime */
#include <cstring>
#include <string>

inline static int make_int(const char *p, int size);

void adjust_timezone(int zone);

tm convert_to_sec_since1970(char *szYYYYMMDDHHMMSS);

int week_number( const std::tm& tm );

std::string format_date(tm timeinfo);

#endif // DATETIME_H
