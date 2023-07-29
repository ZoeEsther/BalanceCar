/**********************************************************************
文 件 名:  	common.h
作    者:   樊一鸣
完成日期:   2020.04.15
***********************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

void delay_us(unsigned int n);
void delay_ms(unsigned int n);
void get_ms(unsigned long *time);


unsigned char XOR_Get(char * str, unsigned char  len);
unsigned char XOR_Check(char * str, unsigned char  len,unsigned char checksum);
unsigned char Sum_Get(char *dat,char len);
unsigned short CRC16Calculate(unsigned char *buff, unsigned char len);




#endif
