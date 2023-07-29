/**********************************************************************

�� �� ��: 	usart.c
��    ��:  ��һ��
�������:   2020.04.16
��		Ҫ: 	����ͨ��


***********************************************************************/

#include "usart.h"
#include "string.h"
#include <stdarg.h>
#include "misc.h"
#include "motor.h"

#include "sys.h"
#include "common.h"

//����ͨ�Ŵ��ڽ��ջ�����
char Uart3Buffer[Uart3BufferSize];
unsigned char Uart3Index = 0;

/**********************************************************************

��    ��:  ������
�������:  2020.04.16
��		Ҫ: ����ģ�鴮��


***********************************************************************/
void USART1Init(void)
{

        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;
         
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	      
/*----------------GPIO����----------------------*/ 

      //USART2_TX   PA.15---PA9
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART2_RX          PA.12____PA10
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	/*----------------USART����----------------------*/
	
	     RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
		
				USART_InitStructure.USART_BaudRate = 9600;//һ������Ϊ9600;
				USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
				USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��Ϊ
				USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
				USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
        
				USART_Init(USART1, &USART_InitStructure);//��ʼ������2
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�2   
        USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 2

	/*----------------NVIC����-----------------------*/

        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;                //

        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        //IRQͨ��ʹ��
        NVIC_Init(&NVIC_InitStructure);        //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1

}


/*
		����ͨ�Ŵ��ڣ�baudrate��0--9600bps��1--115200bps
*/
void USART3Init(char baudrate)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;	//���崮�ڳ�ʼ���ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;
	/* config USART3 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);
		/* config GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB	, ENABLE);
	/* USART3 GPIO config */
	/* Configure USART3 Tx (PB.10) as alternate function push-pull �������ģʽ*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	/* Configure USART3 Rx (PB.11) as input floating ��������ģʽ*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	  
	/* USART3 mode config */
	
	if(baudrate == 0)
		USART_InitStructure.USART_BaudRate = 9600;//������9600
	else if(baudrate == 1)
		USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����RTSCTSӲ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ʹ�ܷ��ͽ���

	USART_Init(USART3, &USART_InitStructure); 
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//ʹ�ܽ����ж�
	USART_Cmd(USART3, ENABLE);
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//��2��
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



void Uart3SendByte( char byte)   //���ڷ���һ���ֽ�
{
  USART_SendData(USART3, byte);        //ͨ���⺯��  ��������
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
void Uart3SendNum(int num)   //���ڷ���һ������
{   
	 USART_SendData(USART3, num>>8);         //���͵���������ݸ�8λ
   while( USART_GetFlagStatus(USART3,USART_FLAG_TC)!= SET);  
	
   USART_SendData(USART3, num&0x00FF);        //���͵���������ݵ�8λ
   while( USART_GetFlagStatus(USART3,USART_FLAG_TC)!= SET);
  
	
}




