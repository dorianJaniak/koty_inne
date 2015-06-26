#include <math.h>

//#include "Logika.h"
#include "LCDtoska.h"

#define true 1
#define false 0

extern volatile int Start ;

extern unsigned int PilkaX;
extern unsigned int PilkaY;

extern unsigned int PilkaPredX;
extern unsigned int PilkaPredY;

//extern unsigned char* buffer;

/*
	Funkcja czekajaca na znak startu, tj.
	naci
*/
void CzekajNaRozpoczecie(){
	Start=false;
	while(!Start)
		__asm__ volatile ("nop");
	
}

void InicjujGre(unsigned int PilkaX,unsigned int PilkaY,
				unsigned int PaletkaX,unsigned int PaletkaY,
				unsigned char* Lbuffer){
	WyczyscGrafike();
	WyczyscTekst();
	zerujBufor(Lbuffer);
	rysujPlanszeBlokow(Lbuffer);
	rysujObramowaniePlanszy(Lbuffer);
	rysujPilke(Lbuffer,PilkaX,PilkaY);
	przemiescPaletke(Lbuffer,PaletkaX);
}

/*
	Funkcja zmieniajaca parametry paletki 
*/
void ZmienParametryPaletki(signed int* X,signed int* Y,
							unsigned char* Lbuffer){
	uint16_t value;
	float odlegloscCM = 10;
	value = 10;
	value += 2;
	CZUJEK_Measure(true);
	
	CZUJEK_GetValue16(&value);
	if(value>45700)
		odlegloscCM=10;
	else if(value<24600)
		odlegloscCM=20;
	else{
		odlegloscCM = 20 - (((float)(value-24600)*10)/((float)(45700-24600)));
	}
	;
	;
	;
	
	odlegloscCM -= 10;
	*X =(int) (128*odlegloscCM)/10;
	
}
/*
	Funkcja nadajaca nowa prędkość piłki, tj programują
	reakcję piłki na uderzenia oraz zmieniają pozostały
	wektor prędkości dekrementując kolejne wartości Pred_X i Pred_Y
	a także jej położenie
*/
void ZmienParametryPilki(signed int* X,signed int* Y,
		signed int* PredX,signed int* PredY,
		signed int* AktPredX,	signed int* AktPredY,
		signed int PaletkaX,  signed int* IloscBlokow,
		unsigned char* Lbuffer ){

		unsigned char KolizjaLewa = kolizjaZLewej(Lbuffer,*X,*Y), 
					  KolizjaPrawa = kolizjaZPrawej(Lbuffer,*X,*Y),
					  KolizjaGora = kolizjaZGory(Lbuffer,*X,*Y), 
					  KolizjaDol = kolizjaZDolu(Lbuffer,*X,*Y);


		if((*AktPredX) == 0 && (*AktPredY) == 0){
			(*AktPredX) = (*PredX);
			(*AktPredY) = (*PredY);
		}
			
		if(abs(*AktPredX)>=abs(*AktPredY)){
			//Pilka porusza sie w prawo
			if((*AktPredX)>0){
				switch(KolizjaPrawa){
					case KOLOR_PUSTY:
						(*AktPredX)--;
						(*X)++;		
						break;
					case KOLOR_BOKI_I_GORA:
						(*AktPredX) = -(*AktPredX);
						(*PredX) = -(*PredX);
						break;
					case KOLOR_BLOCZKI:
						(*AktPredX) = -(*AktPredX);
						(*PredX) = -(*PredX);
						skasujBlokiPrawa(Lbuffer,(*X),(*Y));
						(*IloscBlokow)--;
						break;
				}
			}
		  	else{
			//Pilka porusza sie w lewo
				switch(KolizjaLewa){
					case KOLOR_PUSTY:
						(*AktPredX)++;
						(*X)--;		
						break;
					case KOLOR_BOKI_I_GORA:
						(*AktPredX) = -(*AktPredX);
						(*PredX) = -(*PredX);
						break;
					case KOLOR_BLOCZKI:
						(*AktPredX) = -(*AktPredX);
						(*PredX) = -(*PredX);
						skasujBlokiLewa(Lbuffer,(*X),(*Y));
						(*IloscBlokow)--;
						break;
				}
			}						
		}
		if(abs(*AktPredY)>=abs(*AktPredX)){
			// ruch w dol
			if(*AktPredY>0){
				switch(KolizjaDol){
					case KOLOR_PUSTY:
						(*AktPredY)--;
						(*Y)++;
						break;
					case KOLOR_BLOCZKI:
						(*AktPredY) = -(*AktPredY);
						(*PredY) = -(*PredY);
						skasujBlokiDol(Lbuffer,(*X),(*Y));
						(*IloscBlokow)--;
						break;							
					case KOLOR_PALETKA:
						//Rozpatrywanie stref paletki			
						if((*AktPredX)>0){
							if((PaletkaX - (*X))>4){
								(*PredX)=0;
								(*PredY)=-3;
								(*AktPredX)=(*PredX);
								(*AktPredY)=(*PredY);
							}
							else if((PaletkaX - (*X))>2){
								(*PredX)=1;
								(*PredY)=-2;
								(*AktPredX)=(*PredX);
								(*AktPredY)=(*PredY);
							} 
							else if((PaletkaX - (*X))<-2){
								(*PredX)=2;
								(*PredY)=-1;
								(*AktPredX)=(*PredX);
								(*AktPredY)=(*PredY);
							} 
							else{
								(*AktPredY) = -(*AktPredY);
								(*PredY)=-(*PredY);	
								(*AktPredX)=(*PredX);
								(*AktPredY)=(*PredY);
							}
						}
						else
							if((PaletkaX - (*X))<-4){
								(*PredX)=0;
								(*PredY)=-3;
								(*AktPredX)=(*PredX);
									(*AktPredY)=(*PredY);
							}
							else if((PaletkaX - (*X))<-2){
								(*PredX)=-1;
								(*PredY)=-2;
								(*AktPredX)=(*PredX);
									(*AktPredY)=(*PredY);
							} 
							else if((PaletkaX - (*X))>2){
								(*PredX)=-2;
								(*PredY)=-1;
								(*AktPredX)=(*PredX);
									(*AktPredY)=(*PredY);
							} 
							else{
								(*PredY)=-(*PredY);
								(*AktPredY)=(*PredY);
							}						
						break;
					case KOLOR_DOL:
						break; //Sprawdzenie przegranej w funkcji 
						// RozegrajGre		
				}

			}
			//Ruch w gore
			else{
				switch(KolizjaGora){
					case KOLOR_PUSTY:
						(*AktPredY)++;
						(*Y)--;
						break;
					case KOLOR_BOKI_I_GORA:
						(*PredY) = -(*PredY);
						(*AktPredY) = - (*AktPredY);
						break;
					case KOLOR_BLOCZKI:
						(*PredY) = -(*PredY);
						(*AktPredY) = - (*AktPredY);
						skasujBlokiGora(Lbuffer,(*X),(*Y));
						(*IloscBlokow)--;
						break;	
				}
			}
		}
			
}


void WyswietlEkranWygranej(){
	
	PiszW(4,4);
	NapiszTekst("Wygrana");
}

void WyswietlEkranPrzegranej(){
	PiszW(4,4);
	NapiszTekst("Luuuzer");
}
void Arkanoid(unsigned char * Lbuffer){
	unsigned int opoznienie;
	signed int IloscBlokow = ILOSC_BLOKOW_W_X*ILOSC_BLOKOW_W_Y,
				 PolozeniePilkiX = 64,	
				 PolozeniePilkiY = 35,
				 PolozeniePaletkiX = 65,
				 PolozeniePaletkiY = 63;
	int KoniecGry = false;
	signed int PredkoscPilkiX = 0,
			   PredkoscPilkiY = 3,
			   AktPredkoscPilkiX = 0,
			   AktPredkoscPilkiY = 3;


	InicjujGre(PolozeniePilkiX,PolozeniePilkiY,
			   PolozeniePaletkiX, PolozeniePaletkiY,Lbuffer);

	CzekajNaRozpoczecie();

	while(!KoniecGry){
		skasujPilke(Lbuffer,PolozeniePilkiX,PolozeniePilkiY);	
		
		ZmienParametryPaletki(&PolozeniePaletkiX,&PolozeniePaletkiY,Lbuffer);

		ZmienParametryPilki(&PolozeniePilkiX,&PolozeniePilkiY,
		&PredkoscPilkiX,&PredkoscPilkiY,&AktPredkoscPilkiX,
		&AktPredkoscPilkiY,PolozeniePaletkiX,&IloscBlokow,
		Lbuffer);
	
		rysujPilke(Lbuffer,PolozeniePilkiX,PolozeniePilkiY);
		przemiescPaletke(Lbuffer,PolozeniePaletkiX);
		
		for(opoznienie = 500; opoznienie >0; opoznienie--)
			delay();
		if(IloscBlokow<=0 || PolozeniePilkiY>=63)
			KoniecGry=true;
	}		
	if(IloscBlokow<=0)
		WyswietlEkranWygranej();
	else
		WyswietlEkranPrzegranej();
	for(opoznienie = 50000; opoznienie >0; opoznienie--)
			delay();
}



