//#include "sys.h"
//#include "usart-2.h"

//#include "common.h"

//#include <string.h>        


//#if 1
//#pragma import(__use_no_semihosting)             
////��׼����Ҫ��֧�ֺ���                 
//struct __FILE 
//{ 
//        int handle; 

//}; 

//FILE __stdout;       
////����_sys_exit()�Ա���ʹ�ð�����ģʽ    
//_sys_exit(int x) 
//{ 
//        x = x; 
//} 
////�ض���fputc���� 
//int fputc(int ch, FILE *f)
//{      
//        while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
//    USART1->DR = (u8) ch;      
//        return ch;
//}
//#endif 

///*ʹ��microLib�ķ���*/
///* 
//int fputc(int ch, FILE *f)
//{
//        USART_SendData(USART1, (uint8_t) ch);

//        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}        

//    return ch;
//}
//int GetKey (void)  { 

//    while (!(USART1->SR & USART_FLAG_RXNE));

//    return ((int)(USART1->DR & 0x1FF));
//}
//*/


////����2���͵����ֽ� 
//void UART2_SendByte(u16 Data)
//{ 
//   EXTI->IMR = 0x00000000;
//   while (!(USART2->SR & USART_FLAG_TXE));
//   USART2->DR = (Data & (uint16_t)0x01FF);         
//   EXTI->IMR = 0xfffff;   
//}



////�����ַ���
//void UART2Write(char* data)
//{
//        u16 i,len;
//        len=strlen((char*)data);

//        for (i=0; i<len; i++){
//                UART2_SendByte(data[i]);
//        }        
//}
//               

////����HEX
//void UART2Write_Hex(char* data, int len)
//{
//        u16 i,len1;
//        len1=len;

//        for (i=0; i<len1; i++){
//                UART2_SendByte(data[i]);
//        }        
//}

//void USART2Init(u32 bound)
//{
//    //GPIO�˿�����
//  //GPIO�˿�����
//        GPIO_InitTypeDef GPIO_InitStructure;
//        USART_InitTypeDef USART_InitStructure;
//        NVIC_InitTypeDef NVIC_InitStructure;
//         
////        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
//				RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//        USART_DeInit(USART2);  //��λ����2
// 

//      //USART2_TX   PA.15
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);

//    //USART2_RX          PA.12
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);  



//    //Usart2 NVIC ����

//        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;                //

//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        //IRQͨ��ʹ��
//        NVIC_Init(&NVIC_InitStructure);        //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1

//   //USART ��ʼ������

//        USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
//        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//        USART_InitStructure.USART_StopBits = USART_StopBits_1;
//        USART_InitStructure.USART_Parity = USART_Parity_No;
//        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;


//        USART_Init(USART2, &USART_InitStructure);
//        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�2   
//    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 


//}

////unsigned int tmpdata;
////void USART2_IRQHandler(void)                        //����2�жϷ������
////{
////        if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�
////                {
////                USART_ClearITPendingBit(USART2,USART_IT_RXNE);
////                tmpdata=USART_ReceiveData(USART2);//(USART2->DR);        //��ȡ���յ�������
////                }
////}


