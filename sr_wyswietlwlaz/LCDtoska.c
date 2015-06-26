/*
 * LCDtoska.c
 *
 *  Created on: Jun 9, 2015
 *      Author: Dorian
 */

#include "LCDtoska.h"
#include "t6963c.h"





unsigned char buffer [(SZEROKOSC_EKRANU+2)*(WYSOKOSC_EKRANU+2)];




void rysujKwadrat(uint8_t bok, uint8_t x, uint8_t y,uint8_t zamalowany)
{
	uint8_t i=0;
	uint8_t j=0;
	if(zamalowany)
		for(i=x; i<x+bok; i++)
			for(j=y; j<y+bok; j++ )
				UstawPixel(i,j,1);
	else
	{
		for(i=x; i<=x+bok; i++)
		{
			UstawPixel(i,y,1);
			UstawPixel(i,y+bok,1);
		}
		for(j=y; j<=y+bok; j++)
		{
			UstawPixel(x,j,1);
			UstawPixel(x+bok,j,1);
		}
	}
}

void UstawPixelBuforowany(unsigned char * bufferL, uint16_t x, uint16_t y, uint16_t color)
{
	if(x < SZEROKOSC_EKRANU+1 && y < WYSOKOSC_EKRANU+1 && x > 0 && y>0)
		UstawPixel(x-1,y-1, (color != KOLOR_PUSTY) );																//Ustawienie piksela na ekraniczku
	bufferL[(SZEROKOSC_EKRANU+2)*y+x] = color;
}

void rysujBlok(unsigned char * bufferL, uint16_t x, uint16_t y)								//Wspó³rzêdne lewego górnego wierzcho³ka bloku
{
	uint16_t i=0,j=0;
	for(i=x; i<x+DLUGOSC_X_BLOKU; i++)
	{
		UstawPixelBuforowany(bufferL, (uint16_t)i, (uint16_t)y, KOLOR_BLOCZKI);
		UstawPixelBuforowany(bufferL, (uint16_t)i, (uint16_t)(y+WYSOKOSC_Y_BLOKU-1), KOLOR_BLOCZKI);
	}
	for(j=y; j<y+WYSOKOSC_Y_BLOKU; j++)
	{
		UstawPixelBuforowany(bufferL, (uint16_t)x,(uint16_t)j, KOLOR_BLOCZKI );
		UstawPixelBuforowany(bufferL, (uint16_t)(x+DLUGOSC_X_BLOKU-1), (uint16_t)j, KOLOR_BLOCZKI );
	}
}

void rysujPlanszeBlokow(unsigned char * bufferL)
{
	unsigned i=0,j=0;
	for(i=0; i<ILOSC_BLOKOW_W_X; i++)
		for(j=0; j<ILOSC_BLOKOW_W_Y; j++)
			rysujBlok(bufferL,i*(DLUGOSC_X_BLOKU + ODSTEP_MIEDZY_BLOKAMI) + 1, j*(WYSOKOSC_Y_BLOKU + ODSTEP_MIEDZY_BLOKAMI) + 1);
}

void rysujObramowaniePlanszy(unsigned char * bufferL)
{
	uint16_t i,j;
	for(i = 0; i < WYSOKOSC_EKRANU+2; i++)
	{
		UstawPixelBuforowany(bufferL,0,i,KOLOR_BOKI_I_GORA);
		UstawPixelBuforowany(bufferL,SZEROKOSC_EKRANU+1,i,KOLOR_BOKI_I_GORA);
	}
	for(j = 0; j < SZEROKOSC_EKRANU+2; j++)
	{
		UstawPixelBuforowany(bufferL,j,0,KOLOR_BOKI_I_GORA);
		UstawPixelBuforowany(bufferL,j,WYSOKOSC_EKRANU+1,KOLOR_DOL);
	}
}

unsigned char wezPixel(unsigned char * bufferL, uint16_t x, uint16_t y)
{
	return bufferL[y*(SZEROKOSC_EKRANU+2)+x];
}

unsigned char czyPixelJestBloku(unsigned char * bufferL, uint16_t x, uint16_t y)
{
	return (wezPixel(bufferL,x,y)==KOLOR_BLOCZKI);
}

unsigned char czyPixelJestPilki(unsigned char * bufferL, uint16_t x, uint16_t y)
{
	return (wezPixel(bufferL,x,y)==KOLOR_PILKA);
}

unsigned char czyPixelJestPaletki(unsigned char * bufferL, uint16_t x, uint16_t y)
{
	return (wezPixel(bufferL,x,y)==KOLOR_PALETKA);
}

unsigned char czyPixelJestPusty(unsigned char * bufferL, uint16_t x, uint16_t y)
{
	return (wezPixel(bufferL,x,y)==KOLOR_PUSTY);
}

void skasujBlok(unsigned char * bufferL, uint16_t x, uint16_t y)				//Jesli okaze sie, ze piksel nie nalezy do bloku to go nie skasuje
{
	unsigned char znalezionoZBloku = czyPixelJestBloku(bufferL, x, y);
	while( znalezionoZBloku )
	{
		UstawPixelBuforowany(bufferL, x, y, KOLOR_PUSTY);
		if( ( znalezionoZBloku = czyPixelJestBloku( bufferL, x+1, y ) ) )								//Piksel po prawo
			x++;
		else if( ( znalezionoZBloku = czyPixelJestBloku( bufferL, x, y-1 ) ) )							//Piksel nad
			y--;			
		else if( ( znalezionoZBloku = czyPixelJestBloku( bufferL, x-1, y ) ) )							//Piksel na lewo
			x--;
		else if( ( znalezionoZBloku = czyPixelJestBloku( bufferL, x, y+1 ) ) )							//Piksel pod
			y++;
	}
}

void skasujBlokiDol(unsigned char * bufferL, uint16_t xpcenter, uint16_t ypcenter)
{
	skasujBlok(bufferL, xpcenter-1, ypcenter+2);
	skasujBlok(bufferL, xpcenter, ypcenter+2);
	skasujBlok(bufferL, xpcenter+1, ypcenter+2);
}

void skasujBlokiGora(unsigned char * bufferL, uint16_t xpcenter, uint16_t ypcenter)
{
	skasujBlok(bufferL, xpcenter-1, ypcenter-2);
	skasujBlok(bufferL, xpcenter, ypcenter-2);
	skasujBlok(bufferL, xpcenter+1, ypcenter-2);
}

void skasujBlokiLewa(unsigned char * bufferL, uint16_t xpcenter, uint16_t ypcenter)
{
	skasujBlok(bufferL, xpcenter-2, ypcenter-1);
	skasujBlok(bufferL, xpcenter-2, ypcenter);
	skasujBlok(bufferL, xpcenter-2, ypcenter+1);
}

void skasujBlokiPrawa(unsigned char * bufferL, uint16_t xpcenter, uint16_t ypcenter)
{
	skasujBlok(bufferL, xpcenter+2, ypcenter-1);
	skasujBlok(bufferL, xpcenter+2, ypcenter);
	skasujBlok(bufferL, xpcenter+2, ypcenter+1);
}

void rysujPilke(unsigned char * bufferL, uint16_t xcenter, uint16_t ycenter)
{
	uint16_t i,j;
	if(xcenter-1 > 0 && xcenter+1 < (SZEROKOSC_EKRANU+1) && ycenter-1 > 0 && ycenter+1< (WYSOKOSC_EKRANU+1))
	{
		for(i=xcenter-1; i<=xcenter+1; i++)
			for(j=ycenter-1; j<=ycenter+1; j++)
				UstawPixelBuforowany(bufferL,i,j,KOLOR_PILKA);
	}
}

void skasujPilke(unsigned char *bufferL, uint16_t xcenter, uint16_t ycenter)
{
	uint16_t i,j;
	if(czyPixelJestPilki(bufferL,xcenter,ycenter) && czyPixelJestPilki(bufferL,xcenter-1,ycenter-1) && czyPixelJestPilki(bufferL,xcenter+1,ycenter+1))
	{
		for(i=xcenter-1; i<=xcenter+1; i++)
			for(j=ycenter-1; j<=ycenter+1; j++)
				UstawPixelBuforowany(bufferL,i,j,KOLOR_PUSTY);
	}
}

void rysujPaletke(unsigned char * bufferL, uint16_t xcenter)
{
	uint16_t i,j;
	if(xcenter > (SZEROKOSC_PALETKI/2) && xcenter+(SZEROKOSC_PALETKI/2) < (SZEROKOSC_EKRANU+2) )
	{
		for(j=WYSOKOSC_EKRANU+1 - WYSOKOSC_PALETKI; j<WYSOKOSC_EKRANU+2; j++)
			for(i=xcenter-(SZEROKOSC_PALETKI/2); i<xcenter+(SZEROKOSC_PALETKI/2); i++)
				UstawPixelBuforowany(bufferL,i,j,KOLOR_PALETKA);
	}
}


void przemiescPaletke(unsigned char * bufferL, uint16_t xcenter)
{
	uint16_t i,j;
	static uint16_t x = SZEROKOSC_EKRANU/2;
	if( xcenter > (SZEROKOSC_PALETKI/2) && xcenter+(SZEROKOSC_PALETKI/2) < (SZEROKOSC_EKRANU+1) )
	{
		for(j=WYSOKOSC_EKRANU+1 - WYSOKOSC_PALETKI; j<WYSOKOSC_EKRANU+2; j++)
			for(i=1; i<SZEROKOSC_EKRANU+1; i++)
				if(czyPixelJestPaletki(bufferL,i,j) && (i > (x+SZEROKOSC_PALETKI/2) || i < (x-SZEROKOSC_PALETKI/2)))
					UstawPixelBuforowany(bufferL,i,j,KOLOR_PUSTY);
		x = xcenter;
		for(j=WYSOKOSC_EKRANU+1 - WYSOKOSC_PALETKI; j<WYSOKOSC_EKRANU+2; j++)
			for(i=xcenter-(SZEROKOSC_PALETKI/2); i<xcenter+(SZEROKOSC_PALETKI/2); i++)
				if(wezPixel(bufferL,i,j)==KOLOR_PUSTY)
					UstawPixelBuforowany(bufferL,i,j,KOLOR_PALETKA);
	}
}

void zerujBufor(unsigned char * bufferL)
{
	uint16_t i,j;
	for(i=0; i<WYSOKOSC_EKRANU+2; i++)
		for(j=0; j<SZEROKOSC_EKRANU+2; j++)
			bufferL[i*(SZEROKOSC_EKRANU+2)+j] = KOLOR_PUSTY;
}

//Zwraca typ obiektu z jakim sie zdezyla pilka
unsigned char kolizjaZPrawej(unsigned char *bufferL, uint16_t xcenter, uint16_t ycenter)
{
	uint16_t x,j;
	x=xcenter+2;
	for(j=ycenter-1; j <= ycenter+1; j++)
		if(wezPixel(bufferL,x,j) != KOLOR_PUSTY)
			return wezPixel(bufferL,x,j);
	return KOLOR_PUSTY;
}

unsigned char kolizjaZLewej(unsigned char *bufferL, uint16_t xcenter, uint16_t ycenter)
{
	uint16_t x,j;
	x=xcenter-2;
	for(j=ycenter-1; j <= ycenter+1; j++)
		if(wezPixel(bufferL,x,j) != KOLOR_PUSTY)
			return wezPixel(bufferL,x,j);
	return KOLOR_PUSTY;
}

unsigned char kolizjaZGory(unsigned char *bufferL, uint16_t xcenter, uint16_t ycenter)
{
	uint16_t y,j;
	y=ycenter-2;
	for(j=xcenter-1; j <= xcenter+1; j++)
		if(wezPixel(bufferL,j,y) != KOLOR_PUSTY)
			return wezPixel(bufferL,j,y);
	return KOLOR_PUSTY;
}

unsigned char kolizjaZDolu(unsigned char *bufferL, uint16_t xcenter, uint16_t ycenter)
{
	uint16_t y,j;
	y=ycenter+2;
	for(j=xcenter-1; j <= xcenter+1; j++)
		if(wezPixel(bufferL,j,y) != KOLOR_PUSTY)
			return wezPixel(bufferL,j,y);
	return KOLOR_PUSTY;
}

void zmienRenderowanaPozycjePilki(unsigned char * bufferL, uint16_t xcenter, uint16_t ycenter)
{
	static uint16_t xc = 0, yc = 0;
	skasujPilke(bufferL,xc,yc);
	rysujPilke(bufferL,xcenter,ycenter);
	xc = xcenter;
	yc = ycenter;
}

void testuj2(unsigned char * bufferL)
{
	unsigned char kolP, kolL, kolG, kolD;
	uint16_t i,j;
	uint16_t xc=96, yc=50;
	uint16_t xcp = 37, ycp=63;
	int16_t vdol = 1, vprawa = 1;
	int16_t vpaletka = 1;
	zerujBufor(buffer);
	rysujObramowaniePlanszy(bufferL);
	rysujPlanszeBlokow(bufferL);
	
	while(1==1)
	{
		przemiescPaletke(bufferL,xcp);
		zmienRenderowanaPozycjePilki(bufferL, xc, yc);
		kolP = kolizjaZPrawej(bufferL,xc,yc);
		kolL = kolizjaZLewej(bufferL,xc,yc);
		kolG = kolizjaZGory(bufferL,xc,yc);
		kolD = kolizjaZDolu(bufferL,xc,yc);
		
		if(kolD != KOLOR_PUSTY)
		{
			vdol = -vdol;
			if(kolG == KOLOR_BLOCZKI)
				for(i=xc-1; i<xc+1; i++)
					skasujBlok(bufferL,i,yc+2);
		}
		else if(kolG != KOLOR_PUSTY)
		{
			vdol = -vdol;
			if(kolG == KOLOR_BLOCZKI)
				for(i=xc-1; i<xc+1; i++)
					skasujBlok(bufferL,i,yc-2);
		}
		
		if(kolP != KOLOR_PUSTY)
		{
			vprawa = -vprawa;
			if(kolP == KOLOR_BLOCZKI)
				for(i=yc-1; i<yc+1; i++)
					skasujBlok(bufferL,xc+2,i);
		}
		else if(kolL != KOLOR_PUSTY)
		{
			vprawa = -vprawa;
			if(kolL == KOLOR_BLOCZKI)
				for(i=yc-1; i<yc+1; i++)
					skasujBlok(bufferL,xc-2,i);
		}
		
		if(xcp == SZEROKOSC_PALETKI/2) vpaletka = -vpaletka;
		else if(xcp == (SZEROKOSC_EKRANU+2)-SZEROKOSC_PALETKI/2) vpaletka = -vpaletka;
		
		//skasujPaletke(bufferL,xcp,ycp);
		
		xc += vprawa;
		yc += vdol;
		xcp += vpaletka;
	}
}

void testuj(unsigned char * bufferL)
{
	uint16_t i,j; 
	zerujBufor(buffer);
	
	rysujPlanszeBlokow(bufferL);
	for(i = (WYSOKOSC_EKRANU+2); i>3; i--)
			for(j = (SZEROKOSC_EKRANU+2); j>0; j--)
				skasujBlok(bufferL,j,i);
		for(i = 6; i<(WYSOKOSC_EKRANU+2); i++)
		{
			rysujBlok(bufferL,16,i);
			for(j=0; j<1000; j++)
				delay();
			skasujBlok(bufferL,18,i);
		}
		rysujBlok(bufferL,16,40);
	for(j=0; j<20000; j++)
		delay();
	
	rysujPlanszeBlokow(bufferL);
	rysujPilke(bufferL,78,45);
	
	for(i=0; i<30; i++)
	{
		rysujBlok(bufferL,90,45);
		skasujBlok(bufferL,90,45);
	}

	skasujPilke(bufferL,78,45);				//Powinno skasowac pilke
	
	rysujPilke(bufferL,70,45);				
	for(i=44; i<=46;i++)							//Nie powinno skasowac pilki
		for(j=69; j<=71; j++)
			if(i!=45 && j!=70)
				skasujPilke(bufferL,j,i);
	for(i=0; i<20; i++)
	{
		rysujBlok(bufferL,90,45);
		for(j=0; j<1000; j++)
			delay();
		skasujBlok(bufferL,90,45);
	}
	skasujPilke(bufferL,70,45);
}

void arkanoid()
{
	testuj2(buffer);
	
}

unsigned char * getBuffer()
{
	return buffer;
}
