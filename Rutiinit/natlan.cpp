#include "natlan.h"
#include <iostream>
#include <assert.h>

NatLan::NatLan(std::string natlan)
{
    //std::cerr << "natlan == " << natlan << std::endl;
    this->natlan = natlan;
    this->update();
}

void NatLan::update()
{
    std::string lastword;
    start.clear();
    end.clear();
    start.push_back(0);

    for(unsigned int i=1;i<this->natlan.length();i++){

        char c = this->natlan.at(i);

        if(lastword!="mm" && lastword!="Mr"){

            if(c=='.'  || c=='!' || c=='?' || c==')'){
                end.push_back(i);
                start.push_back(i+1);
                lastword="";
                //i+=3;
            }
            if(c=='('){
                if(start.back() < i){
                    end.push_back(i-1);
                    start.push_back(i);
                    //i+=3;
                }
                lastword="";
            }
        }

        if(c==' '){
            lastword="";
            if(start.back() == i){
               start.back() = i+1;
            }
        }else{
            lastword+=c;
        }
    }

    if(start.back() < this->natlan.length()){
        end.push_back(this->natlan.length());
    }else{
        start.pop_back();
    }
    assert((start.size() == end.size()));
}

unsigned long NatLan::size() const
{
    return this->start.size();
}

std::string NatLan::at(unsigned long i) const
{
    assert(i<size());
    return this->natlan.substr(start.at(i), end.at(i) - start.at(i)+1);
}

NatLan NatLan::commonPhrases(const NatLan &nl2) const
{
    std::string pal;

    for(unsigned long i1=0; i1<this->size(); i1++){

        unsigned long pit = this->end.at(i1) - this->start.at(i1);
        std::string str = this->at(i1);

        for(unsigned long i2=0; i2<nl2.size(); i2++){
            if(pit == nl2.end.at(i2) - nl2.start.at(i2)){
                if(str==nl2.at(i2)){
                    pal+=str+" ";
                    break;
                }
            }
            //std::cout << "str == " << str << std::endl;
        }

    }
    return NatLan(pal);
}

std::string NatLan::toString() const
{
    return this->natlan;
}

std::string NatLan::remove(unsigned long i)
{
    start.erase(start.begin() + i);
}


