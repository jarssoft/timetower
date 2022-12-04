#include "stringrutins.h"
#include <ctype.h>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <assert.h>

void split(const std::string& s, char c, std::vector<std::string>& v) {
   std::string::size_type i = 0;
   std::string::size_type j = s.find(c);

   //oma lisäys: Jos ei voida jakaa, kopioidaan kokonaisena
   if(j == std::string::npos){
       v.push_back(s);
   }

   while (j != std::string::npos) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j == std::string::npos)
         v.push_back(s.substr(i, s.length()));
   }

}

int string_to_int(std::string text) {
    char *endptr;
    const char* kausistr = text.c_str();
    int luku=strtol(text.c_str(), &endptr, 10);
    if (endptr != kausistr) {
        return luku;
    }else{
        return -1;
    }
}

int longest_word(std::string s) {
    int counter = 0;
    int max_word = -1;
    int len = s.length();
    std::string max = " ";

    for (int i = 0; i < len; i++)
    {
        if(s[i] != ' ') {
            //max.append(s[i]);
            counter++;
        }
        if(s[i] == ' ' || i == len - 1) {
            if(counter > max_word) {
                max_word = counter;
                //here,handle the end of the string
                if(i == len - 1)
                    max = s.substr(i + 1 - max_word, max_word);
                else
                    max = s.substr(i - max_word, max_word);
             }

            counter = 0;
        }
    }
    return max_word;
}

std::string before(std::string s, std::string delimiter) {
    std::string token = s.substr(0, s.find(delimiter));
    return token;
}

std::string after(std::string s, std::string delimiter) {
    std::string token = s.substr(s.find(delimiter)+delimiter.size());
    return token;
}

std::string first_sentence(std::string s) {

    bool numero=false;
    std::string lastword;
    unsigned long alkpituus=s.length();

    for(unsigned int i=1;i<s.length();i++){
        char c = s.at(i);
        //if((!numero && lastword!="mm" && lastword!="Mr") || c==')'){
        if(lastword!="mm" && lastword!="Mr"){
            //if(c=='.' || c=='!' || c=='?'){
            if(c=='.' || c==',' || c=='!' || c=='?' || c==')'){
                return s.substr(0, i+1);
            }
            if(c=='('){
                return s.substr(0, i);
            }
        }
        numero=isdigit(c);

        if(c==' '){
            lastword="";
        }else{
            lastword+=c;
        }
    }

    //ei saa palauttaa tyhjää
    assert(s.length()>0 || alkpituus==0);

    return s;
}

std::string shorten_text(std::string s, unsigned int l)
{
    std::string pal;
    std::string lause;

    //while((pal+lause).size()<=l && s.length()>0){
    while((pal.size()+lause.size())<=l && s.length()>0){
        pal+=lause;
        s = s.substr(lause.size());
        lause=first_sentence(s);
    }

    if(pal.length()>0 && pal.length()<s.length()){
        pal+="..";
    }

    return pal;

    assert(pal.size()<=l);
}

std::string bestPhrase(std::string s, unsigned int (*ptr)(std::string))
{
    std::string pal;
    std::string lause;
    std::string paraslause="";
    unsigned int l = s.size();
    unsigned int parhaatPisteet=0;

    while((pal.size()+lause.size()/2)<=l && s.length()>0){
        pal+=lause;
        s = s.substr(lause.size());
        lause=first_sentence(s);
        int pisteet = (*ptr)(lause);
        if(pisteet>parhaatPisteet){
            paraslause = lause;
        }
    }

    return paraslause;
}

std::string commonPhrases(std::string s1, const std::string s2)
{
    std::string pal;
    std::string lause;

    while(s1.length()>0){

        //tarkistaa, että merkkijono lyhenee joka kerralla
        //assert(s1.length()<viimepituus);

        s1 = s1.substr(lause.size());
        lause=first_sentence(s1);

        //std::cout << lause << std::endl;

        if(s2.find(lause) != std::string::npos){
            pal+=lause;
        }

    }

    return pal;
}

std::string last_sentence(std::string s) {

    for(int i=s.length()-2;i>-1;i--){
        char c = s.at(i);

        if(c=='.' || c=='!' || c=='?'){
            return s.substr(i+2);
        }

    }
    return s;

}

std::string finnishHyphenation(const std::string s, const std::string separator) {

    const char *vokaalit = "aeiouyäöAEIOUYÄÖ";
    const char *konsonantit = "bcdfghjklmnqprstvwxzBCDFGHJKLMNPQRSTVWXZ";

    std::string pal="";
    std::string tavu="";
    const int loppu=s.length()-10; //loppuosa, jota ei tavuteta

    std::vector<bool> tyypit;
    for(int i=0;i<s.length();i++){

        char c = s.at(i);
        tavu+=c;

        //päivitetään kirjaintyyppivektori
        if(strchr(vokaalit, c)){
            tyypit.push_back(false);
        }else{
            if (strchr(konsonantit, c)){
                tyypit.push_back(true);
            }else{
                tyypit.clear();
            }
        }

        //katkotaan tavut
        if(tyypit.size()>2 && i<loppu){
            if((!tyypit.rbegin()[0]) && (tyypit.rbegin()[1])){
                //ei yksikirjaimisia tavuja
                if(tyypit.size()-2>1){
                    pal+=tavu.substr(0, tavu.length()-2)+separator;
                    tavu = tavu.erase(0, tavu.length()-2);
                }
            }
        }
    }

    return pal+tavu;
}

unsigned int pointstest(std::string phrase)
{
    if(phrase.size()>80){
        return 0;
    }
    return (phrase.find(" 19") != -1 || phrase.find(" 20") != -1 ? 1000-phrase.size() : 0 );
}

int mainddffd(int argc, char **argv){
    //std::cout << finnishHyphenation("Lähihoitaja on toisen asteen oppilaitoksissa opetettava sosiaali- ja terveysalan tutkinto.", "&shy;") << std::endl;
    //std::cout << bestPhrase(" Starsky & Hutch. Saman nimiseen tv-sarjaan perustuva vauhdikas toimintakomedia poliisiparivaljakosta. Virkaintoinen Starsky (Ben Stiller) ja rento Hutch (Owen Wilson) pääsevät murhatutkimuksen kautta suuren huumerikollisen jäljille. USA 2004. Ohjaaja: Todd Phillips. Pääosissa: Ben Stiller, Owen Wilson, Snoop Dogg, Fred Williamson, Vince Vaughn, Juliette Lewis. ", &pointstest) << std::endl;

    /*
    std::cout << bestPhrase("(The Invention of Lying, komedia, USA, 2009) Ricky Gervaisin käsikirjoittama komedia kertoo maailmasta, jossa kukaan ei osaa valehdella. Gervaisin esittämä Mark kuitenkin oppii valehtelun jalon taidon ja päättää käyttää kykyä hyväkseen. 91 min. Ohjaus: Matthew Robinson, Ricky Gervais. Pääosissa: Jeffrey Tambor, Jennifer Garner, Jonah Hill, Ricky Gervais, Rob Lowe.", &pointstest) << std::endl;
    std::cout << bestPhrase(" Nuori mies tekee tuttavuutta kahden rikkaan, hupsun ja rakkaudettoman rouvan kanssa esittäytyen pianonvirittäjäksi. Hän kehittää tyttöystävänsä kanssa taidokkaan juonen tarkoituksena päästää kiinni naisten rahoihin. Kira Muratovan draamakomediassa pääosia esittää Alla Demidova, Georgiy Deliev, Renata Litvinova ja Nina Ruslanova. (Venäjä/Ukraina 2004) MV. HD ", &pointstest) << std::endl;
    std::cout << bestPhrase("(8 Mile, draama, USA, 2002) Karuista lähtökohdista ponnistava valkoinen duunari Jimmy (Eminem) yrittää lyödä läpi mustien hallitsemissa rap-piireissä 1990-luvun puolivälin Detroitissa. 102 min. Ohjaus: Curtis Hanson. Pääosissa: Brittany Murphy, De'Angelo Wilson, Eminem, Eugene Byrd, Evan Jones, Kim Basinger, Mekhi Phifer, Omar Benson Miller, Taryn Manning. ", &pointstest) << std::endl;
    std::cout << bestPhrase(" (Dad's Army, komedia, USA, 2016) Komedia seuraa osa-aikaisia sotilaita toisen maailmansodan aikana. Kun sota lähestyy loppua, joukkue saa vierailun kauniilta naispuoliselta journalistilta samaan aikaan, kun he alkavat epäillä, että heidän joukossaan saattaa olla saksalainen vakooja. Pian joukko huomaa, että heillä saattaakin olla mahdollisuus vaikuttaa sodan tapahtumiin. 100 min. Ohjaus: Oliver Parker. Pääosissa: Mark Tandy, Russell Balogh, Andrew Havill. ", &pointstest) << std::endl;
    std::cout << bestPhrase(" Tuntemattomaksi jääneessä nykyelokuvan merkkiteoksessa keski-ikäinen, keskiluokkainen brysseliläinen yksinhuoltaja Jeanne Dielman puuhastelee kotiaskareiden parissa, laittaa ruokaa vaiteliaalle pojalleen ja ottaa iltapäivisin vastaan herrasmiehiä. Päivästä toiseen, erääseen päivään asti. Pääosassa Delphine Seyrig. Ohjaus: Chantal Akerman. (Jeanne Dielman, 23, quai du commerce, 1080 Bruxelles, Belgia 1975) HD ", &pointstest) << std::endl;
    std::cout << bestPhrase("Suicide Squad. Salainen valtion organisaatio rekrytoi kokoelman vangittuja superkonnia. Heille tarjotaan mahdollisuus taistella hyvien puolella mahdottomassa tehtävässä vastapalveluksena lyhyemmistä rangaistuksista. Pääosissa: Margot Robbie, Will Smith, Jared Leto, Joel Kinnaman, Viola Davis, Jai Courtney, Adewale Akinnuoye-Agbaje, Karen Fukuhara, Jay Hernandez. Ohjaus: David Ayer, USA 2016. Uusinta. ", &pointstest) << std::endl;
    std::cout << bestPhrase("(Divergent 2014). Scifi-toimintaelokuva sijoittuu tulevaisuuden yhteiskuntaan, jossa ihmiset on jaettu ominaisuustestien mukaan viiteen ryhmään. Nuori tyttö Tris joutuu testiin ja saa kuulla olevansa outolintu ja siksi uhka järjestäytyneelle yhteiskunnalle ja sitä johtavalle diktaattorille. Pääosissa: Shailene Woodley, Theo James, Kate Winslet, Ashley Judd, Jai Courtney. Ohjaus: Neil Burger. (133') ", &pointstest) << std::endl;
    std::cout << bestPhrase("(Pojkarna, Ruotsi 2014) Maaginen tarina Kimistä, Bellasta ja Momosta, kolmesta 14-vuotiaasta tytöstä lapsuuden ja aikuisuuden välimaastossa. Päivisin tyttöjä kiusataan koulussa, mutta öisin he juovat salaperäisestä kukasta nektaria, joka muuttaa heidät pojiksi. Poikien maailman vapaus kiehtoo ystävyksiä, jotka ovat tottuneet tyttöjen kehoihin kohdistuviin monenlaisiin rajoituksiin. Kasvusta, muutoksesta, rakkaudesta ja ystävyydestä kertova elokuva saa pohtimaan, onko sukupuoli asia, johon synnytään, vai jotakin, jonka luomme itse. Elokuva perustuu Jessica Schiefauerin samannimiseen palkittuun romaaniin. Ohjaaja: Alexandra Keining. Päärooleissa: Josefin Nelden, Emrik Öhlander, Louise Nyvall, Wilma Holmén, Tuva Jagell, Vilgot Ostwald Vesterlund, Alexander Gustavsson. HD", &pointstest) << std::endl;
    std::cout << bestPhrase("(Den alvarsamma leken, Ruotsi 2016) Uusi tulkinta ruotsalaisen Hjalmar Söderbergin samannimisestä romaanista kertoo toimittaja Arvid Stjärnblomin ja Lydia Stillen mutkikkaasta suhteesta 1900-luvun alun Tukholmassa. Pari rakastuu intohimoisesti, mutta molemmat ajautuvat tahoillaan avioliittoon muiden kanssa. Mitä tapahtuu kun he myöhemmin kohtaavat? Draamaelokuva rakkaudesta ja sen seurauksista. Ohjaaja: Pernilla August. Rooleissa mm. Sverrir Gudnason, Karin Franz Körlof ja Michael Nyqvist. HD", &pointstest) << std::endl;
    std::cout << bestPhrase("Kuukauden westerninä naisohjaaja Kelly Reichardtin erilainen, realistinen lännennäkemys, tosipohjainen draama uudisasukkaista, jotka vuoden 1845 Oregonissa yrittävät löytää tietään erämaan halki ihmisten ilmoille. Pääosissa Michelle Williams ja Bruce Greenwood. (USA 2010) HD", &pointstest) << std::endl;
    std::cout << bestPhrase("No Country for Old Men, USA, 2007. O: Joel Cohen, Ethan Cohen. P: Tommy Lee Jones, Javier Bardem, Josh Brolin. Llewelyn Moss saa sattumalta käsiinsä miljoonien edestä huumerahoja ja joutuu pakenemaan perässään olevaa julmaa tappajaa. ", &pointstest) << std::endl;
    std::cout << bestPhrase("(Scary Movie 4/USA 2006). 2000-luvun menestyneimmän komediaelokuvien sarjan neljännen osan on ohjannut Mies ja alaston ase -suosikkien tekijämestari David Zucker. Elokuvaa on ollut kirjoittamassa myös Jim Abrahams, joka loi Mies ja alaston ase -menestykset yhdessä David Zuckerin kanssa. Pääosissa: Anna Faris, Regina Hall, Craig Bierko, Bill Pullman, Leslie Nielsen, Michael Madsen. Ohjaus: David Zucker. (79')", &pointstest) << std::endl;
    std::cout << bestPhrase("SUOMEN TV-ENSI-ILTA! (Personal Shopper, psykologinen trilleri, Ranska/Saksa/Tsekki/Belgia, 2017) 27-vuotias Pariisiin muuttanut Maureen yrittää selvittää, onko hänellä yliluonnollisia voimia - olihan hänen äkillisesti kuollut kaksoisveljensä meedio. Pian Maureen alkaa saada omituisia tekstiviestejä tuntemattomasta numerosta. 105 min. Ohjaus: Olivier Assayas. ", &pointstest) << std::endl;
    std::cout << bestPhrase("", &pointstest) << std::endl;
    */

    std::cout << commonPhrases(
                     "Flo joutuu katsomaan vierestä Phoeben elämää Steffyn ja Liamin lapsena, kun Wyatt vie hänet Spencereiden rantapiknikille. Ridge on ylpeä Thomasin omistautumisesta Hopen hyvinvoinnille, mutta samainen asia saa Brooken karvat pystyyn. Thomas yrittää tehdä vaikutusta Hopeen työpanoksellaan. Amerikkalainen saippuasarja Los Angelesin muotimaailmasta.",
                     "Thomas uhittelee Xanderille ja Zoelle, jotka tupsahtavat häiritsemään hänen herkkää hetkeään Hopen kanssa. Hän haluaa pitää huolen, että totuus Phoebesta ei milloinkaan saavuta Hopen korvia. Emma piileksii taas vaaterekkien lomassa kuuntelemassa muiden intiimejä keskusteluja. Flon ja Wyattin pikniksuunnitelmat menevät mönkään. Amerikkalainen saippuasarja Los Angelesin muotimaailmasta.") << std::endl;

    std::cout << commonPhrases(
                     "Mäkihypyn miesten Raw Air lentomäki joukkuekilpailu HS240. Vikersund, Norja. Selostus Mikko Hannula.",
                     "Mäkihypyn miesten Raw Air lentomäen kilpailu HS240. Vikersund, Norja. Selostus Mikko Hannula.") << std::endl;

    std::cout << commonPhrases(
                     "",
                     "") << std::endl;


}

/* Erase First Occurrence of given substring from main string. */
void eraseSubStr(std::string & mainStr, const std::string & toErase)
{
   // Search for the substring in string
   size_t pos = mainStr.find(toErase);

   if (pos != std::string::npos)
   {
       // If found then erase it from string
       mainStr.erase(pos, toErase.length());
   }
}

// function to remove characters and
// print new string
void removeSpecialCharacter(std::string & s)
{
    for (unsigned int i = 0; i < s.size(); i++) {

        // Finding the character whose
        // ASCII value fall under this
        // range
        if ((s[i] < 'A' || s[i] > 'Z') &&
            (s[i] < 'a' || s[i] > 'z') && s[i] != '\xE4')
        {
            //std::cout << (int)s[i] << std::endl;

            // erase function to erase
            // the character
            s.erase(i, 1);
            i--;
        }
    }
}
