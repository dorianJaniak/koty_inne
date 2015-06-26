#include <ctime>

void delay_ms(int ms_time)
{
	clock_t pocz = clock();
	clock_t koniec;
	do{
	koniec = clock();
	}while(koniec-pocz<ms_time);
}