/**********************************************************************

�� �� ��: 	beep.c
��    ��: ������
�������:  2020.05.15
��		Ҫ��������

***********************************************************************/
#include "beep.h"

//��������ʼ������
//�Ա�LED�Ƶĳ�ʼ���������ǻᷢ��ֻ�йܽŸı��ˡ�
void BEEP_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  //����������Ĺܽ�PB12����ʼ����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}



