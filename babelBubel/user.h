#include <iostream>
#include <cstdlib>
#include <ctime>
#include <list>
using namespace std;


class Game;
class ElementData;
class Map;

#ifndef PLAYER_H
#define PLAYER_H

class Player {

public:
	void makeStep(unsigned int & x, unsigned int & y);
};


class God {

public:
	bool isGodMakingStep(unsigned int mapHeight, int godTemper);
	int getGodTemper(unsigned int highestBlock, unsigned int destroyed, unsigned int pyramidBlocks, Map & map);
	int makeStep(Game & gameMng, unsigned int & x, unsigned int & y);
};
#endif