#ifndef __system_h__
#define __system_h__

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

#include <STC15F2K60S2.H>
#include <intrins.h>
#include "drive.h"

#include "i2c.h"
#include "ds1302.h"

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
	bits b;
	u8 hex;
}hextobin;

#endif