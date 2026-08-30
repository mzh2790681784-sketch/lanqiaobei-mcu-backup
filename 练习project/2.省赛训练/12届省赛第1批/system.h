#ifndef __SYSTEM_H__
#define __SYSTEM_H__

typedef unsigned char u8;
typedef char s8;
typedef unsigned int u16;
typedef int s16;
typedef unsigned long u32;
typedef long s32;

#include <STC15F2K60S2.H>
#include <INTRINS.h>
#include "drive.h"

#include "SMG.h"
#include "KEY.h"

#include "I2C.h"
#include "onewire.h"

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