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
///////按键为 PA11


void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//引延肎PIO_Init函数结构体声明
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //设置端口使能时钟
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		    
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;   //配置引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);      //引脚初始化
	GPIO_SetBits(GPIOA,GPIO_Pin_11);           //高电平输出
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11); //选择管脚GPIOA的11管脚作为外部中断线
	
	//EXTI_DeInit();                   //将外设EXTI寄存器重设为缺省值
  EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;   //设置EXTI线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;    //设置输入线路下降沿为终端请求
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;  //使能
	EXTI_Init(&EXTI_InitStructure);
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	NVIC_InitStructure.NVIC_IRQChannel =EXTI15_10_IRQn ;            //选择中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            //响应优先级
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
//	     printf("PA11引脚终端开启！");
}
