/*
 * Callbacks_Globals.c
 *
 *  Created on: Jun 28, 2023
 *      Author: kamil
 */


/*
 * Callbacks.c
 *
 *  Created on: Jun 28, 2023
 *      Author: kamil
 */

#include "Callbacks_Globals.h"
#include "Definition.h"

#include "Computations.h"


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



#include <string.h>
#include <stdint.h>
#include <stdbool.h>



/* @brief: ADC Interrupt Service Routine.
 	 * 		Wait for ADC conversion complete interrupt,
	 * 		then:
	 * 			 compute the actual current value,
	 * 			 calculate the RMS value after an predetermined number of measurements,
	 * 			 toggle the pin referent to a new RMS conversion,
	 * 			 trigger the PDB timer.
 */
void ADC_IRQHandler(void)
{
	/* Toggle the pin PTD7 each time it enters this routine */
	gbCocoFlag = !gbCocoFlag;
	PINS_DRV_WritePin(ADC_COCO_IO,ADC_COCO_PIN, gbCocoFlag);

	/* Get channel result from ADC channel */
	ADC_DRV_GetChanResult(ADC_INSTANCE, 0U, (uint16_t *)&gu16CurrentRawValue);

	/* Update the counter of the number of conversions done */
	gu16NrConvDone++;

	/* Process the readings to get the value in Amperes */
	gfCurrentValue = CurrentADC(gu16CurrentRawValue);

	/* Compute the sum of the squared values of current needed to the RMS computation */
	RMSCurrentAux += gfCurrentValue*gfCurrentValue;

	/* After a predefined number of readings, compute the RMS */
	if (gu16NrConvDone == gu16RMSBuffer)
	{
		/* Toggle the pin to indicate the RMS computation was done */
		gbRMSCoCo = !gbRMSCoCo;
		PINS_DRV_WritePin(RMS_COCO_IO, RMS_COCO_PIN, gbRMSCoCo);

		/* Compute the current RMS value */
		gfRMSCurrent = (float) sqrt(RMSCurrentAux/gu16NrConvDone);

		/*Update the RMS index */
		gu16RMSIdx ++;

		/* Clear the squared sum u8I2CBufferHDI */
		RMSCurrentAux = 0;

		/* Clear the counter of the RMS */
		gu16NrConvDone = 0;
	}


	/* Set ADC conversion complete flag */
	gbADCConvDone = true;

}


void timing_pal_GAS_callBack(void * userData)
{
	(void)userData;

	/* Receive Data from Gasboard */
	LPUART_DRV_ReceiveData(INST_LPUART_GASBOARD, GAS_RX_buffer, GAS_RX_BUFFER_SIZE);

	/* Ignore bits without significant data and convert the others to float */
	for (uint8_t i = 0; i<GAS_SD_BUFFER_SIZE; i++)
	{
		fsigData[i] = (float)GAS_RX_buffer[i+4];
	}

	/* Compute the characteristics of the flow */
	gfO2concentration = (fsigData[0]*256 + fsigData[1])/10; //(Vol %)
	gfO2flow          = (fsigData[2]*256 + fsigData[3])/10; //(L/min)
	gfO2temperature   = (fsigData[4]*256 + fsigData[5])/10; //(C)

	/* Set Gasboard communication complete flag */
	gbGASDone = true;
	gbGASflag = !gbGASflag;
}

void timing_pal_SFM_callBack(void * userData)
{
	(void)userData;

	/* Receive Data from SFM */
	LPI2C_DRV_MasterReceiveData(INST_LPI2C1, SFM_RX_buffer,SFM_RX_BUFFER_SIZE, false);

	 /* Compute the flow from the received data */
	 gfFlowSMF = FlowSFM (SFM_RX_buffer);

	/* Set SFM communication complete flag */
	gbSFMDone = true;
	gbSFMflag = !gbSFMflag;
}



/* Callbacks not used, but kept to facilitate the expansion of the code */

void timing_pal_CO2_callBack(void * userData)
{
	(void)userData;

}


void timing_pal_MPR_callBack(void * userData)
{
	(void)userData;
}


