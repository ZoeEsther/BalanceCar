/**********************************************************************

文 件 名:  	bsp.c
作    者:   樊一鸣
完成日期:   2020.04.15


***********************************************************************/

#include "stdio.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "common.h"
#include "MPU6050.h"
#include "oled.h"
#include "bsp.h"
#include "ADC.h"
#include "usart.h"
#include "motor.h"
#include "I2C.h"
#include "ultrasonic.h"
#include "syn6288.h"
#include "usart.h"
#include "led.h"
#include "beep.h"


/* 
	使能SWD， 失能JTAG
	PB3,PB4,PA15做普通IO使用（必须失能JTAG） 
*/
void SWDConfig(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	
}




/*
	读取单片机12字节 ID
*/
#define UniqueID_Addr 0x1FFFF7E8U
void GetUniqueID(char* ID)
{
	char i;
	char *p = (char*)UniqueID_Addr; 
	for(i=0; i<12; i++){
		*ID++ = *p++;
	}
}


/*
	设置蓝牙名称
*/
void SetBlueToothName(void)
{
	char temp[32];
	char check;
	GetUniqueID(temp);
	check = XOR_Get(temp, 12);
	sprintf(temp, "\r\nAT+NAMEBalancingCar_voice-%04X\r\n",(int)check);
	Uart3SendStr(temp);
	delay_ms(100);
}


/*
	bsp初始化
*/
void BspInit(void)
{
	SWDConfig();
	
	ADCInit();				//ADC初始化

//	USART1Init();			//串口1初始化-底板预留下载及调试用
	USART1Init();      //串口 2初始化--语音模块
	USART3Init(0);			//串口3初始化-用于蓝牙


	TIM1_Cap_Init();		//TIM1初始化-用于超声波跟随功能
	TIM3_PWM_Init(); 		//PWM初始化
	TIM2_Encoder_Init();	//TIM2正交解码初始化-用于测速
	TIM4_Encoder_Init();	//TIM4正交解码初始化-用于测速
	
	i2cInit();	 			//I2C初始化

	OLED_Init();			//OLED初始化
	OLED_Init();			//OLED初始化
	
	MPU6050_Init();		    //MPU6050初始化
	
	LED_Init();    //初始化灯
	BEEP_Init();		//初始化蜂鸣器
	
	UltraSelfCheck();							//	超声模块开机自检
	
	delay_ms(500);			//延时0.5s，等待蓝牙模块启动
	Uart3SendStr("\r\nAT+BAUD8\r\n"); //配置蓝牙串口波特率为115200 ( 原波特率9600 ) 
	USART3Init(1);					//更改UART3波特率为115200
	delay_ms(20);
	SetBlueToothName();		//配置蓝牙模块名称
}

