/*
 * Computations.c
 *
 *  Created on: Jun 5, 2023
 *      Author: kamil
 */

#include "Cpu.h"
#include "Computations.h"

 /* Parameters to compute the actual pressure based on the number of counts of the HDI sensor */
 /*Pressure range */
 const uint64_t Pmax = 2000; //mbar
 const uint64_t Pmin = 0;

 /* Digital range */
 const uint64_t Out_max_d = 29490;
 const uint64_t Out_min_d = 3277;


 /* Compute the current value based on the ADC data */
 float CurrentADC(uint16_t gu16CurrentRawValue)
 {
	 /* Polynomial parameters from data fit to convert the raw ADC value to the actual current in Amperes*/
	const float fCurrentPol[2] = {0.00169404924302870, -0.0138943629841409};
	//const float fVoltagePol[2] = {0.00880332914534114, -0.273383180824596};

	float fCurrentValue;
	fCurrentValue = ((float) gu16CurrentRawValue)*fCurrentPol[0] + fCurrentPol[1];

 	return fCurrentValue;
 }


uint64_t SensitivyHDI (void)
{

	 /* Sensitivity */
	 uint64_t sensitivity;
	 sensitivity = (Out_max_d - Out_min_d)/(Pmax-Pmin);

	 return sensitivity;
}


/* Compute the pressure based on the number of counts read */
uint64_t PressureHDI (uint8_t*  u8I2CBufferHDI, uint64_t sensitivity)
{
	/* Number of counts read by the sensor */
	uint16_t u16CountsHDI;
	uint16_t gu16PressureHDI;

	u16CountsHDI = (u8I2CBufferHDI[0]<<8)|u8I2CBufferHDI[1];
	gu16PressureHDI = (u16CountsHDI - Out_min_d)/sensitivity + Pmin;

	return gu16PressureHDI;
}

/* Compute the flow from the SFM sensor */
float FlowSFM (uint8_t*  u8I2CBufferSFM)
{
	/* Parameters to compute the actual flow based on the number of counts of the SFM sensor */
	const float SFLOW_SFM = 120;
	const float OFFSET_SFM= 32768;

	uint16_t u16CountsSMF;
	float fFlowSFM;

	u16CountsSMF = (u8I2CBufferSFM[0]<<8)|u8I2CBufferSFM[1];
	fFlowSFM = (((float) u16CountsSMF - OFFSET_SFM))/SFLOW_SFM;

	return fFlowSFM;

}


/* Compute the CO2 level from the CO2 sensor */
uint16_t ComputeCo2(uint8_t* CO2_RX_buffer)
{
	return (uint16_t) (CO2_RX_buffer[0]<<8)|CO2_RX_buffer[1];
}

/* Compute the TVOC level from the CO2 sensor */
uint16_t ComputeTVOC(uint8_t* CO2_RX_buffer)
{
	return (uint16_t) (CO2_RX_buffer[7]<<8)|CO2_RX_buffer[8];
}

/* Compute the pressure from the MPR sensor */
float ComputePressureMPR(uint8_t* MPR_RX_buffer)
{
	/* Combine the bytes */
	uint32_t countsPressMPR;
	//countsPressMPR = (MPR_RX_buffer[1]<<16)|(MPR_RX_buffer[2]<<8)|(MPR_RX_buffer[3]);
	countsPressMPR = (MPR_RX_buffer[1]<<16)|(MPR_RX_buffer[2]<<8)|(MPR_RX_buffer[3]);
	// countsPressMPR = MPR_RX_buffer[3] + MPR_RX_buffer[2]*256 + MPR_RX_buffer[1]*65536;

	 /* Transform the counts in percentage of the pressure */
	float fPressMPR_P100;
	fPressMPR_P100 = ((float)countsPressMPR / 16777215) * 100;

	/* Pressure reading [bar, psi, kPa, etc.] */
	float fPressureMPR;

	/* Values defined based on:
	 * Transfer Function A: 10% to 90% of 2^24 counts*/
	const float outputmax = 15099494; // output at maximum pressure [counts]
	const float outputmin = 1677722; // output at minimum pressure [counts]
	const float pmax = 2.5;          // maximum value of pressure range [bar, psi, kPa, etc.]
	const float pmin = 0;            // minimum value of pressure range [bar, psi, kPa, etc.]


	fPressureMPR = ((countsPressMPR - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin;

	return fPressureMPR;

}


/* Compute the temperature from the MPR sensor */
float ComputeTemperatureMPR(uint8_t* MPR_RX_buffer)
{

	/* Combine the bytes */
	uint32_t countsTempMPR;
	countsTempMPR = (MPR_RX_buffer[4]<<16)|(MPR_RX_buffer[5]<<8)|(MPR_RX_buffer[6]);

	 /* Transform the counts in actual temperature */
	float fTempMPR;
	fTempMPR = ((float)countsTempMPR * 200 / 16777215) - 50;

	return countsTempMPR;

}


