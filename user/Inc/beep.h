/**********************************************************************
文 件 名: 	beep.h
作    者: 宁婷婷
完成日期:  2020.05.15
概		要：蜂鸣器
***********************************************************************/
#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"

//与LED的宏定义类似，通过简单的代码来指示蜂鸣器的开关。
#define BEEP_ON  GPIO_SetBits( GPIOB, GPIO_Pin_12 )
#define BEEP_OFF GPIO_ResetBits( GPIOB, GPIO_Pin_12 )

void BEEP_Init(void);

#endif



