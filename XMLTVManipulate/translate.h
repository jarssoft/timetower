#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <vector>
#include <string>
#include <cstddef>

class Translate
{

public:
    Translate();
    void loadFromCVS(char * filename);
    void loadFromRuutuCVS(char * filename);
    void loadFromDplayCVS(char * filename);
    void loadFromPetscanCVS(char * filename, char * lang);
    void loadFromMediawiki(char * filename);

    void newSearch();
    bool getProgram(std::string title, std::string& url, std::string& category);

private:
    std::vector<std::string> titles;
    std::vector<std::string> urls;
    std::vector<std::string> categories;

    ptrdiff_t pos;

};

#endif // TRANSLATE_H
