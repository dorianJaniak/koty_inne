#include "game.h"


int Game::putBlock(unsigned int x, unsigned int y,  Map & map, unsigned int who)
	{
		ElementData temp = map(x,y);
		bool isP[4] = {false,false,false,false};								//isPossible, jesli uda sie gdzies go zaczepic
		if(temp[DIM_WEIGHT_TAB] != EMPTY) return GAME_BUSY_BLOCK;		//Jesli blok juz jest zajety
		if(y==0)										//Kiedy znajduje sie na samym dole nie trzeba nic sprawdzac
			isP[2] = true;
		else //if(who!=PS_GOD_BLOCK)											//W przeciwnym wypadku trzeba sprawdzic czy graniczy z dowolnym innym blokiem
		{								
			if(x>0)	isP[0] = map(x-1,y,DIM_WHO_TAB)!=EMPTY;							//Element po lewej
			if(x<map.getWidth()-1 ) isP[1] = map(x+1,y,DIM_WHO_TAB)!=EMPTY;	//Element po prawej
			isP[2] = map(x,y-1,DIM_WHO_TAB)!=EMPTY;							//Element ponizej
			if(y<map.getHeight()-1) isP[3] = map(x,y+1,DIM_WHO_TAB)!=EMPTY;		//Element powyzej
		}
		if((isP[0] || isP[1]) && !isP[2] && !isP[3] && who!=PS_GOD_BLOCK) who = PS_USER_BRIDGE;		//Jesli nie ma nic nad i pod to jest to most
		if(isP[0] || isP[1] || isP[2])							//Jesli mozna ustawic klocek to akceptuj i wyjdz z funkcji
		{	
			map(x,y,DIM_WHO_TAB) = who;
			if(y == map.getHeight()-1 ) return GAME_THE_END;	//Jesli okazuje sie, ze udalo sie postawic bloczek i dotknelismy nieba to koniec gry
			return 0;
		}
		else return GAME_IN_THE_AIR_BLOCK;
	}

void Game::clearBlock(unsigned int x, unsigned int y, Map & map)	//Kasowanie bloku ze zwiekszeniem licznika rozbitych
	{
		clearBlock((ElementData*)&map(x,y));
	}

void Game::clearBlock(ElementData * block)
	{
		block->operator[](DIM_WEIGHT_TAB) = block->operator[](DIM_LEFT_TO_DO_TAB) = block->operator[](DIM_WHO_TAB) = EMPTY;
	}

TYP Game::findWeightPath(unsigned int x, unsigned int y, list<ElementData*> * path, Map & map, int x_last)
	{	
		static int iterX[] = {0,-1,-1,1,1};
		static int iterY[] = {-1,-1,0,-1,0};
		path->push_back((ElementData *)&map(x,y));
		if(y==0)
			return map(x,y,DIM_WEIGHT_TAB);
		TYP result = 0;
		for(int i=0; i<5 && result == 0; i++)
			if(x+iterX[i] >= 0 && x+iterX[i] < map.getWidth() && ( x_last == 0 || x-x_last != x+iterX[i]))			//czy nie wykraczam poza mape
				if( map(x+iterX[i] , y+iterY[i] , DIM_WEIGHT_TAB )>0 && map( x+iterX[i] , y+iterY[i] , DIM_WEIGHT_TAB)!=EMPTY) 
					result = findWeightPath( x+iterX[i] , y+iterY[i] ,path, map , iterX[i]);
		if(result != 0)
		{
			if(map(x,y,DIM_WEIGHT_TAB)<result) return map(x,y,DIM_WEIGHT_TAB);
			else return result;	//zwraca minimalna wage w razie jak znajdzie sciezke
		}
		else
		{
			path->pop_back();
			return 0;
		}
	}

void Game::subtractFromPath(list<ElementData*> * path, TYP subtrahend)
	{
		list<ElementData*>::iterator temp = path->begin();
		(*temp)->operator [](DIM_LEFT_TO_DO_TAB) -= subtrahend;
		path->pop_front();
		for(list<ElementData*>::iterator it = path->begin(); it!=path->end(); it++)
			((*it)->operator[](DIM_WEIGHT_TAB)) -= subtrahend;
	}

list<ElementData*> * Game::testWeight(Map & map)
	{
		list<ElementData*> * toDestroy = new list<ElementData*>;
			for(int i=1; i<map.getHeight(); i++)
				for(int j=0; j<map.getWidth(); j++)
					if(map(j,i,DIM_LEFT_TO_DO_TAB)!=EMPTY)
					{	
						double result = 1;
						while(map(j,i,DIM_LEFT_TO_DO_TAB)!=0 && result>0)		//Operacja odliczania wagi
						{
							list<ElementData*> path;
							result = findWeightPath(j,i,&path,map);		
							if(result > 0.0)
							{
								if(map(j,i,DIM_LEFT_TO_DO_TAB)>result)
									subtractFromPath(&path,result);
								else
									subtractFromPath(&path,map(j,i,DIM_LEFT_TO_DO_TAB));
							}
						}
						if(map(j,i,DIM_LEFT_TO_DO_TAB)!= 0)
						{
							toDestroy->push_back((ElementData*)&map(j,i));
						}
					}
		return toDestroy;
	}

TYP sumWeights(Map & map)
{
	TYP sum = 0;
	for(int i=0; i<map.getHeight(); i++)
		for(int j=0; j<map.getWidth(); j++)
			if(map(j,i,DIM_WEIGHT_TAB)!=EMPTY)
				sum+= map(j,i,DIM_WEIGHT_TAB);
	return sum;
}	

list<ElementData*> * Game::testWind(Map & map, Wind & wind)
{
	list<ElementData*> * toDestroy = new list<ElementData*>;
	TYP sum = sumWeights(map);
	TYP dir,force;
	wind.getWind(dir,force);
	int i=1;
	for(i; i<_highestBlock+1 && sum-force>0; i++)
		sum-=force;
	if(i<_highestBlock) //To oznacza, ze wiatr nie zostal zamortyzowany
	{
		for(int k = i; k<=_highestBlock; k++)		//chodzenie po wysokosciach
		{
			bool breaker = false;
			if(dir==WIND_RIGHT_DIR)
			{
				for(int j=0; j<map.getWidth() && !breaker; j++)
					if(map(j,i,DIM_WHO_TAB)!=EMPTY)
					{
						toDestroy->push_back(&map(j,k));
						breaker = true;
					}
			}
			else
			{
				for(int j=map.getWidth()-1; j>=0 && !breaker; j--)
					if(map(j,i,DIM_WHO_TAB)!=EMPTY)
					{
						toDestroy->push_back(&map(j,k));
						breaker = true;
					}
			}
		}
	}
	return toDestroy;
}

int Game::destroyElements(list<ElementData*>* toDestroy)
	{
		int dstrBlocks = 0;
		if(toDestroy!=NULL)
		{
			for(list<ElementData*>::iterator it = toDestroy->begin(); it!=toDestroy->end(); it++)
			{
				clearBlock(*it);
				dstrBlocks++;
			}
			toDestroy->clear();
		}
		return dstrBlocks;
	}

int Game::askUserToDo(Map & map, int & wait)
	{
		unsigned int x,y;
		do
		{
			_printer.printMessage(USER_ASKED_TO_DO,cout);	//Prosba o wpisanie danych
			_user.makeStep(x,y);							//uzytkownik wpisuje dane
		}while(x>map.getWidth()-1 || y>map.getHeight()-1);
		//Wstawianie bloku
		int result = putBlock(x,y,map);
		//reakcja na wstepny test czy w ogole sie da polozyc blok
		if(result == GAME_BUSY_BLOCK)
		{	
			_printer.printMessage(GAME_BUSY_BLOCK,cout);
			wait++;
		}
		else if(result == GAME_IN_THE_AIR_BLOCK)			//Jesli w powietrzu kamien
		{
			_printer.makeBoomConsole();
			_printer.printMessage(GAME_IN_THE_AIR_BLOCK, cout);	//To prosba, aby nie klasc w powietrzu kamieni
			bool destroyedBlock = false;
			for(int i= y; i>=0 && !destroyedBlock; i--)		//Sprawdzenie czy spadl kamien na jakis inny
			{
				if(map(x,i,DIM_WEIGHT_TAB)!=EMPTY)		//Jesli spadl to trzeba o tym poinformowac i usunac kamien
				{
					_destroyedBlocks++;
					_printer.printMessage(GAME_SAY_YOU_DESTROYED, cout);
					clearBlock(x,i,map);
					destroyedBlock = true;
					wait ++;
				}//Szukanie czy mozna rozbic jakis blok
			}
			_usedBlocks++;
			_destroyedBlocks++;
			wait++;
		}
		if(result == 0)
		{	
			_usedBlocks++;
		}
		return result;
	}

void Game::askGodToDo(Map & map, int & wait)
	{
		unsigned int x, y;
		int tBdstr = _god.makeStep((*this),x,y);	//Ile zostanie zniszczonych
		if(tBdstr>0)
		{
			_printer.printMessage(GOD_DECIDE_TO_DO,cout);
			int result = putBlock(x,y,_map,PS_GOD_BLOCK);
			prepareWeights(_map);
			list<ElementData *> * path = testWeight(_map);
			if(path->size() > 0)
			{
				if(_map(x,y,DIM_WHO_TAB) != path->back()->operator [](DIM_WHO_TAB))
				{
					_destroyedBlocks += destroyElements(path);
					_printer.printMessage(GOD_SAY_OMG,cout);
					_printer.makeBoomConsole();
				}
				else
					_printer.printMessage(GOD_BLESS_YOU, cout);
				wait++;	
			}
			delete path;
		}
	}

void Game::askWindToDo(Map & map, int & wait)
{
	list<ElementData *> * toDestroy = testWind(map,_wind);
	if(toDestroy->size() > 0)		//Jesli udalo sie cos zniszczyc
	{
		_destroyedBlocks += destroyElements(toDestroy);
		_printer.printMessage(GAME_SAY_WIND_DESTROYED, cout);
		_printer.makeBoomConsole();
		wait ++ ;
	}
	_wind.addWind();
}

void Game::prepareWeights(Map & map)
	{	
		for(int j = 0; j<map.getWidth(); j++)
			if(map(j,map.getHeight()-1,DIM_WHO_TAB)==PS_USER_BLOCK)
			{
				map(j,map.getHeight()-1,DIM_WEIGHT_TAB) = PS_FIRST_DURABILITY;
				map(j,map.getHeight()-1,DIM_LEFT_TO_DO_TAB) = PS_WEIGHT_USER_BLOCK;
			}
		for(int i=map.getHeight()-2; i>=0; i--)
			for(unsigned int j=0; j<map.getWidth(); j++)
				if(map(j,i,DIM_WHO_TAB)!=EMPTY)	//Gdy jest jakis element
				{	
					if(map(j,i+1,DIM_WHO_TAB)==EMPTY || map(j,i+1,DIM_WHO_TAB)==PS_GOD_BLOCK)			//Gdy nie ma nad nim zadnego innego bloku uzytkownika
					{	
						if(map(j,i,DIM_WHO_TAB)==PS_USER_BLOCK)	//Jesli to uzytkownik postawil blok	
							map(j,i,DIM_WEIGHT_TAB) = PS_FIRST_DURABILITY;			//Wytrzymalosc materialu
						else if(map(j,i,DIM_WHO_TAB)==PS_GOD_BLOCK)										//Jesli jednak Bog
							map(j,i,DIM_WEIGHT_TAB) = PS_FIRST_DURABILITY_GOD_BLOCK;
						else														//moze byc jeszcze most postawiony
							map(j,i,DIM_WEIGHT_TAB) = PS_FIRST_DURABILITY_BRIDGE;
					}
					else 	//Jesli nad wybranym blokiem jest inny blok uzytkownika
					{
							map(j,i,DIM_WEIGHT_TAB) = map(j,i+1,DIM_WEIGHT_TAB) + PS_DIF_BTW_DURABILITY;		//to zwiekszamy wytrzymalosc tego bloku ponizej o troche
					}
					//Nadawanie wagi blokom
					if(map(j,i,DIM_WHO_TAB) == PS_USER_BLOCK)						//W zaleznosci od tego kto to zrobil to nadajemy odpowiednia wage blokowi
						map(j,i,DIM_LEFT_TO_DO_TAB) = PS_WEIGHT_USER_BLOCK;
					else if(map(j,i,DIM_WHO_TAB) == PS_GOD_BLOCK)
						map(j,i,DIM_LEFT_TO_DO_TAB) = PS_WEIGHT_GOD_BLOCK;
					else
						map(j,i,DIM_LEFT_TO_DO_TAB) = PS_WEIGHT_USER_BRIDGE;
				}
	}