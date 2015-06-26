#include "delayer.h"
#include <list>
using namespace std;
#include "map.h"
#include "interface.h"
#include "user.h"
#include "wind.h"

class Player;
class Wind;

#ifndef GAME_H
#define GAME_H

class Game {
	friend class God;
	Map _map;
	Interface _printer;	//Obiekt wyswietlajacy wszystko na ekranie
	Player _user;		//Gracz
	
	God _god;
				//Mapa
	Wind _wind;
	unsigned int _usedBlocks;		//statystyka uzytych klockow
	unsigned int _destroyedBlocks;	//statystyka zniszczonych klockow
	unsigned int _godTemper;			//to czy Bog sie cieszy
	unsigned int _countedPyramid;		//Ilosc maksymalna blokow
	unsigned int _highestBlock;		//Najwyzej polozony blok
	int countPyramid(Map & map)
	{
		int sum = 0;
		for(int i=map.getHeight(); i>0; i--)
		{	sum += i;	}
		return sum;
	}
	int countHighestBlock(Map & map)
	{
		int highestBlock = 0;
		for(int i=map.getHeight()-1 ; i>0 && highestBlock == 0; i--)
			for(int j=0; j<map.getWidth() && highestBlock == 0; j++)
				if(map(j,i,DIM_WEIGHT_TAB)!=EMPTY)
					highestBlock = i;
		return highestBlock;
	}
	public:
	int putBlock(unsigned int x, unsigned int y,  Map & map, unsigned int who = PS_USER_BLOCK);
	void clearBlock(unsigned int x, unsigned int y, Map & map);	//Kasowanie bloku ze zwiekszeniem licznika rozbitych
	void clearBlock(ElementData * block);
	TYP findWeightPath(unsigned int x, unsigned int y, list<ElementData*> * path, Map & map, int x_last = 0);
	void subtractFromPath(list<ElementData*> * path, TYP subtrahend);
	list<ElementData*> * testWeight(Map & map);
	list<ElementData*> * testWind(Map & map, Wind & wind);
	int destroyElements(list<ElementData*>* toDestroy);

	Game(unsigned int mapWidth, unsigned int mapHeight) : _printer(mapWidth,mapHeight),  _map(mapWidth,mapHeight), _wind()
	{	
		_godTemper = GT_SMILE;
		_usedBlocks = _destroyedBlocks = 0;
		_map.clear();
		_printer.prepareFrames();
		_countedPyramid = countPyramid(_map);
		_wind.prepareWinds();
		MainLoop();
	}
	~Game()
	{	}
	void MainLoop()
	{
		int result = 0;
		while(result!=GAME_THE_END)							//Przerywamy petle jesli gra sie skonczyla
		{
			int waitForMessage = 0;							//Mnoznik opoznienia 
			_printer.print(cout);							//Wyswietlam interfejs
			result = askUserToDo(_map,waitForMessage);		//Pytanie do uzytkownika
			list<ElementData*> * toDestroy = NULL;			//Zapamietywanie sciezek do usuniecia
			int amountOfDestroyed = 0;
			if(result!= GAME_THE_END)
			{
				do{
					prepareWeights(_map);						//przeliczenie wag
					toDestroy = testWeight(_map);				//wykonanie testu obciazenia
					amountOfDestroyed = destroyElements(toDestroy);		//liczba usunietych elementow
					if(amountOfDestroyed>0)
					{
						_printer.printMessage(GOD_SAY_CAN_YOU_HEAR_BOOM,cout,&amountOfDestroyed);		
						_printer.makeBoomConsole();
						waitForMessage++;
					}
					_destroyedBlocks += amountOfDestroyed;		//dodajemy do puli
				}while(amountOfDestroyed>0);					//test wykonujemy az do skutku
				_highestBlock = countHighestBlock(_map);		//sprawdzamy najwyzej polozony blok (potrzebne dalej)	
				askWindToDo(_map,waitForMessage);				//Sprawdzamy czy wiatr powali budowle
				askGodToDo(_map,waitForMessage);				//Sprawdzamy co zrobi Bog
				delete toDestroy;
				do{	//Ponowny test wagi
					prepareWeights(_map);						//przeliczenie wag
					toDestroy = testWeight(_map);				//wykonanie testu obciazenia
					amountOfDestroyed = destroyElements(toDestroy);		//liczba usunietych elementow
					if(amountOfDestroyed>0)
					{
						_printer.printMessage(GOD_SAY_CAN_YOU_HEAR_BOOM,cout,&amountOfDestroyed);		
						_printer.makeBoomConsole();
						waitForMessage++;
					}
					_destroyedBlocks += amountOfDestroyed;		//dodajemy do puli
				}while(amountOfDestroyed>0);					//test wykonujemy az do skutku
				delay_ms(WAITING_TIME*waitForMessage);			//opozniamy czas wyswietlania komunikatow
				fillInterface();								//uzupelniamy w pamieci interfejsu wszelkie luki
				_printer.refreshConsole(cout);					//odswiezamy widok - konsole
			}
			else
			{	
				prepareWeights(_map);						//przeliczenie wag
				 testWeight(_map);
				fillInterface();								//uzupelniamy w pamieci interfejsu wszelkie luki
				_printer.refreshConsole(cout);
			}
		}
		endTheGame();
	}
	int askUserToDo(Map & map, int & wait);
	void askGodToDo(Map & map, int & wait);
	void askWindToDo(Map & map, int & wait);
	void prepareWeights(Map & map);
	void fillInterface()
	{	
		_godTemper = _god.getGodTemper(_highestBlock, _destroyedBlocks, _countedPyramid, _map);
		_printer.fillInterface(_map,_godTemper,_usedBlocks,_destroyedBlocks,_wind);
	}

	int countPkt()
	{	
		double aspect = (double)_map.getHeight()/(double)_map.getWidth();
		return (int)(aspect * (double)(((int)_countedPyramid*PKT_AMP_PYRAMID_BLOCKS - (int)_usedBlocks)*(int)PKT_PER_UNUSED_BLOCK - (int)PKT_PER_DESTROYED_BLOCK*(int)_destroyedBlocks));
	}
	void endTheGame()
	{
		int pktResult = countPkt();
		_printer.refreshConsole(cout);
		_printer.drawStatistics(pktResult,_destroyedBlocks,_usedBlocks,_countedPyramid);		//Same suche statystyki
		_printer.refreshConsole(cout);
		_printer.print(cout);
		cout << "Ta wieza dotknela nieba!\n";
		delay_ms(WAITING_TIME);
		int widthTXT [] = TXTS_WIDTH;
		int heightTXT [] = TXTS_HEIGHT;
		int leftTXT [] = TXTS_LEFT;
		int topTXT [] = TXTS_TOP;
		char * stringsTXT [] = TXTS_STRING;
		for(int i=0; i<TXTS_COUNT; i++)
		{
				_printer.drawWindow(widthTXT[i],heightTXT[i],topTXT[i],leftTXT[i],stringsTXT[i]);
				_printer.refreshConsole(cout);
				_printer.print(cout);
				delay_ms(2*WAITING_TIME);
		}
		_printer.refreshConsole(cout);
		_printer.prepareFrames();
		_printer.fillInterface(_map,GT_ANGRY,_usedBlocks,_destroyedBlocks,_wind);
		_printer.makeBoomConsole();
		_printer.print(cout);
		_printer.makeBoomConsole();
		_printer.makeChaos();
		_printer.makeMatrix();
		_printer.refreshConsole(cout);
		delay_ms(1000);
		_printer.showGodEnding();
	}
};


#endif