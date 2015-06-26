/* ###################################################################
**     Filename    : Events.h
**     Project     : wyswietlwlaz
**     Processor   : MK40DN512VLQ10
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : CodeWarrior ARM C Compiler
**     Date/Time   : 2015-06-07, 15:53, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
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

#ifdef __cplusplus
extern "C" {
#endif 

/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MK40DN512LQ10]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMIINT(void);


void CZUJEK_OnEnd(void);
/*
** ===================================================================
**     Event       :  CZUJEK_OnEnd (module Events)
**
**     Component   :  CZUJEK [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void CZUJEK_OnCalibrationEnd(void);
/*
** ===================================================================
**     Event       :  CZUJEK_OnCalibrationEnd (module Events)
**
**     Component   :  CZUJEK [ADC]
**     Description :
**         This event is called when the calibration has been finished.
**         User should check if the calibration pass or fail by
**         Calibration status method./nThis event is enabled only if
**         the <Interrupt service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void przycisk_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  przycisk_OnInterrupt (module Events)
**
**     Component   :  przycisk [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
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
