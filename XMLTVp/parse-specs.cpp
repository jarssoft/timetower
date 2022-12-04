#include "parse-specs.h"
#include "../Rutiinit/stringrutins.h"
#include <regex>

enum {N, VALMISTUSMAA, VALMISTUSVUOSI, ALKUPERAINENNIMI, KESTO, LAJITYYPPI, OHJAAJA, NAYTTELIJAT};

#define MAAT            std::string("(Suomi|Ruotsi|Norja|Tanska|Venäjä|USA|Yhdysvallat|Argentiina|Japani|Englanti|Iso-Britannia|Ranska|Italia|Saksa|Kanada|Brasilia)")
#define MAAYHDISTELMATK std::string("(")+std::string(MAAT)+"(/"+std::string(MAAT)+")*)"
#define MAAYHDISTELMATL std::string("(")+std::string(MAAT)+"(, "+std::string(MAAT)+")*)"
#define VUODET          std::string("([12][90][0-9]{2})")
#define LAJITYYPIT      std::string("([A-ZÅÄÖa-zäåö\\-/]+)")
#define ALKUPERAISET    std::string("([^,\\)]+)")
#define KESTOT          std::string("([[:digit:]]+)")
#define NIMET           std::string("([A-ZÅÄÖ][a-zäåö]+ (af )?[A-ZÅÄÖ][a-zäåö]+)")
#define NIMIYHDISTELMAT std::string("(")+std::string(NIMET)+"(, "+std::string(NIMET)+")*)"

namespace xmltvp {

struct tuotantopattern{
  std::regex regex;
  int groups[10];
};

tuotantopattern tt1 = {std::regex(MAAYHDISTELMATK+" "+VUODET), {VALMISTUSMAA, N, N, N, VALMISTUSVUOSI}};
tuotantopattern tt2 = {std::regex("\\("+MAAT+" "+VUODET+", "+KESTOT+"'\\)"), {VALMISTUSMAA, VALMISTUSVUOSI, KESTO}};
tuotantopattern tt3 = {std::regex("\\("+VUODET+"\\)"), {VALMISTUSVUOSI}};
tuotantopattern tt4 = {std::regex("\\("+ALKUPERAISET+", "+MAAT+" "+VUODET+"\\)"), {ALKUPERAINENNIMI, VALMISTUSMAA, VALMISTUSVUOSI}};
tuotantopattern tt5 = {std::regex("\\("+ALKUPERAISET+", "+LAJITYYPIT+", "+MAAYHDISTELMATL+", "+VUODET+"\\)"), {ALKUPERAINENNIMI, LAJITYYPPI, VALMISTUSMAA, N, N, N, VALMISTUSVUOSI}};
tuotantopattern tt6 = {std::regex("\\("+ALKUPERAISET+" "+VUODET+"\\)"), {ALKUPERAINENNIMI, VALMISTUSVUOSI}};
tuotantopattern tt7 = {std::regex("\\("+ALKUPERAISET+"/"+MAAT+" "+VUODET+"\\)"), {ALKUPERAINENNIMI, VALMISTUSMAA, VALMISTUSVUOSI}};
tuotantopattern tt8 = {std::regex("O: "+NIMET+"\\."), {OHJAAJA}};
tuotantopattern tt9 = {std::regex("N: "+NIMIYHDISTELMAT+"\\."), {NAYTTELIJAT}};
std::vector<tuotantopattern> tcapts{tt1, tt2, tt3, tt4, tt5, tt6, tt7, tt8, tt9};

//N: Ansa Ikonen, Santeri Karilo, Ekke Hämäläinen, Annie Sundman, Aili Somersalmi.

xmltvp::tuotanto_ns parse_tuotanto(const std::string phrase){

    xmltvp::tuotanto_ns op;
    std::smatch matches;

    for(std::vector<tuotantopattern>::iterator it = tcapts.begin(); it != tcapts.end(); ++it) {
        if(std::regex_search(phrase, matches, (*it).regex)) {

            for (size_t i = 1; i < matches.size(); ++i) {

                op.kaikki = matches[0].str();

                int jaksokey = (*it).groups[i-1];

                if(jaksokey==VALMISTUSMAA){
                    op.country=matches[i].str();
                }

                if(jaksokey==VALMISTUSVUOSI){
                    op.year=matches[i].str();
                }

                if(jaksokey==ALKUPERAINENNIMI){
                    op.alkuperainennimi=matches[i].str();
                }

                if(jaksokey==KESTO){
                    op.kesto=string_to_int(matches[i].str());
                }

                if(jaksokey==OHJAAJA){
                    op.ohjaaja=matches[i].str();
                }

                if(jaksokey==NAYTTELIJAT){
                    op.nayttelijat=matches[i].str();
                }
            }
        }
    }
    return op;
}
}
