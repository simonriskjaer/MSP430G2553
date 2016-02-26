/*
 * easySPI.c
 *
 *  Created on: 25/02/2016
 *      Author: Simon Mogensen
 */

#include "easyConvert.h"
#include "easySPI.h"
#include "easyGPIO.h"
#include "msp430.h"
#include "Global_elements.h"



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

		// Disable both DAC's before use. Otherwise idle voltage is the internal ref voltage of 2.0x volts amplified by the external opamp to 2.5 volts.
		dacWrite(1, DISABLE);
		dacWrite(2, DISABLE);
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

// Note: The dacWrite function uses 2 global arrays; firstbyte[8] & secondbyte[8]. Theese arrays are set by the dacPrep function, which is called in this function.
 void dacWrite(int channel, int value)
 {

	 // Call dacPrep function to initialize firstbyte[] and secondbyte[] arrays for use, with the input parameters of the dacWrite function.
	 dacPrep(channel, value);

	 // Switch on the selected channel (DAC A / DAC B)
	 switch (channel){

	 case 1:
	 {
		 switch (value)
		 {
		 // A set of default min/max configurations using preset binary values
		 case MIN:
			 spiWrite(DAC, 0b01010000, 0b00000001, NA, NA, NA);
			 break;
		 case MAX:
			 spiWrite(DAC, 0b01011111, 0b11111111, NA, NA, NA);
			 break;
		// Put the DAC in shutdown mode
		 case DISABLE:
			 spiWrite(DAC, 0b01101111, 0b11111111, NA, NA, NA);
		 // Used under normal operation
		 default:
			 spiWrite(DAC, firstbyte_decimal(), secondbyte_decimal(), NA, NA, NA);
			 break;
		}
	 }
	break;

	 case 2:
		 switch (value)
		 {
		 // A set of default min/max configurations using preset binary values
		 case MIN:
			 spiWrite(DAC, 0b11010000, 0b00000001, NA, NA, NA);
			 break;
		 case MAX:
			 spiWrite(DAC, 0b11011111, 0b11111111, NA, NA, NA);
			 break;
		// Put the DAC in shutdown mode
		 case DISABLE:
			 spiWrite(DAC, 0b11101111, 0b11111111, NA, NA, NA);
		 // Used under normal operation
		 default:
			 spiWrite(DAC, firstbyte_decimal(), secondbyte_decimal(), NA, NA, NA);
			 break;
		 }
		 break;
	}
}


// Used to configure the ADC
void adcMode(char mode)
{
	switch (mode)
	{
	// Mode 1 - Continuous conversion etc.
	case 1:
	    /*
	     * 1. byte = Communications register - input which register address there is to be written to.
	     * 2. byte = Setting up the ADC for continuous read, and doing so in the ADC_CONTROL REGISTER (RS[5:0] = 0, 0, 0, 0, 1) Page 78 in AD7124-8 datasheet
	     * 3. byte = Communications register - input which register address there is to be written to.
	     * 4. byte = Selecting power mode, operation mode and clock select
	     */
		spiWrite(ADC, 0b01000000, NA, NA, NA, NA);
		// spiRead(ADC)
		break;
	// Mode 2 - Different functionality
	case 2:
		spiWrite(ADC, NA, NA, NA, NA, NA);
		break;
	}

	// Set the global variable to 1, indicating that the ADC is initialized
	adcModeSet = 1;
}

// Used to read the values stored in the ADC's shift registers. When registers of a certain size is selected (eg. the status register) - the "char bytes" argument will be ignored.
// "char bytes" only applies when reading in continous mode, and the bytes read depends on the ADC's resolution settings.
unsigned long adcRead(char reg, char bytes)
{
	// Check if adcMode has initialized the adc module
	if (adcModeSet == 0)
		{
		// Initialize for default operation
		adcMode(MODE1);
		}

	// Initialize return variable
	unsigned long result = 0;
	// Initialize count variables
	int cnt = 0;
	int cnt2 = 1;

	switch (reg)
	{
		case CONTINUOUS_READ:
			// Select ADC
			digitalWrite(ADC_CS_PORT, ADC_CS_PIN, LOW);

			while (cnt < bytes)
			{
			// Make sure USCI_A0 TX buffer is ready
			while  (!(IFG2 &   UCA0TXIFG));
			// Transmit dummy byte
				UCA0TXBUF  =  DUMMY_BYTE;
				// Wait for dummy bytes to be sent
			while  (!(IFG2 &   UCA0TXIFG));
				// Load UCA0RX buffer into rx array
				rxArray[cnt] = UCA0RXBUF;
			cnt++;
			}
			// Deselct ADC
			digitalWrite(ADC_CS_PORT, ADC_CS_PIN, HIGH);

			// Make result variable
			cnt = 0;
			result = rxArray[cnt];
			while (cnt < bytes)
			{
				result <<= 8;
				result |= rxArray[cnt2];
				cnt++;
				cnt2++;
			}

	    break;

		case SINGLE_READ:
			// Select ADC
			digitalWrite(ADC_CS_PORT, ADC_CS_PIN, LOW);

			while (cnt < bytes)
			{

			// Make sure USCI_A0 TX buffer is ready
			while  (!(IFG2 &   UCA0TXIFG));
			// Transmit dummy byte
				UCA0TXBUF  =  DUMMY_BYTE;
				// Wait for dummy bytes to be sent
			while  (!(IFG2 &   UCA0TXIFG));
				// Load UCA0RX buffer into rx array
				rxArray[cnt] = UCA0RXBUF;
			cnt++;
			}
			// Deselct ADC
			digitalWrite(ADC_CS_PORT, ADC_CS_PIN, HIGH);

			// Make result variable
			cnt = 0;
			result = rxArray[cnt];
			while (cnt < bytes)
			{
				result <<= 8;
				result |= rxArray[cnt2];
				cnt++;
				cnt2++;
			}

	}
	return result;
}

// Function used to control the 4 I/O pins within the ADC
void adcGPIO(char pin, char state)
{
	// Initialize temp variable
	unsigned char temp = 0;
	// Set spiMode to ADC
	spiMode(ADC);

	switch (state){

	case LOW:
		switch (pin)
		{
		case 1:
			// Pin 1 low
			// Modify array
			adcPinState[3] = 0;
			adcPinState[7] = 1;
			// Load the return of adcPinState_decimal() into a temporary char, so that the function does not ruin the timing of the spiWrite function.
			temp = adcPinState_decimal();
			// First byte is the adress of the IO_CONTROL REGISTER
			// Second byte contains the pin state values needed to be changed
			// Third and fourth are not important. See documentation for more information.
			spiWrite(ADC, 0b00000011, temp, 0b00000000, 0b01111110, NA);
			break;
		case 2:
			// Pin 2 low
			adcPinState[2] = 0;
			adcPinState[6] = 1;
			// Load the return of adcPinState_decimal() into a temporary char, so that the function does not ruin the timing of the spiWrite function.
			temp = adcPinState_decimal();
			spiWrite(ADC, 0b00000011, temp, 0b00000000, 0b01111110, NA);
			break;
		case 3:
			// Pin 3 low
			adcPinState[1] = 0;
			adcPinState[5] = 1;
			// Load the return of adcPinState_decimal() into a temporary char, so that the function does not ruin the timing of the spiWrite function.
			temp = adcPinState_decimal();
			spiWrite(ADC, 0b00000011, temp, 0b00000000, 0b01111110, NA);
			break;
		case 4:
			// pin 4 low
			adcPinState[0] = 0;
			adcPinState[4] = 1;
			// Load the return of adcPinState_decimal() into a temporary char, so that the function does not ruin the timing of the spiWrite function.
			temp = adcPinState_decimal();
			spiWrite(ADC, 0b00000011, temp, 0b00000000, 0b01111110, NA);
			break;
	    }
	break;
	case HIGH:
		switch (pin)
		{
		case 1:
			// Pin 1 high
			adcPinState[3] = 1;
			adcPinState[7] = 1;
			// Load the return of adcPinState_decimal() into a temporary char, so that the function does not ruin the timing of the spiWrite function.
			temp = adcPinState_decimal();
			spiWrite(ADC, 0b00000011, temp, 0b00000000, 0b01111110, NA);
			break;
		case 2:
			// Pin 2 high
			adcPinState[2] = 1;
			adcPinState[6] = 1;
			// Load the return of adcPinState_decimal() into a temporary char, so that the function does not ruin the timing of the spiWrite function.
			temp = adcPinState_decimal();
			spiWrite(ADC, 0b00000011, temp, 0b00000000, 0b01111110, NA);
			break;
		case 3:
			// Pin 3 high
			adcPinState[1] = 1;
			adcPinState[5] = 1;
			// Load the return of adcPinState_decimal() into a temporary char, so that the function does not ruin the timing of the spiWrite function.
			temp = adcPinState_decimal();
			spiWrite(ADC, 0b00000011, temp, 0b00000000, 0b01111110, NA);
			break;
		case 4:
			// Pin 4 high
			adcPinState[0] = 1;
			adcPinState[4] = 1;
			// Load the return of adcPinState_decimal() into a temporary char, so that the function does not ruin the timing of the spiWrite function.
			temp = adcPinState_decimal();
			spiWrite(ADC, 0b00000011, temp, 0b00000000, 0b01111110, NA);
			break;
		}
	}
}

// In order to use a port on the ADC as an digital output (Note: Only output - not input), the pin must first be set up for it. Furthermore, if a pin should be used as an analog input for
// the ADC, it must first be disabled as a gpio pin. This function enables disabling the gpio pin, so that it can be used as an analog input for the ADC.
void adcGpioMode(char pin, char enable_disable_gpio)
{
	// Initialize temp variable
	unsigned char temp = 0;
	// Set SPI mode to ADC
	spiMode(ADC);
	switch (enable_disable_gpio){

	case ENABLE:
		switch (pin) {
		case 1:
			// Enable the given pin
			adcPinState[0] = 1;
			// Set output of it to 0
			adcPinState[4] = 0;
			// Load the return of adcPinState_decimal() into a temporary char, so that the function does not ruin the timing of the spiWrite function.
			temp = adcPinState_decimal();
			spiWrite(ADC, 0b00000011, temp, 0b00000000, 0b01111110, NA);
			break;
		case 2:
			// Enable the given pin
			adcPinState[1] = 1;
			// Set output of it to 0
			adcPinState[5] = 0;
			// Load the return of adcPinState_decimal() into a temporary char, so that the function does not ruin the timing of the spiWrite function.
			temp = adcPinState_decimal();
			spiWrite(ADC, 0b00000011, temp, 0b00000000, 0b01111110, NA);
			break;
		case 3:
			// Enable the given pin
			adcPinState[2] = 1;
			// Set output of it to 0
			adcPinState[6] = 0;
			// Load the return of adcPinState_decimal() into a temporary char, so that the function does not ruin the timing of the spiWrite function.
			temp = adcPinState_decimal();
			spiWrite(ADC, 0b00000011, temp, 0b00000000, 0b01111110, NA);
			break;
		case 4:
			// Enable the given pin
			adcPinState[3] = 1;
			// Set output of it to 0
			adcPinState[7] = 0;
			// Load the return of adcPinState_decimal() into a temporary char, so that the function does not ruin the timing of the spiWrite function.
			temp = adcPinState_decimal();
			spiWrite(ADC, 0b00000011, temp, 0b00000000, 0b01111110, NA);
			break;
		}
	case DISABLE:
		switch (pin) {
		case 1:
			// Disable the given pin
			adcPinState[0] = 0;
			// Set output of it to 0
			adcPinState[4] = 0;
			// Load the return of adcPinState_decimal() into a temporary char, so that the function does not ruin the timing of the spiWrite function.
			temp = adcPinState_decimal();
			spiWrite(ADC, 0b00000011, temp, 0b00000000, 0b01111110, NA);
		case 2:
			// Disable the given pin
			adcPinState[1] = 0;
			// Set output of it to 0
			adcPinState[5] = 0;
			// Load the return of adcPinState_decimal() into a temporary char, so that the function does not ruin the timing of the spiWrite function.
			temp = adcPinState_decimal();
			spiWrite(ADC, 0b00000011, temp, 0b00000000, 0b01111110, NA);
		case 3:
			// Disable the given pin
			adcPinState[2] = 0;
			// Set output of it to 0
			adcPinState[6] = 0;
			// Load the return of adcPinState_decimal() into a temporary char, so that the function does not ruin the timing of the spiWrite function.
			temp = adcPinState_decimal();
			spiWrite(ADC, 0b00000011, temp, 0b00000000, 0b01111110, NA);
		case 4:
			// Disable the given pin
			adcPinState[3] = 0;
			// Set output of it to 0
			adcPinState[7] = 0;
			// Load the return of adcPinState_decimal() into a temporary char, so that the function does not ruin the timing of the spiWrite function.
			temp = adcPinState_decimal();
			spiWrite(ADC, 0b00000011, temp, 0b00000000, 0b01111110, NA);
		}
	}
}
// The ADC resets all of it's registers if 64 consecutive 1's are clocked into it. This can be handy for soft reboots of the system.
void adcReset()
{
	int i = 0;
	spiMode(ADC);
	// Send 64 consecutive 1's
	digitalWrite(ADC_CS_PORT, ADC_CS_PIN, LOW); // Chip select LOW
	while(i != 10)
	{
	  while  (!(IFG2 &   UCA0TXIFG));    //  USCI_A0 TX  buffer  ready?
	  // Transmit 0xFF
			UCA0TXBUF  =  0xFF;
	i++;
	}
	digitalWrite(ADC_CS_PORT, ADC_CS_PIN, HIGH); // Chip select HIGH
}
