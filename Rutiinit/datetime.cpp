#include "datetime.h"
#include <stdio.h>
#include <assert.h>
#include <iostream>

int timezoneadjust=0;

using namespace std;

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

tm convert_to_sec_since1970(const char *szYYYYMMDDHHMMSS)
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

    assert(Tm.tm_year>100);

    return Tm;
}


int tm_to_weeknumber( const std::tm& tm ) {

    constexpr int DAYS_PER_WEEK = 7 ;

    const int wday = tm.tm_wday ;

    //muunnetaan muotoon, jossa ma=0 ja su=6
    const int delta = wday ? wday-1 : DAYS_PER_WEEK-1 ;

    //lasketaan päivät päivästä 1.1.2018, joka oli vuoden eka päivä ja maanantai
    int daysfrom2018 = ((tm.tm_year-118)*365) + tm.tm_yday;

    int week_number = ( daysfrom2018 + DAYS_PER_WEEK - delta ) / DAYS_PER_WEEK ;

    //assert(week_number>0);
    //assert(week_number<53)

    //2019
    if(week_number>52){
        week_number-=52;
    }

    //2020
    if(week_number>52){
        week_number-=52;
    }

    //2021
    if(week_number>53){
        week_number-=52;
    }

    //2022
    if(week_number>52){
        week_number-=52;
    }


    assert(week_number>0);
    assert(week_number<=53);

    return week_number;
}

tm weeknumber_to_tm(int year, int week_number, int weekday)
{
    assert(year>=2019);
    //assert(year<=2020);
    assert(weekday>=0);
    assert(weekday<=0);
    assert(week_number>=0);
    assert(week_number<=53);

    week_number-=1;
    if(year>2018){
        week_number+=52;
    }
    if(year>2019){
        week_number+=52;
    }
    if(year>2020){
        week_number+=53;
    }
    if(year>2021){
        week_number+=52;
    }
    //ennusteet:
    if(year>2022){
        week_number+=52;
    }
    if(year>2023){
        week_number+=52;
    }

    struct tm firstdayofyear;
    firstdayofyear.tm_year=2018-1900;
    firstdayofyear.tm_mon=0;
    firstdayofyear.tm_mday=1;
    firstdayofyear.tm_hour=0;
    firstdayofyear.tm_min=0;
    firstdayofyear.tm_sec=0;
    firstdayofyear.tm_isdst=0;
    mktime(&firstdayofyear);

    struct tm now = firstdayofyear;
    now.tm_mday += week_number*7 + weekday;
    mktime(&now);
    //timestart.tm_year=2019-1900;

    //time_t now = time(nullptr); // store current time
    //struct tm timeinfo = *localtime(&now);

    //time_t previous = mktime(&timestart);
    //time_t now = mktime(&timeinfo);

    //double diff = difftime(previous, now);

    //cout << "Previous time was " << ctime(&previous) << endl;

    //cout << "Weekday of firstdayofyear is " << (firstdayofyear.tm_wday + 6) % 7 << endl;

    //cout << "Vuodessa on " << diff/(7*24*60*60) << " viikkoa" << endl;

    return now;
}

//for testing...
//int main(int argc, char **argv)
int test()
{
    printf("%d\n", weeknumber_to_tm(2020, 31, 0).tm_mday);
    return 0;

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

std::string format_time_of_year(const tm timeinfo)
{
  char buffer[80];
  strftime(buffer, sizeof(buffer),"%d.%m", &timeinfo);
  std::string str(buffer);
  return str;
}

std::string format_time(const tm timeinfo)
{
  char buffer[80];
  strftime(buffer, sizeof(buffer),"%H.%M", &timeinfo);
  std::string str(buffer);
  return str;
}

std::string format_date_xmltv(const tm timeinfo)
{
  char buffer[80];
  strftime(buffer, sizeof(buffer),"%Y%m%d%H%M%S", &timeinfo);
  std::string str(buffer);
  return str;
}

std::string format_date_ical(const tm timeinfo)
{
  char buffer[80];
  strftime(buffer, sizeof(buffer),"%Y%m%dT%H%M%S", &timeinfo);
  std::string str(buffer);
  return str;
}

int weekNumberInISO8601(const tm timeinfo)
{
    int weekday = timeinfo.tm_wday-1;
    if(weekday<0){
        weekday=6;
    }
    return weekday;
}

/*int maindf(int argc, char **argv)
{
    time_t now = time(nullptr); // store current time
    struct tm today = *localtime(&now);
    cout << "Nyt on viikko " << tm_to_weeknumber(today) << endl;

    struct tm timeinfo = weeknumber_to_tm(2022, 1, 0);
    time_t previous = mktime(&timeinfo);
    cout << "Vuoden 22 viikon 1 maanantai oli " << ctime(&previous) << endl;

    int viikko = tm_to_weeknumber(timeinfo);
    cout << "Takaisin viikon numeroiksi " << viikko << endl;

    return 0;
}
*/

bool isRedDay(const tm timeinfo)
{
    return  (timeinfo.tm_wday==0 || (
                 //uudenvuodenpäivä
                 (timeinfo.tm_mday==1 && timeinfo.tm_mon==0)
                 //loppiainen
                 ||(timeinfo.tm_mday==6 && timeinfo.tm_mon==0)
                 //vappu
                 ||(timeinfo.tm_mday==1 && timeinfo.tm_mon==4)
                 //itsenäisyyspäivä
                 ||(timeinfo.tm_mday==6 && timeinfo.tm_mon==11)
                 //joulupäivä ja tapaninpäivä
                 ||((timeinfo.tm_mday>=25 && timeinfo.tm_mday<=26) && timeinfo.tm_mon==11)
                 )) ||  (

            timeinfo.tm_year==120 && (
                //pääsiäinen
                (timeinfo.tm_mday==10 && timeinfo.tm_mon==3)
                || (timeinfo.tm_mday==13 && timeinfo.tm_mon==3)
                //helatorstai
                || (timeinfo.tm_mday==21 && timeinfo.tm_mon==4)
                //juhannuspäivä
                || (timeinfo.tm_mday==20 && timeinfo.tm_mon==5)
                //pyhäinpäivä
                || (timeinfo.tm_mday==31 && timeinfo.tm_mon==9)
                )) ||  (

            timeinfo.tm_year==121 && (
                //pääsiäinen
                   (timeinfo.tm_mday==2 && timeinfo.tm_mon==3)
                || (timeinfo.tm_mday==4 && timeinfo.tm_mon==3)
                || (timeinfo.tm_mday==5 && timeinfo.tm_mon==3)
                //helatorstai
                || (timeinfo.tm_mday==13 && timeinfo.tm_mon==4)
                //juhannuspäivä
                || (timeinfo.tm_mday==26 && timeinfo.tm_mon==5)
                //pyhäinpäivä
                || (timeinfo.tm_mday==6 && timeinfo.tm_mon==10)
                )) ||  (

            timeinfo.tm_year==122 && (
                //pääsiäinen
                   (timeinfo.tm_mday==15 && timeinfo.tm_mon==3)
                || (timeinfo.tm_mday==17 && timeinfo.tm_mon==3)
                || (timeinfo.tm_mday==18 && timeinfo.tm_mon==3)
                //helatorstai
                || (timeinfo.tm_mday==26 && timeinfo.tm_mon==4)
                //juhannuspäivä
                || (timeinfo.tm_mday==25 && timeinfo.tm_mon==5)
                //pyhäinpäivä
                || (timeinfo.tm_mday==5 && timeinfo.tm_mon==10)
                )) ||  (

            timeinfo.tm_year==123 && (
                //pääsiäinen
                   (timeinfo.tm_mday==7 && timeinfo.tm_mon==3)
                || (timeinfo.tm_mday== 9 && timeinfo.tm_mon==3)
                || (timeinfo.tm_mday==10 && timeinfo.tm_mon==3)
                //helatorstai
                || (timeinfo.tm_mday==18 && timeinfo.tm_mon==4)
                //juhannuspäivä
                || (timeinfo.tm_mday==24 && timeinfo.tm_mon==5)
                //pyhäinpäivä
                || (timeinfo.tm_mday==4 && timeinfo.tm_mon==10)
                ));
}


bool kesaAika(tm &aika){

    tm firstday;
    firstday.tm_hour = 3; firstday.tm_min = 0; firstday.tm_sec = 0;
    firstday.tm_mon = 2;  firstday.tm_mday = 29;
    time_t first = mktime(&firstday);

    //tm lastday;
    //lastday.tm_hour = 3; lastday.tm_min = 0; lastday.tm_sec = 0;
    //lastday.tm_mon = 9;  lastday.tm_mday = 25;
    //time_t last = mktime(&lastday);

    //assert(difftime(mktime(&aika), first)<=0);

    return difftime(mktime(&aika), first)>0;
}
