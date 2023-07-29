/**********************************************************************
文 件 名: 	syn6288.h
作    者:  宁婷婷
完成日期:  2020.05.24
概		要: 语音播报
***********************************************************************/
#ifndef __SYN6288_H
#define __SYN6288_H    
#include "sys.h"
//#include "stdio.h"
//#include "string.h"
//#include "stm32f10x.h"
//#include "common.h"

//#define  PLAY  PBin(3)

void SYN_FrameInfo(uint8_t Music,uint8_t *HZdata);
void YS_SYN_Set(uint8_t *Info_data);
void PrintCom(uint8_t *DAT,uint8_t len);
void USART_Send_Byte(uint16_t mydata);/*改串口只要改这个接口就行了*/
//void voice_init(void);   /*初始化*/
void Play_Sound(uint8_t *HZdata);  /*播放声音*/


extern u8 text1[];
//extern u8 text2[];
//extern u8 text3[];
//extern u8 text4[];
//extern u8 text5[];
//extern u8 text6[];
//extern u8 text7[];
//extern u8 text8[];
//extern u8 text9[];
//extern u8 text10[];
//extern u8 text11[];
//extern u8 text12[];

#endif
