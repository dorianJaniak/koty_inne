/*
 * LCDtoska.h
 *
 *  Created on: Jun 9, 2015
 *      Author: Dorian
 */
#include <inttypes.h>




#ifndef LCDTOSKA_H_
#define LCDTOSKA_H_

#define SZEROKOSC_EKRANU		128
#define WYSOKOSC_EKRANU			64
#define DLUGOSC_X_BLOKU 		7
#define WYSOKOSC_Y_BLOKU		3
#define ODSTEP_MIEDZY_BLOKAMI	2
#define ILOSC_BLOKOW_W_X		(SZEROKOSC_EKRANU/(DLUGOSC_X_BLOKU+ODSTEP_MIEDZY_BLOKAMI))
#define ILOSC_BLOKOW_W_Y		5

#define BOK_PILKI				3			//paradoks

#define SZEROKOSC_PALETKI		19
#define WYSOKOSC_PALETKI		2

#define KOLOR_PUSTY						0
#define KOLOR_BOKI_I_GORA	1
#define KOLOR_DOL			2
#define KOLOR_BLOCZKI					3
#define KOLOR_PILKA						4
#define KOLOR_PALETKA					5








void rysujKwadrat(uint8_t bok, uint8_t x, uint8_t y, uint8_t zamalowany);

void UstawPixelBuforowany(unsigned char * bufferL, uint16_t x, uint16_t y, uint16_t color);
void rysujBlok(unsigned char * bufferL, uint16_t x, uint16_t y);
void rysujPlanszeBlokow(unsigned char * bufferL);
void rysujObramowaniePlanszy(unsigned char * bufferL);
unsigned char wezPixel(unsigned char * bufferL, uint16_t x, uint16_t y);
unsigned char czyPixelJestBloku(unsigned char * bufferL, uint16_t x, uint16_t y);
unsigned char czyPixelJestPilki(unsigned char * bufferL, uint16_t x, uint16_t y);
unsigned char czyPixelJestPaletki(unsigned char * bufferL, uint16_t x, uint16_t y);
unsigned char czyPixelJestPusty(unsigned char * bufferL, uint16_t x, uint16_t y);
void skasujBlok(unsigned char * bufferL, uint16_t x, uint16_t y);
void skasujBlokiDol(unsigned char * bufferL, uint16_t xpcenter, uint16_t ypcenter);
void skasujBlokiGora(unsigned char * bufferL, uint16_t xpcenter, uint16_t ypcenter);
void skasujBlokiLewa(unsigned char * bufferL, uint16_t xpcenter, uint16_t ypcenter);
void skasujBlokiPrawa(unsigned char * bufferL, uint16_t xpcenter, uint16_t ypcenter);
void rysujPilke(unsigned char * bufferL, uint16_t xcenter, uint16_t ycenter);
void skasujPilke(unsigned char *bufferL, uint16_t xcenter, uint16_t ycenter);
void rysujPaletke(unsigned char * bufferL, uint16_t xcenter);
void przemiescPaletke(unsigned char * bufferL, uint16_t xcenter);
void zerujBufor(unsigned char * bufferL);
unsigned char kolizjaZPrawej(unsigned char *bufferL, uint16_t xcenter, uint16_t ycenter);
unsigned char kolizjaZLewej(unsigned char *bufferL, uint16_t xcenter, uint16_t ycenter);
unsigned char kolizjaZGory(unsigned char *bufferL, uint16_t xcenter, uint16_t ycenter);
unsigned char kolizjaZDolu(unsigned char *bufferL, uint16_t xcenter, uint16_t ycenter);
void zmienRenderowanaPozycjePilki(unsigned char * bufferL, uint16_t xcenter, uint16_t ycenter);
void testuj2(unsigned char * bufferL);
void testuj(unsigned char * bufferL);
void arkanoid();
unsigned char * getBuffer();



#endif /* LCDTOSKA_H_ */
