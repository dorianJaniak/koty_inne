#include "const.h"

#ifndef WIND_H
#define WIND_H

class Wind {
	TYP * _mem ;
	friend class Interface;
public:
	Wind();
	void addWind();
	void moveWinds();
	void getWind(TYP & direction, TYP & force);
	void prepareWinds();
};




#endif