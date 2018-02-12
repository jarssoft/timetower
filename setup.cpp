#include "setup.h"

bool makesvg=false;

void setMakeSVG(bool amakesvg)
{
    makesvg=amakesvg;
}

bool getMakeSVG()
{
    return makesvg;
}
