//���Ƚ��з�������
#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

#define ON  0
#define OFF 1

/* ������κ꣬ʹ�÷�������C++�е��������� */
/* ����ʹ�õĵ�Ƭ��������LED�ƣ�����һ���ǵ�Դ�ơ�
�����������Ǹ��û�ʹ�õģ���˶��������ƿ��صĺꡣ */
#define LED2(a) if(a) GPIO_SetBits( GPIOA, GPIO_Pin_8 ); else GPIO_ResetBits( GPIOA, GPIO_Pin_8)

void LED_Init(void);

#endif