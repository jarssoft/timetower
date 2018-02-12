#ifndef SVG_WRITER_H
#define SVG_WRITER_H

#include <string>
#include "xml_writer.hpp"
#include "sending.h"
#include "table_writer.h"

class Svg_writer : public Table_writer
{
public:
    Svg_writer(Writer &awriter);

    void start_row(int tick);

    void end_row();

    void addCell(program_t program, std::vector<episode_t> jointEpisodes, sendingshape_t shape);

    void close();

private:
    Writer &writer;
};

#endif // SVG_WRITER_H
