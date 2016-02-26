/*
 * easySPI.c
 *
 *  Created on: 25/02/2016
 *      Author: Simon Mogensen
 */

#include "easySPI.h"
#include "easyGPIO.h"
#include "msp430.h"



// SPI mode function
void spiMode(char device)
{
	switch(device){

	// SPI Setup for the MCP4822 DAC
	case DAC:
		pinMode(DAC_CS_PORT, DAC_CS_PIN, OUTPUT, 0); // Chip select on port 1.0
		digitalWrite(DAC_CS_PORT, DAC_CS_PIN, HIGH); // Set CS high (because CSn)
		pinMode(DAC_LATCH_PORT, DAC_LATCH_PIN, OUTPUT, 0); // DAC latch

		P1SEL  =   BIT1    |   BIT2    |   BIT4; // Select pin 1.1, 1.2 & 1.4 for SPI communication
		P1SEL2 =   BIT1    |   BIT2    |   BIT4;

		UCA0CTL1   =   UCSWRST; // Clear all other parameters, and set "software reset enable" to 1
		UCA0CTL0   =   0; // Clear all parameters

		// Clock fase select, 1 = Data is captured on the first UCLK edge and changed on the following edge.
		// MSB first select, 1 = MSB first.
		// Master mode select, 1 = Master mode
		// Synchronous mode enable, 1 = synchronous mode
		// 3-wire mode is selected, because the UCMODE bits are set to zero.
	    UCA0CTL0    =  UCCKPH   +  UCMSB   +   UCMST   +   UCSYNC;

		UCA0CTL1   |=  UCSSEL_2;   //  SMCLK is selected for the USCI clock

	    UCA0BR0    |=  0x02;   // USCI_A0 Baud rate control register 0 /2
		UCA0MCTL   =   0;  // Modulation control register, 0 = No modulation
		UCA0CTL1   &=  ~UCSWRST; // Set "software reset enable" to 0

	break;

	// SPI Setup for the AD7124-8 ADC Devleopement board
	case ADC:
		pinMode(ADC_CS_PORT, ADC_CS_PIN, OUTPUT, 0); // Chip select on port 1.5
		digitalWrite(ADC_CS_PORT, ADC_CS_PIN, HIGH); // Set CS high (because CSn)

		P1SEL  =   BIT1    |   BIT2    |   BIT4; // Select pin 1.1, 1.2 & 1.4 for SPI communication
		P1SEL2 =   BIT1    |   BIT2    |   BIT4;

		UCA0CTL1   =   UCSWRST; // Clear all other parameters, and set "software reset enable" to 1
		UCA0CTL0   =   0; // Clear all parameters

		// UCCKPL - Clock polarity select, 1 = Clock idle high
		// UCCKPH - Clock fase select, 1 = Data is captured on the second UCLK edge and changed on the first.
		// UCMSB  - MSB first select, 1 = MSB first.
		// UCMST  - Master mode select, 1 = Master mode
		// UCSYNC - Synchronous mode enable, 1 = synchronous mode
		// 3-wire mode is selected, because the UCMODE bits are set to zero.
	    UCA0CTL0    =  UCCKPL  +  UCCKPH  +  UCMSB   +   UCMST   +   UCSYNC;

		UCA0CTL1   |=  UCSSEL_2;   //  SMCLK is selected for the USCI clock

	    UCA0BR0    |=  0x02;   // USCI_A0 Baud rate control register 0 /2
		UCA0MCTL   =   0;  // Modulation control register, 0 = No modulation
		UCA0CTL1   &=  ~UCSWRST; // Set "software reset enable" to 0
	break;
	}
}

// SPI write function. Used to send datapackets of 8 bits each. Use "NA" as argument if only 1 data packet needs to be sent.
void spiWrite(char device, int byte1, int byte2, int byte3, int byte4, int byte5)
{
	switch (device){

	// SPI device: DAC
	case 1:

		// Begin the write operation
			digitalWrite(DAC_LATCH_PORT, DAC_LATCH_PIN, HIGH); // DAC latch HIGH
			digitalWrite(DAC_CS_PORT, DAC_CS_PIN, LOW); // Chip select LOW
			// Check if data packet needs to be sent
		if ( byte1!= NA) {
			// Send 4 config bits and first 4 databits
		  while  (!(IFG2 &   UCA0TXIFG));    //  USCI_A0 TX  buffer  ready?
				UCA0TXBUF  =  byte1;
		}
			// Check if data packet needs to be sent
		if (byte2 != NA) {
			// Send last 8 data bits
	      while  (!(IFG2 &   UCA0TXIFG));    //  USCI_A0 TX  buffer  ready?
				UCA0TXBUF  =  byte2;
		}
			digitalWrite(DAC_CS_PORT, DAC_CS_PIN, HIGH); // Chip select HIGH
			digitalWrite(DAC_LATCH_PORT, DAC_LATCH_PIN, LOW); // DAC latch LOW
	break;

	// SPI device: ADC
	case 2:
		// Begin write operation
			digitalWrite(ADC_CS_PORT, ADC_CS_PIN, LOW); // Chip select LOW

		if (byte1 != NA) {
					// Send last 8 data bits
			// Send first byte of data
		  while  (!(IFG2 &   UCA0TXIFG));    //  USCI_A0 TX  buffer  ready?
				UCA0TXBUF  =  byte1;
		}
			// Check if a second data packet needs to be sent
		if (byte2 != NA) {
			// Send last byte of data
		  while  (!(IFG2 &   UCA0TXIFG));    //  USCI_A0 TX  buffer  ready?
				UCA0TXBUF  =  byte2;
		}
			// Check if a third data packet needs to be sent
		if (byte3 != NA) {
			// Send last byte of data
		  while  (!(IFG2 &   UCA0TXIFG))
		  {
			  ;    //  USCI_A0 TX  buffer  ready?
		  }
				UCA0TXBUF  =  byte3;
		}
		// Check if a third data packet needs to be sent
		if (byte4 != NA) {
		// Send last byte of data
			while  (!(IFG2 &   UCA0TXIFG));    //  USCI_A0 TX  buffer  ready?
			UCA0TXBUF  =  byte4;
		}
		else
		{
			// Do nothing
		}

		if (byte5 != NA) {
		// Send last byte of data
			while  (!(IFG2 &   UCA0TXIFG));    //  USCI_A0 TX  buffer  ready?
			UCA0TXBUF  =  byte5;
		}

		    digitalWrite(ADC_CS_PORT, ADC_CS_PIN, HIGH); // Chip select HIGH
	}
}


