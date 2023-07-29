/**********************************************************************
文 件 名:  COMMUNICATE_H
作    者:   樊一鸣
完成日期:   2020.04.15
***********************************************************************/
#ifndef _COMMUNICATE_H
#define _COMMUNICATE_H


typedef enum _PIDType{AnglePID, SpeedPID,}PIDType;


extern char StatusFlag;

void Parse(char *dataInput);
void ResponseIMU(void);
void ResponsePID(PIDType type);
void ResponseStatus(void);



#endif

