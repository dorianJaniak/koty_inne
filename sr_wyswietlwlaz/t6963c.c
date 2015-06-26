//-------------------------------------------------------------------------------------------------
// Graphic LCD with Toshiba T6963 controller
// Copyright (c) Radoslaw Kwiecien, 2007r
// http://en.radzio.dxp.pl/t6963/
// Compiler : avr-gcc
//-------------------------------------------------------------------------------------------------
#include "T6963C.h"
#include "Cpu.h"
#include "Events.h"
#include "DB0.h"
#include "BitIoLdd1.h"
#include "DB1.h"
#include "BitIoLdd2.h"
#include "DB2.h"
#include "BitIoLdd3.h"
#include "DB3.h"
#include "BitIoLdd4.h"
#include "DB4.h"
#include "BitIoLdd5.h"
#include "DB5.h"
#include "BitIoLdd6.h"
#include "DB6.h"
#include "BitIoLdd7.h"
#include "DB7.h"
#include "BitIoLdd8.h"
#include "FS.h"
#include "BitIoLdd9.h"
#include "WR.h"
#include "BitIoLdd10.h"
#include "RD.h"
#include "BitIoLdd11.h"
#include "CE.h"
#include "BitIoLdd12.h"
#include "CD.h"
#include "BitIoLdd13.h"
#include "RESET.h"
#include "BitIoLdd14.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

void delay(void) {
	volatile unsigned char i;
	for (i = 0; i < (20); i++) {
		asm("nop");
	}
}
void UstawWR(unsigned kalifornia) {
	WR_PutVal(kalifornia);
}
void UstawDB0(unsigned stan) {
	DB0_PutVal(stan);
}
void UstawDB1(unsigned stan) {
	DB1_PutVal(stan);
}
void UstawDB2(unsigned stan) {
	DB2_PutVal(stan);
}
void UstawDB3(unsigned stan) {
	DB3_PutVal(stan);
}
void UstawDB4(unsigned stan) {
	DB4_PutVal(stan);
}
void UstawDB5(unsigned stan) {
	DB5_PutVal(stan);
}
void UstawDB6(unsigned stan) {
	DB6_PutVal(stan);
}
void UstawDB7(unsigned stan) {
	DB7_PutVal(stan);
}
void UstawFS(unsigned stan) {
	FS_PutVal(stan);
}
void UstawRD(unsigned stan) {
	RD_PutVal(stan);
}
void UstawCE(unsigned stan) {
	CE_PutVal(stan);
}
void UstawCD(unsigned stan) {
	CD_PutVal(stan);
}
void UstawRESET(unsigned stan) {
	RESET_PutVal(stan);
}

void UstawDDDR(uint8_t wartosci)
{
	DB0_SetDir(wartosci & 0x01);
	DB1_SetDir(wartosci & 0x02);
	DB2_SetDir(wartosci & 0x04);
	DB3_SetDir(wartosci & 0x08);
	DB4_SetDir(wartosci & 0x10);
	DB5_SetDir(wartosci & 0x20);
	DB6_SetDir(wartosci & 0x40);
	DB7_SetDir(wartosci & 0x80);
}


void UstawD(unsigned int data){
//	UstawDDDR(0xFF);
	UstawDB0(data & 0x01);
	UstawDB1(data & 0x02);
	UstawDB2(data & 0x04);
	UstawDB3(data & 0x08);
	UstawDB4(data & 0x10);
	UstawDB5(data & 0x20);
	UstawDB6(data & 0x40);
	UstawDB7(data & 0x80);
}

uint8_t ZczytajD(){
	uint8_t result = 0;
//	UstawDDDR(0x00);
	result = DB0_GetVal();
	result |= DB1_GetVal() << 1;
	result |= DB2_GetVal() << 2;
	result |= DB3_GetVal() << 3;
	result |= DB4_GetVal() << 4;
	result |= DB5_GetVal() << 5;
	result |= DB6_GetVal() << 6;
	result |= DB7_GetVal() << 7;
//	UstawDDDR(0xFF);
	return result;
}
unsigned char SprawdzStatus() {

	unsigned char Dane;
	UstawDDDR(0x00);
	
	UstawRD(0);
	UstawCE(0);

    delay();

    Dane = ZczytajD();

	UstawDDDR(0xff);
	UstawRD(1);
	UstawCE(1);	

		
	
    return Dane;
}
void NapiszDane(unsigned int data) {

    while(!(SprawdzStatus() & 0x03))
        ;
    
    UstawD(data);
    delay();
    UstawWR(0);
	UstawRD(1);
	UstawCD(0);
	UstawCE(0);
	
    delay(); // Nie wiem czy dobrze

    UstawWR(1);
	UstawCD(1);
	UstawCE(1);
	
}

void NapiszKomende(unsigned int Komenda) {
    while(!SprawdzStatus() & 0x03)
        ;
    UstawD(Komenda);

    delay();
    UstawWR(0);
	UstawRD(1);
	UstawCE(0);
		
    delay(); // Nie wiem czy dobrze

    UstawWR(1);
	UstawCE(1);
	
}
void UstawAdresWskaznika(unsigned int adres){
	NapiszDane(adres & 0xff);
	NapiszDane(adres >> 8);
	NapiszKomende(0x24); //Wybranie adresu wskaznika.

}
void NapiszDaneWyswietlania(unsigned int Data){
	
	NapiszDane(Data);
	NapiszKomende(0xC0); //Pisanie danych  

}


unsigned char CzytajDane(void) {
	uint8_t tmp;
	while (!(SprawdzStatus() & 0x03))
		;
	UstawDDDR(0x00);

	UstawRD(0);
	UstawCD(0);
	UstawCE(0);
	delay();
	tmp = ZczytajD();
	UstawRD(1);
	UstawCD(1);
	UstawCE(1);
	UstawDDDR(0xFF);
	return tmp;
}

void WyczyscTekst()
{
	unsigned i =0;
	UstawAdresWskaznika(0x00);
	for(i=0; i<30*128/8; i++)		
		NapiszDaneWyswietlania(0);
	
}


void WyczyscGrafike()
{
	unsigned i =0;
	UstawAdresWskaznika(16*8);
	for(i=0; i<64*16; i++)
		NapiszDaneWyswietlania(0);
}


void NapiszZnak(char charCode) {
	NapiszDaneWyswietlania(charCode - 32);
}


void NapiszTekst(char * string) {
	while (*string) {
		NapiszZnak(*string++);
	}
}



void PiszW(unsigned char x, unsigned char y) {
	unsigned int adres;

	adres = x + (16 * y);

	UstawAdresWskaznika(adres);
}


void UstawPixel(unsigned int x, unsigned int y, unsigned char kolor){

	unsigned char pom;
	unsigned int adres = 128 +x/8 + 16*y;
 	
	UstawAdresWskaznika(adres);

	NapiszKomende(0xc5); //Wybranie czytania.
	pom = CzytajDane();

	if(kolor)
		pom |= (1<< (8 -1 - (x % 8)));
	else
		pom &= ~(1<< (8 -1 - (x % 8)));

	NapiszDaneWyswietlania(pom);
}


void Inicjacja(void) {
	int i = 0;
	UstawWR(1);
	UstawRD(1);
	UstawCD(1);
	UstawRESET(1);
	UstawFS(1);
	UstawCE(1);
	UstawRESET(0);
	
	for (i = 0; i < 10; i++)
		delay();
	UstawRESET(1);

	UstawFS(0);
    NapiszDane(128 & 0xff);	//mlodsze bajty
    NapiszDane(128 >> 8);	//starsze bajty
    NapiszKomende(0x42);	//Adres pocz¹tkowy grafiki
    
    NapiszDane(0x00);	//mlodsze bajty
    NapiszDane(0x00);	//starsze bajty
    NapiszKomende(0x40);	//Adres pocz¹tkowy tekstu

    //Ustawienie obszaru grafiki.
    NapiszDane(16);
    NapiszDane(0);
    NapiszKomende(0x43);
    
    //Ustawienie obszaru tekstowego.
    NapiszDane(16);
    NapiszDane(0x00);
    NapiszKomende(0x41);
    

	NapiszKomende(0x9c);

	NapiszKomende(0x80);
	WyczyscGrafike();
	WyczyscTekst();
}
