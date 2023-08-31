/*
 * Definition.h
 *
 *  Created on: Jun 15, 2023
 *      Author: kamil
 */

#ifndef SOURCES_DEFINITION_H_
#define SOURCES_DEFINITION_H_



/* ADC Readings */
#define ADC_INSTANCE 	0UL
#define ADC_CHN			0U /* PTA0 */
#define ADC_VREFH  		3.3f
#define ADC_VREFL  		0.0f
#define PDB_INSTANCE	0UL
#define ADC_COCO_IO		PTC
#define ADC_COCO_PIN	3
#define RMS_COCO_IO		PTC
#define RMS_COCO_PIN	14
#define RMS_BUFFER_SIZE 500// 0.2 sec 20kHz



/* I2C Communication */
#define I2C_BUFFER_SIZE 2U
#define I2C_TIMEOUT 1U

/* Gasboard LPUART Communication */
#define GAS_TX_BUFFER_SIZE  4U
#define GAS_RX_BUFFER_SIZE 12U
#define GAS_SD_BUFFER_SIZE 8U   /* Significant data */

/* I2C Communication for SFM */
#define SFM_RX_BUFFER_SIZE 2U
#define SFM_TX_BUFFER_SIZE 2U
#define SFM_ADDRESS        0x40

/* I2C Communication for CO2 */
#define CO2_RX_BUFFER_SIZE 9U
#define CO2_TX_BUFFER_SIZE 1U
#define CO2_ADDRESS        0x5A

/* LPIT channel used */
#define LPIT_CHANNEL_GAS	    0UL
#define LPIT_CHANNEL_SFM	    1UL
#define LPIT_CHANNEL_AUTO	    2UL
#define LPIT_CHANNEL_MPR	    3UL

/* Period in nanosecond unit */
#define GAS_PERIOD_BY_NS	480000000UL  /* The period is 0.5 second */
#define SFM_PERIOD_BY_NS	10000000UL  /* The period is 0.5 second */
#define AUTO_PERIOD_BY_NS	10000000UL /* The period is 10000 micro seconds */
#define MPR_PERIOD_BY_NS	500000000UL /* The period is 1 second. The actual period is going to be the
										   double of this value, because it requires two communications.  */

/* SPI Communication */
#define SPI_DELAY            10U     /* Delays between the clock and the chip enable */
#define MPR_OP_BUFFER_SIZE   3U      /* Request the sensor to enter Operation Mode */
#define MPR_DATA_BUFFER_SIZE 4U      /* Buffer size of the data */
#define SPI_TIMEOUT          100U    /* Time to return from the SPI transfer function */

#define OUTPUT_DIR 1





#endif /* SOURCES_DEFINITION_H_ */
