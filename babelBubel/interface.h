#include <ctime>
#include <iostream>
#include <string>
#include "delayer.h"
#include "const.h"


class Map;
class Wind;
using namespace std;



#define CONSOLE_WIDTH 80	//Ile znakow zmiesci sie w oknie konsoli

#ifndef INTERFACE_H
#define INTERFACE_H

class Interface{
	char * _buff;	//To co ma zostac wyswietlone
	char * _numVertic; //Pionowe indeksy
	char * _numHoriz; //Poziome indeksy
	char * _title;		//tytul calego okna
	unsigned int _w; //Szerokosc pola gry
	unsigned int _h; //Wysokosc pola gry
	unsigned int _widSize;
	unsigned int _heiSize;
	bool _theEndGame;
	clock_t _beginT;
	clock_t _currentT;
	bool _error;		//Gdy wystapi blad
		static const char _iE = ' ';	//Bloki kreslace interfejs
		static const char _iH = '|';
		static const char _iW = '-';
		static const char _iP = '+';
		static const char _bN = '\xDB'; //219 - blok normalny
		static const char _bD2 = '\xB2'; // damaged - blok uszkodzony
		static const char _bD3 = '\xB1'; // jeszcze bardziej uszkodzony
		static const char _bD4 = '\xB0'; // strasznie uszkodzony
		static const char _bG = '\x9E'; // siano od Boga
		static const char _bB = '\xDF'; // most od uzytkownika (lipa taka, ze nie mozna zobaczyc rozkladu masy)
		static const char _wR = WIND_RIGHT_CHAR;
		static const char _wL = WIND_LEFT_CHAR;
	void prepareNumLines();
	void prepareTitle();
public:
	static void Intoduce(ostream & strumien);
	Interface(unsigned int width, unsigned int height);
	void clear();
	void prepareFrames();
	int drawWindow(unsigned int width, unsigned int height, unsigned int top, unsigned int left, char * fill = NULL, bool onlyFillment = false, bool reverseVertical = false);
	int drawTimeWindow();			//Uzupelnia ramke z czasem
	char * convertMapToChar(Map & map);
	void fillInterface(Map & map, unsigned int godTemper, unsigned int allBlocks, unsigned int destroyedBlocks, Wind & wind);	//Wypelnianie wiekszosci interfejsu
	void print(ostream & stream);	//Wyswietlenie przygotowanego interfejsu
	void printMessage(int messageId, ostream & stream, void * additionalData = NULL); //Wyswietlanie komunikatow
	static void refreshConsole(ostream & stream);	//Odswiezenie widoku w konsoli
	void makeBoomConsole();
	char & operator()(unsigned int col, unsigned int row)
	{
		return _buff[row*_widSize + col];
	}
	//Obsluga bledow interfejsu
	int error();
	void clearError()
	{
		_error = false;
	}
	void drawStatistics(int pktResult,unsigned int destroyedBlocks, unsigned int usedBlocks, unsigned int countedPyramid)
	{	
		cout << "Gratulacje! Udalo Ci sie zbudowac wieze Babel\n";
		cout << "\n stosunek wysokosci mapy do szerokosci: " << (double)_h/(double)_w;
		cout << "\n + punkty podstawowe: + " << countedPyramid*PKT_AMP_PYRAMID_BLOCKS << " x " << PKT_PER_UNUSED_BLOCK << " = " << countedPyramid*PKT_AMP_PYRAMID_BLOCKS * PKT_PER_UNUSED_BLOCK;
		cout << "\n - kara za uzyte bloki: - " << usedBlocks << " x " << PKT_PER_UNUSED_BLOCK << " = " << usedBlocks*PKT_PER_UNUSED_BLOCK;
		cout << "\n - kara za rozbite bloki: - " << destroyedBlocks << " x " << PKT_PER_DESTROYED_BLOCK << " = " << destroyedBlocks*PKT_PER_DESTROYED_BLOCK;
		cout << "\n\t SUMA PUNKTOW (przemnozona przez stosunek rozmiaru mapy): " << pktResult;
		
#if PLATFORM == 1
		cout << "\n\n Jesli chcesz zobaczyc dalsza czesc wynikow wpisz jakikolwiek znak \ni wcisnij ENTER";
#elif PLATFORM == 0
		cout << "\n\n To juz koniec, niestety, ale gdybys korzystal z Windowsa otrzymalbys cos jeszcze ;)\n";
#endif
		char c;
		cin >> c;
	}
	void makeChaos();
	void flowInterfaceForMatrix();
	void makeMatrix();
	void showGodEnding();
};


#endif