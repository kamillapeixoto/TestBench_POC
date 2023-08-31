/*
 * Init.h
 *
 *  Created on: Jun 2, 2023
 *      Author: kamil
 */
#include "Cpu.h"

#ifndef SOURCES_INIT_H_
#define SOURCES_INIT_H_

/* Initialize Variables */
void Init_var(void);


/* Initialize pins */
void Init_pins(void);

/* Initialize ADC peripheral*/
IRQn_Type Init_ADC(void);

/* Initialize PBD peripheral*/
void Init_PDB(void);

/* Initialize LPIT peripheral*/
void Init_LPIT(void);

/* Initialize LPI2C peripheral*/
void Init_LPI2C(void);

/* Initialize Timing PAL peripheral */
void Init_PAL(void);

/* Initialize Gasboard related peripherals */
void Init_GAS(void);

/* Initialize SFM related peripheral */
void Init_SFM(void);

/* Initialize CO2 related peripheral */
void Init_CO2(void);

/* Initialize MPR related peripheral */
void Init_MPR(void);

/* Initialize Automation timer */
void Init_AUTO(void);

#endif /* SOURCES_INIT_H_ */
