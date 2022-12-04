#include "ohjelmapaikka.h"

Ohjelmapaikka::Ohjelmapaikka(std::string kanava, int viikkoaika, std::string edeltava, std::string seuraava, bool hyppy)
{
    this->kanava=kanava;
    this->viikkoaika=viikkoaika;
    this->edeltava=edeltava;
    this->seuraava=seuraava;
    this->hyppy=hyppy;
}

std::string Ohjelmapaikka::getEdeltava() const
{
    return edeltava;
}

int Ohjelmapaikka::isHyppy() const
{
    return this->hyppy;
}

int Ohjelmapaikka::getViikkoaika() const
{
    return viikkoaika;
}

std::string Ohjelmapaikka::getSeuraava() const
{
    return seuraava;
}

void Ohjelmapaikka::setSeuraava(const std::string &value)
{
    seuraava = value;
}
