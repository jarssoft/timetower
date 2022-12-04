#ifndef MAN_PROGRAMME_H
#define MAN_PROGRAMME_H

#include <string>

class ManProgramme
{
public:
    ManProgramme();

private:
    std::string old_title;
    std::string new_title;
    std::string new_categories;
    std::string new_url;
};

#endif // MAN_PROGRAMME_H
