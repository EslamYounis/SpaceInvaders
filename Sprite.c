// Author: Eslam Younis
// Date: 22/7/2016

#include "Nokia5110.h"
//#include "PLL.h"
#include "stdio.h"
#include "Sprite.h"
#include "Graphics.h"
#include "stdbool.h"	
#include "Random.h"

#define NumberofFireNodes			15

enum Move {middle, left, right};
enum Move direction;

// Functoins Declaration 
static void SpriteDraw(void);
const unsigned char * (*GraphicEnemyPtr[3])(void);
unsigned char  (*GraphicEnemySizePtr[6])(void);
static void DifficultyLevel_1(const unsigned char Level);
static void DifficultyLevel_2(const unsigned char Level);
static void DifficultyLevel_3(const unsigned char Level);
static void TheMonster(const unsigned char Level);
static void MonsterMove(void);
static void SpriteMove(void);

// Golbal Variables 
static GraphicsElement SpriteList[10];
static GraphicsElement SpriteFireList[NumberofFireNodes];
static unsigned long SpriteFirePosition;
static unsigned char SpriteFireSpeed;
static unsigned char NumberofSpritesAlive;
static unsigned char NumberofSprites;

void Sprite_Init()
{
	GraphicEnemyPtr[0] = Graphics_SmallEnemy30PointA;
	GraphicEnemyPtr[1] = Graphics_SmallEnemy10PointA;
	GraphicEnemyPtr[2] = Graphics_SmallEnemy20PointA;
}

void Play_Next_Level(const unsigned char Level)
{
	if(Level <= Level_4)
	{
		DifficultyLevel_1(Level);
	}
	else if(Level <= Level_8)
	{
		DifficultyLevel_2(Level);
	}
	else if(Level <= Level_14)
	{
		DifficultyLevel_3(Level);
	}
	else if(Level == Level_15)
	{
		TheMonster(Level);
	}
}

static void DifficultyLevel_1(const unsigned char Level)
{
	unsigned char i;
	unsigned long RandomNumber = 0;
	
	direction = right;
	SpriteFireSpeed = 100;
	NumberofSprites = 5;
	NumberofSpritesAlive = NumberofSprites;
	
	for(i = 0; i < 5; i++)
	{
		if(Level == Level_4)
		{
			RandomNumber = Random32();
		}
		SpriteList[i].Width =  Graphics_Enemy30_Width();// 16;
		SpriteList[i].Height = Graphics_Enemy30_Height();
		SpriteList[i].X = 2 + i*SpriteList[i].Width;
		SpriteList[i].Y = SpriteList[i].Height;
		SpriteList[i].Graphic = (*GraphicEnemyPtr[(Level + RandomNumber)%3])();
		SpriteList[i].Alive = true;
		SpriteList[i].Threshold = 0;
	}
	SpriteDraw();
}

static void DifficultyLevel_2(const unsigned char Level)
{
	unsigned char i;
	unsigned long RandomNumber = 0;
	
	direction = right;
	SpriteFireSpeed = 75;
	NumberofSprites = 7;
	NumberofSpritesAlive = NumberofSprites;
	
	if(Level == Level_8)
	{
		SpriteFireSpeed = 50;
	}
	
	for(i = 0; i < 5; i++)
	{
		if(Level == Level_8)
		{
			RandomNumber = Random32();
		}
		SpriteList[i].Width =  Graphics_Enemy30_Width();// 16;
		SpriteList[i].Height = Graphics_Enemy30_Height();
		SpriteList[i].X = 2 + i*SpriteList[i].Width;
		SpriteList[i].Y = SpriteList[i].Height;
		SpriteList[i].Graphic = (*GraphicEnemyPtr[((Level - Level_4) + RandomNumber)%3])();
		SpriteList[i].Alive = true;
		SpriteList[i].Threshold = 0;
	}
	
	RandomNumber = 0;
	
	for(i = 5; i < 7; i++)
	{
		if(Level == Level_8)
		{
			RandomNumber = Random32();
		}
		SpriteList[i].Width =  Graphics_Enemy30_Width();// 16;
		SpriteList[i].Height = Graphics_Enemy30_Height();
		SpriteList[i].X = 2 + 8 + 16 + (i - 5)*SpriteList[i].Width;
		SpriteList[i].Y = 11 + SpriteList[i].Height;
		SpriteList[i].Graphic = (*GraphicEnemyPtr[((Level - Level_4) + RandomNumber)%3])();
		SpriteList[i].Alive = true;
		SpriteList[i].Threshold = 0;
	}
	//NumberofSprites = 7;
	SpriteDraw();
}

static void DifficultyLevel_3(const unsigned char Level)
{
	unsigned char i;
	unsigned long RandomNumber = 0;
	
	direction = right;
	SpriteFireSpeed = 50;
	NumberofSprites = 10;
	NumberofSpritesAlive = NumberofSprites;
	
	if(Level == Level_13 || Level == Level_14)
	{
		SpriteFireSpeed = 25;
	}
	
	for(i = 0; i < 5; i++)
	{
		if(Level >= Level_12)
		{
			RandomNumber = Random32();
		}
		SpriteList[i].Width =  Graphics_Enemy30_Width();// 16;
		SpriteList[i].Height = Graphics_Enemy30_Height();
		SpriteList[i].X = 2 + i*SpriteList[i].Width;
		SpriteList[i].Y = SpriteList[i].Height;
		SpriteList[i].Graphic = (*GraphicEnemyPtr[((Level - Level_9) + RandomNumber)%3])();
		SpriteList[i].Alive = true;
		SpriteList[i].Threshold = 0;
	}
	
	RandomNumber = 0;
	
	for(i = 5; i < 10; i++)
	{
		if(Level >= Level_12)
		{
			RandomNumber = Random32();
		}
		SpriteList[i].Width =  Graphics_Enemy30_Width();// 16;
		SpriteList[i].Height = Graphics_Enemy30_Height();
		SpriteList[i].X = 2 + (i - 5)*SpriteList[i].Width;
		SpriteList[i].Y = 11 + SpriteList[i].Height;
		SpriteList[i].Graphic = (*GraphicEnemyPtr[((Level - Level_9)+ RandomNumber)%3])();
		SpriteList[i].Alive = true;
		SpriteList[i].Threshold = 0;
	}
	//NumberofSprites = 7;
	SpriteDraw();
}

static void TheMonster(const unsigned char Level)
{
	direction = right;
	SpriteFireSpeed = 10;
	NumberofSprites = 1;
	NumberofSpritesAlive = NumberofSprites;
	
	SpriteList[0].Width =  Graphics_Monster_Width();
	SpriteList[0].Height = Graphics_Monster_Height();
	SpriteList[0].X = Graphics_Monster_Width();
	SpriteList[0].Y = Graphics_Monster_Height() + 2;
	SpriteList[0].Graphic = Graphics_Monster();
	SpriteList[0].Alive = true;
	SpriteList[0].Threshold = 0;
	
	SpriteDraw();
}

void Sprite_Fire_Speed(unsigned char NewSpeed)
{
	SpriteFireSpeed = NewSpeed;
}

static void SpriteDraw()
{
	unsigned char i;
		
	//Nokia5110_PrintBMP(20, 20, Graphics_Monster(), 0);
	//Nokia5110_DisplayBuffer();
	
	for(i = 0; i < NumberofSprites; i++)
	{
		if(SpriteList[i].Alive)
		{
			Nokia5110_PrintBMP(SpriteList[i].X, SpriteList[i].Y, SpriteList[i].Graphic, SpriteList[i].Threshold);
			Nokia5110_DisplayBuffer();
		}
	}
}

void Sprite_Move()
{
	if(NumberofSprites == 1)
	{
		MonsterMove();
	}
	else
	{
		SpriteMove();
	}
}

void SpriteMove(void)
{
	unsigned char i;
	
	if(direction == right)
	{
		if((SpriteList[4].X + SpriteList[4].Width + 1) <= 84)
		{
			for(i = 0; i < NumberofSprites; i++)
			{
				SpriteList[i].X++;
			} 
		}
		else 
		{
			direction = left;
		}
	}
	
	if(direction == left)
	{
		if((SpriteList[0].X - 1) >= 0)
		{
			for(i = 0; i < NumberofSprites; i++)
			{
				SpriteList[i].X--;
			}
		}
		else
		{
			for(i = 0; i < NumberofSprites; i++)
			{
				SpriteList[i].X++;
			} 
			direction = right;
		}
	}

	SpriteDraw();
}

void MonsterMove(void)
{
	unsigned char i;
	
	if(direction == right)
	{
		if((SpriteList[0].X + SpriteList[0].Width + 1) <= 84)
		{
			for(i = 0; i < NumberofSprites; i++)
			{
				SpriteList[i].X++;
			} 
		}
		else 
		{
			direction = left;
		}
	}
	
	if(direction == left)
	{
		if((SpriteList[0].X - 1) >= 0)
		{
			for(i = 0; i < NumberofSprites; i++)
			{
				SpriteList[i].X--;
			}
		}
		else
		{
			for(i = 0; i < NumberofSprites; i++)
			{
				SpriteList[i].X++;
			} 
			direction = right;
		}
	}
	
	SpriteDraw();
}

void Sprite_Fire(void)
{
	unsigned char i, j;
	unsigned long RandomNumber;
	GraphicsElement temp;

	for(i = 0; i < NumberofSprites; i++)
	{
		if(SpriteList[i].Alive)
		{
			RandomNumber = Random32()%SpriteFireSpeed;
			if(RandomNumber == 2)
			{					
				SpriteFirePosition = SpriteList[i].X + (SpriteList[i].Width/2);
				temp.X = SpriteFirePosition;
				temp.Y = SpriteList[i].Y + Graphics_Laser_Height() - 2;
				temp.Graphic = Graphics_Laser0();
				temp.Width = Graphics_Laser_Width();
				temp.Height = Graphics_Laser_Height(); 
				temp.Threshold = 0;
				temp.Alive = true;
				
				for(j = 0; j < NumberofFireNodes; j++)
				{
					if(SpriteFireList[j].Alive != true)
					{
						SpriteFireList[j] = temp;
						break;
					}
				}
			}
		}
	}
}

void Sprite_Fire_MoveDown(void)
{
	unsigned char i;
	
	for(i = 0; i < NumberofFireNodes; i++)
	{
		if(SpriteFireList[i].Alive == true)
		{
			if(SpriteFireList[i].Y >= 48)
			{
				SpriteFireList[i].Alive = false;
			}
			else
			{
				SpriteFireList[i].Y = SpriteFireList[i].Y + 2;
			}
		}
	}
	
	for(i = 0; i < NumberofFireNodes; i++)
	{
		if(SpriteFireList[i].Alive == true)
		{
			Nokia5110_PrintBMP(SpriteFireList[i].X, SpriteFireList[i].Y, SpriteFireList[i].Graphic, SpriteFireList[i].Threshold); 
		}
	}
}

void Sprite_Fire_Clear(void)
{
	unsigned char i;
	
	for(i = 0; i < NumberofFireNodes; i++)
	{
		SpriteFireList[i].Alive = false;
	}
}

GraphicsElement * Sprite_List(void)
{
	return SpriteList;
}

GraphicsElement * Sprite_Fire_List(void)
{
	return SpriteFireList;
}

unsigned char Sprites_Number(void)
{
	return NumberofSprites;
}

unsigned char Sprites_Fire_Number(void)
{
	return NumberofFireNodes;
}

void Sprite_Is_Killed(void)
{
	NumberofSpritesAlive--;
}

unsigned char Sprites_Alive_Count(void)
{
	return NumberofSpritesAlive;
}
