#ifndef INTERFEJS_H
#define INTERFEJS_H

#include "proces.h"
#include "OpPozycji.h"
#include "kopiec.h"
#include "permutacja.h"

#include <list>
#include <queue>
using namespace std;

class Interfejs
{
    unsigned int iloscProcesow;                     //
    Permutacja optymalna;
    unsigned int upB;
  //  Kopiec zbiorNOryg;

    void carlier(Kopiec* zbiorNOryg);
    unsigned int schrage(bool podzial, Kopiec *zbiorN, Permutacja &wynik);                                  //Schrange z podzialem i bez
    void testyEliminacyjne(Permutacja & perm, int b, int c, int UB);
    unsigned hFunkcja(unsigned b, unsigned c, Permutacja & perm, unsigned & minR, unsigned &suma);
    Kopiec* wczytajZPliku(const char * nazwaPliku);
    //void wyswietl();
    void doPliku();                                 //domyslnie do pliku "out.txt"
    bool wyznaczABC(Permutacja &perm,int& a, int& b, int& c);
    void wyznaczRPiQPiPPi(int& erpi, int& qupi, int& pepi, int a, int b, int c, Permutacja& perm);
    unsigned maxLB(unsigned b, unsigned c, Permutacja & perm, unsigned lb);
public:
    Interfejs(const char* nazwa_pliku);
    ~Interfejs();

};

#endif // INTERFEJS_H
