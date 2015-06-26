#ifndef PROCES_H
#define PROCES_H
#include <cfloat>
#include <iostream>
using namespace std;

struct Proces {
    unsigned int ID;
    unsigned int R, P, Q;
    unsigned int suma, grupa;
    float iloraz;

    Proces(unsigned int id, unsigned int r, unsigned int p, unsigned int q) : ID(id), R(r), P(p), Q(q)
    {
        suma = R+Q;
        if(Q != 0)
            iloraz = (float)R/(float)Q;
        else
            iloraz = FLT_MAX;
    }
    ~Proces()
    {
        cout << "Usunalem obiekt\n";
    }
};

#endif // PROCES_H
