#ifndef SVG_WRITER_H
#define SVG_WRITER_H

#include <string>
#include "xml_writer.hpp"
#include "sending.h"
#include "writer.h"

class Svg_writer : public Table_writer
{
public:
    Svg_writer(Writer &awriter);

    void start_row();

    void end_row();

    void addTimeHeader(const int t, const int span);

    void addCell(sendingGroup sg, sendingshape_t shape, sendingGroup sgdaily);

    void close();

private:
    Writer &writer;
    void drawRect(int x, int y, int width, int height, std::string css_class);
};

#endif // SVG_WRITER_H
