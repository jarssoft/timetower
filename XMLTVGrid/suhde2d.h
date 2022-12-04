#ifndef TULOSTETTAVA_H
#define TULOSTETTAVA_H


class Suhde2D
{
public:
    Suhde2D();

public:

   virtual int getVaakaTyyppi() const = 0;

   virtual int getPystyTyyppi() const = 0;

    //palauttaa esimerkin joltain olevassa olevalta riviltä
   virtual int getRivilta(int i) const = 0;

    //palauttaa esimerkin joltain olevassa olevalta sarakkeelta
   virtual int getSarakkeelta(int i) const = 0;

   virtual int getP(int x, int y) const = 0;

   virtual int getWidth() const = 0;

   virtual int getHeight() const = 0;

   virtual int countP() const = 0;

};

#endif // TULOSTETTAVA_H
