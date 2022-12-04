#ifndef WEEKTIMES_H
#define WEEKTIMES_H

#include <string>
#include <QDateTime>
#include "programme.h"

namespace xmltvp {

//palauttaa ohjelman alkamisajan sekunteina tiettyyn aikaan verrattuna
qint64 alkaa(const xmltvprogramme &p, QDateTime dt);

//palauttaa ohjelman loppumisajan sekunteina tiettyyn aikaan verrattuna
qint64 loppuu(const xmltvprogramme &p, QDateTime dt);

}

#endif // WEEKTIMES_H
