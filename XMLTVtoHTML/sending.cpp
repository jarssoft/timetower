#include "sending.h"
#include "sending.h"


bool sending_t::operator ==(const sending_t& c){
    return (this->start.tm_yday==c.start.tm_yday && this->start.tm_hour==c.start.tm_hour && this->start.tm_min==c.start.tm_min);
}

unsigned int sending_t::kesto() const
{
    tm start = this->start;
    tm end = this->end;
    return (int)(difftime(mktime(&end), mktime(&start))/60);
}

bool program_t::operator ==(const program_t &c)
{
    //auto other = dynamic_cast<const Circle*>(&c);
    //return other != 0 and /* check equality */;

    if(getMakeSVG()){
        //return categories==c.categories;
    }

    //return title==c.title && year==c.year && season==c.season && director==c.director;
    return title==c.title;
}

