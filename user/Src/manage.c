/**********************************************************************

�� �� ��: 	manage.c
��    ��:    ��һ��
�������:   2020.05.01
��		Ҫ: 	


***********************************************************************/

#include "manage.h"



const char FirmwareVer[] = "3.33";
const char EEPROMVer[]  = "2.00";
const char MCUVer[] = "STM32F103C8T6";

//ϵͳ����ʱ���������������ʼ������ÿ����1
unsigned short  g_RunTime = 0;

//��ص�ѹ��ʵ��ֵ*100
unsigned short  g_BatVolt=0;

//С������ģʽ:ң��ģʽ�����������ϡ�����������ģʽ
unsigned char g_CarRunningMode = CONTROL_MODE;


