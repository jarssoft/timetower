#include "writer_svg.h"

Svg_writer::Svg_writer(Writer &awriter): writer(awriter)
{
    writer.openElt("svg");

    writer.attr("version", "1.1");
    writer.attr("xmlns", "http://www.w3.org/2000/svg");
    writer.attr("xmlns:xlink", "http://www.w3.org/1999/xlink");
    //writer.attr("width", "70");
    //writer.attr("height", "96");



    writer.attr("viewBox", "0 0 70 96");
    writer.attr("class", "framed");

    drawRect(0, 0, 70, 96, "None");

}

void Svg_writer::start_row()
{
    //no operation needed
}

void Svg_writer::end_row()
{
    //no operation needed
}

void Svg_writer::addTimeHeader(const int t, const int span)
{

}

void Svg_writer::addCell(sendingGroup sg, sendingshape_t shape, sendingGroup sgdaily)
{
    program_t program = sg.getProgram();

    if(shape.rowspan+1 > 2
            //&& program.categories !="series Series "
            //&& program.categories !="series "
            && program.categories !="None"){
        drawRect(shape.d*10, shape.t/3,
                 10*(shape.colspan+1),
                 (shape.rowspan+1)/3, program.categories);
    }

}

void Svg_writer::close()
{
    /*
    writer.openElt("text");
    writer.attr("id", "esikatselu");
    writer.attr("x", std::to_string(0));
    writer.attr("y", std::to_string(15));
    writer.attr("font-size", "smaller");   
    writer.attr("fill", "black");
    writer.content("I love SVG!");
    //<text x="0" y="15" fill="red">I love SVG!</text>
    writer.closeElt("text");
    */
    writer.closeElt("svg");
}

void Svg_writer::drawRect(int x, int y, int width, int height, std::string css_class)
{

    writer.openElt("rect");
    writer.attr("class", css_class);
    writer.attr("x", std::to_string(x));
    writer.attr("y", std::to_string(y));
    writer.attr("width", std::to_string(width));
    writer.attr("height", std::to_string(height));
    writer.openElt("title");

    writer.content(css_class.c_str());

    writer.closeElt("title");
    writer.closeElt("rect");

}
