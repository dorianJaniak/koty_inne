#include <iostream>
#include <string>
#include <ctime>
#include "interfejs.h"
using namespace std;


int main()
{
    string odpowiedz;
    cout << "Podaj nazwe pliku wejsciowego: ";
    cin >> odpowiedz;

    clock_t pocz = clock();
    Interfejs program(odpowiedz.c_str());
    clock_t koni = clock();
    cout << "Wykonanie programu zajelo: " << (koni-pocz) << "ms";

    cin.ignore();
    getchar();
    return 0;
}

