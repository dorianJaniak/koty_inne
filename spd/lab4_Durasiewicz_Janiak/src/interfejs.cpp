#include "interfejs.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>


Interfejs::Interfejs(const char *nazwa_pliku, bool podzial) : zbiorNOryg(oppozycjiN), m_podzial(podzial)
{
    maxCzas = 0;
    wczytajZPliku(nazwa_pliku);
    zbiorG = new Kopiec(oppozycjiG);
    zbiorN = new Kopiec(&zbiorNOryg);
    sortuj();
    doPliku();
}

void Interfejs::sortuj()
{
    unsigned int aktCzas = 0;
    while(!zbiorG->empty() || !zbiorN->empty())
    {
        while(!zbiorN->empty() && zbiorN->top()->R <= aktCzas)      //tutaj dodawana jest ta nasza wymyslona zmienna, ktora powoduje
                                                                                //roznice w szeregowaniu
        {
            Proces* element = zbiorN->top();
            zbiorG->push(element);
            zbiorN->pop();
            if(lista.size() > 0)
                if(element->Q > lista.back()->Q && m_podzial )
                {
                    lista.back()->P = aktCzas-element->R; aktCzas = element->R;
                    if(lista.back()->P > 0)
                        zbiorG->push(lista.back());
                }
        }
        if(zbiorG->empty())
        {
            aktCzas = zbiorN->top()->R;
        }
        else
        {
            lista.push_back(zbiorG->top());
            aktCzas += zbiorG->top()->P;
            zbiorG->pop();
            maxCzas = max(maxCzas, aktCzas+lista.back()->Q);
        }
    }
}

void Interfejs::wczytajZPliku(const char * nazwaPliku)
{
    ifstream daneWej;
    daneWej.open(nazwaPliku,ios_base::in);   //Otwarcie strumienia
    if(!daneWej)                            //Czy nie wystąpił błąd
    {
        cerr << "Blad: Nie udalo sie otworzyc pliku!\n"; daneWej.close();
        return;
    }
    daneWej >> iloscProcesow;             //Wczytanie ilości procesów ze strumienia string do zmienna

    for(unsigned int i=0; i<iloscProcesow && daneWej.good(); i++)       //Dopoki nie wystapi blad lub wszystkie procesy zostana wczytane
    {
        unsigned int R,P,Q;
        daneWej >> R >> P >> Q;           //Wyciąganie danych
        Proces* proces = new Proces(i+1,R,P,Q);
        zbiorNOryg.push(proces);            //Wpisanie danych do kontenera, ktory nie bedzie edytowany
    }
    daneWej.close();
    if(!daneWej.good())
        cerr << "Blad: Wystapil blad w trakcie wczytywania danych z pliku!\n";
}


void Interfejs::wyswietl()
{
    cout << 1 << ' ' << lista.size() << endl;
    for(list<Proces*>::iterator it= lista.begin(); it != lista.end(); ++it)
    {
        Proces* wsk = *it;
        cout << wsk->ID << ' ';
    }
    cout << endl << maxCzas;
}

 void Interfejs::doPliku()
 {
    ofstream strumien("out.txt");
    if(!strumien.good())
    {
        cerr << "Blad: Nie udalo sie zapisac pliku wynikowego";
        return;
    }
     strumien << 1 << ' ' << lista.size() << endl;
     for(list<Proces*>::iterator it= lista.begin(); it != lista.end(); ++it)
     {
         Proces* wsk = *it;
         strumien << wsk->ID << ' ';
     }
     strumien << endl << maxCzas;
     strumien.close();
 }

 Interfejs::~Interfejs()
 {
     lista.clear();
     if(zbiorN != NULL)
        delete zbiorN;
     if(zbiorG != NULL)
        delete zbiorG;
 }
