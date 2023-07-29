/**********************************************************************

文 件 名: 	usart.c
作    者:  樊一鸣
完成日期:   2020.04.16
概		要: 	蓝牙通信


***********************************************************************/

#include "usart.h"
#include "string.h"
#include <stdarg.h>
#include "misc.h"
#include "motor.h"

#include "sys.h"
#include "common.h"

//蓝牙通信串口接收缓冲区
char Uart3Buffer[Uart3BufferSize];
unsigned char Uart3Index = 0;

/**********************************************************************

作    者:  宁婷婷
完成日期:  2020.04.16
概		要: 语音模块串口


***********************************************************************/
void USART1Init(void)
{

        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;
         
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	      
/*----------------GPIO配置----------------------*/ 

      //USART2_TX   PA.15---PA9
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART2_RX          PA.12____PA10
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	/*----------------USART配置----------------------*/
	
	     RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
		
				USART_InitStructure.USART_BaudRate = 9600;//一般设置为9600;
				USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
				USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验为
				USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
				USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
        
				USART_Init(USART1, &USART_InitStructure);//初始化串口2
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断2   
        USART_Cmd(USART1, ENABLE);                    //使能串口 2

	/*----------------NVIC配置-----------------------*/

        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;                //

        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        //IRQ通道使能
        NVIC_Init(&NVIC_InitStructure);        //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1

}


/*
		蓝牙通信串口，baudrate：0--9600bps、1--115200bps
*/
void USART3Init(char baudrate)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;	//定义串口初始化结构体
	NVIC_InitTypeDef NVIC_InitStructure;
	/* config USART3 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);
		/* config GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB	, ENABLE);
	/* USART3 GPIO config */
	/* Configure USART3 Tx (PB.10) as alternate function push-pull 推拉输出模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	/* Configure USART3 Rx (PB.11) as input floating 浮点输入模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	  
	/* USART3 mode config */
	
	if(baudrate == 0)
		USART_InitStructure.USART_BaudRate = 9600;//波特率9600
	else if(baudrate == 1)
		USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ;//无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//禁用RTSCTS硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//使能发送接收

	USART_Init(USART3, &USART_InitStructure); 
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//使能接收中断
	USART_Cmd(USART3, ENABLE);
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//第2组
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



void Uart3SendByte( char byte)   //串口发送一个字节
{
  USART_SendData(USART3, byte);        //通过库函数  发送数据
  while( USART_GetFlagStatus(USART3,USART_FLAG_TC)!= SET);  
}

void Uart3SendBuff(char *buf, u16 len)
{
	unsigned short i;
	for(i=0; i<len; i++)Uart3SendByte(*buf++);
}
void Uart3SendStr(char *str)
{
	unsigned short i,len;
	len = strlen(str);
	for(i=0; i<len; i++)Uart3SendByte(*str++);
}
void Uart3SendNum(int num)   //串口发送一个数字
{   
	 USART_SendData(USART3, num>>8);         //发送电池容量数据高8位
   while( USART_GetFlagStatus(USART3,USART_FLAG_TC)!= SET);  
	
   USART_SendData(USART3, num&0x00FF);        //发送电池容量数据低8位
   while( USART_GetFlagStatus(USART3,USART_FLAG_TC)!= SET);
  
	
}




