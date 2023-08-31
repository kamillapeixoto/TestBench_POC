
/*
 * This program reads an ADC channel that is connected to the current signal of the power supply,
 * and convert it to the actual current measurement.
 * The ADC readings are (pre) triggered using the PDB (Programmable Delay Block).
 */

/* MODULE main */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "pin_mux.h"
#include "adConv1.h"
#include "lpuart_gasboard.h"
#include "dmaController1.h"
#include "pdly1.h"
#include "clockMan1.h"
#include "freemaster.h"
#include "lpi2c1.h"
#include "lpuart_gasboard.h"
#include "LPSPI_MPR.h"


#include "Definition.h"
#include "Init.h"
#include "Computations.h"
#include "Callbacks_Globals.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "automationdefinit.h"
#include "automationfunctions.h"




volatile int exit_code = 0;

/* User includes (#include below this line is not
 *  maintained by Processor Expert) */


#include <string.h>
#include <stdint.h>
#include <stdbool.h>



/*!
 \brief The main function for the project.
 \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
 */
int main(void)
{



	/* Write your local variable definition here */
	Init_varAuto();

	Init_var();

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
	PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
#endif
	/*** End of Processor Expert internal initialization.                    ***/

	/* Initialize and configure clocks
	 * 	-	see clock manager component for details
	 */
	CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
			g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
	CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);


	/* Initialize pins */
	Init_pins();



	/*Initialize ADC Instance*/
	IRQn_Type adcIRQ = Init_ADC();

	/* The interrupt trigger is the ADC Coco flag, thus it enters the IRQ when the conversion is done*/
	INT_SYS_InstallHandler(adcIRQ, &ADC_IRQHandler, (isr_t*) 0);

	/* Enable ADC 1 interrupt */
	INT_SYS_EnableIRQ(adcIRQ);

	/*Initialize PDB Instance*/
	Init_PDB();

	/* Initialize Timing PAL peripheral */
	Init_PAL();

	/* Initialize Automation timer */
	Init_AUTO();

	/*Initialize Gasboard LPUART and Timing PAL Instances*/
	Init_GAS();

	/* Initialize SFM related peripheral */
	Init_SFM();

    /* Trigger PDB counter to perform the ADC conversions */
	PDB_DRV_SoftTriggerCmd(PDB_INSTANCE);


	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.1 [05.21]
 **     for the Freescale S32K series of microcontrollers.
 **
 ** ###################################################################
 */
