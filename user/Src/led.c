/**********************************************************************
�� �� ��: 	led.c
��    ��: ������
�������:  2020.05.15
��		Ҫ������������
***********************************************************************/
#include "stm32f10x.h"
#include "led.h"

//LED��ʼ������
void LED_Init(void)
{
    
    /* ����һ��GPIO_InitTypeDef���͵Ľṹ�� */
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* ��GPIOA������ʱ�� */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
    
    /* ѡ��Ҫ���Ƶ�GPIOA���ţ�����Ϊ��������������������Ϊ50MHz��*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    
    
    /* �ر�����LED�� */
    GPIO_SetBits( GPIOA, GPIO_Pin_8 );
}



