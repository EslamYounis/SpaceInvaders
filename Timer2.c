//// DAC.c
//// Runs on LM4F120 or TM4C123, 
//// edX lab 13 
//// Implementation of the 4-bit digital to analog converter
//// Daniel Valvano, Jonathan Valvano
//// December 29, 2014
//// Port B bits 3-0 have the 4-bit DAC

//#include "Timer2.h"
//#include "..//tm4c123gh6pm.h"

//// You can use this timer only if you learn how it works
//void Timer2_Init(unsigned long period)
//{ 
//  unsigned long volatile delay;
//  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
//  delay = SYSCTL_RCGCTIMER_R;
//  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
//  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
//  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
//  TIMER2_TAILR_R = period-1;    // 4) reload value
//  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
//  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
//  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
//  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
//// interrupts enabled in the main program after all devices initialized
//// vector number 39, interrupt number 23
//  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
//  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
//}

//void Timer2A_Handler(void)
//{ 
//  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
// // TimerCount++;
//  //Semaphore = 1; // trigger
//}