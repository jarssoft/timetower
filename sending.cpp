#include "sending.h"


bool program_t::operator ==(const program_t &c)
{
    //auto other = dynamic_cast<const Circle*>(&c);
    //return other != 0 and /* check equality */;

    if(getMakeSVG()){
        return categories==c.categories;
    }

    /*
    if(title=="Korean olympialaiset")
        return false;

    if(title=="Talviolympialaiset 2018")
        return false;
    */

    return title==c.title && year==c.year && season==c.season && director==c.director;
}



