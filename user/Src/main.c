/**********************************************************************

文 件 名: 	main.c
作    者:   樊一鸣、宁婷婷
完成日期:   2020.05.10
概		要: 	

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

	
//秒级任务
void SecTask()
{
	if(SoftTimer[0])return;
	else{
		SoftTimer[0] = 1000;
	}
	g_RunTime++; 			// 记录运行时间
	g_BatVolt = GetBatVoltage(); // 读取电池电压
	
  if(g_BatVolt/100.0 < 6.8)
		{
			BEEP_ON;		//电池电量不足时  报警
			delay_ms(1000);
			BEEP_OFF;
			LED2(ON);	//电池电量不足时,LED灯
			delay_ms(1000);
			Play_Sound("电池电量不足");
		}
}

/*
	主函数入门，另外，控制功能函数在stm32f10x_it.c执行文件的滴答定时器中断服务函数里循环执行。
*/
int main(void)
{	
	BspInit();				//初始化BSP
	
	PIDInit(); 				//初始化PID
	
	CarUpstandInit(); 	//初始化系统参数
	
	SysTick_Init();			//初始化定时器	
	
	ShowHomePageInit();  //OLED显示页面初始化
  
	while (1)
	{
		
		SecTask();			//秒级任务
		
		
		if(SoftTimer[1] == 0)
		{// 每隔50ms 执行一次
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
				if(IsUltraOK())UltraControl(0);	//超声波跟随模式
	 		}
			if(g_CarRunningMode == ULTRA_AVOID_MODE){
				if(IsUltraOK())UltraControl(1);	//超声波避障模式
	 		}
	
		}	
   
		
	}
}


/******************* ************/


