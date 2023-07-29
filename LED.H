//首先进行防卫声明
#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

#define ON  0
#define OFF 1

/* 定义带参宏，使用方法类似C++中的内联函数 */
/* 本次使用的单片板有三个LED灯，其中一个是电源灯。
另外两个才是给用户使用的，因此定义两个灯开关的宏。 */
#define LED2(a) if(a) GPIO_SetBits( GPIOA, GPIO_Pin_8 ); else GPIO_ResetBits( GPIOA, GPIO_Pin_8)

void LED_Init(void);

#endif