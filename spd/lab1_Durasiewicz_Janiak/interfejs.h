#ifndef INTERFEJS_H
#define INTERFEJS_H

#include "proces.h"
#include "OpPozycji.h"

#include <list>
#include <queue>
using namespace std;

class Interfejs
{
    unsigned int maxCzas;                           //cmax, ale optymalny (czyli dla optymalnego posortowania)
    unsigned int iloscProcesow;                     //
    list<Proces*> lista;                            //lista już posortowanych procesów
    list<Proces*> listaOpt;                         //najbardziej optymalna lista
    priority_queue<Proces*,vector<Proces*>,OpPozycjiN> zbiorNOryg;     //zbiór elementów, z których wybierany będzie ten o min.R, ALE!
                                                                       //w tym zbiorze nie beda dokonywane zadne operacje, posluzy on tylko do kopiowania danych
    priority_queue<Proces*,vector<Proces*>,OpPozycjiG> * zbiorG;        //zbiór chwilowo gotowych elementów (wybierany ten o max.Q)
    priority_queue<Proces*,vector<Proces*>,OpPozycjiN> * zbiorN;        //zbior sortujacy wzgledem R (minimum) operacyjny

    void sortuj(unsigned int przedzial = 0);                        //Schrange z naszym ulepszeniem
    void wczytajZPliku(const char * nazwaPliku);
    void wyswietl();
    void zamienListy(unsigned int czas);                            //zamienia liste lista z listaOpt i czysci zmienna lista
    void przygotujKopie();                                          //kopiuje dane ze zbioru zbiorNOryg do zbiorN, aby moc edytowac zbiorN
    unsigned int wyliczCzas();
    void doPliku();                                                 //domyslnie do pliku "out.txt"
public:
    Interfejs(const char* nazwa_pliku);
    ~Interfejs();

};

#endif // INTERFEJS_H
