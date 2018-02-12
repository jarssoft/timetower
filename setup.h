#ifndef SETUP_H
#define SETUP_H

#define FIRSTHOUR               0
#define LASTHOUR                24

#define DAYSINWEEK              7
#define HOURSINDAY              (LASTHOUR-FIRSTHOUR)

#define TICKSINHOUR             12
#define TICKSLENGHT             (TICKSINHOUR/60.0)
#define TICKSINDAY              (HOURSINDAY*TICKSINHOUR)
#define HOUROFDAYCHANGE         FIRSTHOUR

#define CATCOLORS               true
#define JOIN_SIMILAR_PROGRAMS   true

#define MAKESVG                 false

void setMakeSVG(bool makesvg);

bool getMakeSVG();

#endif // SETUP_H
