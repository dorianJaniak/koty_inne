#include "interfejs.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <limits>

Kopiec* zPermutacjiDoKopca(Permutacja& perm)
{
    Kopiec * kopiec = new Kopiec(oppozycjiN);
    for(vector<TYP>::iterator it=perm.m_dane.begin(); it != perm.m_dane.end(); ++it)
    {
        TYP wart = *it;
        kopiec->push(wart);
    }
    return kopiec;
}

Interfejs::Interfejs(const char *nazwa_pliku)
{
    upB = INT_MAX;
    Kopiec * zbiorNOryg = wczytajZPliku(nazwa_pliku);
    carlier(zbiorNOryg);
    doPliku();
}

void Interfejs::carlier(Kopiec* zbiorNOryg)
{
    Kopiec * zbiorN = new Kopiec(zbiorNOryg);
    unsigned int lowB = 0;

    Permutacja temp;
    Permutacja wynik;
    unsigned lb = schrage(true,zbiorN,temp);
    if(lb >= upB)
    {
        delete zbiorNOryg;
        return;
    }
    zbiorN = new Kopiec(zbiorNOryg);
    unsigned int U = schrage(false,zbiorN,wynik);       //1 krok
    wynik.policzCZak();
    if(U<upB)                                           //2 krok
    {
        upB = U;
        optymalna = wynik;
    }
    int a,b,c,rpi,qpi,ppi;
    if(wyznaczABC(wynik,a,b,c)==false)                  //3 krok
    {
      /*  if(zbiorN!=NULL)
        {
            delete zbiorN;
            zbiorN = NULL;
        }*/
        if(zbiorNOryg != NULL)
        {
            delete zbiorNOryg;
            zbiorNOryg = NULL;
        }
        return;                                         //4 krok
    }
    wyznaczRPiQPiPPi(rpi,qpi,ppi,a,b,c,wynik);            //5 krok

    int rzapamietane = wynik.m_dane[c].R;
    wynik.m_dane[c].R = rpi;//max(rzapamietane,rpi+ppi);      //6 krok
    {
        lowB = maxLB(b,c,wynik,lb);
        testyEliminacyjne(wynik,b,c,upB);
     //   Permutacja temp;
     //   lowB = schrage(true,zPermutacjiDoKopca(wynik),temp);    //7 krok
        if(lowB<upB)                                        //8 krok
        {
            Kopiec * kopczyk = zPermutacjiDoKopca(wynik);
            carlier(kopczyk);         //9 krok
          //  if(kopczyk != NULL)
          //      delete kopczyk;
        }
        wynik.m_dane[c].R = rzapamietane;                   //10 krok
    }

    int qzapamietane = wynik.m_dane[c].Q;
    wynik.m_dane[c].Q = qpi;//max(qzapamietane,qpi+ppi);
    {
      //  Permutacja temp;
      //  Kopiec * kopczyk = zPermutacjiDoKopca(wynik);
      //  lowB = schrage(true,kopczyk,temp);
        lowB = maxLB(b,c,wynik,lb);
        testyEliminacyjne(wynik,b,c,upB);
        if(lowB<upB)
        {
            carlier(zPermutacjiDoKopca(wynik));
        }
        wynik.m_dane[c].Q = qzapamietane;
    }
  /*  if(zbiorN != NULL)
    {
        delete  zbiorN;
        zbiorN = NULL;
    }*/
    if(zbiorNOryg != NULL)
    {
        delete  zbiorNOryg;
        zbiorNOryg = NULL;
    }
}

bool Interfejs::wyznaczABC(Permutacja & perm, int& a, int& b, int& c)
{
    int aktCMax=0;
    for(int i=0; i<perm.m_dane.size(); i++)      //Ponieważ liczymy od 0
    {
        if(perm.m_cZ[i]+perm.m_dane[i].Q == perm.m_cMax) // > aktCMax
        {
            b=i; aktCMax= perm.m_cZ[i]+perm.m_dane[i].Q;
        }
    }
    //for(a=b; perm.m_cZ[a-1] >= perm.m_dane[a].R && a>0; a--);


    a=b;
    int sumka = 0;
    for(int j=b; j>=0; j--)
    {
        sumka += perm.m_dane[j].P;
        if(perm.m_cMax == perm.m_dane[j].R + sumka + perm.m_dane[b].Q)
            a=j;
    }

    for(c=b; perm.m_dane[c].Q >= perm.m_dane[b].Q && c>=a; c--);
    if(perm.m_dane[c].Q < perm.m_dane[b].Q)
        return true;
    return false;
}

void Interfejs::wyznaczRPiQPiPPi(int& erpi, int& qupi, int& pepi, int a, int b, int c, Permutacja& perm)
{
  //  erpi = perm.m_dane[c+1].R;
  //  qupi = perm.m_dane[c+1].Q;
    pepi = 0;
    erpi = perm.m_dane[c+1].R;
    for(int j=c+1; j<=b; j++)
    {
        if(perm.m_dane[j].R < erpi) erpi = perm.m_dane[j].R;
      //  if(perm.m_dane[j].Q < qupi) qupi = perm.m_dane[j].Q;
        pepi += perm.m_dane[j].P;
    }
    qupi = max( (unsigned)(pepi + perm.m_dane[b].Q) , perm.m_dane[c].Q);
    erpi = max( (unsigned)(erpi + pepi) , perm.m_dane[c].R);
}

unsigned Interfejs::hFunkcja(unsigned b, unsigned c, Permutacja & perm, unsigned & minR, unsigned &suma)
{
    suma=0;
    minR = INT_MAX;
    unsigned q = INT_MAX;
    for(unsigned j=c; j<=b; j++)
    {
        if(perm.m_dane[j].R < minR) minR = perm.m_dane[j].R;
        if(perm.m_dane[j].Q < q) q = perm.m_dane[j].Q;
        suma+= perm.m_dane[j].P;
    }
    return (minR + q + suma);
}

unsigned Interfejs::maxLB(unsigned b, unsigned c, Permutacja & perm, unsigned lb)
{
    unsigned hcb, hcpb;
    unsigned psuma, minR;
    hcb = hFunkcja(b,c,perm,minR,psuma);
    hcpb = hFunkcja(b,c+1,perm,minR,psuma);
    return max(max(hcpb,hcb),lb);
}

unsigned int Interfejs::schrage(bool podzial, Kopiec * zbiorN, Permutacja & wynik)
{
    Kopiec * zbiorG = new Kopiec(oppozycjiG);
    unsigned int aktCzas = 0;
    while(!zbiorG->empty() || !zbiorN->empty())
    {
        while(!zbiorN->empty() && zbiorN->top().R <= aktCzas)      //tutaj dodawana jest ta nasza wymyslona zmienna, ktora powoduje
                                                                                //roznice w szeregowaniu
        {
            Proces element = zbiorN->top();
            zbiorG->push(element);
            zbiorN->pop();
            if(wynik.m_dane.size() > 0)
                if(element.Q > wynik.m_dane.back().Q && podzial )
                {
                    wynik.m_dane.back().P = aktCzas-element.R; aktCzas = element.R;
                    if(wynik.m_dane.back().P > 0)
                        zbiorG->push(wynik.m_dane.back());
                }
        }
        if(zbiorG->empty())
        {
            aktCzas = zbiorN->top().R;
        }
        else
        {
            wynik.m_dane.push_back(zbiorG->top());
            aktCzas += zbiorG->top().P;
            zbiorG->pop();
            wynik.m_cMax = max(wynik.m_cMax, aktCzas+wynik.m_dane.back().Q);
        }
    }
   // cout << zbiorN->empty() << endl;
    delete zbiorG;
    zbiorG = NULL;
    delete zbiorN;      //Dodana linijka - to tutaj tracimy pamięć
    zbiorN = NULL;
    return wynik.m_cMax;
}

void Interfejs::testyEliminacyjne(Permutacja & perm, int b, int c, int UB)
{
    unsigned minR, psum;
    unsigned h = hFunkcja(b,c+1,perm,minR,psum);
    unsigned przedzial = UB - h;
    for(int i=0; i<c+1; i++)
    {
        if( perm.m_dane[i].P > przedzial)
        {
            if(perm.m_dane[i].R + perm.m_dane[i].P + psum + perm.m_dane[b].Q >= UB)
                perm.m_dane[i].R = max(perm.m_dane[i].R, minR + psum);
            else if(minR + perm.m_dane[i].P + psum + perm.m_dane[i].Q >= UB)
                perm.m_dane[i].Q = max(perm.m_dane[i].Q, perm.m_dane[b].Q + psum);
        }
    }
    for(int i=b+1; i<perm.m_dane.size();i++)
    {
        if( perm.m_dane[i].P > przedzial)
        {
            if(perm.m_dane[i].R + perm.m_dane[i].P + psum + perm.m_dane[b].Q >= UB)
                perm.m_dane[i].R = max(perm.m_dane[i].R, minR + psum);
            else if(minR + perm.m_dane[i].P + psum + perm.m_dane[i].Q >= UB)
                perm.m_dane[i].Q = max(perm.m_dane[i].Q, perm.m_dane[b].Q + psum);
        }
    }
}

Kopiec * Interfejs::wczytajZPliku(const char * nazwaPliku)
{

    ifstream daneWej;
    daneWej.open(nazwaPliku,ios_base::in);   //Otwarcie strumienia
    if(!daneWej)                            //Czy nie wystąpił błąd
    {
        cerr << "Blad: Nie udalo sie otworzyc pliku!\n"; daneWej.close();
        return NULL;
    }
    daneWej >> iloscProcesow;             //Wczytanie ilości procesów ze strumienia string do zmienna

    Kopiec * zbiorNOryg = new Kopiec(oppozycjiN);
    for(unsigned int i=0; i<iloscProcesow && daneWej.good(); i++)       //Dopoki nie wystapi blad lub wszystkie procesy zostana wczytane
    {
        unsigned int R,P,Q;
        daneWej >> R >> P >> Q;           //Wyciąganie danych
        Proces proces(i+1,R,P,Q);
        zbiorNOryg->push(proces);            //Wpisanie danych do kontenera, ktory nie bedzie edytowany
    }
    daneWej.close();
    if(!daneWej.good())
        cerr << "Blad: Wystapil blad w trakcie wczytywania danych z pliku!\n";
    return zbiorNOryg;
}


/*void Interfejs::wyswietl()
{
    cout << 1 << ' ' << lista.size() << endl;
    for(list<TYP>::iterator it= lista.begin(); it != lista.end(); ++it)
    {
        Proces wsk = *it;
        cout << wsk.ID << ' ';
    }
    cout << endl << maxCzas;
}*/

 void Interfejs::doPliku()
 {
    ofstream strumien("out.txt");
    if(!strumien.good())
    {
        cerr << "Blad: Nie udalo sie zapisac pliku wynikowego";
        return;
    }
     strumien << 1 << ' ' << optymalna.m_dane.size() << endl;
     for(vector<TYP>::iterator it= optymalna.m_dane.begin(); it != optymalna.m_dane.end(); ++it)
     {
         Proces wsk = *it;
         strumien << (wsk.ID-1) << ' ';
     }
     strumien << endl << optymalna.m_cMax;
     strumien.close();
 }

 Interfejs::~Interfejs()
 {

 }
