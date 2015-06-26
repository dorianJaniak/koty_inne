#ifndef OPPOZYCJI_H
#define OPPOZYCJI_H

#include "konfiguracja.h"
#include "proces.h"

struct OpPozycjiN {
    bool operator()(Proces* const & a, Proces* const & b)
    {
        if(a->R > b->R)         //true
            return true;
        else
            return false;
    }
};

struct OpPozycjiG {
    bool operator() (Proces* const & a, Proces* const & b)
    {
        if(a->Q < b->Q)
            return true;
        else
            return false;
    }
};

#endif // OPPOZYCJI_H
