/*
 * Init.c
 *
 *  Created on: Jun 2, 2023
 *      Author: kamil
 */

#include "Init.h"

#include "Cpu.h"
#include "pin_mux.h"
//#include "osif1.h"
#include "adConv1.h"
#include "lpuart_gasboard.h"
#include "dmaController1.h"
#include "pdly1.h"
#include "clockMan1.h"
#include "freemaster.h"
//#include "lpit1.h"
#include "lpi2c1.h"
#include "timing_pal1.h"
#include "LPSPI_MPR.h"

#include "Definition.h"
#include "Callbacks_Globals.h"
#include "automationdefinit.h"


uint64_t u64LPITResolution;

/* Read O2 concentrations on the default mode (extended mode = 0% to 100%, default mode = 20.5% to 95.6% ) */
uint8_t	 GAS_TX_buffer[GAS_TX_BUFFER_SIZE] = {0x11, 0x01, 0x01, 0xED};
// Obs: This must be declared as global, otherwise it changes its value during the cycles



/* Initialize Variables */
void Init_var(void)
{

	/* Variable to count the number of conversions done and included on the RMS calculation */
	gu16NrConvDone = 0U;

	/* Variable to count and index the RMS calculation that is used to verify the
	 * reliability of the data in FreeMaster */
	gu16RMSIdx = 0U;

	/* Number total of conversions used to compute one RMS value. Global to allow the change in FreeMaster */
	gu16RMSBuffer = RMS_BUFFER_SIZE;

	/* Sum of the squared values of the current */
	RMSCurrentAux = 0;

	/* Flag used to store if an ADC IRQ was executed */
	gbADCConvDone = false;

	/* Flag used to toggle the pin when the program enters the ADC interrupt routine */
	gbCocoFlag = false;

	/* Flag used to toggle the pin when the program finishes the RMS calculation */
	gbRMSCoCo = false;

	/* Clear all the flags */
		gbSFMDone = false;
		gbSFMflag = false;
		gbGASDone = false;
		gbGASflag = false;



}


/* Initialize pins */
void Init_pins(void)
{
	PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
	/* Set the pins to be toggled as output */
	PINS_DRV_SetPinDirection(ADC_COCO_IO, ADC_COCO_PIN, OUTPUT_DIR);
	PINS_DRV_SetPinDirection(RMS_COCO_IO, RMS_COCO_PIN, OUTPUT_DIR);

}


/* Initialize ADC peripheral*/
IRQn_Type Init_ADC(void)
{
		adConv1_ChnConfig0.channel = ADC_CHN;

		ADC_DRV_ConfigConverter(ADC_INSTANCE, &adConv1_ConvConfig0);
		ADC_DRV_ConfigChan(ADC_INSTANCE, 0UL, &adConv1_ChnConfig0);

		IRQn_Type adcIRQ;

		switch(ADC_INSTANCE)
		{
		case 0UL:
			adcIRQ = ADC0_IRQn;
			break;
		case 1UL:
			adcIRQ = ADC1_IRQn;
			break;
		default:
			adcIRQ = ADC1_IRQn;
			break;
		}

		return adcIRQ;
}


/* Initialize PBD peripheral*/
void Init_PDB(void)
{

	int32_t u32PreTriggerDelay = 466U;
	int32_t u32ModulusValue = u32PreTriggerDelay + 1U;


	PDB_DRV_Init(PDB_INSTANCE, &pdly1_InitConfig0);
	PDB_DRV_ConfigAdcPreTrigger(PDB_INSTANCE, ADC_CHN, &pdly1_AdcTrigInitConfig0);
	PDB_DRV_SetTimerModulusValue(PDB_INSTANCE, u32ModulusValue);
	PDB_DRV_SetAdcPreTriggerDelayValue(PDB_INSTANCE, ADC_CHN, 0U, u32PreTriggerDelay);

	PDB_DRV_LoadValuesCmd(PDB_INSTANCE);

}


/* Initialize Timing PAL peripheral */
void Init_PAL(void)
{
	/* Initialize TIMING over LPIT */
	TIMING_Init(INST_TIMING_PAL1, &timing_pal1_InitConfig);
	/* Get tick resolution in nanosecond unit for TIMING over LPIT */
	TIMING_GetResolution(INST_TIMING_PAL1, TIMER_RESOLUTION_TYPE_NANOSECOND, &u64LPITResolution);
}


/* Initialize Gasboard related peripheral */
void Init_GAS(void)
{
	 /*Initialize Gasboard UART Instance*/
	LPUART_DRV_Init(INST_LPUART_GASBOARD, &lpuart_gasboard_State, &lpuart_gasboard_InitConfig0);

	/* Read O2 concentrations on the DEFAULT mode (extended mode = 0% to 100%, default mode = 20.5% to 95.6% ) */
	LPUART_DRV_SendData(INST_LPUART_GASBOARD,GAS_TX_buffer,GAS_TX_BUFFER_SIZE);

	/* Start LPIT channel 0 counting with the period is 0.5 second,
	   the period in tick = the period in nanosecond / LPIT tick resolution in nanosecond */
	TIMING_StartChannel(INST_TIMING_PAL1, LPIT_CHANNEL_GAS, GAS_PERIOD_BY_NS/u64LPITResolution);


}

/* Initialize SFM related peripheral */
void Init_SFM(void)
{


	/* Initialize LPI2C Master configuration */
	LPI2C_DRV_MasterInit(INST_LPI2C1, &lpi2c1_MasterConfig0, &lpi2c1MasterState);  // This cannot go inside of
											// the Init.c function because the communication does not initialize there.


	/* Request Data from the SFM sensor */
	uint8_t u8_TX_SFM [SFM_TX_BUFFER_SIZE] = {0x10, 0x00};
	LPI2C_DRV_MasterSendData(INST_LPI2C1, u8_TX_SFM,SFM_TX_BUFFER_SIZE, true);

    /* Start LPIT channel 0 counting with the period is 0.5 second,
    	   the period in tick = the period in nanosecond / LPIT tick resolution in nanosecond */
    TIMING_StartChannel(INST_TIMING_PAL1, LPIT_CHANNEL_SFM, SFM_PERIOD_BY_NS/u64LPITResolution);

}

/* Initialize CO2 related peripheral */
void Init_CO2(void)
{



    /* Start LPIT channel 2 counting with the period is 1 second,
    	   the period in tick = the period in nanosecond / LPIT tick resolution in nanosecond */
 //   TIMING_StartChannel(INST_TIMING_PAL1, LPIT_CHANNEL_CO2, CO2_PERIOD_BY_NS/u64LPITResolution);

}

/* Initialize MPR related peripheral */
void Init_MPR(void)
{

	   /* Start SPI Communication */



	    	/* Delays between the clock and the chip enable */
	    	LPSPI_DRV_MasterSetDelay(LPSPI_MPR, SPI_DELAY, SPI_DELAY, SPI_DELAY);


    /* Start LPIT channel 3 counting with the period is 1 second,
    	   the period in tick = the period in nanosecond / LPIT tick resolution in nanosecond */
    TIMING_StartChannel(INST_TIMING_PAL1, LPIT_CHANNEL_MPR, MPR_PERIOD_BY_NS/u64LPITResolution);

}

/* Initialize Automation timer */
void Init_AUTO(void)
{

	PINS_DRV_SetPinDirection(GPIO_PORT1,ONOFF,OUTPUT_DIR);
	PINS_DRV_SetPinDirection(GPIO_PORT1,PLUS,OUTPUT_DIR);
	PINS_DRV_SetPinDirection(GPIO_PORT1,MINUS,OUTPUT_DIR);
	PINS_DRV_SetPinDirection(GPIO_PORT1,BELL,OUTPUT_DIR);



	LPUART_DRV_Init(INST_LPUART1, &lpuart1_State, &lpuart1_InitConfig0);

    /* Start LPIT channel 2 counting with the period is 1 second,
    	   the period in tick = the period in nanosecond / LPIT tick resolution in nanosecond */
    TIMING_StartChannel(INST_TIMING_PAL1, LPIT_CHANNEL_AUTO, AUTO_PERIOD_BY_NS/u64LPITResolution);

}


