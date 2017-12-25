// Author: Eslam Younis
// Date: 22/7/2016

// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the edX Lab 15
// In order for other students to play your game
// 1) You must leave the hardware configuration as defined
// 2) You must not add/remove any files from the project
// 3) You must add your code only this this C file
// I.e., if you wish to use code from sprite.c or sound.c, move that code in this file
// 4) It must compile with the 32k limit of the free Keil

// April 10, 2014
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Required Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0 (SW1)
// special weapon fire button connected to PE1 (SW2)
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power
// Ground        (GND, pin 2) ground
// SSI0Fss       (SCE, pin 3) connected to PA3
// Reset         (RST, pin 4) connected to PA7
// Data/Command  (D/C, pin 5) connected to PA6
// SSI0Tx        (DN,  pin 6) connected to PA5
// SSI0Clk       (SCLK, pin 7) connected to PA2
// back light    (LED, pin 8) not connected, consists of 4 white LEDs which draw ~80mA total

#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "Sprite.h"
#include "Switch.h"
#include "Graphics.h"
#include "Spaceship.h"
#include "DataType.h"
#include "Sound.h"
#include "LED.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Timer2_Init(unsigned long period);
//void SysTick_Init(unsigned long period);
void Delay5ms(unsigned long count);   // time delay in 0.005 seconds
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
bool Spcaeship_Alive(void);
void Sprites_Life(void);
void Spcaeship_Shield_Status(void);
void ShieldGraphicsPtr_init(void);
void Player_Fire(void);
void Player_Change_Weapon(void);
bool Game_Over(void);
void Game_Next_Level(void);
void Score_Update(void);

const unsigned char * (*ShieldGraphicsPtr[4])(void);

unsigned long TimerCount;
unsigned long Semaphore;
unsigned char NumberOfTrials;
unsigned long Score;
unsigned char SW1debounce, SW2debounce;
unsigned char GameLevel;

int main(void)
{
	Score = 0;
	NumberOfTrials = 3;
	SW1debounce = 0;
	SW2debounce = 0;
	
  TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
//TExaS_Init(NoLCD_NoScope);
	
	Nokia5110_Init();
	Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();      // draw buffer
	Timer2_Init(2666666);
	Sound_Init();
	Switch_Init();
	LED_Init();
  Spaceship_Init();
	Spaceship_Shield_Init();
	Sprite_Init();
	ShieldGraphicsPtr_init();
	Random_Init(NVIC_ST_CURRENT_R);
 // Sprite_Fire_Speed(100);
	GameLevel = Level_1;

	Game_Next_Level();
	
	while(1)
	{
		if(Semaphore)
		{
			//Play_Again();
			if(Game_Over() == true)
			{
				continue;
			}
			
			Game_Next_Level();
			Nokia5110_DisplayBuffer();
			
			Nokia5110_ClearBuffer();
		  Sprite_Move();
			Sprite_Fire();
			Sprite_Fire_MoveDown();
			Spaceship_Move();
			Spaceship_Shield();
			Player_Fire();
			Player_Change_Weapon();
			Spaceship_Fire_MoveUp();
			Nokia5110_DisplayBuffer();

			Spcaeship_Shield_Status();
			Sprites_Life();
			
			if(!Spcaeship_Alive())
			{
				//ship died
				NumberOfTrials--;
				if(NumberOfTrials > 0)
				{
					Delay5ms(20);
					Spaceship_Init();
				}
			}
			
			Nokia5110_DisplayBuffer();
			Semaphore = 0;
		}
  }
}

void Player_Fire(void)
{
	if(SW1_Pressed()) 
	{
		SW1debounce++;
		//Delay5ms(30);
		//if(SW1debounce >= 2)  
		{
			//LED1_On();
			//LED1_Toggle();
			Sound_Shoot();
			Spaceship_Fire();
			SW1debounce = 0;
		}
	}
	else
	{
		SW1debounce = 0;
	}
}

void Player_Change_Weapon()
{
	if(SW2_Pressed())
	{
		SW2debounce++;
		
		if(SW2debounce == 3)
		{
			//LED2_Toggle();
			//Sound_Fastinvader4();
			Sound_Highpitch();
			Spaceship_Change_Weapon();
			SW2debounce = 0;
		}
	}
	else
	{
		SW2debounce = 0;
	}
}

void Score_Update(void)
{
	Score++;
}

void Game_Next_Level()
{
	if(Sprites_Alive_Count() == 0)
	{
		Nokia5110_ClearBuffer();
		
		if(GameLevel > Level_15)
		{
			Nokia5110_Clear();
			Nokia5110_SetCursor(1, 1);
			Nokia5110_OutString("Well Done");
			//Nokia5110_SetCursor(1, 2);
			//Nokia5110_OutString("Nice try,");
			Nokia5110_SetCursor(1, 3);
			Nokia5110_OutString("Total Score");
			Nokia5110_SetCursor(2, 4);
			Nokia5110_OutUDec(Score);
			return;
		}
		//Jmup to next level
		Spaceship_Fire_Clear();
		Sprite_Fire_Clear();		
		Nokia5110_Clear();
		Nokia5110_SetCursor(2, 2);
		Nokia5110_OutString("Level ");
		Nokia5110_OutUDec(GameLevel + 1);
		Delay5ms(50);              // delay 5 sec at 50 MHz
		
		Play_Next_Level(GameLevel);
		GameLevel++;
	}
}

bool Game_Over(void)
{
	if(NumberOfTrials <= 0)
	{
		Delay5ms(50);              // delay 5 sec at 50 MHz
		Nokia5110_Clear();
		Nokia5110_SetCursor(1, 1);
		Nokia5110_OutString("GAME OVER");
		Nokia5110_SetCursor(1, 2);
		Nokia5110_OutString("Nice try,");
		Nokia5110_SetCursor(1, 3);
		Nokia5110_OutString("Total Score");
		Nokia5110_SetCursor(2, 4);
		Nokia5110_OutUDec(Score);
		return true;
	}
	else
	{
		return false;
	}
}

void ShieldGraphicsPtr_init(void)
{
	ShieldGraphicsPtr[0] = Graphics_Bunker0;
	ShieldGraphicsPtr[1] = Graphics_Bunker1;
	ShieldGraphicsPtr[2] = Graphics_Bunker2;
	ShieldGraphicsPtr[3] = Graphics_Bunker3;
}

bool Spcaeship_Alive(void)
{
	GraphicsElement * SpriteFireList;
	GraphicsElement * SpaceshipData;
	unsigned char SpriteFireNumber;
	unsigned char i;
	unsigned char X,Y;
	
	SpriteFireList = Sprite_Fire_List();
	SpriteFireNumber = Sprites_Fire_Number();
	SpaceshipData = Spaceship_Data();
	
	for(i = 0; i < SpriteFireNumber; i++)
	{
		if(SpriteFireList[i].Alive)
		{
			if((SpriteFireList[i].X < (SpaceshipData->X + SpaceshipData->Width)) && ((SpriteFireList[i].X + SpriteFireList[i].Width)> SpaceshipData->X))
			{
				if(SpriteFireList[i].Y > (SpaceshipData->Y - SpaceshipData->Height))
				{
					// Spaceship is destroied 
					SpaceshipData->Alive = false;
					SpriteFireList[i].Alive = false;
					X = (SpaceshipData->X + (SpaceshipData->Width >> 1))- (Graphics_BigExplosion_Width() >> 1);
					Y = (SpaceshipData->Y - (SpaceshipData->Height >> 1)) + (Graphics_BigExplosion_Height() >> 1);
					Nokia5110_PrintBMP(X, Y, Graphics_BigExplosion0(), 0);
					Nokia5110_DisplayBuffer();
					Sound_Explosion();
					return false;
				}
			}
		}
	}
	return true;
}

void Sprites_Life(void)
{
	GraphicsElement * SpaceshipFireList;
	GraphicsElement * SpriteList;
	unsigned char SpaceshipFireNumber;
	unsigned char SpriteNumber;
	unsigned char i, j;
	unsigned char X,Y;
	
	SpaceshipFireList = Spaceship_Fire_List();
	SpriteList = Sprite_List();
	SpaceshipFireNumber = Spaceship_Fire_Number();
	SpriteNumber = Sprites_Number();
	
	for(i = 0; i < SpaceshipFireNumber; i++)
	{
		if(SpaceshipFireList[i].Alive)
		{
			for(j = 0; j < SpriteNumber; j++)
			{
				if(SpriteList[j].Alive)
				{
					if(((SpaceshipFireList[i].X + SpaceshipFireList[i].Width) > SpriteList[j].X) && (SpaceshipFireList[i].X < (SpriteList[j].X + SpriteList[j].Width)))
					{
						if((SpaceshipFireList[i].Y - SpaceshipFireList[i].Height) < SpriteList[j].Y )
						{
							// A sprite was fired
							SpriteList[j].Alive = false;
							SpaceshipFireList[i].Alive = false;
							Sprite_Is_Killed();
							Score_Update();
							X = (SpriteList[j].X + (SpriteList[j].Width >> 1))- (Graphics_BigExplosion_Width() >> 1);
							Y = (SpriteList[j].Y - (SpriteList[j].Height >> 1)) + (Graphics_BigExplosion_Height() >> 1);
							Nokia5110_PrintBMP(X, Y, Graphics_SmallExplosion0(), 0);
							Nokia5110_DisplayBuffer();
							Sound_Killed();
						}
					}
				}
			}
		}
	}
}

void Spcaeship_Shield_Status(void)
{
	GraphicsElement * SpriteFireList;
	GraphicsElement * SheildData;
	unsigned char SpriteFireNumber;
	unsigned char i;
	unsigned char X,Y;
	static unsigned char ShieldLifeLevel = 1;
	
	SpriteFireList = Sprite_Fire_List();
	SpriteFireNumber = Sprites_Fire_Number();
	SheildData = Spaceship_Shield_Data();
	
	for(i = 0; i < SpriteFireNumber; i++)
	{
		if(SpriteFireList[i].Alive)
		{
			if((SpriteFireList[i].X < (SheildData->X + SheildData->Width)) && ((SpriteFireList[i].X + SpriteFireList[i].Width)> SheildData->X))
			{
				if(SpriteFireList[i].Y > (SheildData->Y - SheildData->Height))
				{
					SpriteFireList[i].Alive = false;
					X = (SheildData->X + (SheildData->Width >> 1))- (Graphics_SmallExplosion_Width() >> 1);
					Y = (SheildData->Y - (SheildData->Height >> 1)) + (Graphics_SmallExplosion_Height() >> 1);
					
					SheildData->Graphic = (*ShieldGraphicsPtr[ShieldLifeLevel])();
					Nokia5110_PrintBMP(X, Y, SheildData->Graphic, 0);
					ShieldLifeLevel++;
					Nokia5110_DisplayBuffer();
					
					if(ShieldLifeLevel >= 4)
					{
						SheildData->Alive = false;
					}	
				}
			}
		}
	}
}

// You can use this timer only if you learn how it works
void Timer2_Init(unsigned long period)
{ 
  unsigned long volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
  TimerCount = 0;
  Semaphore = 0;
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}

void Timer2A_Handler(void)
{ 
  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
  TimerCount++;
  Semaphore = 1; // trigger
}



void Delay5ms(unsigned long count)
{
	unsigned long volatile time;
  
	while(count>0)
	{
    time = 36362;  // 0.1sec at 80 MHz
    while(time)
		{
	  	time--;
    }
    count--;
  }
}

void Delay100ms(unsigned long count)
{
	unsigned long volatile time;
  
	while(count>0)
	{
    time = 727240;  // 0.1sec at 80 MHz
    while(time)
		{
	  	time--;
    }
    count--;
  }
}

  /*Nokia5110_PrintBMP(0, ENEMY10H - 1, SmallEnemy10PointA, 0); //ENEMY10H
  Nokia5110_PrintBMP(16, ENEMY10H - 1, SmallEnemy20PointA, 0);
  Nokia5110_PrintBMP(32, ENEMY10H - 1, SmallEnemy20PointA, 0);
  Nokia5110_PrintBMP(48, ENEMY10H - 1, SmallEnemy30PointA, 0);
  Nokia5110_PrintBMP(64, ENEMY10H - 1, SmallEnemy30PointA, 0);
  Nokia5110_DisplayBuffer();     // draw buffer

  Delay100ms(50);              // delay 5 sec at 50 MHz

  Nokia5110_Clear();
  Nokia5110_SetCursor(1, 1);
  Nokia5110_OutString("GAME OVER");
  Nokia5110_SetCursor(1, 2);
  Nokia5110_OutString("Nice try,");
  Nokia5110_SetCursor(1, 3);
  Nokia5110_OutString("Earthling!");
  Nokia5110_SetCursor(2, 4);
  Nokia5110_OutUDec(1234);*/

