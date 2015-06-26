#include "interface.h"
#include "const.h"
#include "delayer.h"
#include "map.h"
#include "wind.h"

char * GT_TABS[3] = {GT_SMILE_TEXT,GT_TEMPERATE_TEXT,GT_ANGRY_TEXT};

char * convertIntToChar(unsigned int value, unsigned int base)
{

	unsigned int temp = value;
	unsigned int powV = 0;
	
	//Sprawdzenie, do ktorej potega
	do 
	{	temp/=base;
		powV++;
	}while(temp>0);
	char * result = new char[powV+1];
	result[powV] = '\0';
	for(unsigned int i=0; i<powV; i++) result[i] = 0;
	do
	{	result[powV-1] = value % base+'0';
		value/=base;
		powV--;
	}while(powV>0);
	return result;
}


void Interface::prepareNumLines()
	{
		_numVertic = new char[_h*2];
		_numHoriz = new char[_w*2];
		for(unsigned int i=0; i<_h; i++)
		{
			char * value = convertIntToChar(i,10);
			if(i<10)
			{	
				_numVertic[i*2] = ' ';
				_numVertic[i*2+1] = value[0];
			}
			else
			{
				_numVertic[i*2] = value[0];
				_numVertic[i*2+1] = value[1];
			}
			delete [] value;
		}
		for(unsigned int i=0; i<_w; i++)
		{
			char * value = convertIntToChar(i,10);
			if(i<10)
			{	
				_numHoriz[i] = ' ';
				_numHoriz[_w+i] = value[0];
			}
			else
			{
				_numHoriz[i] = value[0];
				_numHoriz[_w+i] = value[1];
			}
			delete [] value;
		}
	}

void Interface::prepareTitle()
	{	
		_title = new char[_w];
		unsigned int i=0;
		while(TITLE[i]!='\0')
		{	
			_title[i] = TITLE[i];
			i++;
		}
		while(i<_w)	_title[i++] = ' ';
	}

void Interface::Intoduce(ostream & stream)
{
	int dec = 0;
	char de2;
	stream << "Czy chcesz przeczytac zasady gry? \n(wpisz 1 i wcisnij ENTER jesli tak, wpisz 0 i wcisnij ENTER jesli nie)\n";
	cin >> dec;
	if(dec==1)
	{
		stream << "Copyright (C) 2014 Dorian Janiak\n\n";
		stream << "Witaj w grze Babel-Bubel!";
		delay_ms(WAITING_TIME);
		stream << "\n\nJesli jeszcze nie grales w ta gre to pomin ten tekst, idz powstawiaj kilka blokow i dopiero tutaj wroc, bo na sucho nic z niego nie wyciagniesz :) \n";
		delay_ms(WAITING_TIME);
		stream << "\nChcialbys graczu poznac zasady gry? Oto one:\n";
		delay_ms(WAITING_TIME);
		stream << "\t Musisz wybudowac wieze siegajaca do nieba (Babel). Twoja wieza musi przede wszystkim wytrzymac swoja mase oraz wiatr, ktorego intensywnosc jest" <<
			" odczuwalna w szczegolnosci gdy zaczynasz stawiac wysokie pietra. Jednak to nie wszystko! Pamietaj, ze tez jest jeszcze sam Stworca, ktory" <<
			" niekoniecznie podziela Twoja architektoniczna wizje i nie omieszka \"dorzucic\" czegos od siebie gdy tylko zblizysz sie do nieba. A wiec dobrze sie zastanow stawiajac kolejne kamienie.\n\n";
		stream << "\n\nJesli bedziesz gotow przejsc dalej wpisz dowolny znak i wcisnij ENTER\n";
		cin >> de2;
		refreshConsole(stream);
		stream << "\n\nObciazenia:\n";
		stream << " - kazdy element wazy i obciaza konstrukcje\n - waga kolejnych blokow musi zostac zamortyzowana przez bloki znajdujace sie ponizej\n" <<
			" - waga moze zostac zamortyzowana przez bloki, z ktorymi dany blok sie styka (po prawej, na ukos w dol na prawo, po lewej, na ukos w dol po lewej, blok ponizej)\n" <<
			" - opis kolejnych blokow:\n\t" << _bG << " - blok, ktory stawia Bog - wytrzymalosc: bardzo mala, waga: ciezki\n\t" << 
			_bN << ' ' << _bD2 << ' ' << _bD3 << ' ' << _bD4 << " - blok, ktory Ty stawiasz - wytrzymalosc: srednia, waga: srednia - im ciemniejszy tym mniej jest w stanie wytrzymac\n\t" <<
			_bB << " - most, ktory Ty stawiasz - wytrzymalosc: srednia, waga: bardzo lekki\n" << " - zwroc jednak uwage, ze w przypadku mostow i blokow Boga nie masz informacji na ile sa one obciazone\n";
		stream << "\n\nJesli bedziesz gotow przejsc dalej wpisz dowolny znak i wcisnij ENTER\n";
		cin >> de2;
		refreshConsole(stream);
		stream << "\n\nWiatr:\n";
		stream << " - w grze wystepuje wiatr, ktory wieje z prawej lub lewej strony z okreslona sila\n - Twoja budowla amortyzuje dzialanie wiatru swoja waga (za lekka sie zawali)\n" <<
			" - kierunek wiatru decyduje o tym, z ktorej strony sie zawali jesli nie wytrzyma jego sily\n";
		stream << "\n\nJesli bedziesz gotow przejsc dalej wpisz dowolny znak i wcisnij ENTER\n";
		cin >> de2;
		refreshConsole(stream);
		stream << "\n\nKonstruowanie budowli:\n";
		stream << " - bloki mozna stawiac na dowolnym innym bloku \n - mosty powstaja przez doczepienie bloku do innego bloku po jego prawej lub lewej stronie \n" <<
			" - jesli postawisz blok \"w powietrzu\" to spadnie on na blok znajdujacy sie pod nim i rozbija sie oba\n";
		stream << "\n\nJesli bedziesz gotow przejsc dalej wpisz dowolny znak i wcisnij ENTER\n";
		cin >> de2;
		refreshConsole(stream);
		stream << "\n\nOgolne zasady:\n";
		stream << " - przez cala gre stawiasz bloki\n - po kazdym postawionym bloku sprawdzana jest wytrzymalosc konstrukcji\n - nastepnie Bog moze zrzucic z nieba (pionowo w dol) swoj blok\n" <<
			" - wykonywany jest test wytrzymalosci wiezy na wiatr\n - jesli chcesz miec duzo punktow sprobuj wybrac bardziej wysoka mape, anizeli szeroka (zostanie to doliczone w finale)\n" <<
			" - wygrana stanowi dotkniecie nieba! Przegranej nie ma\n";
		stream << "\n\nI to wszystko, powodzenia ;) \n Wpisz dowolny znak i wcisnij ENTER\n";
		cin >> de2;
		refreshConsole(stream);
	}
}

Interface::Interface(unsigned int width, unsigned int height)  : _w(width), _h(height)
	{	
		_theEndGame = false;
		_error = false;
		_widSize = _w + WP_NUMVERTICAL_WIDTH + WP_WIND_TITLE_WIDTH + 2*WP_FRAME;	//Szerokosc calego interfejsu
		_heiSize = _h+ WP_TITLE_HEIGHT + WP_NUMHORIZONTAL_HEIGHT + 2*WP_FRAME;	//Wysokosc calego interfejsu
		if(_heiSize<WP_MIN_HEIGHT)	_heiSize = WP_MIN_HEIGHT; //Poniewaz bok okna potrzebuje wlasnie takiej wysokosci
		if(_widSize<WP_MIN_WIDTH) _widSize = WP_MIN_WIDTH; 
		//Sprawdzenie czy uda sie zmiescic okno
		if(_widSize <= CONSOLE_WIDTH)
		{
			_buff = new char[_widSize * _heiSize];
			if(_buff == NULL) _error = true;
		}
		else
			_error = true;
		prepareNumLines();
		prepareTitle();
		_beginT = clock();
	}

void Interface::clear()
	{	
		unsigned int size = _widSize * _heiSize;
		while(size>0) _buff[--size] = _iE;
	}

void Interface::prepareFrames()
	{
		clear();
		drawWindow(_w, WP_TITLE_HEIGHT , WP_TITLE_TOP , WP_TITLE_LEFT , _title); //Narysowanie tytulu
		drawWindow(WP_WIND_TITLE_WIDTH, WP_WIND_TITLE_HEIGHT, WP_WIND_TITLE_TOP, WP_WIND_TITLE_LEFT_ADD+_w, WIND_TEXT);	//Narysowanie tytulu : wiatr
		drawWindow(_w , _h , WP_MAP_TOP , WP_MAP_LEFT );	//Narysowanie tla do planszy
		drawWindow( WP_GOD_WIDTH , WP_GOD_HEIGHT , WP_GOD_TOP , WP_GOD_LEFT_ADD+_w ); //Miejsce na minke Boga
		drawWindow( WP_WIND_WIDTH , WP_WIND_HEIGHT , WP_WIND_TOP ,WP_WIND_LEFT_ADD + _w); //Miejsce na wiatr
		drawWindow( WP_TIME_WIDTH , WP_TIME_HEIGHT , WP_TIME_TOP ,WP_TIME_LEFT_ADD + _w); //Miejsce na czas
		drawWindow( WP_NUMVERTICAL_WIDTH , _h , WP_NUMVERTICAL_TOP , WP_NUMVERTICAL_LEFT ,_numVertic,true,true); //Linijka pionowa indeksow
		drawWindow(_w, WP_NUMHORIZONTAL_HEIGHT , WP_NUMHORIZONTAL_TOP_ADD + _h, WP_NUMHORIZONTAL_LEFT ,_numHoriz,true);	//Linijka pozioma indeksow
		drawWindow( WP_STATISTICS_WIDTH , WP_STATISTICS_HEIGHT , WP_STATISTICS_TOP , WP_STATISTICS_LEFT_ADD + _w); //Miejsce na statystyki (Uzytych / rozbite)
	}

int Interface::drawWindow(unsigned int width, unsigned int height, unsigned int top, unsigned int left, char * fill, bool onlyFillment , bool reverseVertical )
	{	
		height+=2;
		width+=2;
		if(top >= 0 && top+height-1 <_heiSize && left>=0 && left+width-1 < _widSize)  
		{
			//Narysowanie ramy okna
			if(!onlyFillment)		//Moze rowniez nie rysowac ramki (bo np. juz zostala raz narysowana)
			{	
				(*this)(left,top) = (*this)(left+width-1,top)= (*this)(left+width-1,top+height-1) = (*this)(left,top+height-1) =_iP;	//Same plusiki
				for(unsigned int i=left+1; i<left+width-1; i++)	(*this)(i,top) = (*this)(i,top+height-1) = _iW;							//Same poziome linie
				for(unsigned int i=top+1; i<top+height-1; i++) (*this)(left,i) = (*this)(left+width-1,i) = _iH;							//Same pionowe linie
			}	//Wypelnienie tablica
			if(fill != NULL)
			{
				if(!reverseVertical)	//Gdy nie ma obracac zawartosci tablicy
				{
					for(unsigned int i=top+1; i<top+height-1; i++)
						for(unsigned int j=left+1; j<left+width-1; j++)
							(*this)(j,i) = fill[(i-top-1)*(width-2)+j-left-1];
				}
				else					//Gdy ma obrocic zawartosc tablicy
				{
					for(unsigned int i=top+height-2; i>top; i--)
						for(unsigned int j=left+1; j<left+width-1; j++)
							(*this)(j,i) = fill[(height-2-(i-top))*(width-2)+j-left-1];
				}
			}
			else	//Kiedy nie wiadomo czym wypelnic - wypelnia znakami pustymi
			{
				for(unsigned int i=top+1; i<top+height-1; i++)
						for(unsigned int j=left+1; j<left+width-1; j++)
							(*this)(j,i) = _iE;
			}
			return 0;
		}
		else return -1;
	}

int Interface::drawTimeWindow()
{
	_currentT = clock();
	static int minutes = 0;
	int time = _currentT - _beginT;
	time/=1000;
	if(minutes == 0)
	{
		if(time>239)
		{
			minutes = 4;
			_beginT = clock();
		}
	}
	else
		if(time>59)
		{
			int addMin = (clock()-_beginT)/60000;
			int restMs = (clock()-_beginT)%60000;
			minutes+= addMin;
			_beginT = clock()-restMs;
		}
	if(minutes>0)
	{
		char * value = convertIntToChar(minutes,10);
		char text [WP_TIME_WIDTH * WP_TIME_HEIGHT + 1] = TIME_M_TEXT;
		int i=0;
		while(value[i]!='\0')
		{
			text[WP_TIME_WIDTH+i] = value[i];
			i++;
		}
		drawWindow(WP_TIME_WIDTH , WP_TIME_HEIGHT , WP_TIME_TOP ,WP_TIME_LEFT_ADD + _w,text,true);
	}
	else
	{
		char * value = convertIntToChar(time,10);
		char text [WP_TIME_WIDTH * WP_TIME_HEIGHT + 1] = TIME_S_TEXT;
		int i=0;
		while(value[i]!='\0')
		{
			text[WP_TIME_WIDTH+i] = value[i];
			i++;
		}
		drawWindow(WP_TIME_WIDTH , WP_TIME_HEIGHT , WP_TIME_TOP ,WP_TIME_LEFT_ADD + _w,text,true);
	}
	return 0;
}
char * Interface::convertMapToChar(Map & map)
{
	char * asciiMap = new char[map.getWidth() * map.getHeight()];
	for(unsigned int i=0; i<map.getHeight(); i++)
		for(unsigned int j=0; j<map.getWidth(); j++)
		{
			ElementData temp = map(j,i);
			char result = ' ';
			if(temp[DIM_WHO_TAB]==PS_USER_BRIDGE) result = _bB;		//Jesli uzytkownik postawil most
			else if(temp[DIM_WHO_TAB]!=PS_GOD_BLOCK)
			{
				if(temp[0]<=PS_FOURTH_DURABILITY) result = _bD4;		//Jesli jest najmniej wytrzymaly
				else if(temp[0]<=PS_THIRD_DURABILITY) result = _bD3;
				else if(temp[0]<=PS_SECOND_DURABILITY) result = _bD2;
				else if(temp[0]<EMPTY) result = _bN;					//Jesli jego wytrzymalosc jest calkowicie w porzadku
			}
			else if(temp[DIM_WHO_TAB]==PS_GOD_BLOCK) result = _bG;	//Jesli jest to jednak Bog

			asciiMap[i*map.getWidth()+j] = result;
		}
	return asciiMap;
}
void Interface::fillInterface(Map & map, unsigned int godTemper, unsigned int allBlocks, unsigned int destroyedBlocks, Wind & wind)
{	
	char * asciiMap = convertMapToChar(map);
	drawWindow(_w,_h,WP_MAP_TOP,WP_MAP_LEFT,asciiMap,true,true);	//Uzupelnienie samej mapy
	drawWindow(WP_GOD_WIDTH , WP_GOD_HEIGHT , WP_GOD_TOP , WP_GOD_LEFT_ADD + _w , GT_TABS[godTemper], true);		//Sam nastroj Pana Boga
	//Przygotowanie statystyk:
	char * statisticTab = new char[WP_STATISTICS_HEIGHT * WP_STATISTICS_WIDTH];
	{
		for(unsigned int i=0; i<WP_STATISTICS_HEIGHT * WP_STATISTICS_WIDTH; i++) statisticTab[i] = STATISTICS_TEXT[i];
		char * value = convertIntToChar(allBlocks,10);
		int i =0;
		while(value[i]!= '\0')
		{
			statisticTab[WP_STATISTICS_WIDTH+i] = value[i];
			i++;
		}
		delete [] value;
		value = convertIntToChar(destroyedBlocks,10);
		i=0;
		while(value[i]!='\0')
		{
			statisticTab[WP_STATISTICS_WIDTH*3+i] = value[i];
			i++;
		}
		delete [] value;
	}
	drawWindow(WP_STATISTICS_WIDTH , WP_STATISTICS_HEIGHT , WP_STATISTICS_TOP , WP_STATISTICS_LEFT_ADD + _w, statisticTab);
	delete [] asciiMap;
	//Przygotowanie wiatru
	char * windTab = new char[(WP_WIND_HEIGHT)*(WP_WIND_WIDTH)];
	for(int i=0; i<WP_WIND_HEIGHT; i++)
	{	
		char * value = convertIntToChar(wind._mem[i*2+WIND_FORCE_TAB],10);
		for(int j=0; j<WP_WIND_WIDTH && value[j]!='\0'; j++)
		{
			windTab[i*2+j+1] = value[j];
		}
		if(wind._mem[i*2+WIND_DIRECTION_TAB] == WIND_RIGHT_DIR) windTab[i*2] = _wR;
		else if(wind._mem[i*2+WIND_DIRECTION_TAB] == WIND_LEFT_DIR) windTab[i*2] = _wL;
		else
		{
			windTab[i*2] = ' ';
			windTab[i*2+1] = '0';
		}
		delete value;
	}
	drawWindow(WP_WIND_WIDTH,WP_WIND_HEIGHT,WP_WIND_TOP,WP_WIND_LEFT_ADD+map.getWidth(),windTab,true);
	delete [] windTab;
}

void Interface::print(ostream & stream)	//Wyswietlenie przygotowanego interfejsu
	{
		if(!_theEndGame)
			drawTimeWindow();
		for(unsigned int i = 0; i<_heiSize; i++)
		{
			for(unsigned int j=0; j<_widSize; j++)
				stream << (*this)(j,i);
			stream << endl;
		}		
	}

void Interface::printMessage(int messageId, ostream & stream, void * additionalData) //Wyswietlanie komunikatow
	{
		switch(messageId)
		{
		case USER_ASKED_TO_DO: stream << "\nGra->\tGdzie chcesz postawic blok? [wiersz kolumna]: "; break;
		case GOD_DECIDE_TO_DO: stream << "\nGra->\tNo to juz po rumakowaniu! Bog interweniuje"; break;
		case GOD_BLESS_YOU: stream << "\nBog->\tblogoslawie Cie marny czlowieku"; break;
		case USER_YOU_LOST: stream << "\nGra->\tStraciles kamieni: " << ((int*)additionalData)[0]; break;
		case GOD_SAY_OMG: stream << "\nBog->\tOMG! XD"; break;
		case GOD_SAY_CAN_YOU_HEAR_BOOM: stream << "\nBog->\tCzy slyszysz ten dzwiek? Twoja buda poszla z dymem XD !"; break;
		case GAME_BUSY_BLOCK: stream << "\nGra->\tTen blok jest juz zajety"; break;
		case GAME_IN_THE_AIR_BLOCK: stream << "Gra->\tNie kladz kamieni w powietrzu"; break;
		case GAME_THE_END: stream <<"Gra->\tGra skonczona"; break;
		case GAME_SAY_YOU_DESTROYED: stream << "\nGra->\tKamien spadl na kamien. Zniszczyles oba!"; break;
		case GAME_SAY_WIND_DESTROYED: stream << "\nGra->\tTwoja budowla nie wytrzymala wiatru!"; break;
		default: stream << "\nGra->\tBrak komunikatu!"; break;
		}
	}

void Interface::refreshConsole(ostream & stream)	//Odswiezenie widoku w konsoli
	{
#if PLATFORM == 1
		system("cls");
#else if PLATFORM == 0
		int i = LINUX_SPACE_BTW_2VIEWS;
		while(i-->0) stream << endl;
#endif
	}

void Interface::makeBoomConsole()
{
#if PLATFORM == 1
	system("color 4E");
	system("color 0E");
	system("color C7");
	//cout << "\a";
	delay_ms(50);
	system("color 47");
	system("color 07");
	system("color C7");
	//cout << "\a";
	//delay_ms(100);
	system("color 07");
#endif
}

int Interface::error() 
	{ 
		if(_error)
		{	
			if(_buff == NULL) return 1;
			if(_widSize > CONSOLE_WIDTH) return 2;
		}
		else return 0;
	}

void Interface::makeChaos()
	{
		clock_t pocz = clock();
		clock_t koniec = clock();
		srand(time(NULL));
		char * colorTab [] = COLOR_CHANGE_TXTS;
		while(koniec-pocz < TIME_FOR_MAP_CHAOS)
		{
			int i1 = (rand()%_w)+WP_MAP_LEFT+1;
			int i2 = (rand()%_w)+WP_MAP_LEFT+1;
			int j1 = (rand()%_h)+WP_MAP_TOP+1;
			int j2 = (rand()%_h)+WP_MAP_TOP+1;
			int color = (rand()%3);//COLOR_CHANGE_PROBALITY);
			TYP var = this->operator ()(i2,j2);
			this->operator ()(i2,j2) = this->operator ()(i1,j1);
			this->operator () (i1,j1) = var;
			refreshConsole(cout);
			print(cout);
			koniec = clock();
			system(colorTab[color]);
		}
		pocz = clock();
		while(koniec-pocz < TIME_FOR_INTERFACE_CHAOS)
		{
			int i1 = (rand()%_widSize);
			int i2 = (rand()%_widSize);
			int j1 = (rand()%_heiSize);
			int j2 = (rand()%_heiSize);
			int color = (rand()%3);//COLOR_CHANGE_PROBALITY);
			TYP var = this->operator ()(i2,j2);
			this->operator ()(i2,j2) = this->operator ()(i1,j1);
			this->operator () (i1,j1) = var;
			refreshConsole(cout);
			print(cout);
			koniec = clock();
			system(colorTab[color]);
		}
		pocz = clock();
		while(koniec-pocz < TIME_FOR_INTERFACE_CHAOS_FULL)
		{
			int i1 = (rand()%_widSize);
			int i2 = (rand()%_widSize);
			int j1 = (rand()%_heiSize);
			int j2 = (rand()%_heiSize);
			char l1 = (rand()%(LAST_CHAR_FOR_CHAOS - FIRST_CHAR_FOR_CHAOS)) + FIRST_CHAR_FOR_CHAOS;
			char l2 = (rand()%(LAST_CHAR_FOR_CHAOS - FIRST_CHAR_FOR_CHAOS)) + FIRST_CHAR_FOR_CHAOS;
			int color = (rand()%COLOR_CHANGE_PROBALITY);
			this->operator ()(i1,j1) = l1;
			this->operator ()(i2,j2) = l2;
			refreshConsole(cout);
			print(cout);
			koniec = clock();
			system(colorTab[color]);
		}

	}

void Interface::flowInterfaceForMatrix()
	{
		for(int i=0; i<_widSize; i++)
		{	
			char temp = this->operator ()(i,0);
			for(int j=0; j<_heiSize-1; j++)
				this->operator ()(i,j) = this->operator ()(i,j+1);
			this->operator ()(i,_heiSize-1) = temp;
		}		
	}

void Interface::makeMatrix()
	{
		clock_t pocz = clock();
		clock_t koniec = clock();
		srand(time(NULL));
		char * colorTab [] = COLOR_CHANGE_MATRIX_TXTS;
		while(koniec-pocz < TIME_FOR_MATRIX)
		{
			flowInterfaceForMatrix();
			int color = (rand()%COLOR_CHANGE_MATRIX_PROBALITY);
			refreshConsole(cout);
			print(cout);
			koniec = clock();
			system(colorTab[color]);
		}

	}

void Interface::showGodEnding()
	{
		_theEndGame = true;
		char lastWords[] = "     \xDB                  \xDB\xDB   \xDB  \xDB\xDB\xDB\xDB\xDB \xDB \xDB \xDB \xDB   \xDB\xDB  \xDB \xDB \xDB \xDB\xDB\xDB \xDB \xDB \xDB \xDB  \xDB\xDB\xDB   \xDB  \xDB\xDB\xDB\xDB    jest    WSZECHMOGACY zuchwalcze     !!!!    ";
		clear();
		drawWindow(12,11,0,0,lastWords);
		system("color 17");
		refreshConsole(cout);
		print(cout);
		delay_ms(WAITING_TIME);
		cout << "A autorem gry jest: Dorian Janiak :P \nMam nadzieje, ze sie podobalo\n";
	}