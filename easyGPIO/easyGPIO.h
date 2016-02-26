/*
 * easyGPIO.h
 *
 *  Created on: 26/01/2016
 *      Author: Simon Mogensen
 */

#ifndef __EASY_GPIO_H__
#define __EASY_GPIO_H__

// Functions
void delay_ms(unsigned int d);
void CPUSpeed(char speed);
void pinMode(int port, int pin, int dir, int pull_up_down_na);
void digitalWrite(int port, int pin, int state);
int digitalRead(int port, int pin);
void wait_for_button();

// digitalWrite ADC macro
#define adcgpio (3)

// Enables easy HIGH and LOW setting
#define HIGH (1)
#define LOW (0)

// Easy setting of port
#define P1 (1)
#define P2 (2)

// Easy INPUT/OUTPUT setting
#define INPUT (0)
#define OUTPUT (1)

// Internal pull up/down resistors
// Disable with "0"
#define DISABLED (0) // Resistors are disabled
#define PU (1) // Enables internal pull up
#define PD (2) // Enables internal pull down

#endif
