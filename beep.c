#include "beep.h"

//蜂鸣器初始化函数
//对比LED灯的初始化函数我们会发现只有管脚改变了。
void BEEP_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  //定义蜂鸣器的管脚PB8并初始化。
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}