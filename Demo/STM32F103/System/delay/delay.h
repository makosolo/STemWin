#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"

__asm  int delay_us(volatile u32 usec);
void delay_ms(volatile u32 msec);

#endif





























