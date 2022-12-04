#include "compare.h"

#include <string>
#include "../Rutiinit/datetime.h"
#include <QString>


namespace xmltvp {

const int TOLERANCE = 1000*60*2.5;


bool perakkain(const xmltvprogramme &p1, const xmltvprogramme &p2, const bool exact)
{
    if(exact){
        return p1.stop.left(14)==p2.start.left(14);
    }else{
        return abs(parsedatetime(p1.stop).msecsTo(parsedatetime(p2.start)))<TOLERANCE;
    }
}

bool equals(const xmltvprogramme &p1, const xmltvprogramme &p2, const bool exact)
{
    if(exact){
        return p1.start.left(14) == p2.start.left(14)
                && p1.title==p2.title && p1.channel==p2.channel;
    }else{
        return abs(parsedatetime(p1.start).msecsTo(parsedatetime(p2.start)))<TOLERANCE
                && p1.title==p2.title && p1.channel==p2.channel;
    }
}

bool samaaika(const xmltvprogramme &p1, const xmltvprogramme &p2, const bool exact)
{
    return aika(timestring(p1.start), p2, exact);
}

bool aika(const QString &starttitle, const xmltvprogramme &p2, const bool exact)
{
    if(exact){
        return starttitle==timestring(p2.start);
                //&& timestring(p1.stop)==timestring(p2.stop);
    }else{
        return abs(parsetime(starttitle).msecsTo(parsedatetime(p2.start).time())) < TOLERANCE;
                //&& abs(parsedatetime(p1.stop).time().msecsTo(parsedatetime(p2.stop).time())) < TOLERANCE;
    }
}

bool aikatitle(const xmltvprogramme &p2, const QString &startime, const QString &title, const bool exact){
    return aika(startime, p2, exact) && title==p2.title;
}


bool samaviikkoaika(const xmltvprogramme &p1, const xmltvprogramme &p2, const bool exact)
{
    return viikkoaika(parsedatetime(p1.start), p2, exact);
}

bool viikkoaika(const QDateTime &dt, const xmltvprogramme &p2, const bool exact)
{
    const qint64 MSECINWEEK = 1000 * 60 * 60 * 24 * 7;
    const int thistolerance = exact ? 0 : TOLERANCE;
    const qint64 start=abs(dt.msecsTo(parsedatetime(p2.start))) % MSECINWEEK;
    //const qint64 stop=abs(parsedatetime(p1.stop).msecsTo(parsedatetime(p2.stop))) % MSECINWEEK;

    return (start <= thistolerance || (MSECINWEEK - start) <= thistolerance);
            //&& (stop <= thistolerance || (MSECINWEEK - stop) <= thistolerance);
}

}
