// Author: Eslam Younis
// Date: 24/7/2016

#ifndef __DATATYPE_H_
#define __DATATYPE_H_

#include "stdbool.h"

//struct node
//{
//	unsigned long X, Y;
//	unsigned long X, Y;
//	bool Alive;
//};
//typedef struct node FireNode;

typedef struct 
{
	unsigned char X, Y, Width, Height, Threshold; 
	const unsigned char *Graphic;
	bool Alive;
}GraphicsElement;

#endif
