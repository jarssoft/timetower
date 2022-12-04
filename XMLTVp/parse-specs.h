#ifndef TUOTANTOTIEDOT_H
#define TUOTANTOTIEDOT_H

#include <string>

namespace xmltvp {

struct tuotanto_ns {
    std::string kaikki;
    std::string country;
    std::string year;
    std::string ohjaaja;
    std::string nayttelijat;
    int kesto;
    std::string alkuperainennimi;

    bool isEmpty(){
        return year=="";//==-1 && episode==-1 && total_episodes==-1;
    }

    tuotanto_ns(): kesto(-1){}

};

tuotanto_ns parse_tuotanto(const std::string ns);

}


#endif // TUOTANTOTIEDOT_H
