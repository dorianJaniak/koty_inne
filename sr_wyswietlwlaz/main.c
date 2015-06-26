/* ###################################################################
**     Filename    : main.c
**     Project     : wyswietlwlaz
**     Processor   : MK40DN512VLQ10
**     Version     : Driver 01.01
**     Compiler    : CodeWarrior ARM C Compiler
**     Date/Time   : 2015-06-07, 15:53, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
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
#include "CZUJEK.h"
#include "AdcLdd1.h"
#include "przycisk.h"
#include "ExtIntLdd1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "t6963c.h"
//#include "graphic.h"
#include "LCDtoska.h"
#include "Logika.h"
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
  unsigned int x,y;
  int i=0, j=0;
  
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
  Inicjacja();
//  GLCD_ClearGraphic();
 // rysujKwadrat(10,0,0,0);
 // PiszW(0,0);
 // NapiszTekst("oscyloskop");
  PORTC_PCR10 |= PORT_PCR_PE_MASK;
  PORTC_PCR10 |= PORT_PCR_PS_MASK;
  for(;;)
  {
	  Arkanoid(getBuffer());
  }
/*  while(1)
  {
	  
  }*
  /*for(x=0; x<50; x++)
	  for(y=0; y<50; y++)
	  {
		  UstawPixel(x,y,1);
	  }*/
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
