
#include "Switch.h"
#include "..//tm4c123gh6pm.h"

void Switch_Init()
{
	unsigned long volatile delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE; 	// activate port E
  delay = SYSCTL_RCGC2_R;    							// allow time to finish activating
  GPIO_PORTE_AMSEL_R &= ~0x03;      			// no analog 
  GPIO_PORTE_PCTL_R &= ~0x000000FF; 			// regular function
  GPIO_PORTE_DIR_R &= ~0x03;      				// make PE3-0 in
  GPIO_PORTE_AFSEL_R &= ~0x03;   					// disable alt funct on PB3-0
  GPIO_PORTE_DEN_R |= 0x03;      					// enable digital I/O on PB3-0
}

unsigned char SW1_Pressed()
{
	 if(GPIO_PORTE_DATA_R & 0x01)   // PE0
	 {
		 return 1;
	 }
	 else
	 {
		 return 0;
	 }
}
	
unsigned char SW2_Pressed()
{
	if(GPIO_PORTE_DATA_R & 0x02)
	{
		 return 1;
	}
	else
	{
	 return 0;
	}
}
