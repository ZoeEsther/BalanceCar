/**********************************************************************

�� �� ��: 	main.c
��    ��:   ��һ����������
�������:   2020.05.10
��		Ҫ: 	

***********************************************************************/
#include "stm32f10x.h"
#include "usart.h"
#include "SysTick.h"
#include "control.h"
#include "debug.h"
#include "communicate.h"
#include "dataflash.h"
#include "common.h"
#include "motor.h"
#include "display.h"
#include "bsp.h"
#include "ADC.h"
#include "ultrasonic.h"
#include "manage.h"
#include "led.h"
#include "beep.h"
#include "syn6288.h"

	
//�뼶����
void SecTask()
{
	if(SoftTimer[0])return;
	else{
		SoftTimer[0] = 1000;
	}
	g_RunTime++; 			// ��¼����ʱ��
	g_BatVolt = GetBatVoltage(); // ��ȡ��ص�ѹ
	
  if(g_BatVolt/100.0 < 6.8)
		{
			BEEP_ON;		//��ص�������ʱ  ����
			delay_ms(1000);
			BEEP_OFF;
			LED2(ON);	//��ص�������ʱ,LED��
			delay_ms(1000);
			Play_Sound("��ص�������");
		}
}

/*
	���������ţ����⣬���ƹ��ܺ�����stm32f10x_it.cִ���ļ��ĵδ�ʱ���жϷ�������ѭ��ִ�С�
*/
int main(void)
{	
	BspInit();				//��ʼ��BSP
	
	PIDInit(); 				//��ʼ��PID
	
	CarUpstandInit(); 	//��ʼ��ϵͳ����
	
	SysTick_Init();			//��ʼ����ʱ��	
	
	ShowHomePageInit();  //OLED��ʾҳ���ʼ��
  
	while (1)
	{
		
		SecTask();			//�뼶����
		
		
		if(SoftTimer[1] == 0)
		{// ÿ��50ms ִ��һ��
			SoftTimer[1] = 20;
//			ResponseIMU();			
//			DebugService();			
			Parse(Uart3Buffer);

		}			
  	
		if(SoftTimer[2] == 0)
		{
			SoftTimer[2] = 20;
			
			if(StatusFlag==0) ShowHomePage1();
      if(StatusFlag==1) ShowHomePage2();

			 Read_Distane();
	
			if(g_CarRunningMode == ULTRA_FOLLOW_MODE){
				if(IsUltraOK())UltraControl(0);	//����������ģʽ
	 		}
			if(g_CarRunningMode == ULTRA_AVOID_MODE){
				if(IsUltraOK())UltraControl(1);	//����������ģʽ
	 		}
	
		}	
   
		
	}
}


/******************* ************/


