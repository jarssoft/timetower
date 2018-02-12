#ifndef HTML_TABLE_WRITER_H
#define HTML_TABLE_WRITER_H

#include <string>
#include <vector>
#include "xml_writer.hpp"
#include "sending.h"
#include "table_writer.h"

/* Class for export HTML-table */
class Html_table_writer : public Table_writer
{
public:

    Html_table_writer(Writer &writer, bool onlytable);

    void start_row(int tick);

    void end_row();

    void addCell(program_t program, std::vector<episode_t> jointEpisodes, sendingshape_t shape);

    void close();

private:

    //add HTML-header
    void generateHeader();

    std::string generateName();

    //create day header
    void generateDayHeader();

    //generate time header
    void generateTimeHeader(int t);


    Writer &writer;
};

#endif // HTML_TABLE_WRITER_H
