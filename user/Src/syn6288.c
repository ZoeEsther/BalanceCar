#include "syn6288.h"
#include "stdio.h"
#include "string.h"
#include "stm32f10x.h"
#include "common.h"
#include "stm32f10x_it.h"


/**********************************************************************
文 件 名: 	syn6288.c
作    者:  宁婷婷
完成日期:  2020.05.24
概		要: 语音播报
***********************************************************************/

void USART_Send_Byte(uint16_t mydata)		//数据发送函数
{
		USART_SendData(USART1,mydata);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
}

void PrintCom(uint8_t *DAT,uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
	 	USART_Send_Byte(*DAT++);
	}	
}

void SYN_FrameInfo(uint8_t Music,uint8_t *HZdata)
{
		 unsigned  char  Frame_Info[50];
     unsigned  char  HZ_Length;  
		 unsigned  char  ecc  = 0;  		
	   unsigned  int i=0; 
		 HZ_Length =strlen((char*)HZdata); 			
  
	
		 Frame_Info[0] = 0xFD ; 		//构造帧头FD
		 Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
		 Frame_Info[2] = HZ_Length + 3; 		//构造数据区长度的低字节
		 Frame_Info[3] = 0x01 ; 				 		 //合成播放命令
		 Frame_Info[4] = 0x01 | Music<<4 ; 	//文字编码格式：GBK
	 
		 for(i = 0; i<5; i++)   	//依次发送构造好的5个帧头字节			
	     {  
	         ecc=ecc^(Frame_Info[i]);		 //对发送的字节进行异或校验
	     }

	   	 for(i= 0; i<HZ_Length; i++)   	//依次发送待合成的文本数据
	     {  
	         ecc=ecc^(HZdata[i]); 				 //对发送的字节进行异或校验
	     }		 
	  
		  memcpy(&Frame_Info[5], HZdata, HZ_Length);
		  Frame_Info[5+HZ_Length]=ecc;
		  PrintCom(Frame_Info,5+HZ_Length+1);
}

/***********************************************************
* 名    称： YS_SYN_Set(u8 *Info_data)
* 功    能： 主函数	程序入口
* 入口参数： *Info_data:固定的配置信息变量
* 作		者： 宁婷婷
* 说    明：本函数用于配置，停止合成、暂停合成等设置 ，默认波特率9600bps。
* 调用方法：通过调用已经定义的相关数组进行配置。
**********************************************************/
void YS_SYN_Set(uint8_t *Info_data)
{
	uint8_t Com_Len;
	Com_Len =strlen((char*)Info_data);
	PrintCom(Info_data,Com_Len);
}

void Play_Sound(uint8_t *HZdata)  /*播放声音*/
{
				
			SYN_FrameInfo(0,HZdata) ;  /*播放*/
		  SYN_FrameInfo(0,HZdata) ;  /*播放*/

	
}

