#include "map.h"

Map::Map(Map & original)
		{
			_w = (original.getWidth());
			_h = original.getHeight();
			_mem = new ElementData[_w * _h];
			if(_mem!=NULL)
				for(unsigned int i=0; i<_h; i++)
					for(unsigned int j=0; j<_w; j++)
					{
						ElementData * temp = new ElementData(original(j,i));
						this->operator ()(j,i) = *temp;
					}
		}

void Map::clear()	//Czysci mape calkowicie
		{	
			unsigned int max = _w * _h;
			while(max>0) 
			{	max--;
				for(int i =0; i<DIMENSION; i++)
					_mem[max][i] = EMPTY;
				
			}
		}

ElementData& Map::operator()(unsigned int col, unsigned int row) 
		{ 
	#if TEST == 1
			if(row>=_h || col>=_w)
			{	
				cerr << "Blad (map.h)! Wykroczyles poza indeks mapy!\n";
				return smieci;
			}
			else
	#endif
			return _mem[_w*row+col];	
		}

TYP& Map::operator()(unsigned int col, unsigned int row, unsigned int dim) 
		{ 
	#if TEST == 1
			if(row>=_h || col>=_w)
			{	
				cerr << "Blad (map.h)! Wykroczyles poza indeks mapy!\n";
				return smieci[0];
			}
			else
	#endif
				return _mem[_w*row+col].operator [](dim);	
		}