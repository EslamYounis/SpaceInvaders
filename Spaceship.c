// Author: Eslam Younis
// Date: 21/7/2016

#include "ADC.h"
#include "Spaceship.h"
#include "Nokia5110.h"
#include "Graphics.h"
#include "stdlib.h"

#define NumberofFireNodes		12

static unsigned long Convert(unsigned long sample);
static void SpaceshipDraw(void);
static void SpaceshipShieldDraw(void);

unsigned long ADCdata;    // 12-bit 0 to 4095 sample
unsigned long Position;   // units 0.001 cm
//unsigned long SpaceshipUpPosition;   // units 0.001 cm

//FireNode *HeadPtr;
//FireNode *TailPtr;
//FireNode *tempPtr;

static GraphicsElement SpaceshipFireList[NumberofFireNodes];
static GraphicsElement PlayerSpaceship;
static GraphicsElement PlayerSpaceshipShield;
static unsigned long FirePosition;

void Spaceship_Init(void)
{
	ADC0_Init();
	PlayerSpaceship.X = 33;
	PlayerSpaceship.Y = 47;
	PlayerSpaceship.Graphic = Graphics_PlayerShip0();
	PlayerSpaceship.Width = Graphics_Player_Width();
	PlayerSpaceship.Height = Graphics_Player_Height();
	PlayerSpaceship.Alive = true;
	PlayerSpaceship.Threshold = 0;

	SpaceshipDraw();
}

void Spaceship_Shield_Init(void)
{	
	PlayerSpaceshipShield.X = 33;
	PlayerSpaceshipShield.Y = 47 - Graphics_Player_Height();
	PlayerSpaceshipShield.Graphic = Graphics_Bunker0();
	PlayerSpaceshipShield.Width = Graphics_Bunker_Width();
	PlayerSpaceshipShield.Height = Graphics_Bunker_Height();
	PlayerSpaceshipShield.Alive = true;
	PlayerSpaceshipShield.Threshold = 0;
	
	SpaceshipShieldDraw();
}

static void SpaceshipDraw(void)
{
	if(PlayerSpaceship.Alive)
	{
		Nokia5110_PrintBMP(PlayerSpaceship.X, PlayerSpaceship.Y, PlayerSpaceship.Graphic, PlayerSpaceship.Threshold); // player Spaceship middle bottom
	}
}

static void SpaceshipShieldDraw(void)
{
	if(PlayerSpaceshipShield.Alive)
	{
		Nokia5110_PrintBMP(PlayerSpaceshipShield.X, PlayerSpaceshipShield.Y, PlayerSpaceshipShield.Graphic, PlayerSpaceshipShield.Threshold);
	}
}
void Spaceship_Move(void)
{
	PlayerSpaceship.X = Spaceship_Position();
	SpaceshipDraw();
}


void Spaceship_Shield(void)
{
	SpaceshipShieldDraw();
}
	
GraphicsElement * Spaceship_Data(void)
{
	return &PlayerSpaceship;
}

GraphicsElement * Spaceship_Shield_Data(void)
{
	return &PlayerSpaceshipShield;
}

unsigned long Spaceship_Position(void)
{
	ADCdata = ADC0_In();
	Position = Convert(ADCdata);
	return Position;
}

void Spaceship_Fire(void)
{
	GraphicsElement temp;
	unsigned char i;
	
	FirePosition = (Spaceship_Position() + (Graphics_Player_Width()/2));
	temp.X = FirePosition;
	temp.Y = 39 + 2;
	temp.Graphic = Graphics_Missile0();
	temp.Width = Graphics_Missle_Width();
	temp.Height = Graphics_Missle_Height();
	temp.Alive = true;
	temp.Threshold = 0;
	
	for(i = 0; i < NumberofFireNodes; i++)
	{
		if(SpaceshipFireList[i].Alive != true)
		{
			SpaceshipFireList[i] = temp;
			break;
		}
	}
}

void Spaceship_Fire_MoveUp(void)
{
	unsigned char i;
	
	for(i = 0; i < NumberofFireNodes; i++)
	{
		if(SpaceshipFireList[i].Alive == true)
		{
			if(SpaceshipFireList[i].Y <= SpaceshipFireList[i].Height)
			{
				SpaceshipFireList[i].Alive = false;
			}
			else
			{
				SpaceshipFireList[i].Y = SpaceshipFireList[i].Y - 6;
			}
		}
	}
	
	for(i = 0; i < NumberofFireNodes; i++)
	{
		if(SpaceshipFireList[i].Alive == true)
		{
			Nokia5110_PrintBMP(SpaceshipFireList[i].X, SpaceshipFireList[i].Y, SpaceshipFireList[i].Graphic, SpaceshipFireList[i].Threshold); // player Spaceship middle bottom
		}
	}
}

void Spaceship_Fire_Clear(void)
{
	unsigned char i;
	
	for(i = 0; i < NumberofFireNodes; i++)
	{
		SpaceshipFireList[i].Alive = false;
		
		if(SpaceshipFireList[i].Alive == true)
		{
			Nokia5110_PrintBMP(SpaceshipFireList[i].X, SpaceshipFireList[i].Y, SpaceshipFireList[i].Graphic, SpaceshipFireList[i].Threshold); // player Spaceship middle bottom
		}
	}
}

GraphicsElement * Spaceship_Fire_List(void)
{
	return SpaceshipFireList;
}

void Spaceship_Change_Weapon(void)
{
}

unsigned char Spaceship_Fire_Number(void)
{
	return NumberofFireNodes;
}
//********Convert****************
// Convert a 12-bit binary ADC sample into a 32-bit unsigned
// fixed-point distance (resolution 0.001 cm).  Calibration
// data is gathered using known distances and reading the
// ADC value measured on PE1.  
// Overflow and dropout should be considered 
// Input: sample  12-bit ADC sample
// Output: 32-bit distance (resolution 0.001cm)
unsigned long Convert(unsigned long sample)
{
  //return ((sample*2000)/4096) + 1;  // 
	return ((sample*66)/4096);  // 
}



/*
bool Spaceship_Fire(unsigned long SpaceshipPosition)
{
	FireNode temp;
	unsigned char i;
	//temp	= (FireNode *)malloc(sizeof(FireNode));
	
//	if(temp == NULL)
//	{
	//	return 0;//false;
	//}
	//else
	//{
		temp.Xposition = SpaceshipPosition;
		temp.Yposition = 39;
		temp.previous = NULL;
		temp.next = NULL;
		temp.life = true;

		for(i = 0; i < 8; i++)
		{
			if(SpaceshipFireList[i].life != true)
			{
				SpaceshipFireList[i] = temp;
				break;
			}
		}
		
		if(HeadPtr == NULL)
		{
			HeadPtr = &temp;
			TailPtr = HeadPtr;
			//free(temp);
		}
		else 
		{		
			while(TailPtr->next != NULL)
			{
				TailPtr = TailPtr->next;
			}
			TailPtr->next = &temp;
			temp.previous = TailPtr;
			TailPtr	= &temp;
			//free(temp);
		}
	}*/
	
	/*tempPtr	= HeadPtr;
	
	//Nokia5110_PrintBMP(10, 10, Graphics_Laser0(), 0); // player Spaceship middle bottom
	//Nokia5110_DisplayBuffer();
	
	if(tempPtr != NULL)
	{
		tempPtr->Yposition--;
		Nokia5110_PrintBMP(tempPtr->Xposition, tempPtr->Yposition, Graphics_Laser0(), 0); // player Spaceship middle bottom
	}
	
	while(tempPtr->next != NULL)
	{
		tempPtr->Yposition--;
		Nokia5110_PrintBMP(tempPtr->Xposition, tempPtr->Yposition, Graphics_Laser0(), 0); // player Spaceship middle bottom
		tempPtr = tempPtr->next;
		Nokia5110_DisplayBuffer();
	}
	
	return 1;//true;
}

void Spaceship_Fire_MoveUp(void)
{
	unsigned char i;
	
	for(i = 0; i < 8; i++)
	{
		if(SpaceshipFireList[i].life == true)
		{
			if(SpaceshipFireList[i].Yposition <= Graphics_Laser_Height())
			{
				SpaceshipFireList[i].life = false;
			}
			else
			{
				SpaceshipFireList[i].Yposition = SpaceshipFireList[i].Yposition - 2;
			}
		}
	}
	
	
	for(i = 0; i < 8; i++)
	{
		if(SpaceshipFireList[i].life == true)
		{
			Nokia5110_PrintBMP(SpaceshipFireList[i].Xposition, SpaceshipFireList[i].Yposition, Graphics_Laser0(), 0); // player Spaceship middle bottom
		}
	}
	tempPtr	= HeadPtr;
	
	//Nokia5110_PrintBMP(10, 10, Graphics_Laser0(), 0); // player Spaceship middle bottom
	//Nokia5110_DisplayBuffer();
	
	if(tempPtr != NULL)
	{
		tempPtr->Yposition--;
		Nokia5110_PrintBMP(tempPtr->Xposition, tempPtr->Yposition, Graphics_Laser0(), 0); // player Spaceship middle bottom
	}
	
	while(tempPtr->next != NULL)
	{
		tempPtr->Yposition--;
		Nokia5110_PrintBMP(tempPtr->Xposition, tempPtr->Yposition, Graphics_Laser0(), 0); // player Spaceship middle bottom
		tempPtr = tempPtr->next;
	}
}
*/
