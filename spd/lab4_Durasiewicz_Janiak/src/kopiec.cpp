#include "kopiec.h"

int potega(int liczba, int wykladnik)
{
	if(wykladnik==1) return liczba;
	return liczba*potega(liczba,wykladnik-1);
}

void Kopiec::napraw()
{	TYP * temp = memory;				//Zapamietanie "starej" pamieci kopca przy uzyciu wskaznika tymczasowego
	memory = new TYP[KOPIEC_MAX];	//Rezerwacja nowej pamieci dla kopca
	int count_temp = count;				//Zapamietanie liczby elementow
	count = 0;							
	for(int i=0; i<count_temp; i++)
		push(temp[i]);					//Wstawianie z sortowaniem
	delete [] temp;						//Usuniecie "starej" pamieci
}

void Kopiec::push(TYP element)
{	memory[count++] = element;			
	sortForPush(count-1);				
}

void Kopiec::sortForPush(int nrElem)
{	
	if(nrElem==0) return;				//Gdy kopiec nie ma elementow
	int nrOjca = (nrElem-1)/2;			
    if(m_cmpFcn(memory[nrElem],memory[nrOjca]))
    {	TYP temp = memory[nrOjca];		//Zmienna tymczasowa
		memory[nrOjca] = memory[nrElem];
		memory[nrElem] = temp;
		sortForPush(nrOjca);			//Rekurencyjne wywolanie funkcji dla ojca elementu o indeksie "nrElem"
	}
}

TYP Kopiec::pop()
{
	TYP temp = memory[0];
	memory[0] = memory[--count];
	sortForPop();
	return temp;
}

void Kopiec::sortForPop(int nrElem )
	{	int nrLewSyna = 2*nrElem+1;
		int nrPrawSyna = 2*nrElem+2;
		int nrWymienianego = 0;
		if(nrLewSyna>=count) return;
		if(nrPrawSyna<=count)
		{
            if(m_cmpFcn(memory[nrPrawSyna],memory[nrLewSyna]))
				nrWymienianego = nrPrawSyna;
			else
				nrWymienianego = nrLewSyna;
		}
		else
			nrWymienianego = nrLewSyna;

		//Etap zamiany elementow
        if(m_cmpFcn(memory[nrWymienianego],memory[nrElem]))
		{
			TYP temp = memory[nrWymienianego];
			memory[nrWymienianego] = memory[nrElem];
			memory[nrElem] = temp;
			sortForPop(nrWymienianego);
		}
	}

int Kopiec::ilePokolen()
{
	int ile_poziomow = 0;
	int count_temp = count;
	while(count_temp > 0)
	{	count_temp /= 2;
		ile_poziomow++;
	}
	return ile_poziomow;
}

bool Kopiec::empty()
{
    if(count>0) return false;
    return true;
}

TYP Kopiec::top()
{
    TYP temp = memory[0];
    return temp;
}

 Kopiec::Kopiec(const Kopiec * data)
 {
     memory = new TYP[KOPIEC_MAX];		//Rezerwacja pamieci
     for(int i=0; i<data->count; i++) memory[i] = data->memory[i];	//Kopiowanie elementow
     count = data->count;					//Wpisanie ilosci elementow
     malejaco = data->malejaco;
     m_cmpFcn = data->m_cmpFcn;
 }


 Kopiec::Kopiec(bool (*cmpFcn)(TYP, TYP) ) : m_cmpFcn(cmpFcn)
 {
     memory = new TYP[KOPIEC_MAX];		//Rezerwacja pamieci
     count = 0;
 }
