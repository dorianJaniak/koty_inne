#ifndef INTERFEJS_H
#define INTERFEJS_H

#include "proces.h"
#include "OpPozycji.h"
#include "kopiec.h"

#include <list>
#include <queue>
using namespace std;

class Interfejs
{
    bool m_podzial;                                 //Czy ma byc sortowanie z podzialem czy bez

    unsigned int maxCzas;                           //cmax, ale optymalny (czyli dla optymalnego posortowania)
    unsigned int iloscProcesow;                     //
    list<Proces*> lista;                            //przechowuje jedynie kolejnosc

    Kopiec zbiorNOryg;
    Kopiec * zbiorG;
    Kopiec * zbiorN;

    void sortuj();                                  //Schrange z podzialem i bez
    void wczytajZPliku(const char * nazwaPliku);
    void wyswietl();
    void doPliku();                                 //domyslnie do pliku "out.txt"
public:
    Interfejs(const char* nazwa_pliku, bool podzial);
    ~Interfejs();

};

#endif // INTERFEJS_H
