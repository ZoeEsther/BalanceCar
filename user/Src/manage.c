/**********************************************************************

文 件 名: 	manage.c
作    者:    樊一鸣
完成日期:   2020.05.01
概		要: 	


***********************************************************************/

#include "manage.h"



const char FirmwareVer[] = "3.33";
const char EEPROMVer[]  = "2.00";
const char MCUVer[] = "STM32F103C8T6";

//系统运行时间计数，，开机后开始计数，每秒增1
unsigned short  g_RunTime = 0;

//电池电压，实际值*100
unsigned short  g_BatVolt=0;

//小车运行模式:遥控模式，超声波避障、超声波跟随模式
unsigned char g_CarRunningMode = CONTROL_MODE;


