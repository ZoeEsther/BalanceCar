#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x_it.h"

 #define KEY PAout(11)        //用位操作定义引脚PA11


void KEY_Init(void);
void KEY_Scan(void);
 #endif



