#ifndef D_H
#define D_H

#include <string>
#include <QVector>
#include <QDateTime>

using namespace std;

namespace xmltvp {

typedef QString Kanava;

class xmltvprogramme
{
public:

    xmltvprogramme(const QString channel, const QString title, const QString start, const QString stop = QString(""));
    xmltvprogramme(const Kanava channel, const Kanava title, const QDateTime start,  const QDateTime stop = QDateTime());

    QString start;
    QString stop;
    QString channel;

    QString title;
    QString subtitle;
    QString desc;
    QString credits;
    QString date;
    //QString category;
    QVector<QString> categories;
    QString keyword;
    QString language;
    QString url;
    QString episodenum;

    bool isValid() const;

    QString toString() const;

    //Palauttaa ohjelman keston sekunteina.
    //Jos loppumisaikaa ei tiedetä, palautetaan -1.
    qint64 kesto() const;
};

bool validChannel(const QString channel);

bool validDate(const QString date);

bool validTitle(const QString title);

QDateTime parsedatetime(const QString aika);

QTime parsetime(const QString aika);

QString formatdatetime(const QDateTime);

//ottaa xmltv:n aikamerkinnästä (yyyyMMddhhmmss) aikaa esittävän osan (hhmmss)
QString timestring(const QString datetime);
}
#endif // D_H
