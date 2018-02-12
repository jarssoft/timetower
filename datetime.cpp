#include "datetime.h"
#include <stdio.h>

int timezoneadjust=0;

void adjust_timezone(int zone){
    if(timezoneadjust>24)
        timezoneadjust=24;
    if(timezoneadjust<-24)
        timezoneadjust=-24;
    timezoneadjust=zone;
}

inline static int make_int(const char *p, int size)
{
    const char *endp;
    int intval = 0;

    endp = p + size;
    while (p < endp)
    {
        intval = intval * 10 + *p - '0';
        p++;
    }
    return intval;
}

tm convert_to_sec_since1970(char *szYYYYMMDDHHMMSS)
{
    struct tm Tm;

    memset(&Tm, 0, sizeof(Tm));
    Tm.tm_year = make_int(szYYYYMMDDHHMMSS +  0, 4) - 1900;
    Tm.tm_mon  = make_int(szYYYYMMDDHHMMSS +  4, 2) - 1;
    Tm.tm_mday = make_int(szYYYYMMDDHHMMSS +  6, 2);
    int tunti  = make_int(szYYYYMMDDHHMMSS +  8, 2)+timezoneadjust;
    if(tunti<0)
        tunti=23;
    Tm.tm_hour = tunti;
    Tm.tm_min  = make_int(szYYYYMMDDHHMMSS + 10, 2);
    Tm.tm_sec  = make_int(szYYYYMMDDHHMMSS + 12, 2);
    //mktime(&Tm);
    timegm(&Tm);

    return Tm;
}

int week_number( const std::tm& tm ) {

    constexpr int DAYS_PER_WEEK = 7 ;

    const int wday = tm.tm_wday ;
    const int delta = wday ? wday-1 : DAYS_PER_WEEK-1 ;
    return ( tm.tm_yday + DAYS_PER_WEEK - delta ) / DAYS_PER_WEEK ;
}

//for testing...
int test()
{
    //struct tm * timeinfo;

    struct tm Tm = convert_to_sec_since1970("20171212173020");
    printf("%s\n", asctime(&Tm));
    //timeinfo = localtime ( &rawtime );
    /*
    struct tm timeinfo = {.tm_mday=1};
    mktime(&timeinfo);
    printf ( "The current date/time is: %s", asctime (&timeinfo) );
    */
    struct tm start;
    start.tm_year=2017-1900;
    start.tm_mon=12-1;
    start.tm_mday=12;
    start.tm_hour=17;
    start.tm_min=10;
    start.tm_sec=10;
    mktime(&start);

    printf("%s\n", asctime(&start));
    printf("%d\n", start.tm_wday);

}


std::string format_date(const tm timeinfo)
{
  char buffer[80];
  strftime(buffer, sizeof(buffer),"%Y-%m-%d", &timeinfo);
  std::string str(buffer);
  return str;
}
