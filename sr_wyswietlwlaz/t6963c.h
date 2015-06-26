//-------------------------------------------------------------------------------------------------
// Graphic LCD with Toshiba T6963 controller
// Copyright (c) Rados³aw Kwiecieñ, 2007r
// http://en.radzio.dxp.pl/t6963/
// Compiler : avr-gcc
//-------------------------------------------------------------------------------------------------

#include <inttypes.h>

#ifndef T6963C_H_
#define T6963C_H_

#define OP_AND 0
#define OP_OR 1

void delay(void);
void UstawD(unsigned int wartosc);
void UstawDDDR(uint8_t wartosci);
void UstawK(uint8_t wartosci, uint8_t operation);
void UstawKDDR(uint8_t wartosci);
uint8_t ZczytajD(void);

void UstawWR(unsigned); // R/w
void UstawRD(unsigned); //
void UstawCE(unsigned);
void UstawCD(unsigned);
void UstawFS(unsigned);
void UstawRESET(unsigned);
void Inicjacja(void);
void UstawPixel(unsigned int x, unsigned int y, unsigned char kolor);


#endif //T6963C_H_
