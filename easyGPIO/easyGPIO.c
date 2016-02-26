/*
 * easyGPIO.c
 *
 *  Created on: 25/02/2016
 *      Author: Simon Mogensen
 */

#include "msp430.h"
#include "easyGPIO.h"
#include "easySPI.h"



// The new delay_ms function.
void delay_ms(unsigned int d) // Based on a 16Mhz clock. Each increment delays the CPU for 1ms
{
	unsigned int i = 0;
	while( i != d)
	{
		__delay_cycles(16000);
		i++;
	}
}

// Set CPU to run at either 1, 8 or 16 Mhz
void CPUSpeed(char speed)
{
	WDTCTL =   WDTPW   +   WDTHOLD;    //  Stop    watchdog    timer

	switch (speed){
	case 1:
		if (CALBC1_1MHZ ==0xFF || CALDCO_1MHZ == 0xFF)
		 {  while(1);                       // If calibration constants erased
		                                           // do not load, trap CPU!!
		 }
		 BCSCTL1 = CALBC1_1MHZ;                    // Set DCO to 1MHz
		 DCOCTL = CALDCO_1MHZ;

	break;

	case 8:
		if (CALBC1_8MHZ ==0xFF || CALDCO_8MHZ == 0xFF)
		 {  while(1);                       // If calibration constants erased
		                                           // do not load, trap CPU!!
		 }
		 BCSCTL1 = CALBC1_8MHZ;                    // Set DCO to 8MHz
		 DCOCTL = CALDCO_8MHZ;
	break;

	case 16:
		if (CALBC1_16MHZ ==0xFF || CALDCO_16MHZ == 0xFF)
		 {  while(1);                       // If calibration constants erased
		                                           // do not load, trap CPU!!
		 }
		 BCSCTL1 = CALBC1_16MHZ;                    // Set DCO to 16MHz
		 DCOCTL = CALDCO_16MHZ;
	break;

	}
}

// Pinmode function. Must be used before pins are usable.
void pinMode(int port, int pin, int dir, int pull_up_down_na)
{
	switch (port){

	case adcgpio:
		switch (pin){

		case 1:
			// Insert adcGpioMode here
			break;

		case 2:
			// Insert adcGpioMode here
			break;

		case 3:
			// Insert adcGpioMode here
			break;

		case 4:
			// Insert adcGpioMode here
			break;
		}
	case 1: // Port 1.X
		switch (pin){

		case 0:
			switch (dir){
						case 1:
						P1DIR |= BIT0; // Set pin 1 on port 1 to output
						P1SEL &= (~BIT0); // Set pin 1 on port 1 as an GPIO pin
						P1OUT &=~BIT0; // Set pin 1 on port 1 LOW
						break;
						default:
						{
							P1SEL &= (~BIT0); // Set pin 1 on port 1 as an GPIO pin
							switch (pull_up_down_na){
							case 0:
								P1REN &= (~BIT0); // disable internal resistors
								break;
							case 1:
								P1REN |= BIT0;  // enable internal resistors
								P1OUT |= BIT0;  // enable internal pull-up
								break;
							case 2:
								P1REN |= BIT0;  // enable internal resistors
								P1OUT &= (~BIT0); // enable internal pull-down
								break;
							}
						}
						break;
			}
			break;
		case 1:
			switch (dir){
						case 1:
						P1DIR ^= BIT1;
						P1SEL &= (~BIT1);
						P1OUT &=~BIT1;
						break;
						default:
						{
							P1SEL &= (~BIT1);
							switch (pull_up_down_na){
							case 0:
								P1REN &= (~BIT1); // disable internal resistors
								break;
							case 1:
								P1REN |= BIT1;  // enable internal resistors
								P1OUT |= BIT1;  // enable internal pull-up
								break;
							case 2:
								P1REN |= BIT1;  // enable internal resistors
								P1OUT &= (~BIT1); // enable internal pull-down
								break;
							}
						}
						break;
			}
			break;
		case 2:
			switch (dir){
						case 1:
						P1DIR ^= BIT2;
						P1SEL &= (~BIT2);
						P1OUT &=~BIT2;
						break;
						default:
						{
							P1SEL &= (~BIT2);
							switch (pull_up_down_na){
							case 0:
								P1REN &= (~BIT2); // disable internal resistors
								break;
							case 1:
								P1REN |= BIT2;  // enable internal resistors
								P1OUT |= BIT2;  // enable internal pull-up
								break;
							case 2:
								P1REN |= BIT2;  // enable internal resistors
								P1OUT &= (~BIT2); // enable internal pull-down
								break;
							}
						}
						break;
		}
			break;
		case 3:
			switch (dir){
						case 1:
						P1DIR ^= BIT3;
						P1SEL &= (~BIT3);
						P1OUT &=~BIT3;
						break;
						default:
						{
							P1SEL &= (~BIT3);
							switch (pull_up_down_na){
							case 0:
								P1REN &= (~BIT3); // disable internal resistors
								break;
							case 1:
								P1REN |= BIT3;  // enable internal resistors
								P1OUT |= BIT3;  // enable internal pull-up
								break;
							case 2:
								P1REN |= BIT3;  // enable internal resistors
								P1OUT &= (~BIT3); // enable internal pull-down
								break;
							}
						}
						break;
			}
			break;
		case 4:
			switch (dir){
						case 1:
						P1DIR ^= BIT4;
						P1SEL &= (~BIT4);
						P1OUT &=~BIT4;
						break;
						default:
						{
							P1SEL &= (~BIT4);
							switch (pull_up_down_na){
							case 0:
								P1REN &= (~BIT4); // disable internal resistors
								break;
							case 1:
								P1REN |= BIT4;  // enable internal resistors
								P1OUT |= BIT4;  // enable internal pull-up
								break;
							case 2:
								P1REN |= BIT4;  // enable internal resistors
								P1OUT &= (~BIT4); // enable internal pull-down
								break;
							}
						}
						break;
			}
			break;
		case 5:
			switch (dir){
						case 1:
						P1DIR ^= BIT5;
						P1SEL &= (~BIT5);
						P1OUT &=~BIT5;
						break;
						default:
						{
							P1SEL &= (~BIT5);
							switch (pull_up_down_na){
							case 0:
								P1REN &= (~BIT5); // disable internal resistors
								break;
							case 1:
								P1REN |= BIT5;  // enable internal resistors
								P1OUT |= BIT5;  // enable internal pull-up
								break;
							case 2:
								P1REN |= BIT5;  // enable internal resistors
								P1OUT &= (~BIT5); // enable internal pull-down
								break;
							}
						}
						break;
			}
			break;
		case 6:
			switch (dir){
						case 1:
						P1DIR ^= BIT6;
						P1SEL &= (~BIT6);
						P1OUT &=~BIT6;
						break;
					    default:
						{
							P1SEL &= (~BIT6);
							switch (pull_up_down_na){
							case 0:
								P1REN &= (~BIT6); // disable internal resistors
								break;
							case 1:
								P1REN |= BIT6;  // enable internal resistors
								P1OUT |= BIT6;  // enable internal pull-up
								break;
							case 2:
								P1REN |= BIT6;  // enable internal resistors
								P1OUT &= (~BIT6); // enable internal pull-down
								break;
							}
						}
						break;
			}
			break;
		case 7:
			switch (dir){
						case 1:
						P1DIR ^= BIT7;
						P1SEL &= (~BIT7);
						P1OUT &=~BIT7;
						break;
						default:
						{
							P1SEL &= (~BIT7);
							switch (pull_up_down_na){
						case 0:
							P1REN &= (~BIT7); // disable internal resistors
							break;
						case 1:
							P1REN |= BIT7;  // enable internal resistors
							P1OUT |= BIT7;  // enable internal pull-up
							break;
						case 2:
							P1REN |= BIT7;  // enable internal resistors
							P1OUT &= (~BIT7); // enable internal pull-down
							break;
							}
						}
						break;
			}
			break;
		default:
		{

		}
		break;
	    }
		break;

case 2: // Port 2.X
		switch (pin){

		case 0:
			switch (dir){
						case 1:
						P2DIR ^= BIT0; //XOR toggler værdien af bit0 aka 0x0001
						P2SEL &= (~BIT0);
						P2SEL2 &= (~BIT0);
						P2OUT &=~BIT0;
						break;
						default:
						{
							P2SEL &= (~BIT0);
							switch (pull_up_down_na){
							case 0:
								P2REN &= (~BIT0); // disable internal resistors
								break;
							case 1:
								P2REN |= BIT0;  // enable internal resistors
								P2OUT |= BIT0;  // enable internal pull-up
								break;
							case 2:
								P2REN |= BIT0;  // enable internal resistors
								P2OUT &= (~BIT0); // enable internal pull-down
								break;
							}
						}
						break;
			}
			break;
		case 1:
			switch (dir){
						case 1:
						P2DIR ^= BIT1;
						P2SEL &= (~BIT1);
						P2SEL2 &= (~BIT1);
						P2OUT &=~BIT1;
						break;
						default:
						{
							P2SEL &= (~BIT1);
							switch (pull_up_down_na){
							case 0:
								P2REN &= (~BIT1); // disable internal resistors
								break;
							case 1:
								P2REN |= BIT1;  // enable internal resistors
								P2OUT |= BIT1;  // enable internal pull-up
								break;
							case 2:
								P2REN |= BIT1;  // enable internal resistors
								P2OUT &= (~BIT1); // enable internal pull-down
								break;
							}
						}
						break;
			}
			break;
		case 2:
			switch (dir){
						case 1:
						P2DIR ^= BIT2;
						P2SEL &= (~BIT2);
						P2OUT &=~BIT2;
						break;
						default:
						{
							P2SEL &= (~BIT2);
							switch (pull_up_down_na){
							case 0:
								P2REN &= (~BIT2); // disable internal resistors
								break;
							case 1:
								P2REN |= BIT2;  // enable internal resistors
								P2OUT |= BIT2;  // enable internal pull-up
								break;
							case 2:
								P2REN |= BIT2;  // enable internal resistors
								P2OUT &= (~BIT2); // enable internal pull-down
								break;
							}
						}
						break;
		}
			break;
		case 3:
			switch (dir){
						case 1:
						P2DIR ^= BIT3;
						P2SEL &= (~BIT3);
						P2SEL2 &= (~BIT3);
						P2OUT &=~BIT3;
						break;
						default:
						{
							P2SEL &= (~BIT3);
							switch (pull_up_down_na){
							case 0:
								P2REN &= (~BIT3); // disable internal resistors
								break;
							case 1:
								P2REN |= BIT3;  // enable internal resistors
								P2OUT |= BIT3;  // enable internal pull-up
								break;
							case 2:
								P2REN |= BIT3;  // enable internal resistors
								P2OUT &= (~BIT3); // enable internal pull-down
								break;
													}
						}
						break;
			}
			break;
		case 4:
			switch (dir){
						case 1:
						P2DIR ^= BIT4;
						P2SEL &= (~BIT4);
						P2SEL2 &= (~BIT4);
						P2OUT &=~BIT4;
						break;
						default:
						{
							P2SEL &= (~BIT4);
							switch (pull_up_down_na){
							case 0:
								P2REN &= (~BIT4); // disable internal resistors
								break;
							case 1:
								P2REN |= BIT4;  // enable internal resistors
								P2OUT |= BIT4;  // enable internal pull-up
								break;
							case 2:
								P2REN |= BIT4;  // enable internal resistors
								P2OUT &= (~BIT4); // enable internal pull-down
								break;
							}
						}
						break;
			}
			break;
		case 5:
			switch (dir){
						case 1:
						P2DIR ^= BIT5;
						P2SEL &= (~BIT5);
						P2SEL2 &= (~BIT5);
						P2OUT &=~BIT5;
						break;
						default:
						{
							P2SEL &= (~BIT5);
							switch (pull_up_down_na){
							case 0:
								P2REN &= (~BIT5); // disable internal resistors
								break;
							case 1:
								P2REN |= BIT5;  // enable internal resistors
								P2OUT |= BIT5;  // enable internal pull-up
								break;
							case 2:
								P2REN |= BIT5;  // enable internal resistors
								P2OUT &= (~BIT5); // enable internal pull-down
								break;
							}
						}
						break;
			}
			break;
		default:
				{

				}
				break;
	    }
	break;
	}
}

// digitalWrite function.
void digitalWrite(int port, int pin, int state)
{
	// pinMode(port, pin, OUTPUT, 0); Causing problems in physical logic output.
	switch (port){
	// adcGPIO function integrated in digitalWrite
	case adcgpio:
		adcGPIO(pin, state);
		break;
	// Port 1 of MSP
	case 1:
		switch (pin){
		case 0:
			switch (state){
			case 0:
				  P1OUT&=~BIT0;
				  break;
			case 1:
				  P1OUT|=BIT0;
				  break;
			}
			break;
		case 1:
			switch (state){
			case 0:
					P1OUT&=~BIT1;
					break;
			case 1:
				    P1OUT|=BIT1;
					break;
			}
			break;
		case 2:
			switch (state){
			case 0:
					P1OUT&=~BIT2;
					break;
			case 1:
					P1OUT|=BIT2;
					break;
						}
			break;
		case 3:
			switch (state){
			case 0:
					P1OUT&=~BIT3;
					break;
			case 1:
					P1OUT|=BIT3;
					break;
						}
			break;
		case 4:
			switch (state){
			case 0:
				  P1OUT&=~BIT4;
				  break;
			case 1:
				  P1OUT|=BIT4;
				  break;
			}
		case 5:
			switch (state){
			case 0:
					P1OUT&=~BIT5;
					break;
			case 1:
				    P1OUT|=BIT5;
					break;
			}
			break;
		case 6:
			switch (state){
			case 0:
					P1OUT&=~BIT6;
					break;
			case 1:
					P1OUT|=BIT6;
					break;
						}
		case 7:
			switch (state){
			case 0:
					P1OUT&=~BIT7;
					break;
			case 1:
					P1OUT|=BIT7;
					break;
						}
			break;

		}
	break;
	// Port 2 of MSP
	case 2:
		switch (pin){
		case 0:
			switch (state){
			case 0:
				  P2OUT&=~BIT0;
				  break;
			case 1:
				  P2OUT|=BIT0;
				  break;
			}
			break;
		case 1:
			switch (state){
			case 0:
					P2OUT&=~BIT1;
					break;
			case 1:
				    P2OUT|=BIT1;
					break;
			}
			break;
		case 2:
			switch (state){
			case 0:
					P2OUT&=~BIT2;
					break;
			case 1:
					P2OUT|=BIT2;
					break;
						}
			break;
		case 3:
			switch (state){
			case 0:
					P2OUT&=~BIT3;
					break;
			case 1:
					P2OUT|=BIT3;
					break;
						}
			break;
		case 4:
			switch (state){
			case 0:
				  P2OUT&=~BIT4;
				  break;
			case 1:
				  P2OUT|=BIT4;
				  break;
			}
			break;
		case 5:
			switch (state){
			case 0:
					P2OUT&=~BIT5;
					break;
			case 1:
				    P2OUT|=BIT5;
					break;
			}
			break;
		}
	}
}

// digitalRead function. The pinMode function must initialize each pin before they can be used with this function
int digitalRead(int port, int pin)
{
	switch (port){
	case 1:
		switch (pin){
		case 1:
			if ( (P1IN & BIT0) != 0 ) // If input of pin 1 port 1 is 1, then...
			{
				return 1;
			}
			else
			{
				return 0;
			}
		case 2:
			if ( (P1IN & BIT2) != 0 ) // If input of pin 1 port 1 is 1, then...
			{
				return 1;
			}
			else
			{
				return 0;
			}
		case 3:
			if ( (P1IN & BIT3) != 0 ) // If input of pin 1 port 1 is 1, then...
			{
				return 1;
			}
			else
			{
				return 0;
			}
		case 4:
			if ( (P1IN & BIT4) != 0 ) // If input of pin 1 port 1 is 1, then...
			{
				return 1;
			}
			else
			{
				return 0;
			}
		case 5:
			if ( (P1IN & BIT5) != 0 ) // If input of pin 1 port 1 is 1, then...
			{
				return 1;
			}
			else
			{
				return 0;
			}
		case 6:
			if ( (P1IN & BIT6) != 0 ) // If input of pin 1 port 1 is 1, then...
			{
				return 1;
					}
			else
			{
				return 0;
			}
		case 7:
			if ( (P1IN & BIT7) != 0 ) // If input of pin 1 port 1 is 1, then...
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	case 2:
		switch (pin){
				case 1:
					if ( (P2IN & BIT0) != 0 ) // If input of pin 1 port 1 is 1, then...
					{
						return 1;
					}
					else
					{
						return 0;
					}
				case 2:
					if ( (P2IN & BIT2) != 0 ) // If input of pin 1 port 1 is 1, then...
					{
						return 1;
					}
					else
					{
						return 0;
					}
				case 3:
					if ( (P2IN & BIT3) != 0 ) // If input of pin 1 port 1 is 1, then...
					{
						return 1;
					}
					else
					{
						return 0;
					}
				case 4:
					if ( (P2IN & BIT4) != 0 ) // If input of pin 1 port 1 is 1, then...
					{
						return 1;
					}
					else
					{
						return 0;
					}
				case 5:
					if ( (P2IN & BIT5) != 0 ) // If input of pin 1 port 1 is 1, then...
					{
						return 1;
					}
					else
					{
						return 0;
					}
		}

	}
	return 99;
}

void wait_for_button()
{
	// Setup launch pad button
	pinMode(1, 3, INPUT, PU);

	// Launchpad button is active low - so buttonpress = 0
	while(digitalRead(1, 3) == 0)
	{

	}

	// Preventing bouncing or errornous second presses
	delay_ms(2);
	while(digitalRead(1, 3) == 1)
	{

	}

	// Preventing bouncing or errornous second presses
	delay_ms(2);
}

