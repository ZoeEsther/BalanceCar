#include "sys.h"
#include "usart-2.h"

#include "delay.h"

#include <string.h>        


#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
        int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
        x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
        while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
        return ch;
}
#endif 

/*使用microLib的方法*/
/* 
int fputc(int ch, FILE *f)
{
        USART_SendData(USART1, (uint8_t) ch);

        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}        

    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/

//串口1发送单个字节 
void UART1_SendByte(u16 Data)
{ 
   EXTI->IMR = 0x00000000;
   while (!(USART1->SR & USART_FLAG_TXE));
   USART1->DR = (Data & (uint16_t)0x01FF);         
   EXTI->IMR = 0xfffff;
}
//串口2发送单个字节 
void UART2_SendByte(u16 Data)
{ 
   EXTI->IMR = 0x00000000;
   while (!(USART2->SR & USART_FLAG_TXE));
   USART2->DR = (Data & (uint16_t)0x01FF);         
   EXTI->IMR = 0xfffff;   
}
//串口3发送单个字节 
void UART3_SendByte(u16 Data)
{ 
   EXTI->IMR = 0x00000000;
   while (!(USART3->SR & USART_FLAG_TXE));
   USART3->DR = (Data & (uint16_t)0x01FF);         
   EXTI->IMR = 0xfffff;   
}
//串口1发送字符串 
void UART1Write(char* data)
{
        u16 i,len;
        len=strlen((char*)data);
//        DIR485_H ;
        for (i=0; i<len; i++){
                UART1_SendByte(data[i]);
        }                
}

//串口2发送字符串
void UART2Write(char* data)
{
        u16 i,len;
        len=strlen((char*)data);

        for (i=0; i<len; i++){
                UART2_SendByte(data[i]);
        }        
}
//串口3发送字符串
void UART3Write(char* data)
{
        u16 i,len;
        len=strlen((char*)data);

        for (i=0; i<len; i++){
                UART3_SendByte(data[i]);
        }        
}                
//串口1发送HEX
void UART1Write_Hex(char* data, int len)
{
        u16 i,len1;
        len1=len;

        for (i=0; i<len1; i++){
                UART1_SendByte(data[i]);
        }        
}
//串口2发送HEX
void UART2Write_Hex(char* data, int len)
{
        u16 i,len1;
        len1=len;

        for (i=0; i<len1; i++){
                UART2_SendByte(data[i]);
        }        
}

void uart_init(u32 bound)
{
    //GPIO端口设置
  //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;
         
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
          RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        USART_DeInit(USART1);  //复位串口1
        USART_DeInit(USART2);  //复位串口2
        USART_DeInit(USART3);  //复位串口2
         //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART1_RX          PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

      //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART2_RX          PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    //USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PB10

    //USART3_RX          PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PB11

   //Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;                //

        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        //IRQ通道使能
        NVIC_Init(&NVIC_InitStructure);        //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1

    //Usart2 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;                //

        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        //IRQ通道使能
        NVIC_Init(&NVIC_InitStructure);        //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
   //Usart3 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                //子优先级3
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        //IRQ通道使能
        NVIC_Init(&NVIC_InitStructure);        //根据指定的参数初始化VIC寄存器  
   //USART 初始化设置

        USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
           USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断1         
    USART_Cmd(USART1, ENABLE);                    //使能串口 

        USART_Init(USART2, &USART_InitStructure);
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断2   
    USART_Cmd(USART2, ENABLE);                    //使能串口 

        USART_Init(USART3, &USART_InitStructure); //初始化串口3
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断3
    USART_Cmd(USART3, ENABLE);                    //使能串口3 
}

unsigned int tmpdata;
void USART3_IRQHandler(void)                        //串口3中断服务程序
{
        if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
                {
                USART_ClearITPendingBit(USART3,USART_IT_RXNE);
                tmpdata=USART_ReceiveData(USART3);//(USART3->DR);        //读取接收到的数据
                }
}