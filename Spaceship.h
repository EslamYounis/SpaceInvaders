// Author: Eslam Younis
// Date: 22/7/2016

#ifndef __SPACESpaceship_H_
#define __SPACESpaceship_H_

#include <stdbool.h>
#include "DataType.h"

void Spaceship_Init(void);
void Spaceship_Shield_Init(void);
void Spaceship_Fire_MoveUp(void);
void Spaceship_Fire(void);
void Spaceship_Move(void);
void Spaceship_Change_Weapon(void);
void Spaceship_Shield(void);
void Spaceship_Fire_Clear(void);
	
GraphicsElement * Spaceship_Fire_List(void);
GraphicsElement * Spaceship_Data(void);
GraphicsElement * Spaceship_Shield_Data(void);

unsigned char Spaceship_Fire_Number(void);
unsigned long Spaceship_Position(void);
#endif 
