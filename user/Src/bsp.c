/**********************************************************************

�� �� ��:  	bsp.c
��    ��:   ��һ��
�������:   2020.04.15


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
	ʹ��SWD�� ʧ��JTAG
	PB3,PB4,PA15����ͨIOʹ�ã�����ʧ��JTAG�� 
*/
void SWDConfig(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	
}




/*
	��ȡ��Ƭ��12�ֽ� ID
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
	������������
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
	bsp��ʼ��
*/
void BspInit(void)
{
	SWDConfig();
	
	ADCInit();				//ADC��ʼ��

//	USART1Init();			//����1��ʼ��-�װ�Ԥ�����ؼ�������
	USART1Init();      //���� 2��ʼ��--����ģ��
	USART3Init(0);			//����3��ʼ��-��������


	TIM1_Cap_Init();		//TIM1��ʼ��-���ڳ��������湦��
	TIM3_PWM_Init(); 		//PWM��ʼ��
	TIM2_Encoder_Init();	//TIM2���������ʼ��-���ڲ���
	TIM4_Encoder_Init();	//TIM4���������ʼ��-���ڲ���
	
	i2cInit();	 			//I2C��ʼ��

	OLED_Init();			//OLED��ʼ��
	OLED_Init();			//OLED��ʼ��
	
	MPU6050_Init();		    //MPU6050��ʼ��
	
	LED_Init();    //��ʼ����
	BEEP_Init();		//��ʼ��������
	
	UltraSelfCheck();							//	����ģ�鿪���Լ�
	
	delay_ms(500);			//��ʱ0.5s���ȴ�����ģ������
	Uart3SendStr("\r\nAT+BAUD8\r\n"); //�����������ڲ�����Ϊ115200 ( ԭ������9600 ) 
	USART3Init(1);					//����UART3������Ϊ115200
	delay_ms(20);
	SetBlueToothName();		//��������ģ������
}

