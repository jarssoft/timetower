#ifndef HTML_TABLE_WRITER_H
#define HTML_TABLE_WRITER_H

#include <string>
#include <vector>
#include "xml_writer.hpp"
#include "sending.h"
#include "writer.h"
#include "../Rutiinit/datetime.h"
#include "../XMLTVGrid/gridp/programmap.h"

#define WEEKTABLE               0
#define WEEKDAYTABLE            1
#define DEBUGLABELS             0

#define CHARPERMINUTE_EVENING   2.2
#define CHARPERMINUTE           1.75
#define CHARPERMINUTE_NIGHT     1.0

/* Class for export HTML-table */
class Html_table_writer : public Table_writer
{
public:

    Html_table_writer(Writer &writer, bool onlytable, int tabletype, int week, const tm monday);

    void start_row();

    void end_row();

    //generate time header
    void addTimeHeader(const int t, const int span);

    void addCell(sendingGroup sg, sendingshape_t shape, sendingGroup sgdaily);

    void close();

private:

    //add HTML-header
    void generateHeader();

    std::string generateName();

    //create day header
    void generateWeekDayHeader();

    void generateWeekHeader(int startweek);



    static std::string formatweektime(tm start, bool space);

    std::string addItem(sendingGroup sg, sendingshape_t shape, bool inshape);

    std::string getEpisodeStr(sendingGroup sg, sendingshape_t shape);

    Writer &writer;
    std::string list;
    tm monday;

    programp::ProgramMap programmap;

    std::string lastTitles[7];
    int lastSpans[7];

};

#endif // HTML_TABLE_WRITER_H
