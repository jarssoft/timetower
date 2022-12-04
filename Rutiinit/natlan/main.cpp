#include <iostream>
#include <../natlan.h>

using namespace std;

int main(){

    //NatLan natlan("Kausi 2, 21/40. Illallisviikon aloittaa varsinainen monitoiminainen, kun menusta vastaa radiojuontaja-toimittaja-näyttelijä Hanna Kinnunen. Hän on kuulemma myös palohälyttimien suurkuluttaja. Klaukkalassa illanvietto sujuu hillityissä tunnelmissa, kunnes koittaa ohjelmanumeron aika. Vieraina matkailun asiantuntija Terhi Alanya, tubettaja Miska Haakana ja mediapersoona Wallu Valpio.");
    //NatLan natlan("Tiimi pohtii, mikä saa litiumpariston syttymään tuleen. Voisiko jäteauto syttyä tuleen useiden paristojen räjähdysten ketjureaktiossa? Lisäksi tiimi selvittää, lentääkö jousiampujan nuoli suoraan. ");
    /*NatLan natlan("Fontana: Il mondo (Jimmy Fontana). \
                  Lewis: How high the moon (Benny Goodman). \
                  Bacharach: Räystäät jos tippuu (Danny). \
                  Giraud: Sous le ciel de Paris (Edith Piaf). \
                  Oliver: Yes indeed! (Dinah Shore). \
                  Lobo: Teresa (Lill Lindfors). \
                  Jobim:Desafinado (Gal Costa). \
                  Rydman: Maaliskuuta (Kari Rydman, laulu, ja yhtye). \
                  Gershwin: I loves you, Porgy (Nina Simone). \
                  Lehtinen: Yli vuorien, yli merien (Tuulikki Eloranta). \
                  Serradell: La Golondrina (Edmundo Ros orkestereineen). \
                  Holland: Baby, Baby, wo ist unsere Liebe (The Supremes). \
                  Denver: Farewell Andromeda (Welcome to my morning) (John Denver).\
                  \
                  Konsertin on koonnut Pasi Hiihtola");*/

    //NatLan natlan("34-vuotiaan Carlan ollessa vain parivuotias, arki isän kanssa päättyi vanhempien päättäessä suhteensa. Carla päätyi äidin mukana koti-Suomeen, isän jäädessä synnyinseuduilleen Italiaan, toisen perheensä luokse. Myöhemmin Carlalle selvisi että isä on kuollut. Jaksossa lähdetään selvittämään ketä kaikkia Carlan sukulaisia on mahdollista löytää Italiasta - ja kuinka hänet otetaan vastaan? Kotimainen realityohjelma. (39') Ohjelman tekstitys teksti-tv:n sivulla 333.");
    //NatLan natlan2("19-vuotiaan Mitten ollessa viisivuotias hänet adoptoitiin Suomeen pietarilaisesta lastenkodista. Biologisesta äidistään Mittellä ei ole mitään muistikuvia. Nyt hän on valmis lähtemään Pietariin kadonneen jäljille selvittämään, miksi äiti luopui hänestä ja löytyykö entisestä kotimaasta muuta perhettä. Unohtumaton ja yllätyksellinen matka tuo eteen tunteiden vuoristoradan. Kotimainen realityohjelma. (41') Ohjelman tekstitys teksti-tv:n sivulla 333.");

    //NatLan natlan("(1952) Aino Räsäsen romaaniin perustuva elokuva. N: Irma Seikkula, Jussi Jurkka, Leena Häkinen, Erkki Viljos, Matti Ranin, Leif Wager. O: Lasse Pöysti. ");




    //NatLan natlan("Turkulais-veljekset Saku ja Kalle yrittävät pubivisa-tiedoillaan peitota seinän, jotta voisivat viedä ensi vuonna 50 vuotta täyttävät vanhempansa reissuun. Kotimainen viihdeohjelma. (42') Ohjelman tekstitys teksti-tv:n sivulla 333.");
    //NatLan natlan2("Liisa-mummin puutarhahaaveista yritetään tehdä totta, kun isoäiti ja lapsenlapsi pääsevät testaamaan strategiaansa seinän edessä. Kotimainen viihdeohjelma. (41') Ohjelman tekstitys teksti-tv:n sivulla 333.");

    NatLan natlan("Amerikkalaisen jalkapallon legenda Tony Siragusa ja näyttelijä sekä juontaja James Cameron suunnittelevat man caveja eli miesluolia miehille, jotka haluavat asuntoonsa oman niin sanotun harrastushuoneensa. Jokainen miesluola suunnitellaan asukkaan toiveiden mukaiseksi: niissä voi olla mediakeskus, minibaari ja jopa oma lounge-alue.");
    NatLan natlan2("Kausi 9. Amerikkalaisen jalkapallon legenda Tony Siragusa ja näyttelijä sekä juontaja James Cameron suunnittelevat man caveja eli miesluolia miehille, jotka haluavat asuntoonsa oman niin sanotun harrastushuoneensa. Jokainen miesluola suunnitellaan asukkaan toiveiden mukaiseksi: niissä voi olla mediakeskus, minibaari ja jopa oma lounge-alue.");
    //NatLan natlan2("alkaa uusintana. Amerikkalaisen jalkapallon legenda Tony Siragusa ja näyttelijä sekä juontaja James Cameron suunnittelevat man caveja eli miesluolia miehille, jotka haluavat asuntoonsa oman niin sanotun harrastushuoneensa. Jokainen miesluola suunnitellaan asukkaan toiveiden mukaiseksi: niissä voi olla mediakeskus, minibaari ja jopa oma lounge-alue.");

    //NatLan natlan("");
    //NatLan natlan("");

    std::cout << "natlan.size() == " <<  natlan.size() << "'" << std::endl;

    for(unsigned long i=0; i<natlan.size(); i++){
        //std::cout << "'" <<  natlan.at(i) << "'" << std::endl;
    }

    //natlan.commonPhrases(natlan2);
    std::cout << "'" <<  natlan.commonPhrases(natlan2).toString() << "'" << std::endl;

}
