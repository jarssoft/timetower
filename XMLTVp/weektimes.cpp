#include "weektimes.h"


namespace xmltvp {

qint64 alkaa(const xmltvprogramme &p, QDateTime dt)
{
    return dt.msecsTo(parsedatetime(p.start))/1000;
}

qint64 loppuu(const xmltvprogramme &p, QDateTime dt)
{
    return dt.msecsTo(parsedatetime(p.stop))/1000;
}

}
