/**********************************************************************
文 件 名: 	display.c
作    者:   樊一鸣
完成日期:   2020.05.5
概		要: 　ＯＬＥＤ屏显示页面设计
***********************************************************************/

#include "stdio.h"
#include "oled.h"
#include "bmp.h"
#include "control.h"
#include "ultrasonic.h"
#include "mpu6050.h"
#include "common.h"
#include "manage.h"
#include "bsp.h"

extern unsigned short BatVol;



/*
	显示logo
*/
void ShowHomePageInit(void)
{
	unsigned char buff0[32]={0};
	unsigned char buff1[32]={0};
	unsigned char buff2[32]={0};
	unsigned char buff3[32]={0};
	unsigned char buff4[32]={0};
	unsigned char buff5[32]={0};
	snprintf((char*)buff0, 21, "	 Balancing car(1)    ");OLED_ShowString(0,0,buff0); 
	snprintf((char*)buff1, 21, "17121873        FYM    ");OLED_ShowString(0,2,buff1);  ///小组名
	snprintf((char*)buff2, 21, "17121879        ZJJ    ");OLED_ShowString(0,3,buff2); 
	snprintf((char*)buff3, 21, "17122029        WYM    ");OLED_ShowString(0,4,buff3); 
	snprintf((char*)buff4, 21, "17122359        NTT    ");OLED_ShowString(0,5,buff4); 
	snprintf((char*)buff5, 21, "17122329        WQY    ");OLED_ShowString(0,6,buff5); 
	delay_ms(1000);
	OLED_Clear();
}


/*
	oled主页刷新函数
	分六步刷新，避免一次刷新时间过长
*/

void ShowHomePage1(void)
{
				unsigned char buff[32]={0};
				static char step1= 0;
	      
//				 g_keytemp=0;
//				
				step1++;
				if(step1 >= 6)step1 = 0;

				//分步执行，缩短单次刷屏时间
				if(step1 == 0)    { OLED_ShowString(0, 0, "Mode: Complementary  ");}

				if(step1 == 1)    {
				   if(IsUltraOK())
						snprintf((char*)buff, 21,  "Distance:  %d(cm)       ", Distance);
				   else
						snprintf((char*)buff, 21,  "Distance:  %s(cm)       ", "xx");
		
						OLED_ShowString(0, 1, buff);
						}

				if(step1 == 2){
						snprintf((char*)buff, 21,  "EncoLeft:  %d         ",g_s16LeftMotorPulse);
						OLED_ShowString(0, 2, buff);
							}
				if(step1== 3){
						snprintf((char*)buff, 21, "EncoRight: %d         ",g_s16RightMotorPulse);
						OLED_ShowString(0, 3, buff);
							}
	
				if(step1 == 4){
						snprintf((char*)buff, 21, "Angle:     %0.1f      ", g_fCarAngle);
						OLED_ShowString(0, 4, buff);
							}
				if(step1 == 5){
						snprintf((char*)buff, 21, "Battery:   %0.1f(V)      ", g_BatVolt/100.0);
						OLED_ShowString(0, 5, buff);		
							}
					
}

void ShowHomePage2(void)
{  
	
  float temp;
	unsigned char buff[32]={0};
	static char step2 = 0;

	step2++;
	if(step2 >= 4)step2 = 0;

	//分步执行，缩短单次刷屏时间
	if(step2 == 0)                       {OLED_ShowString(0, 0, "balancing car");}
	if(step2 == 1)                       {snprintf((char*)buff, 21, "Distance:  %s(cm)       ", "xx");
		                                   OLED_ShowString(0, 2, buff);}
	if(step2 == 2)                       {snprintf((char*)buff, 21, "Battery:   %0.1f(V)      ", g_BatVolt/100.0);
	                                     OLED_ShowString(0, 4, buff);	}
	if(step2 == 3){
		temp=g_BatVolt/100.0;   //电池电压值
		if(7.3<temp )                     {snprintf((char*)buff, 21, "Battery:   100%         ");OLED_ShowString(0, 6, buff);		}
		else if(7.1<temp && temp<7.3)    {snprintf((char*)buff, 21, "Battery:   80%          ");OLED_ShowString(0, 6, buff);		}
		else if(6.9<temp && temp<7.1)    {snprintf((char*)buff, 21, "Battery:   60%          ");OLED_ShowString(0, 6, buff);		}
		else if(6.7<temp && temp<6.9)    {snprintf((char*)buff, 21, "Battery:   40%          ");OLED_ShowString(0, 6, buff);		}
		else if(6.5<temp && temp<6.7)    {snprintf((char*)buff, 21, "Battery:   20%          ");OLED_ShowString(0, 6, buff);		}
		else if(temp<6.5)                {snprintf((char*)buff, 21, "Battery low!!!          ");OLED_ShowString(0, 6, buff);		}
		}
	}
// void OLEDToggle(void)
//{
////	static char oledflag=0;
////	if(oledflag==0)
////	{ 
////		oledflag = 1;
////		ShowHomePage1();
////	}
////	else{
////		oledflag = 0;
////		ShowHomePage2();
////	}
//}




