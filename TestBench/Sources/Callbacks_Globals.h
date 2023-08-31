/*
 * Callbacks_Globals.h
 *
 *  Created on: Jun 28, 2023
 *      Author: kamil
 */

#ifndef SOURCES_CALLBACKS_GLOBALS_H_
#define SOURCES_CALLBACKS_GLOBALS_H_

/*
 * Callbacks.h
 *
 *  Created on: Jun 28, 2023
 *      Author: kamil
 */

#ifndef SOURCES_CALLBACKS_H_
#define SOURCES_CALLBACKS_H_


#include "Cpu.h"

#include "Definition.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>



/* -----------------------  ADC  -----------------------  */

/* Variable to store the raw value from ADC conversion. Global to be modified inside the interrupt */
volatile uint16_t gu16CurrentRawValue;

/* Variable to store the converted current value */
volatile float gfCurrentValue;

/* Variable to store the current RMS value */
volatile float gfRMSCurrent;


/* Variable to count the number of conversions done and included on the RMS calculation */
volatile uint16_t gu16NrConvDone;

/* Variable to count and index the RMS calculation that is used to verify the
 * reliability of the data in FreeMaster */
uint16_t gu16RMSIdx;

/* Number total of conversions used to compute one RMS value. Global to allow the change in FreeMaster */
uint16_t gu16RMSBuffer;

/* Sum of the squared values of the current */
float RMSCurrentAux;

/* Flag used to store if an ADC IRQ was executed */
volatile bool gbADCConvDone;

/* Flag used to toggle the pin when the program enters the ADC interrupt routine */
volatile bool gbCocoFlag;

/* Flag used to toggle the pin when the program finishes the RMS calculation */
volatile bool gbRMSCoCo;




/* ---------------------  Gasboard  ---------------------  */

/* Values measured by the Gasboard 7500H sensor through UART communication. */

/* O2 Concentration in % */
float gfO2concentration;

/* O2 Flow in Liters per minute */
float gfO2flow;

/* O2 Temperature in Celsius */
float gfO2temperature;

/* Declare buffer used to store the received data */
uint8_t	GAS_RX_buffer[GAS_RX_BUFFER_SIZE];

/* Stores only the significant bytes of the read data */
float fsigData[GAS_SD_BUFFER_SIZE];

/* Flag used to store if an Gasboard ISR was executed*/
volatile bool gbGASDone;
volatile bool gbGASflag;


/* -----------------------  SFM  -----------------------  */

/* Declare buffer used to store the received data */
uint8_t	SFM_RX_buffer[SFM_RX_BUFFER_SIZE];

/* Flow measured from the SFM sensor */
float gfFlowSMF;

/* Declaration of the flags to check the callbacks are executed */
volatile bool gbSFMDone;
volatile bool gbSFMflag;



/* Allocate memory for the LPI2C driver state structure */
lpi2c_master_state_t lpi2c1MasterState;


void ADC_IRQHandler(void);

void timing_pal_GAS_callBack(void * userData);

void timing_pal_SFM_callBack(void * userData);



/* Callbacks not used, but kept to facilitate the expansion of the code */

void timing_pal_CO2_callBack(void * userData);

void timing_pal_MPR_callBack(void * userData);



#endif /* SOURCES_CALLBACKS_H_ */





#endif /* SOURCES_CALLBACKS_GLOBALS_H_ */
