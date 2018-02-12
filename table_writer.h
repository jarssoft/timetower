#ifndef TABLE_WRITER_H
#define TABLE_WRITER_H

#include <vector>
#include "sending.h"

//for exporting weekly table
class Table_writer
{
public:

    virtual void start_row(int tick) = 0;

    virtual void end_row() = 0;

    virtual void addCell(program_t program, std::vector<episode_t> jointEpisodes, sendingshape_t shape) = 0;

    virtual void close() = 0;
};

#endif // TABLE_WRITER_H
