#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits.h>
#include <string>
#include <ctime>

using namespace std;

int * dane;
int * Cmax;
int * F;
int * odniesienia;
int * optymalnaPerm;
unsigned int ilePermutacji;
unsigned int iloscProc;
unsigned int karaOpt;

string nazwa = "data";
string nazwaKoncowki[] = {"10.txt", "11.txt", "12.txt", "13.txt", "14.txt", "15.txt", "16.txt",
                 "17.txt", "18.txt", "19.txt", "20.txt" };




inline int max(int a, int b)
{
    return ((a>b) ? a : b);
}

unsigned int nrWiersza(int indeks)
{
    int wiersz = 0;
    while((indeks/=2) != 0)
        wiersz++;
    return wiersz;
}

void policzCmax(int indeks)
{
    int wynik = 0;
    for( unsigned int maska = 1; wynik == 0; maska*=2)
    {
        wynik = maska & indeks;
        if(wynik != 0)
        {
            int wiersz = nrWiersza(wynik);
            Cmax[indeks] = Cmax[indeks - wynik] + dane[wiersz*3];
        }
    }
}

void policzKare(int indeks)
{
    F[indeks] = INT_MAX;
    for(unsigned int maska = 1; maska <= indeks; maska *= 2)
    {
        int wynik = maska & indeks;
        if(wynik != 0)
        {
            int wiersz = nrWiersza(wynik);
            int tmp = F[indeks-wynik] + max( 0, (Cmax[indeks]-dane[wiersz*3+2])*dane[wiersz*3+1]);
            if(tmp < F[indeks])
            {
                F[indeks] = tmp;
                odniesienia[indeks] = indeks-wynik;
            }
        }
    }
}

void wypiszKolejke(int ileProcesow)
{
    int indeks = (int)(ilePermutacji)-1;
    int * kolejnosc = new int[ileProcesow];
    int pozycjaKol = ileProcesow-1;             //pomaga zapisać do tablicy dane wynikowe
    while(indeks != 0)
    {
        int indeks2 = odniesienia[indeks];
        kolejnosc[pozycjaKol] = nrWiersza(indeks - indeks2);
        indeks = indeks2;
        pozycjaKol--;
    }

    for(int i= 0; i<ileProcesow; i++)
        cout << (kolejnosc[i]+1) << ' ';
    cout << endl << endl;

    delete [] kolejnosc;
}

void wyliczKoszt(int T[])
{
    int czasZak = 0, kara = 0;
    for(int i=0; i<iloscProc; i++)
    {
        czasZak += dane[T[i]*3];
        kara += max(0,( czasZak - dane[T[i]*3+2] ) * dane[T[i]*3+1]  );
    }
    if(kara<karaOpt)
    {
        karaOpt = kara;
        for(int j=0; j<iloscProc; j++)
            optymalnaPerm[j] = T[j];
    }
}


void permutuj (int T[], int aktPoz)
{
  if( aktPoz < iloscProc-1 )
  {
    for(int i = aktPoz; i < iloscProc; i++)
    {
      int tmp = T[aktPoz];
      T[aktPoz] = T[i];
      T[i] = tmp;

      permutuj(T, aktPoz+1);

      tmp = T[aktPoz];
      T[aktPoz] = T[i];
      T[i] = tmp;
    }
  }
  else wyliczKoszt(T);
}

void wypiszPermutacje()
{
    for(int i=0; i<iloscProc; i++)
        cout << (optymalnaPerm[i]+1) << ' ';
}

int main()
{
    clock_t pocz = clock();
    for(int indPliku = 0; indPliku <= 10; indPliku++)
    {
//Wczytanie pliku
        string nazwaPliku = nazwa + nazwaKoncowki[indPliku];
        ifstream daneWej;
        daneWej.open( nazwaPliku.c_str(),ios_base::in );
        if(!daneWej)
        {
            cerr << "Blad: Nie udalo sie otworzyc pliku!\n";
            daneWej.close();
            return -1;
        }
        daneWej >> iloscProc;
        ilePermutacji = pow(2,iloscProc);

//Stworzenie tablic oraz wczytanie danych z pliku
        odniesienia = new int[ilePermutacji];
        Cmax = new int[ilePermutacji];
        dane = new int[iloscProc*3];
        F = new int[ilePermutacji];
        F[0] = Cmax[0] = odniesienia[0] = 0;        //Inicjalizacja zmiennych
        for(unsigned int i=0; i<iloscProc; i++)
            daneWej >> dane[i*3] >> dane[i*3+1] >> dane[i*3+2];

        clock_t poczA = clock();
        for(unsigned int liczba = 1; liczba < ilePermutacji; liczba++)
        {
            policzCmax(liczba);
            policzKare(liczba);
        }

//Wypisanie wyników
        cout << endl << "Czas wykonania WiTi: " << (clock() - poczA) << endl;
        cout << "Kara optymalna: "<< F[ilePermutacji-1] << endl;
        cout << "Optymalne uszeregowanie: ";
        wypiszKolejke(iloscProc);

        delete [] dane;
        delete [] Cmax;
        delete [] F;
        delete [] odniesienia;
    }
    cout << endl << endl << "Czas wykonania programu: " << (clock()-pocz) << " ms";



//Wariant brute-force
    for(int indPliku = 0; indPliku <= 10; indPliku++)
    {
//Wczytanie pliku
        string nazwaPliku = nazwa + nazwaKoncowki[indPliku];
        ifstream daneWej;
        daneWej.open( nazwaPliku.c_str(),ios_base::in );
        if(!daneWej)
        {
            cerr << "Blad: Nie udalo sie otworzyc pliku!\n";
            daneWej.close();
            return -1;
        }
        daneWej >> iloscProc;

//Wczytanie danych z pliku
        dane = new int[iloscProc*3];
        for(unsigned int i=0; i<iloscProc; i++)
            daneWej >> dane[i*3] >> dane[i*3+1] >> dane[i*3+2];

        karaOpt = INT_MAX;
        optymalnaPerm = new int[iloscProc];
        int * tab = new int[iloscProc];
        for(int z=0; z<iloscProc; z++) tab[z] = z;

//Algorytm brute-force
        clock_t pocz = clock();
        permutuj(tab,0);
        cout << endl << "Czas wykonania BF: " << (clock() - pocz) << endl;
        cout << "Kara optymalna: " << karaOpt << endl;
        cout << "Optymalne uszeregowanie: ";
        wypiszPermutacje();
        cout << endl;

        delete [] optymalnaPerm;
        delete [] tab;
        delete [] dane;
    }

    cin.ignore();
    getchar();
}

