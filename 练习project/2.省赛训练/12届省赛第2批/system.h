#ifndef __SYSTEM__H__
#define __SYSTEM__H__

typedef unsigned char u8;
typedef char s8;
typedef unsigned int u16;
typedef int s16;

#include <STC15F2K60S2.H>
#include <INTRINS.H>
#include "drive.h"
#include "timer.h"


#include "I2C.h"


#include "key.h"
#include "smg.h"




typedef struct 
{
	u8 b0:1;
	u8 b1:1;
	u8 b2:1;
	u8 b3:1;
	u8 b4:1;
	u8 b5:1;
	u8 b6:1;
	u8 b7:1;
}bits;

typedef union
{
	u8 hex;
	bits b;
}hextobin;

#endif