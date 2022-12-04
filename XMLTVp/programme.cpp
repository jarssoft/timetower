#include "programme.h"
#include <QRegularExpression>

namespace xmltvp {

const QString DATETIMEFORMAT = "yyyyMMddhhmmss";
const QString TIMEFORMAT = "hhmmss";
const QRegularExpression CHANNELFORMAT("^([a-z0-9]+\\.)+[a-z]+$");


QString timestring(const QString datetime){
    return datetime.left(14).right(6);
}

QDateTime parsedatetime(const QString xmltvdatetime){
    return QDateTime::fromString(xmltvdatetime.left(14), DATETIMEFORMAT);
}

QTime parsetime(const QString xmltvdatetime){
    return QTime::fromString(timestring(xmltvdatetime), TIMEFORMAT);
}

QString formatdatetime(const QDateTime dt)
{
    return dt.toString(DATETIMEFORMAT);
}

xmltvprogramme::xmltvprogramme(const QString channel, const QString title, const QString start, const QString stop) :
    start(start), stop(stop), channel(channel), title(title) {
    //assert(isValid());
}

xmltvprogramme::xmltvprogramme(const Kanava channel, const Kanava title, const QDateTime start, const QDateTime stop) :
    start(formatdatetime(start)), stop(formatdatetime(stop)), channel(channel), title(title) {
    assert(isValid());
}

bool validChannel(const QString channel)
{
    QRegularExpressionMatch match = CHANNELFORMAT.match(channel);
    return match.hasMatch();
}

bool validDate(const QString date)
{
    return parsedatetime(date).isValid();
}


bool validTitle(const QString title)
{
    return title.size()>0;// && !title.contains("(");
}

bool xmltvprogramme::isValid() const
{
    return validChannel(channel)
            && (stop=="" || (validDate(stop) && start<stop))
            && validTitle(title);
}

QString xmltvprogramme::toString() const
{
    return this->channel+":"+this->start+":"+this->title;
}

qint64 xmltvprogramme::kesto() const
{
    if(this->stop==""){
        return -1;
    }
    return (parsedatetime(this->start).msecsTo(parsedatetime(this->stop)))/1000;
}

}
