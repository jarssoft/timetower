#include "setup.h"

bool makesvg=false;

extern int FIRSTHOUR=5;
extern int HOUROFDAYCHANGE=5;
extern int LASTHOUR=29;
extern int TICKSINDAY=HOURSINDAY*TICKSINHOUR;

void setMakeSVG(bool amakesvg)
{
    makesvg=amakesvg;
}

bool getMakeSVG()
{
    return makesvg;
}

void setMidnight(int midnight)
{
    FIRSTHOUR=midnight;
    HOUROFDAYCHANGE=midnight;
    LASTHOUR=midnight+24;
    TICKSINDAY = HOURSINDAY*TICKSINHOUR;
}
