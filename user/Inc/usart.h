/**********************************************************************
文 件 名:  	usart.h
作    者:   樊一鸣、宁婷婷
完成日期:   2020.04.15
***********************************************************************/

#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "sys.h"

#define USART1_REC_LEN     200    //定义最大接收字节数 200


extern u8  USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART1_RX_STA;         		//接收状态标记	

void USART1Init(void);
//void usart2_execute(void);    /*串口2接收到串口屏数据后执行子函数*/

#define Uart3BufferSize 128

extern char Uart3Buffer[];			//蓝牙通信串口接收缓冲区
extern unsigned char Uart3Index;

//void USART1Init(void);
void USART3Init(char baudrate);


//void Uart1SendByte(char byte) ;
//void Uart1SendBuff(char *buf, unsigned short len);
//void Uart1SendStr(char * str);
	
void Uart3SendByte(char byte)  ;
void Uart3SendBuff(char *buf, unsigned short len);
void Uart3SendStr(char *str);
void Uart3SendNum(int num);

#define BluetoothOut(str) Uart3SendStr(str)
#define BluetoothOutNum(num) Uart3SendNum(num)
#define BluetoothOutBuff(buf) Uart3SendBuff(buf,len)

#endif /* __USART1_H */

