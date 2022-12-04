#include "parse-subtable.h"
#include "programme.h"

namespace xmltvp {

const int VALITOLERANSSI=1000*60*60*3;
const QRegularExpression SUBTIMEFORMAT("([1-2]?[0-9]\\.[0-5][0-9]|[0-2][0-9]:[0-5][0-9]) ");

std::vector<subprogram> parse_subtable(const QDateTime start, const QString desc){
    std::vector<subprogram> subprogrammes;
    QRegularExpressionMatchIterator i = SUBTIMEFORMAT.globalMatch(desc);
    QDateTime lastdt=start;

    while (i.hasNext()) {
        const QRegularExpressionMatch match = i.next();
        if (match.hasMatch()) {
            subprogram programme;
            QDateTime substart;

            {
                QString timestring = match.captured();
                if(timestring.size()==5){
                    timestring="0"+timestring;
                }
                timestring.replace(".","");
                timestring.replace(":","");
                timestring.replace(" ","");
                timestring+="00";
                assert(timestring.length()==6);

                substart = nexttime(start, parsetime(timestring));
                if(lastdt.msecsTo(substart)<0 || lastdt.msecsTo(substart)>VALITOLERANSSI){
                    return subprogrammes;
                }

                lastdt=substart;

                programme.start=formatdatetime(substart);
            }

            {
                const int offset = match.capturedEnd();
                const int lenght = i.hasNext() ? i.peekNext().capturedStart()-1 : desc.size();
                programme.desc = desc.mid(offset, lenght-offset);
            }

            if (programme.desc.size()>1) {
                subprogrammes.push_back(programme);
            }
        }
    }

    return subprogrammes;
}

QDateTime nexttime(const QDateTime dt, const QTime time)
{
    QDateTime pal(dt.date(), time);
    if(pal<dt){
        pal = QDateTime(dt.date().addDays(1), time);
    }
    return pal;
}

}
