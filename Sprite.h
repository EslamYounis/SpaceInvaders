// Author: Eslam Younis
// Date: 22/7/2016

#ifndef __SPRITE_H_
#define __SPRITE_H_

#include "DataType.h"

enum GameLevel 
{
	Level_1,
	Level_2,
	Level_3,
	Level_4,
	Level_5,
	Level_6,
	Level_7,
	Level_8,
	Level_9,
	Level_10,
	Level_11,
	Level_12,
	Level_13,
	Level_14,
	Level_15
};

void Sprite_Init(void);
void Sprite_Move(void);
void Sprite_Fire(void);
void Sprite_Fire_MoveDown(void);
void Sprite_Fire_Speed(unsigned char NewSpeed);
void Sprite_Is_Killed(void);
void Sprite_Fire_Clear(void);

GraphicsElement * Sprite_List(void);
GraphicsElement * Sprite_Fire_List(void);

unsigned char Sprites_Number(void);
unsigned char Sprites_Fire_Number(void);
unsigned char Sprites_Alive_Count(void);


void Play_Next_Level(const unsigned char level);

#endif
