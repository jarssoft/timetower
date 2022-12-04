#include <iostream>
#include "../XMLTVp/access.h"
#include "../XMLWriter/xml_writer.hpp"
#include "weeklyrule.h"
#include <vector>
#include <algorithm>
#include <ohjelmanimi.h>

#include "gridp/programmap.h"

using namespace std;

int mainpp(int argc, char* argv[])
{
    programp::ProgramMap map(argv[1]);

    if(map.mapOfProgram.find("Rikospaikka") != map.mapOfProgram.end()){
        programp::program valittu = map.mapOfProgram["Rikospaikka"];
        std::cout << valittu.sendingtimes << std::endl;
    }

}
