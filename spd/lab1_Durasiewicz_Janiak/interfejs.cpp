#include "interfejs.h"
#include "konfiguracja.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>


Interfejs::Interfejs(const char *nazwa_pliku)
{
    //Inicjalizacje
    maxCzas = INT_MAX;              //Poki co nie znalezlismy lepszego rozwiazania niz nieskonczonosc
    zbiorN = NULL;                  //Poniewaz teraz to sa wskazniki
    zbiorG = NULL;

    clock_t pocz, kon;              //Zmienne posluza do odmierzania czasu, aby nie przekroczyc 1s
    pocz = clock();
    kon = clock();

    //Wlasciwa czesc algorytmu
    wczytajZPliku(nazwa_pliku);
    for(int i=POCZ_TESTU; i<KONI_TESTU && kon-pocz < MS_TESTU; i++)         //Test polega na dodawaniu do jednego z warunkow dodatkowej wartosci,
                                                                            //ktora powoduje roznice w szeregowaniu
                                                                            //wartosci zdefiniowane w konfiguracja.h
    {
        przygotujKopie();
        sortuj(i);
        unsigned int cmax = wyliczCzas();
        if(cmax < maxCzas)                                                  //Jesli aktualna lista ma mniejsze cmax to zamieniamy
            zamienListy(cmax);
        else
            lista.clear();                                                  // a jesli nie to czyscimy liste, bo zaraz rozpoczniemy proces od nowa
        kon = clock();                                                      //odmierzamy czas
    }
//    wyswietl();
    doPliku();
}

void Interfejs::sortuj(unsigned int przedzial)              //standardowy algorytm z naszym udoskonaleniem
{
    unsigned int aktCzas = 0;                               //wskazuje czas jaki juz przeanalizowalismy
    while(!zbiorG->empty() || !zbiorN->empty())
    {
        while(!zbiorN->empty() && zbiorN->top()->R <= (aktCzas+przedzial))      //tutaj dodawana jest ta nasza wymyslona zmienna, ktora powoduje
                                                                                //roznice w szeregowaniu
        {
            zbiorG->push(zbiorN->top());
            zbiorN->pop();
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
        }
    }
}

void Interfejs::wczytajZPliku(const char * nazwaPliku)
{
    ifstream daneWej;
    daneWej.open(nazwaPliku,ios_base::in);   //Otwarcie strumienia
    if(!daneWej)                            //Czy nie wystąpił błąd
    {
        cerr << "Blad: Nie udalo sie otworzyc pliku!\n";
        daneWej.close();
        return;
    }

    string temp;
    getline(daneWej,temp);                  //Wczytanie pierwszej linii z pliku
    istringstream konwerter(temp);

    konwerter >> iloscProcesow;             //Wczytanie ilości procesów ze strumienia string do zmienna

    for(unsigned int i=0; i<iloscProcesow && daneWej.good() && konwerter.good(); i++)       //Dopoki nie wystapi blad lub wszystkie procesy zostana wczytane
    {
        unsigned int R,P,Q;
        getline(daneWej,temp);              //Wczytanie kolejnych linii
        temp = temp + ' ';
        konwerter.str(temp);
        konwerter >> R >> P >> Q;           //Wyciąganie danych
        Proces * proces = new Proces(i+1,R,P,Q);
        zbiorNOryg.push(proces);            //Wpisanie danych do kontenera, ktory nie bedzie edytowany
    }
    daneWej.close();
    if(!daneWej.good() || !konwerter.good())
        cerr << "Blad: Wystapil blad w trakcie wczytywania danych z pliku!\n";
}

void Interfejs::zamienListy(unsigned int czas)
{
    maxCzas = czas;                         //Zapisujemy najbardziej optymalny czas
    listaOpt.swap(lista);                   //Zamieniamy listy elementami
    lista.clear();                          //Czyścimy tymczasową listę ( ze starymi najbardziej optymalnymi elementami )
}

void Interfejs::przygotujKopie()
{
    if(zbiorN != NULL)                      //zbiorN jest tylko tymczasowy, a wiec po kazdej petli nalezy usunac z niego dane
        delete zbiorN;
    if(zbiorG == NULL)                      //zbiorG jest tworzony tylko raz na poczatku dzialania programu
        zbiorG = new priority_queue<Proces*,vector<Proces*>,OpPozycjiG>();
    zbiorN = new priority_queue<Proces*,vector<Proces*>,OpPozycjiN>(zbiorNOryg);        //po kazdym usunieciu zbioruN nalezy utworzyc go na nowo i
                                                                                        //skopiowac dane do niego z oryginalnego zbioru
}

unsigned int Interfejs::wyliczCzas()
{
   int zap = 0;                                //zapamietany czas (bo analizujemy pokolei procesy i przesuwamy sie)
   unsigned int mCzas = 0;                     //inaczej nasze cmax dla aktualnej listy
   for(list<Proces*>::iterator it= lista.begin(); it != lista.end(); ++it)     //Tutaj jest aktualna lista, a nie ta najlepsza!
   {
       Proces * wsk = *it;
       if( (zap - (int)wsk->R) < 0 )
           zap = wsk->R;
       zap += wsk->P;

       if(mCzas < ( zap+wsk->Q ) )
           mCzas = zap+wsk->Q;
   }
   return mCzas;
}

void Interfejs::wyswietl()
{
    cout << 1 << ' ' << listaOpt.size() << ' ';
    for(list<Proces*>::iterator it= listaOpt.begin(); it != listaOpt.end(); ++it)
    {
        Proces * wsk = *it;
        cout << wsk->ID << ' ';
    }
    cout << maxCzas;
}



 void Interfejs::doPliku()
 {
    ofstream strumien("out.txt");
    if(!strumien.good())
    {
        cerr << "Blad: Nie udalo sie zapisac pliku wynikowego";
        return;
    }
     strumien << 1 << ' ' << listaOpt.size() << ' ';
     for(list<Proces*>::iterator it= listaOpt.begin(); it != listaOpt.end(); ++it)
     {
         Proces * wsk = *it;
         strumien << wsk->ID << ' ';
     }
     strumien << maxCzas;
     strumien.close();
 }

 Interfejs::~Interfejs()
 {
    //Trzeba pokasowac dane
     lista.clear();
     listaOpt.clear();
     if(zbiorN != NULL)
        delete zbiorN;
     if(zbiorG != NULL)
        delete zbiorG;
 }
