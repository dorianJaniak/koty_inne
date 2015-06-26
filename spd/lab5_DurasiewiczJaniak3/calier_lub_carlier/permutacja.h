#ifndef PERMUTACJA_H
#define PERMUTACJA_H
#include <list>
#include <vector>
#include "kopiec.h"

class Permutacja
{

public:
    vector<TYP> m_dane;
    vector<int> m_cZ;       //Czas końca
    unsigned int m_cMax;
    Permutacja();
    Permutacja(Permutacja &perm);
    ~Permutacja();
    void policzCZak();

};

#endif // PERMUTACJA_H
