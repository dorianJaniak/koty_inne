#include "permutacja.h"

Permutacja::Permutacja() : m_cMax(0)
{

}

Permutacja::Permutacja( Permutacja & perm) : m_cMax(0)
{
    for(vector<TYP>::iterator it=perm.m_dane.begin(); it != perm.m_dane.end(); ++it)
        m_dane.push_back(*it);
}

Permutacja::~Permutacja()
{
   // m_dane.clear();
   // m_cZ.clear();
    //m_dane.clear();
    m_dane.resize(0);
    m_cZ.resize(0);
    m_dane.clear();
    m_cZ.clear();
    //std::vector<TYP>().swap(m_dane);
    //std::vector<int>().swap(m_cZ);
}

void Permutacja::policzCZak()
{
    if(m_dane.size()>0)
    {
        int cp = m_dane[0].R;
        for(int i=0; i<m_dane.size()-1; i++)
        {
            unsigned int cz = 0;
            cz = cp + m_dane[i].P;
            m_cZ.push_back(cz);

            cp = max(m_dane[i+1].R,cz);
        }
        int cz = 0;
        cz = cp + m_dane[m_dane.size()-1].P;
        m_cZ.push_back(cz);
    }
}
