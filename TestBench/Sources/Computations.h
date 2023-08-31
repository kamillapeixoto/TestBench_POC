/*
 * Computations.h
 *
 *  Created on: Jun 5, 2023
 *      Author: kamil
 */

#ifndef SOURCES_COMPUTATIONS_H_
#define SOURCES_COMPUTATIONS_H_

#include "Cpu.h"

/* Compute the current value based on the ADC data */
float CurrentADC(uint16_t gu16CurrentRawValue);

/* Compute the Sensitivity of the HDI sensor */
uint64_t SensitivyHDI (void);

/* Compute the pressure from the HDI sensor */
uint64_t PressureHDI (uint8_t*  u8I2CBufferHDI, uint64_t sensitivity);

/* Compute the flow from the SFM sensor */
float FlowSFM (uint8_t*  u8I2CBufferSFM);

/* Compute the CO2 level from the CO2 sensor */
uint16_t ComputeCo2(uint8_t* CO2_RX_buffer);

/* Compute the TVOC level from the CO2 sensor */
uint16_t ComputeTVOC(uint8_t* CO2_RX_buffer);

/* Compute the pressure from the MPR sensor */
float ComputePressureMPR(uint8_t* MPR_RX_buffer);

/* Compute the temperature from the MPR sensor */
float ComputeTemperatureMPR(uint8_t* MPR_RX_buffer);

#endif /* SOURCES_COMPUTATIONS_H_ */
