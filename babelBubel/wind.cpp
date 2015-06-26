#include "wind.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
using namespace std;

Wind::Wind()
	{
		_mem = new TYP [2 * (WP_WIND_HEIGHT)];
		for(int i=0; i<(2 * (WP_WIND_HEIGHT)); i++)
		{	
			_mem[i] = 0;
		}
	}

void Wind::moveWinds()
{
	for(int i=0; i<WP_WIND_HEIGHT-1; i++)
	{	
		_mem[i*2+WIND_DIRECTION_TAB] = _mem[(i+1)*2+WIND_DIRECTION_TAB];
		_mem[i*2+WIND_FORCE_TAB] = _mem[(i+1)*2+WIND_FORCE_TAB];
	}
	_mem[(WP_WIND_HEIGHT-1)*2 + WIND_DIRECTION_TAB] = 0;
	_mem[(WP_WIND_HEIGHT-1)*2 + WIND_FORCE_TAB] = 0;	
}


void Wind::addWind()
{
	srand(time(NULL));
	moveWinds();		//Przesuwamy najpierw wiaterek
	_mem[(WP_WIND_HEIGHT-1)*2+WIND_DIRECTION_TAB] = rand()%3 - 1;	//losowanie kierunku
	_mem[(WP_WIND_HEIGHT-1)*2+WIND_FORCE_TAB] = rand()%(WIND_MAX-WIND_MIN)+WIND_MIN;	//losowanie sily
}

void Wind::getWind(TYP & direction, TYP & force)
{
	direction = _mem[WIND_DIRECTION_TAB];
	force = _mem[WIND_FORCE_TAB];
}

void Wind::prepareWinds()
{
	for(int i=0; i<WP_WIND_HEIGHT; i++)
	{
		addWind();
	}
}