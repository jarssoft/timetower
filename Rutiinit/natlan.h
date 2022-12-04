#include <iostream>
#include <vector>
#include <string>

class NatLan {

 public:
    NatLan(std::string natlan);

    unsigned long size() const;

    std::string at(unsigned long i) const;

    NatLan commonPhrases(const NatLan &nl2) const;

    std::string toString() const;

    std::string remove(unsigned long i);

 private:
    //päivittää lauseindeksin
    void update();


    std::string natlan;
    std::vector<unsigned long> start;
    std::vector<unsigned long> end;
};

