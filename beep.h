#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"

//��LED�ĺ궨�����ƣ�ͨ���򵥵Ĵ�����ָʾ�������Ŀ��ء�
#define BEEP_ON  GPIO_SetBits( GPIOB, GPIO_Pin_12 )
#define BEEP_OFF GPIO_ResetBits( GPIOB, GPIO_Pin_12 )

void BEEP_Init(void);

#endif