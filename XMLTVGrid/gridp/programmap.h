#ifndef PROGRAMMAP_H
#define PROGRAMMAP_H

#include <string>
#include <map>

namespace programp {

struct program{

    std::string title;
    std::string sendingtimes;
    int count;

};

class ProgramMap
{
public:

    void initMap(const char *file);

    ProgramMap(const char* file);

    std::map<std::string, program> mapOfProgram;

};

}

#endif // PROGRAMMAP_H
