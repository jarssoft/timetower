#ifndef OHJELMANIMI_H
#define OHJELMANIMI_H

#include <string>

class Ohjelmanimi
{
public:
    // Default constructor will be needed if inserting into vector
    Ohjelmanimi() : nimi(), url() {}

    Ohjelmanimi( std::string const& f, std::string const& l) :
        nimi( f ), url( l ) {}

    // std::sort will require operator< to compile.
    // WARNING: function does not provide a total ordering
    //   (ie, is "Jane Doe" < "John Doe" necessarily given the following code?)
    bool operator<( Ohjelmanimi const& rhs ) const
       { return nimi < rhs.nimi; }

    bool operator==(const Ohjelmanimi& other) const
    {
        return this->getNimi() == other.getNimi();
    }

    std::string getNimi() const;

    std::string getUrl() const;

private:
    std::string nimi;
    std::string url;

};


#endif // OHJELMANIMI_H
