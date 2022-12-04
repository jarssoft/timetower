#include "programmap.h"
#include "parseprogramlist.h"
#include "programnames.h"


namespace programp {

ProgramMap::ProgramMap(const char *file)
{
    //initMap(file);

}

void ProgramMap::initMap(const char* file){

    if(initPrograms(file)){

        while(nextProgram()){

            struct program prog;

            prog.title = programp::get(programp::title);
            prog.sendingtimes = programp::get(programp::sendingtimes);
            prog.count = std::stoi(programp::get(programp::count));

            mapOfProgram[prog.title] = prog;
        }

        closePrograms();
    }
}

}
