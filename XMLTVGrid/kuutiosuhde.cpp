#include "kuutiosuhde.h"


long KuutioTyyppi::rank() const
{

    return this->tyyppix + this->tyyppiy + this->tyyppiz;

    //tyyppix & (~tyyppiy);


    /*
     * return  (this->tyyppix & (~(this->tyyppiy | this->tyyppiz))) +
            (this->tyyppiy & (~(this->tyyppix | this->tyyppiz))) +
            (this->tyyppiz & (~(this->tyyppix | this->tyyppiy)));

    /*
    if(this->tyyppix && this->tyyppiy && this->tyyppiz){
        return 1;
    }else{
        return 0;
    }
    */
}

KuutioTyyppi KuutioTyyppi::yhdista(const KuutioTyyppi &tyyppi) const
{
    return KuutioTyyppi{
            (this->tyyppix & tyyppi.tyyppix),
            (this->tyyppiy & tyyppi.tyyppiy),
            (this->tyyppiz & tyyppi.tyyppiz)
    };
}

const char *dnames[3] = {"x", "y", "z"};

std::string KuutioTyyppi::toString() const
{
    std::string str;

    str += "    tyyppi" + std::string(dnames[SUUNTAX]) + " " + tyyppi_to_string(tyyppix) + "\n";
    str += "    tyyppi" + std::string(dnames[SUUNTAY]) + " " + tyyppi_to_string(tyyppiy) + "\n";
    str += "    tyyppi" + std::string(dnames[SUUNTAZ]) + " " + tyyppi_to_string(tyyppiz) + "\n";


    return str;
}

Kuutiosuhde::Kuutiosuhde(Neliosuhde *top, Neliosuhde *bottom, Neliosuhde *left, Neliosuhde *right, Neliosuhde *front, Neliosuhde *behind) :
    top(top), bottom(bottom), left(left), right(right), front(front), behind(behind) {}

int Kuutiosuhde::getPystyTyyppi() const
{
    return front->getPystyTyyppi() & behind->getPystyTyyppi() &
            left->getPystyTyyppi() & right->getPystyTyyppi();
}

int Kuutiosuhde::getVaakaTyyppi() const
{
    return front->getVaakaTyyppi() & behind->getVaakaTyyppi() &
            top->getVaakaTyyppi() & bottom->getVaakaTyyppi();
}

int Kuutiosuhde::getSyvyysTyyppi() const
{
    return top->getPystyTyyppi() & bottom->getPystyTyyppi() &
            left->getVaakaTyyppi() & right->getVaakaTyyppi();
}

KuutioTyyppi Kuutiosuhde::getKuutioTyyppi() const
{
    return KuutioTyyppi{getVaakaTyyppi(), getPystyTyyppi(), getSyvyysTyyppi()};
}

bool Kuutiosuhde::sovita(const Kuutiosuhde* kuutio, int suunta) const
{
    if(suunta==SUUNTAX){
        return this->right  == kuutio->left;
    }
    if(suunta==SUUNTAY){
        return this->bottom == kuutio->top;
    }
    if(suunta==SUUNTAZ){
        return this->behind == kuutio->front;
    }

    if(suunta==SUUNTAX+VASTAAN){
        return this->left  == kuutio->right;
    }
    if(suunta==SUUNTAY+VASTAAN){
        return this->top == kuutio->bottom;
    }
    if(suunta==SUUNTAZ+VASTAAN){
        return this->front == kuutio->behind;
    }
}

void Kuutiosuhde::etsiKuutiot(std::vector<Neliosuhde> &neliot, std::vector<Kuutiosuhde> &kuutiot)
{
    const int koko=neliot.size();

    for(int x1=0;x1<koko;x1++){
        Neliosuhde& top=neliot.at(x1);

        for(int y1=x1;y1<koko;y1++){
            Neliosuhde& left=neliot.at(y1);

            if(top.getY1() == left.getX1()){

                for(int z1=x1;z1<koko;z1++){
                    Neliosuhde& front=neliot.at(z1);

                    if(top.getX1() == front.getX1() && left.getY1() == front.getY1()){

                        for(int z2=z1;z2<koko;z2++){
                            Neliosuhde& behind=neliot.at(z2);

                            if(top.getX2()== behind.getX1() && left.getY2() == behind.getY1()){

                                for(int y2=z1;y2<koko;y2++){
                                    Neliosuhde& right=neliot.at(y2);

                                    if(front.getY2() == right.getY1() && top.getY2() == right.getX1() &&
                                            behind.getY2() == right.getY2()){

                                        for(int x2=y2;x2<koko;x2++){
                                            Neliosuhde& bottom=neliot.at(x2);

                                            if(front.getX2() == bottom.getX1() &&
                                                    behind.getX2() == bottom.getX2() &&
                                                    left.getX2() == bottom.getY1() &&
                                                    right.getX2() == bottom.getY2()){

                                                Kuutiosuhde kuutio(&top, &bottom, &left, &right, &front, &behind);
                                                if(kuutio.getVaakaTyyppi() && kuutio.getPystyTyyppi() && kuutio.getSyvyysTyyppi()){
                                                //if(kuutio.getKuutioTyyppi().rank()>65000){
                                                //if(kuutio.getKuutioTyyppi().rank()>105000){
                                                    kuutiot.push_back(kuutio);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    std::cerr << "löytyi " << kuutiot.size() << " kpl" << std::endl;
}

