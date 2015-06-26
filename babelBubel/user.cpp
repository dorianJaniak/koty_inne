#include "user.h"
#include "game.h"
#include "map.h"

void Player::makeStep(unsigned int & x, unsigned int & y)
	{	
		cin >> y >> x;		//Najpierw wiersz, potem kolumna
		if(cin.fail())
		{
			cin.clear();
			cin.ignore(1024,'\n');
		}
	}



bool God::isGodMakingStep(unsigned int mapHeight, int godTemper)		//Bog podejmuje decyzje czy wykonuje ruch
	{
		srand(time(NULL));
		double amp = 0.0;
		if(godTemper == GT_TEMPERATE) amp = GT_TEMPERATE_THRESH;
		else if(godTemper == GT_ANGRY) amp = GT_ANGRY_THRESH;
		unsigned int result = (unsigned int)((GT_FULL_THRESH-amp)*(mapHeight)*GT_DIVISOR_THRESH);
		result = rand() % result;
		if(result == 0)
		{	
			return true;
		}
		else return false;
	}
int God::getGodTemper(unsigned int highestBlock, unsigned int destroyed, unsigned int pyramidBlocks, Map & map)
	{
		static unsigned int dstr = 0;
		if(destroyed - dstr > 0)
		{	dstr = destroyed;
			return GT_SMILE;		//Gdy uzytkownik sam sobie zepsul bloki to Bog jest wesoly
		}
		dstr = destroyed;
		//Znalezienie najwyzej polozonego punktu
		
		if(highestBlock > GT_ANGRY_THRESH * map.getHeight())
			return GT_ANGRY;
		else if(highestBlock > GT_TEMPERATE_THRESH * map.getHeight())
			return GT_TEMPERATE;
		else
			return GT_SMILE;
	}
int God::makeStep(Game & gameMng, unsigned int & x, unsigned int & y)
	{
		int tBDstr = 0;
		if(isGodMakingStep(gameMng._map.getHeight(),gameMng._godTemper))
		{	
			for(int j=0; j<gameMng._map.getWidth(); j++)
			{	
				bool foundElem = false;
				for(int i = gameMng._map.getHeight()-1; i>1 && !foundElem; i--)
				{
					Map copied(gameMng._map);
					if(gameMng.putBlock(j,i,copied,PS_GOD_BLOCK)==0)
					{
						if(gameMng._map(j,i-1,DIM_WEIGHT_TAB)!=EMPTY)		//Jesli znaleziono element ponizej to konczymy zabawe
							foundElem = true;
						gameMng.prepareWeights(copied);
						list<ElementData *> * path = gameMng.testWeight(copied);
						if(path->size()>tBDstr)
						{	
							tBDstr = path->size();
							x = j;
							y = i;
						//	copied = Map(gameMng._map);
						}
						delete path;
					}
				}
			}
		}
		return tBDstr;
	}
