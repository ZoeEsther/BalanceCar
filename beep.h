#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"

//与LED的宏定义类似，通过简单的代码来指示蜂鸣器的开关。
#define BEEP_ON  GPIO_SetBits( GPIOB, GPIO_Pin_12 )
#define BEEP_OFF GPIO_ResetBits( GPIOB, GPIO_Pin_12 )

void BEEP_Init(void);

#endif