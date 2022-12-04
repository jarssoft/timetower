#ifndef PARSE_H
#define PARSE_H

#include <vector>
#include <QRegularExpression>
#include <QTime>
#include <QDateTime>

namespace xmltvp {

struct subprogram{
    QString start;
    QString desc;
};

/** Sometimes a small programtable includes in a desctription.
    This function parses it and returns starting datetime & description as struct. */
std::vector<subprogram> parse_subtable(const QDateTime dt, const QString desc);

/** Next time after datetime. */
QDateTime nexttime(const QDateTime dt, const QTime time);

}

#endif // PARSE_H
