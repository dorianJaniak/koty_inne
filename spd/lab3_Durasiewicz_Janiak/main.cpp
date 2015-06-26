#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <utility>
#include <limits>
#include <climits>
#include <ctime>
#include <algorithm>

#define MODYFIKACJA 0

using namespace std;

ofstream wyjscie;

struct OpPozycji {
    bool operator()(pair<int,int> const & a, pair<int,int> const & b)
    {
        if(a.second == b.second)
        {
            if(a.first > b.first)
                return true;
            else
                return false;
        }
        if(a.second < b.second)
            return true;
        else
            return false;
    }
};

class Proces {
public:
    Proces(int ID, vector<int> & czasyProcesu) : czasy(czasyProcesu), id(ID)
    {
        r.resize(czasy.size());
        q.resize(czasy.size());
        for(unsigned int i=0; i<r.size(); i++)
            r[i] = q[i] = 0;
    }
    vector<int> czasy;
    int id;
    vector<int> r;
    vector<int> q;
};

Proces * pusty = NULL;

int * pobierzDane(string& nazwaPliku, int & wierszy, int & kolumn)
{
    ifstream plik;
    plik.open(nazwaPliku.c_str());
    if(!plik)
    {
        cerr << "Blad: Nie udalo sie otworzyc pliku: " << nazwaPliku.c_str();
        return NULL;
    }
    plik >> wierszy >> kolumn;
    int * dane = new int[wierszy*kolumn];

    for(int j=0; j<wierszy*kolumn; j++)
        plik >> dane[j];
    return dane;
}

pair<int,int> najlepszaPozycja(vector<Proces*>& szereg, Proces * aktualny)
{
    int mincmax = INT_MAX;
    int minjpos = 0;
    for(int j=0; j<=(signed int)szereg.size(); j++)
    {

        Proces* procNast = pusty;
        Proces* procPoprz = pusty;

        if(j<(signed int)szereg.size())
            procNast = szereg[j];
        if((j-1)>=0)
            procPoprz = szereg[j-1];

        int maxcmax = 0;
        for(int z=aktualny->czasy.size()-1; z>=0; z--)
        {
            int cmax1 = procPoprz->czasy[z]+procPoprz->r[z];
            for(int h=0; h<(signed int)aktualny->czasy.size()-z; h++)
            {
                int cmax2 = 0;
                for(int k=0; k<=h; k++)
                    cmax2 += (aktualny->czasy[z+k]);
                cmax2 += (procNast->czasy[z+h] + procNast->q[z+h]);
                if(cmax1+cmax2 > maxcmax)                   //Czy dana kombinacja jest lepsza?
                    maxcmax = cmax1+cmax2;      //Zapamietanie kombinacji
            }
        }
        if(maxcmax < mincmax)
        {
            mincmax = maxcmax;
            minjpos = j;
        }
    }
    return pair<int,int>(minjpos,mincmax);
}

void obliczRiQ(vector<Proces*>& szereg, int indeksR, int indeksQ)
{
	if(szereg.size()==0) return;
    int procesow = szereg.size();
    int maszyn = szereg[0]->czasy.size();
    for(int i=indeksR; i<procesow && i>=0; i++)
    {
        Proces * poprzedni = pusty;
        Proces * aktualny = szereg[i];
        if(i>0) poprzedni = szereg[i-1];
        aktualny->r[0] = poprzedni->r[0]+poprzedni->czasy[0];
        for(int j=1; j<maszyn; j++)
            aktualny->r[j] = max(poprzedni->r[j]+poprzedni->czasy[j],aktualny->r[j-1]+aktualny->czasy[j-1]);
    }
    for(int i=indeksQ; i>=0 && i<procesow; i--)
    {
        Proces * nastepny = pusty;
        Proces * aktualny = szereg[i];
        if(i<procesow-1)
            nastepny = szereg[i+1];
        aktualny->q[maszyn-1] = nastepny->q[maszyn-1]+nastepny->czasy[maszyn-1];
        for(int j=maszyn-2; j>=0; j--)
            aktualny->q[j] = max(nastepny->q[j]+nastepny->czasy[j],aktualny->q[j+1]+aktualny->czasy[j+1]);
    }
}

//Regula 3
int wytypujProcesMOD(vector<Proces*>& szereg)
{
    int procesow = szereg.size();
    int maszyn = szereg[0]->czasy.size();
    int typowanyProces = -1;
    int maxObciazenia = 0;
    int j=0;
    for(int i=0; j<maszyn && i<procesow;)
    {
        int k = i;
        Proces * aktualny = szereg[i];
        Proces * nastepny = pusty;
        if(i<procesow-1) nastepny = szereg[i+1];

        int obciazenie = 0;
        for(; j<maszyn-1 && i==k; j++)
        {
            obciazenie += aktualny->czasy[j];
            if(aktualny->r[j+1] == nastepny->r[j])
            {
                if(aktualny->czasy[j+1] < nastepny->czasy[j]) i++;
            }
            else if(aktualny->r[j+1] < nastepny->r[j]) i++;
        }
        if(j==maszyn-1)
        {
            obciazenie += aktualny->czasy[j];
            i++;
        }
        if(obciazenie > maxObciazenia)
        {
            maxObciazenia = obciazenie;
            typowanyProces = i-1;
        }
        obciazenie = 0;
    }
    return typowanyProces;
}

void utworzProcesPusty(int maszyn)
{
    vector<int> zerowy;
    for(int h=0; h<maszyn; h++)
       zerowy.push_back(0);
    pusty = new Proces(-1,zerowy);
}

void algorytmNEH(int * dane, int wierszy, int kolumn)
{
    int ostatecznyCmax = 0;
    //Policzenie czasow procesow - uszeregowanie
    priority_queue<pair<int,int>,vector<pair<int,int> >,OpPozycji> kolejka;
    for(int i=0; i<wierszy; i++)
    {
        int czasProcesu= 0;
        for(int j=0; j<kolumn; j++)
            czasProcesu += dane[i*kolumn+j];
        kolejka.push(pair<int,int>(i,czasProcesu));
    }
    vector<Proces*> szereg;
    while(!kolejka.empty())
    {
        int id = kolejka.top().first;
        vector<int> tmp;
        for(int j=0; j<kolumn; j++)
            tmp.push_back(dane[id*kolumn+j]);
        Proces * aktualny = new Proces(id,tmp);

        pair<int,int> minPar = najlepszaPozycja(szereg,aktualny);
        int minjpos = minPar.first;
        int mincmax = minPar.second;
        //Tutaj juz wybrana najlepsza pozycja
        int z;
        vector<Proces*>::iterator it;
        for(it = szereg.begin(), z=0; z<minjpos; z++, ++it);
        szereg.insert(it,aktualny);
        obliczRiQ(szereg,minjpos,minjpos);

        ostatecznyCmax = mincmax;

#if MODYFIKACJA == 1
        if(szereg.size()>0)
        {
            int typowanyIndeks = wytypujProcesMOD(szereg);
            int nieRusz = minjpos;
            if(typowanyIndeks < minjpos) nieRusz -=1;

            Proces * typowany = szereg[typowanyIndeks];
            vector<Proces*>::iterator it2 = szereg.begin();
            for(it2 = szereg.begin(), z=0; z<typowanyIndeks; z++, ++it2);
            szereg.erase(szereg.begin()+(typowanyIndeks));

            obliczRiQ(szereg,typowanyIndeks,typowanyIndeks);

            minPar = najlepszaPozycja(szereg,typowany);
            minjpos = minPar.first;
            mincmax = minPar.second;
            //Tutaj juz wybrana najlepsza pozycja
            for(it = szereg.begin(), z=0; z<minjpos; z++, ++it);
            szereg.insert(it,typowany);
            obliczRiQ(szereg,minjpos,minjpos);

            ostatecznyCmax = mincmax;
        }
#endif
        kolejka.pop();
    }

    wyjscie << ostatecznyCmax << endl;
    for(int i=0; i<szereg.size();i++)
        wyjscie <<  (szereg[i]->id+1) << " ";
    wyjscie << endl << endl;
}

int main()
{
    wyjscie.open("out.txt");
    clock_t pocz = clock();
    for(int i=0; i<=120; i++)
    {
        string nazwaPliku = string("data");
        ostringstream ss;
        ss << i;
        if(ss.str().size() == 1) nazwaPliku+=string("00");
        else if(ss.str().size() == 2)nazwaPliku+=string("0");
        nazwaPliku+=(ss.str()+string(".txt"));

        int wierszy, kolumn;
        int * dane = pobierzDane(nazwaPliku,wierszy,kolumn);
        if(dane == NULL) return -1;

        utworzProcesPusty(kolumn);
        wyjscie << "Plik: " << i << endl;
        algorytmNEH(dane,wierszy,kolumn);
        delete [] dane;
    }
    clock_t koni = clock();
    cout << "Czas: " << koni - pocz <<endl;
    getchar();
    return 0;
}

