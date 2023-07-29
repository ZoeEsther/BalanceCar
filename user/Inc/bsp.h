/**********************************************************************
文 件 名:  	bsp.h
作    者:   樊一鸣
完成日期:   2020.04.15
***********************************************************************/
#ifndef _BSP_H
#define _BSP_H

#include "stm32f10x_gpio.h"






#define LED1On		GPIO_ResetBits(GPIOA,GPIO_Pin_3)
#define LED1Off		GPIO_SetBits(GPIOA,GPIO_Pin_3)

//#define LED2On		GPIO_ResetBits(GPIOA,GPIO_Pin_12) // 亮
//#define LED2Off		GPIO_SetBits(GPIOA,GPIO_Pin_12)    //灭



void BspInit(void);




#endif

