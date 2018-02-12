#include "svg_writer.h"

Svg_writer::Svg_writer(Writer &awriter): writer(awriter)
{
    writer.openElt("svg");
    writer.attr("version", "1.1");
    writer.attr("xmlns", "http://www.w3.org/2000/svg");
    writer.attr("xmlns:xlink", "http://www.w3.org/1999/xlink");
    writer.attr("width", "70");
    writer.attr("height", "96");
}

void Svg_writer::start_row(int tick)
{
    //no operation needed
}

void Svg_writer::end_row()
{
    //no operation needed
}

void Svg_writer::addCell(program_t program, std::vector<episode_t> jointEpisodes, sendingshape_t shape)
{
    if(shape.rowspan+1 > 2){
        writer.openElt("rect");
        writer.attr("class", program.categories);
        writer.attr("x", std::to_string(shape.d*10));
        writer.attr("y", std::to_string(shape.t/3));
        writer.attr("width", std::to_string(10*(shape.colspan+1)));
        writer.attr("height", std::to_string((shape.rowspan+1)/3));
        writer.closeElt();
    }

    //<rect class="Movies" x="5" y="5" width="10" height="10" />
}

void Svg_writer::close()
{
    writer.closeElt();
}
