/**********************************************************************
文 件 名:  	ULTRASONIC_H
作    者:   樊一鸣
完成日期:   2020.04.15
***********************************************************************/

#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

extern int Distance;


void TIM1_Cap_Init(void);	
void Read_Distane(void);
char InfraredDetect(void);
void UltraSelfCheck(void);
int IsUltraOK(void);



#endif

