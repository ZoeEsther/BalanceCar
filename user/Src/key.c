#include "key.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_it.h"
#include "display.h"
#include "control.h"
#include "common.h"
#include "bsp.h"
#include <stdio.h>
#include "oled.h"
///////����Ϊ PA11


void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//�����GPIO_Init�����ṹ������
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //���ö˿�ʹ��ʱ��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		    
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;   //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);      //���ų�ʼ��
	GPIO_SetBits(GPIOA,GPIO_Pin_11);           //�ߵ�ƽ���
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11); //ѡ��ܽ�GPIOA��11�ܽ���Ϊ�ⲿ�ж���
	
	//EXTI_DeInit();                   //������EXTI�Ĵ�������Ϊȱʡֵ
  EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;   //����EXTI��·Ϊ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;    //����������·�½���Ϊ�ն�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;  //ʹ��
	EXTI_Init(&EXTI_InitStructure);
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	NVIC_InitStructure.NVIC_IRQChannel =EXTI15_10_IRQn ;            //ѡ���ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            //��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
void KEY_Scan(void)
{ 	
//  if(g_keytemp==1)
//	{
//	   if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)
//		 {
//			 delay_ms(500);
//			 if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)
//			 { 
//          ShowHomePage1();
//				  g_keytemp=0;
//			 }				 
//		 }
//	
//	}
//	else if(g_keytemp==0) 
//	     {
//				 
//				 ShowHomePage2();
//			 }
//			
//	     printf("PA11�����ն˿�����");
}
