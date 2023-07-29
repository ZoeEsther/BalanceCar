/**********************************************************************
�� �� ��:  	DATAFLASH_H
��    ��:   ��һ��
�������:   2020.04.15
***********************************************************************/

#ifndef _DATAFLASH_H
#define _DATAFLASH_H


void ProgramFlash(unsigned int addr, char* data, unsigned short len);
void ReadFlash(unsigned int addr, char* data, unsigned short len);
void PIDInit(void);
void PIDWrite(char flag);
void PIDRead(void);
void PIDReset(char flag);


#endif

