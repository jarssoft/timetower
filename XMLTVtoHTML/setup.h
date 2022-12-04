#ifndef SETUP_H
#define SETUP_H

//#define FIRSTHOUR               5
/*#define LASTHOUR                5+24 //30*/
extern int FIRSTHOUR;
extern int LASTHOUR;
extern int HOUROFDAYCHANGE;
extern int TICKSINDAY;


#define DAYSINWEEK              7
//#define HOURSINDAY              (LASTHOUR-FIRSTHOUR)
#define HOURSINDAY              24

#define TICKSINHOUR             12
#define TICKSLENGHT             (TICKSINHOUR/60.0)
//#define TICKSINDAY              (HOURSINDAY*TICKSINHOUR)
//#define HOUROFDAYCHANGE         FIRSTHOUR

#define CATCOLORS               true
#define JOIN_SIMILAR_PROGRAMS   true

#define MAKESVG                 false

void setMakeSVG(bool makesvg);

bool getMakeSVG();

void setMidnight(int);

#endif // SETUP_H
