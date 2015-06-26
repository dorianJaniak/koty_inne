#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <utility>
#include <ctime>
#include <list>
#include <limits>

const int ILOSC_ZESTAWOW = 81;
using namespace std;
ofstream zapis;

unsigned l1=0;
unsigned l2=0;
unsigned l3=0;

struct Operacja;
class Zestaw;

struct OpPozycji {
    bool operator()(pair<unsigned,unsigned> const & a, pair<unsigned,unsigned> const & b)       //Zwraca True gdy A ma się znajdować przed B; False gdy trzeba odwrócić
    {
        if(a.second < b.second)     //B ma wieksze P niz A
            return true;
        else if(a.second > b.second)
            return false;
        else
        {
            if(a.first > b.first)   //B ma nizszy indeks niz A
                return true;
            else
                return false;
        }
    }
};

struct Operacja
{
    unsigned id;
    unsigned czas;
    unsigned maszyna;
    unsigned R;
    unsigned Q;
    unsigned PT;                        //Poprz Tech
    unsigned NT;                        //Nast Tech
    unsigned PK;                        //Poprz kolej
    unsigned NK;                        //Nast kolej
    unsigned LP;                        //Licz Poprz
    bool wykreslony;
    Operacja(unsigned ID =0, unsigned Maszyna=0, unsigned Czas=0) :      //Operacja zerowy
        id(ID), czas(Czas), maszyna(Maszyna)
    {
        R=0;
        Q=0;
        PT = NT = PK = NK = 0;
        LP = 0;
        wykreslony = false;
    }
    Operacja(const Operacja & op) :
        id(op.id), czas(op.czas), maszyna(op.maszyna),
        R(op.R), Q(op.Q),
        PT(op.PT), NT(op.NT), PK(op.PK), NK(op.NK),
        LP(op.LP), wykreslony(op.wykreslony)
    {    }
};

Operacja pOperacja = Operacja();                            //Pusta operacja

class Zestaw
{
public:
    priority_queue<pair<unsigned,unsigned>,vector<pair<unsigned,unsigned> >, OpPozycji> wstepneUszeregowanie;       //Pierwszy parametr to ID, drugi to CZAS_P
    string nazwaZestawu;
    Operacja * dane;
    unsigned * kolejnosc; //= new unsigned[ileOperacji - 1];
    unsigned ileZadan;
    unsigned ileMaszyn;
    unsigned ileOperacji;

    Zestaw(string nazwa, unsigned iloscZadan, unsigned iloscMaszyn) :
        nazwaZestawu(nazwa),
        ileZadan(iloscZadan),
        ileMaszyn(iloscMaszyn)
    {
        ileOperacji = ileZadan*ileMaszyn+1;
        kolejnosc = new unsigned[ileOperacji - 1];
        dane = new Operacja[ileOperacji];                   //Na starcie wszystkie operacje sa puste
    }
    ~Zestaw()
    {
        delete [] dane;
        delete [] kolejnosc;
    }
    Operacja& operator[](unsigned index)                         //Pobiera poszczegolna operacje o danym numerze INDEX
    {
        if(index > ileOperacji)
            return pOperacja;
        else
            return dane[index];
    }
    void uszeregujWstepnie()                                //Wrzuca wszystkie procesy do wstepneUszeregowanie
    {
        for(unsigned i=1; i<ileOperacji; i++)                //Procesu zerowego nie bierze sie pod uwage
            wstepneUszeregowanie.push(pair<unsigned,unsigned>(dane[i].id,dane[i].czas));
    }

    void wyznElemTech()
    {
        for(unsigned i = 1; i < ileOperacji; i++)
        {
            if(i % ileMaszyn == 0)
                dane[i].NT = 0;
            else
                dane[i].NT = i + 1;
            if((i-1)% ileMaszyn == 0)
                dane[i].PT = 0;
            else
                dane[i].PT = i - 1;
        }
    }

    void przygotujRQ()
    {
        for(unsigned i = 1; i < ileOperacji; i++)
        {
          //  dane[i].LP = 0;
            if(dane[i].PT != 0)
                dane[i].LP++;
            if(dane[i].PK != 0)
                dane[i].LP++;

            //Inna wersja - bez ifow
//            dane[dane[i].NT].LP++;
//            dane[dane[i].NK].LP++;
            dane[i].wykreslony = false;
        }
        //dane[0].
    }

    void wyswietlCos()
    {
        for(unsigned i = 1; i< ileOperacji; i++)
        {
            cout << i << " " << dane[i].LP << " " << dane[i].PT << " " << dane[i].NT << " " << dane[i].PK << " " << dane[i].NK << " " << dane[i].wykreslony << endl;
        }
        cout << endl;
    }

    void wyznaczRQ(unsigned wstawionyID=0)
    {
        przygotujRQ();
     //   unsigned * kolejnosc = new unsigned[ileOperacji - 1];
        unsigned indeks = 0;
        unsigned poczKolejki = 0;
        unsigned indeksWKolejnosci = 0;

        while(indeks < ileOperacji - 1)
        {
            for(unsigned i = 1; i < ileOperacji; i++)
            {
                if(dane[i].LP == 0 && !dane[i].wykreslony)
                    kolejnosc[indeks++] = dane[i].id;
            }
            if(kolejnosc[poczKolejki]==wstawionyID) indeksWKolejnosci = poczKolejki;
            while(poczKolejki != indeks)
            {
                dane[dane[kolejnosc[poczKolejki]].NK].LP--;
                dane[dane[kolejnosc[poczKolejki]].NT].LP--;
                dane[kolejnosc[poczKolejki]].wykreslony = true;
                poczKolejki++;
            }
        }

        for(unsigned i = indeksWKolejnosci; i < ileOperacji - 1; i++)
        {
            unsigned j = kolejnosc[i];
            dane[j].R = dane[j].czas + max(dane[dane[j].PK].R,dane[dane[j].PT].R);
            unsigned k = kolejnosc[ileOperacji - 2 - i];
            dane[k].Q = dane[k].czas + max(dane[dane[k].NK].Q, dane[dane[k].NT].Q);
        }

       // delete [] kolejnosc;
    }

    void aktualizujPoprzINast(unsigned poprzID, unsigned wstawionyID, unsigned nastID)
    {
        dane[poprzID].NK = wstawionyID;
        dane[nastID].PK = wstawionyID;
        dane[wstawionyID].PK = poprzID;
        dane[wstawionyID].NK = nastID;
    }
    unsigned obliczCMax()
    {
        unsigned max = 0;
        for(unsigned i=1; i<ileOperacji; i++)
        {
            if(dane[i].R > max) max = dane[i].R;
            if(dane[i].Q > max) max = dane[i].Q;
        }
        return max;
    }
};

struct Hala{
    list<unsigned>* maszyny;
    Hala (unsigned ileMaszyn){
        maszyny = new list<unsigned>[ileMaszyn+1];
        for(unsigned i=0; i<ileMaszyn+1; i++)
        {
            maszyny[i].push_back(0);
            maszyny[i].push_back(0);
        }
    }
    ~Hala(){
        delete [] maszyny;
    }
};

inline unsigned maxF(unsigned w1, unsigned w2)
{
    return (w1 > w2 ? w1 : w2) ;
}

unsigned obliczWartoscNaPozycji(unsigned procID, unsigned PoprzKolejID, unsigned NastKolejID, Zestaw*zestaw)
{
    unsigned PoprzTechID = zestaw->dane[procID].PT;
    unsigned NastTechID = zestaw->dane[procID].NT;
    return ( maxF( zestaw->dane[PoprzTechID].R, zestaw->dane[PoprzKolejID].R ) + maxF( zestaw->dane[NastTechID].Q, zestaw->dane[NastKolejID].Q ));
}

void zapiszDoPliku(Hala & hala, Zestaw*zestaw)
{
    //Zapis do pliku
    zapis << zestaw->nazwaZestawu.c_str() << endl;
    zapis << zestaw->obliczCMax() << endl;
    for(unsigned i=0; i<zestaw->ileMaszyn+1; i++)
    {
        zapis << "M" << i << ": ";
        std::list<unsigned>::iterator it = hala.maszyny[i].begin();
        for(++it; *it!=0/*it!=hala.maszyny[i].end()*/; ++it)
            zapis << zestaw->dane[*it].id << " ";
        zapis << endl;
    }
    zapis << endl;
}

void insa(Zestaw*zestaw)
{
    Hala hala(zestaw->ileMaszyn);
    zestaw->uszeregujWstepnie();
    zestaw->wyznElemTech();
    zestaw->wyznaczRQ();
    while(zestaw->wstepneUszeregowanie.size())
    {
        unsigned idProc = zestaw->wstepneUszeregowanie.top().first;             //Pobranie ID elementu z najwyzszym czasem wykonania
        unsigned maszyna = zestaw->dane[idProc].maszyna;                        //Pobranie numeru maszyny, na ktorej wykonac ma sie operacja
        std::list<unsigned>::iterator it = hala.maszyny[maszyna].begin();
        std::list<unsigned>::iterator itPoprzKolej = it;                        //Iterator na poprzedni element (poprzednik kolejnosciowy)
        ++it;                                                                   //Ponieważ pierwszym procesem jest proces 0 to od razu idziemy do kolejnego procesu                              //
        std::list<unsigned>::iterator itMinWartNaPozycji = it;                  //Zapamietujemy pozycje, na ktorej jest minimalna wyliczona wartosc
        unsigned minWartNaPozycji = UINT_MAX;                                   //Zapamietujemy wartosc wyliczona dla tej pozycji
        for(; it != hala.maszyny[maszyna].end(); ++it, ++itPoprzKolej)
        {
            unsigned wartoscNaPozycji = obliczWartoscNaPozycji(idProc,*itPoprzKolej,*it,zestaw);
            if(minWartNaPozycji > wartoscNaPozycji)
            {
                minWartNaPozycji = wartoscNaPozycji;
                itMinWartNaPozycji = it;
            }
        }
        unsigned nastID = *itMinWartNaPozycji;                                  //Zapamietanie numeru ID procesu nastepnego zanim wstawimy w to miejsce nowe ID
        hala.maszyny[maszyna].insert(itMinWartNaPozycji,idProc);                //Wstawiamy na najlepszej pozycji
        --itMinWartNaPozycji;--itMinWartNaPozycji;
        unsigned poprzID = *itMinWartNaPozycji;
        zestaw->aktualizujPoprzINast(poprzID,idProc,nastID);

      //  if(hala.maszyny[maszyna].size()>3)
            zestaw->wyznaczRQ(idProc);
        zestaw->wstepneUszeregowanie.pop();
    }

    zapiszDoPliku(hala,zestaw);
    cout << "Skonczony zestaw: " << zestaw->nazwaZestawu << endl;
}


void wczytajZestawy(string nazwaPliku, Zestaw* zestawy[])
{
    char buff[1024];
    unsigned ileZadan, ileMaszyn, olewana;
    ifstream plik;
    string nazwaZestawu;

    plik.open(nazwaPliku.c_str(),ios_base::in);
    if(!plik) {
        cerr << "Blad: Nie udalo sie otworzyc pliku!\n"; plik.close(); return;  }
    for(unsigned int i=0; i<ILOSC_ZESTAWOW; ++i)
    {
        do{
            plik.getline(buff,1024);                                    //Wczytanie linia po linii danych z pliku
            if(!plik.good())                                            //Jesli nastepuje blad to czyscimy flage stanu strumienia, aby mogl dzialac dalej
                plik.clear();
            nazwaZestawu = string(buff);
        }while(nazwaZestawu.find("data")==string::npos);                //Jesli nazwa zestawu zawiera tekst "data" to oznacza, ze to jest wlasnie linia z nazwa zestawu i petla zostaje przerwana
        plik >> ileZadan >> ileMaszyn >> olewana;                       //Tutaj znajduje sie do niczego nie potrzebna wartosc, ktora olewamy
        zestawy[i] = new Zestaw(nazwaZestawu,ileZadan,ileMaszyn);       //Tworzymy nowy zestaw danych
        for(unsigned j = 0; j<ileZadan; ++j)
        {
            plik >> olewana;                                            //Na poczatku kazdej nowej linii z zadaniem znajduje sie niepotrzebna w tym zadaniu dana
            unsigned maszyna = 0, czas = 0;
            for(unsigned k = 0; k<ileMaszyn; k++)                       //Pobieramy w linii kolejne operacja dla danego zadania
            {
                plik >> maszyna >> czas;
                (*zestawy[i])[j*ileMaszyn+k+1] = Operacja(j*ileMaszyn+k+1 , maszyna , czas);                   //zestaw i-ty, wstawianie wartosci poszczegolnych operacji
            }
        }
    }
    plik.close();
}

int main()
{
    Zestaw * zestawy[ILOSC_ZESTAWOW];       //zestaw wskaznikow na zestawy
    zapis.open("out.txt");
    clock_t pocz = clock();
    wczytajZestawy("insa.data.txt",zestawy);

    for(unsigned i=1; i<ILOSC_ZESTAWOW; i++)     //ILOSC_ZESTAWOW
        insa(zestawy[i]);

    clock_t koni = clock();
    cout << "\n\nCzas dzialania aplikacji: " << koni-pocz << "ms\n";
    cout << "Funk1: " << l1 << "\nFunk2: " << l2 << "\nFunk3: " << l3;
    getchar();
    zapis.close();
    //delete [] zestawy;
    return 0;
}

