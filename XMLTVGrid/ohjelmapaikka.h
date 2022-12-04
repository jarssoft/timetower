#ifndef OHJELMAPAIKKA_H
#define OHJELMAPAIKKA_H

#include <string>

class Ohjelmapaikka
{
public:
    Ohjelmapaikka(std::string kanava, int viikkoaika, std::string edeltava, std::string seuraava, bool edeltavaId);

    std::string getEdeltava() const;
    std::string getSeuraava() const;
    void setSeuraava(const std::string &value);

    int isHyppy() const;

    int getViikkoaika() const;

private:
    std::string edeltava;
    std::string seuraava;
    int viikkoaika;
    std::string kanava;
    int hyppy;
};




#endif // OHJELMAPAIKKA_H
