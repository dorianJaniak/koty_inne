
#include "const.h"

#if TEST == 1
	#include <iostream>
	using namespace std;
#endif

#ifndef MAP_H
#define MAP_H

class ElementData {
	public:
	TYP _tab[DIMENSION];
	TYP & operator[](int dim)
	{	
		return _tab[dim];
	}
	TYP & operator=(TYP a)
	{
		_tab[0] = a;
		return _tab[0];
	}
/*	operator TYP&()
	{	return _tab[0];	}*/
};

//Klasa jest tylko struktura danych bez zadnych "inteligentnych" rozwiazan
class Map {
	#if TEST == 1
		ElementData smieci;
	#endif
		ElementData * _mem;		//Wskaznik na pamiec blokow
		unsigned int _w;			//Szerokosc
		unsigned int _h;			//Wysokosc
	public:
		Map(unsigned int width, unsigned int height) 
		{	
			_w = width;
			_h = height;
			_mem = new ElementData[_w*_h];		
		}
		Map(Map & original);
		~Map()
		{	
			if(_mem != NULL) delete [] _mem;	
			_mem = NULL;
		}
		void clear();	//Czysci mape calkowicie
		ElementData& operator()(unsigned int col, unsigned int row);
		TYP& operator()(unsigned int col, unsigned int row, unsigned int dim) ;
		unsigned int getWidth() 
		{ 
			return _w;	
		}
		unsigned int getHeight() 
		{ 
			return _h;	
		}

	#if TEST == 1
		void testPrint(ostream & stream)
		{	
			for(unsigned int i=0; i<_h; i++)
			{	
				for(unsigned int j=0; j<_w; j++)
					if(_mem[i*_w + j][DIM_WEIGHT_TAB]!=EMPTY) stream << _mem[i*_w + j][DIM_WEIGHT_TAB] << "\t";
					else stream << ".\t";
				stream << endl;
			}
		}
	#endif

};

#endif