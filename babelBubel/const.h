#include <float.h>

//Platforma, na ktorej uruchomiony zostanie program
#ifndef PLATFORM
#define PLATFORM 1	
// 0 - Linux, 1 - Windows
#endif

//Potrzebne do odœwie¿ania ekranu (tylko dla Linuxa)
#ifndef LINUX_SPACE_BTW_2VIEWS
#define LINUX_SPACE_BTW_2VIEWS 80
#endif

//Ogólny typ zmiennych
#ifndef TYP
	#define TYP double
#endif
#ifndef DIM_ALL_TABS
#define DIM_ALL_TABS
	#define DIMENSION 3
#define DIM_WEIGHT_TAB 0
#define DIM_WHO_TAB 1
#define DIM_LEFT_TO_DO_TAB 2
#endif

#ifndef EMPTY
#define EMPTY DBL_MAX
#endif

#ifndef TEST
	#define TEST 1
// 0 - wylaczone, 1 - wlaczone
#endif

#ifndef STRINGS
#define STRINGS
	#define TITLE "\"Babel-bubel\"\0"
	#define WIND_TEXT "Wiatr: "
	#define TIME_S_TEXT "Czas   s"
	#define TIME_M_TEXT "Czas   m"
	#define STATISTICS_TEXT "Uzytych       Rozbite       "
#endif

#ifndef WINDOWS_POSITIONS
#define WINDOWS_POSITIONS
	#define WP_FRAME					2
	#define WP_NUMVERTICAL_LEFT			0
	#define WP_NUMVERTICAL_WIDTH		2
	#define WP_TITLE_LEFT				WP_NUMVERTICAL_LEFT + WP_NUMVERTICAL_WIDTH + WP_FRAME - 1
	#define WP_MAP_LEFT					WP_TITLE_LEFT
	#define WP_NUMHORIZONTAL_LEFT		WP_TITLE_LEFT
	#define WP_WIND_TITLE_LEFT_ADD		WP_TITLE_LEFT+1 
	#define WP_WIND_TITLE_WIDTH			7
	#define WP_GOD_LEFT_ADD				WP_TITLE_LEFT+1
	#define WP_GOD_WIDTH				4
	#define WP_TIME_LEFT_ADD			WP_TITLE_LEFT+1
	#define WP_TIME_WIDTH				WP_GOD_WIDTH
	#define WP_STATISTICS_LEFT_ADD		WP_TITLE_LEFT+1
	#define WP_STATISTICS_WIDTH			WP_WIND_TITLE_WIDTH
	#define WP_WIND_LEFT_ADD			WP_TITLE_LEFT + WP_GOD_WIDTH + WP_FRAME
	#define WP_WIND_WIDTH				WP_WIND_TITLE_WIDTH - WP_GOD_WIDTH - 1

	#define WP_TITLE_TOP				0
	#define WP_TITLE_HEIGHT				1
	#define WP_WIND_TITLE_TOP			WP_TITLE_TOP
	#define WP_WIND_TITLE_HEIGHT		WP_TITLE_HEIGHT
	#define WP_MAP_TOP					WP_TITLE_TOP + WP_TITLE_HEIGHT + WP_FRAME -1
	#define WP_NUMVERTICAL_TOP			WP_MAP_TOP
	#define WP_GOD_TOP					WP_MAP_TOP
	#define WP_WIND_TOP					WP_MAP_TOP
	#define WP_GOD_HEIGHT				4
	#define WP_TIME_TOP					WP_GOD_HEIGHT + WP_GOD_TOP + 1
	#define WP_TIME_HEIGHT				2
	#define WP_STATISTICS_TOP			WP_TIME_TOP + WP_TIME_HEIGHT + 1
	#define WP_STATISTICS_HEIGHT		4
	#define WP_WIND_HEIGHT				WP_GOD_HEIGHT + WP_TIME_HEIGHT + 1
	#define WP_NUMHORIZONTAL_HEIGHT		2
	#define WP_NUMHORIZONTAL_TOP_ADD	WP_MAP_TOP + 1

	#define WP_MIN_WIDTH	WP_TITLE_LEFT + 4 + WP_WIND_TITLE_WIDTH + WP_FRAME
	#define WP_MIN_HEIGHT	WP_STATISTICS_TOP + WP_STATISTICS_HEIGHT + WP_FRAME
#endif

#ifndef MESSAGES_NUMBERS
#define MESSAGES_NUMBERS
	#define WAITING_TIME 2000
	#define USER_ASKED_TO_DO 0 //prosba o to aby uzytkownik wykonal ruch
	#define USER_YOU_LOST 1 //ile stracil uzytkownik kamieni
	#define GOD_DECIDE_TO_DO 2 //Bog podjal decyzje o ruchu
	#define GOD_BLESS_YOU 3 //Bog odpuszcza sobie ruch
	#define GOD_SAY_OMG 4 //Bog mowi OMG
	#define GOD_SAY_CAN_YOU_HEAR_BOOM 5 //Bog mowi: Czy slyszysz ten dzwiek? To Twoja idea wlasnie legla w gruzach
	#define GAME_BUSY_BLOCK 6 //Gra mowi, ze blok jest zajety
	#define GAME_IN_THE_AIR_BLOCK 7 //Polozony blok w powietrzu
	#define GAME_THE_END 8 //koniec gry
	#define GAME_SAY_YOU_DESTROYED 9 //zniszczyles sobie blok
	#define GAME_SAY_WIND_DESTROYED 10	//gra mowi ze wiatr zniszczyl blko
#endif

#ifndef POINTING_SYSTEM
#define POINTING_SYSTEM
//Jak bardzo wytrzymale sa dane bloki i ile waza
	#define PS_FIRST_DURABILITY	5
	#define PS_DIF_BTW_DURABILITY 1
	#define PS_SECOND_DURABILITY 2
	#define PS_THIRD_DURABILITY 1
	#define PS_FOURTH_DURABILITY 0
	#define PS_WEIGHT_USER_BLOCK 2
	#define PS_WEIGHT_GOD_BLOCK 3
	#define PS_WEIGHT_USER_BRIDGE 0.5
	#define PS_FIRST_DURABILITY_GOD_BLOCK 2		//wczesniej bylo 1
	#define PS_FIRST_DURABILITY_BRIDGE 5		//wczesniej bylo 3
	#define PS_GOD_BLOCK 3
	#define PS_USER_BLOCK 2
	#define PS_USER_BRIDGE 4
#endif

#ifndef GOD_TEMPER
#define GOD_TEMPER
	#define GT_SMILE 0
	#define GT_TEMPERATE 1
	#define GT_ANGRY 2
	#define GT_SMILE_TEXT "Bog:0 0  w  \\_/ "
	#define GT_TEMPERATE_TEXT "Bog:0 0  w  /-\\ "
	#define GT_ANGRY_TEXT "Bog:\\ /  w  xxx "
	#define GT_TEMPERATE_THRESH 0.3
	#define GT_ANGRY_THRESH 0.6
	#define GT_FULL_THRESH 1.0
	#define GT_DIVISOR_THRESH 2.0		//Dzieli wzmocnienie w przypadku wyliczania mozliwosci reakcji Boga
#endif

#ifndef WIND_SET
#define WIND_SET
	#define	WIND_RIGHT_DIR 1
	#define WIND_LEFT_DIR -1
	#define WIND_RIGHT_CHAR '>'
	#define WIND_LEFT_CHAR '<'
	#define WIND_NO_WIND 0
	#define WIND_MAX 6			//Nie moze byc wieksze od 9
	#define WIND_MIN 1
	#define WIND_DIRECTION_TAB 0
	#define WIND_FORCE_TAB 1

#endif

#ifndef ENDGAME
#define ENDGAME
#define PKT_AMP_PYRAMID_BLOCKS 5
#define PKT_PER_UNUSED_BLOCK 4
#define PKT_PER_DESTROYED_BLOCK 8
#define TXTS_COUNT 3
#define TXTS_WIDTH {9,10,8}
#define TXTS_HEIGHT {1,2,2}
#define TXTS_STRING {{"zuchwalec"} , {"Twoj Babelto BUBEL!!"}, {"zniszcze  GO! "}}
#define TXTS_LEFT {4,2,6}
#define TXTS_TOP {3,10,1}
#define TIME_FOR_MAP_CHAOS 6000
#define TIME_FOR_INTERFACE_CHAOS 6000
#define TIME_FOR_INTERFACE_CHAOS_FULL 12000
#define COLOR_CHANGE_PROBALITY 6
#define COLOR_CHANGE_TXTS {{"color 07"},{"color 0F"}, {"color 97"}, {"color C7"}, {"color 4E"}, {"color 07"}}
#define FIRST_CHAR_FOR_CHAOS '\xB0'
#define LAST_CHAR_FOR_CHAOS '\xDF'
#define TIME_FOR_MATRIX 8000
#define COLOR_CHANGE_MATRIX_PROBALITY 4
#define COLOR_CHANGE_MATRIX_TXTS {{"color 02"}, {"color 0A"}, {"color 07"}, {"color 2A"}} 
#endif