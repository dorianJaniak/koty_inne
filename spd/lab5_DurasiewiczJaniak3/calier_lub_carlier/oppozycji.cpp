#include "OpPozycji.h"
#include "proces.h"

bool oppozycjiN (Proces a, Proces b)
{
    if(a.R < b.R)         //true
        return true;
    else
        return false;
}

bool oppozycjiG (Proces a, Proces b)
{
    if(a.Q > b.Q)
        return true;
    else
        return false;
}
