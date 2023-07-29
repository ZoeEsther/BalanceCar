/**********************************************************************
文 件 名: 	led.c
作    者: 宁婷婷
完成日期:  2020.05.15
概		要：电量警报灯
***********************************************************************/
#include "stm32f10x.h"
#include "led.h"

//LED初始化函数
void LED_Init(void)
{
    
    /* 定义一个GPIO_InitTypeDef类型的结构体 */
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* 打开GPIOA的外设时钟 */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
    
    /* 选择要控制的GPIOA引脚；设置为推挽输出；最大速率设置为50MHz；*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    
    
    /* 关闭所有LED灯 */
    GPIO_SetBits( GPIOA, GPIO_Pin_8 );
}



