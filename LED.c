// Author: Eslam Younis
// Date: 22/7/2016

#ifndef __LED_H_
#define __LED_H_

#include "LED.h"

#include "..//tm4c123gh6pm.h"

// **************DAC_Init*********************
// Initialize 4-bit DAC 
// Input: none
// Output: none
void LED_Init(void)
{
	unsigned long volatile delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; 	// activate port B
  delay = SYSCTL_RCGC2_R;    							// allow time to finish activating
  GPIO_PORTB_AMSEL_R &= ~0xC0;//30;      			// no analog 
  GPIO_PORTB_PCTL_R &= ~0xFF000000; 			// regular function
  GPIO_PORTB_DIR_R |= 0xC0;//0x30;      					// make PB3-0 out
	GPIO_PORTB_DR8R_R |= 0xC0;//0x30;
  GPIO_PORTB_AFSEL_R &= ~0xC0;//0x30;   					// disable alt funct on PB3-0
  GPIO_PORTB_DEN_R |= 0xC0;//0x30;      					// enable digital I/O on PB3-0
}

void LED1_Toggle(void)
{
	GPIO_PORTB_DATA_R ^= 0x40;
}

void LED2_Toggle(void)
{
	GPIO_PORTB_DATA_R ^= 0x80;
}

void LED1_On(void)
{
	GPIO_PORTB_DATA_R |= 0x40;
}

void LED2_On(void)
{
	GPIO_PORTB_DATA_R |= 0x80;
}

void LED1_Off(void)
{
	GPIO_PORTB_DATA_R &= ~0x40;
}

void LED2_Off(void)
{
	GPIO_PORTB_DATA_R &= ~0x80;
}

#endif
