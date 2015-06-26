#ifndef KOPIEC_HH
#define KOPIEC_HH

#include <iostream>
#include <iomanip>
#include "proces.h"
#define COUT_WIDTH 4
#define KOPIEC_MAX 1024
#define TYP Proces
using namespace std;

int potega(int liczba, int wykladnik);	//Zwraca potege zmiennej "liczba" podniesionej do zmiennej "wykladnik"

class Kopiec {
    bool (*m_cmpFcn)(TYP,TYP) ;
    bool malejaco;                       //Domyślnie malejaco
	TYP * memory;						//Wskaznik na tablice elementow kopca
	int count;							//Licznik elementow
	void sortForPush(int nrElem);		//Sortuje w strone korzenia zaczynajac od elementu o indeksie "nrElem"
	void sortForPop(int nrElem = 0);	//Sortuje w strone mlodszych pokolen zaczynajac od elementu o indeksie "nrElem"
	int nrLewegoSyna(int nrElem)		//Zwraca numer lewego syna elementu o indeksie "nrElem"
	{	return 2*nrElem+1;	}
	int nrPrawegoSyna(int nrElem)		//Zwraca numer prawego syna elementu o indeksie "nrElem"
	{	return 2*nrElem+2;	}
	int nrOjca(int nrElem)				//Zwraca numer ojca elementu o indeksie "nrElem"
	{	return (nrElem-1)/2;	}
public:
    ~Kopiec()							//Usuwa pamiec dynamiczna kopca
    {  delete [] memory;	}
	void napraw();						//Rezerwuje nowa pamiec dla kopca oraz wstawia pojedynczo elementy funkcja "sortForPush"
	void push(TYP element);				//Wstawia nowy element do pamieci, inkrementuje licznik i wywoluje funkcje "sortForPush"
	TYP pop();							//Zwraca wartosc korzenia oraz wywoluje funkcje sortujaca "sortForPop"
	int ilePokolen();					//Zwraca liczbe pokolen znajdujacych sie w drzewie		

    bool empty();
    TYP top();
    Kopiec(const Kopiec * data);
    Kopiec(bool (*cmpFcn)(TYP,TYP) );

};

#endif
