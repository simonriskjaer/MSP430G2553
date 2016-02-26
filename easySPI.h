/*
 * easySPI.h
 *
 *  Created on: 04/02/2016
 *      Author: Simon Mogensen
 */

// spiRead output array
#ifndef __EASY_SPI_H__
#define __EASY_SPI_H__

/************************************************************
 Pre declarations
 ************************************************************/
void spiMode(char device);
void spiWrite(char device, int byte1, int byte2, int byte3, int byte4, int byte5);

/************************************************************
 DAC and ADC macros
 ************************************************************/
// Device select
#define DAC (1)
#define ADC (2)

// DAC channel select
#define channel_a (1)
#define channel_b (2)

// Disable macro for DAC
#define DISABLE (0)

// CS and Latch pins for DAC
#define DAC_CS_PORT (1)
#define DAC_CS_PIN (0)
#define DAC_LATCH_PORT (2)
#define DAC_LATCH_PIN (5)

// CS pins for ADC
#define ADC_CS_PORT (1)
#define ADC_CS_PIN (5)

// ADC modes
#define MODE1 (1)
#define MODE2 (2)
#define MODE3 (3)

// Dummy char
#define DUMMY_BYTE (0xFF)

// Null definition. The value is over the maximum of 4096 that can be interpreted by the DAC hardware.
#define NA (5000)

// Channel gpio enable/disable
#define ENABLE (1)
#define DISABLE (0)

#endif
