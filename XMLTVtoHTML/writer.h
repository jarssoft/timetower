#ifndef TABLE_WRITER_H
#define TABLE_WRITER_H

#include <vector>
#include "sending.h"
#include "sendinggroup.h"

//for exporting weekly table
class Table_writer
{
public:

    virtual void start_row() = 0;

    virtual void end_row() = 0;

    virtual void addTimeHeader(const int t, const int span) = 0;

    virtual void addCell(sendingGroup sg, sendingshape_t shape, sendingGroup sgdaily) = 0;

    virtual void close() = 0;
};

#endif // TABLE_WRITER_H
