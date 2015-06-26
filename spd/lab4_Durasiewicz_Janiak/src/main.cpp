#include <iostream>
#include <string>
#include <ctime>
#include "interfejs.h"
using namespace std;


int main()
{
    bool podzial;
    string odpowiedz;
    cout << "Podaj nazwe pliku wejsciowego: ";
    cin >> odpowiedz;
    cout << "Z podzialem [0 - bez, 1 - z] : ";
    cin >> podzial;

    clock_t pocz = clock();
    Interfejs program(odpowiedz.c_str(),podzial);
    clock_t koni = clock();
    cout << "Wykonanie programu zajelo: " << (koni-pocz) << "ms";

    cin.ignore();
    getchar();
    return 0;
}

