/**********************************************************************
�� �� ��:  	usart.h
��    ��:   ��һ����������
�������:   2020.04.15
***********************************************************************/

#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "sys.h"

#define USART1_REC_LEN     200    //�����������ֽ��� 200


extern u8  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART1_RX_STA;         		//����״̬���	

void USART1Init(void);
//void usart2_execute(void);    /*����2���յ����������ݺ�ִ���Ӻ���*/

#define Uart3BufferSize 128

extern char Uart3Buffer[];			//����ͨ�Ŵ��ڽ��ջ�����
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

