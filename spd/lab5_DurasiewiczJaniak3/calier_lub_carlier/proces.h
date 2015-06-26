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

    Proces(unsigned int id = -1, unsigned int r = -1, unsigned int p = -1, unsigned int q = -1) : ID(id), R(r), P(p), Q(q)
    {
        suma = R+Q;
        if(Q != 0)
            iloraz = (float)R/(float)Q;
        else
            iloraz = FLT_MAX;
    }
    Proces(const Proces & proc) : ID(proc.ID), R(proc.R), P(proc.P), Q(proc.Q), suma(proc.suma), grupa(proc.grupa), iloraz(proc.iloraz)
    {
    }
    ~Proces()
    {
    }
};

//bool operator> (const Proces & a)

#endif // PROCES_H
