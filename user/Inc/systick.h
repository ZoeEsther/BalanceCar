/**********************************************************************
文 件 名:  	bsp.c
作    者:   樊一鸣
完成日期:   2020.04.15

***********************************************************************/

#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"


extern unsigned short SoftTimer[5];

void SoftTimerCountDown(void);
void SysTick_Init(void);


#endif /* __SYSTICK_H */

