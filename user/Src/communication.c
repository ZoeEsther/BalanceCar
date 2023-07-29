/**********************************************************************

文 件 名: 	communicate.c
作    者:  樊一鸣
完成日期:  2020.05.5
概		要: 手机APP模式切换


***********************************************************************/

#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "communicate.h"
#include "mpu6050.h"
#include "control.h"
#include "debug.h"
#include "usart.h"
#include "dataflash.h"
#include "common.h"
#include "manage.h"

#include "stm32f10x_gpio.h"
#include "syn6288.h"

char StatusFlag=0;

/*
	上报姿态数据  （不开机也可发送）
*/
void ResponseIMU(void)
{
	char temp[32];	
  sprintf(temp, "V,%.0f,%.0f,%.0f\r\n", g_fGravityAngle+180,g_fGyroAngleSpeed+180,g_fCarAngle+180);
	BluetoothOut(temp);
  
}

/*
	上报PID值
*/
void ResponsePID(PIDType type)
{
	char temp[32];
	if(type==AnglePID)
		sprintf(temp, "AP,%.2f,%.2f,%.2f\r\n", 
		g_tCarAnglePID.P,
		g_tCarAnglePID.I,
		g_tCarAnglePID.D);
	else if(type == SpeedPID)
		sprintf(temp, "SP,%.2f,%.2f,%.2f\r\n", 
		g_tCarSpeedPID.P,
		g_tCarSpeedPID.I,
		g_tCarSpeedPID.D);
	BluetoothOut(temp);
}

/*
	上报电池电量
*/
void ResponseInfo(void)
{
	char temp[32];
  sprintf(temp, "R,%d,%d,\r\n", g_BatVolt*10,	g_RunTime);
	BluetoothOut(temp);
	
}


/*
	上报状态信息
*/
void ResponseStatus(void)
{
	char temp[32];
  sprintf(temp, "R,%d,%d\r\n", g_BatVolt*10,	g_RunTime);
	BluetoothOut(temp);
}

/*
	算取距离位数
*/
int number(int num)
{
	int i=0;
	do
	{
		num=num/10;
		i++;
	}while(num!=0);
	return i;
}
/*
	APP数据解析函数
	协议规范:帧头+长度+内容+校验
	如:#17,CM,-27.314,-5.716*56;  帧头# 长度17  内容CM,-27.314,-5.716 校验56，长度为内容字节数
*/
void Parse(char *dataInput)
{
	unsigned char len,check,ret;
	char *p = dataInput;
	float fTemp1, fTemp2,fTemp3;
	int check_sum,sum,scdistance,i;
	int AX=0,BX=0,v=6;//AX加速的参数，BX减速的参数,v此刻速度参数

#if COMMUNICATE_DEBUG_EN > 0
	if(Uart3Index){
		DebugOutBuff(Uart3Buffer, Uart3Index);
		DebugOutStr("\r\n");
	}
#endif
	while(((p-dataInput)<Uart3Index))
	{
		if(*p == '#')// 
		{
			len = (unsigned char)atof(strtok(p+1, ","));//length of data
			if((len>0)&&(len<100))//长度大于100的数据忽略
			{
				if(len<10){
					check = (unsigned char)atof(strtok(p+len+4, ";"));
					ret = XOR_Check(p+3, len, check);
					p += 3;
				}
				else if ((len>11)&&(len<14))
				{
					check=(unsigned char)atof(strtok(p+len+5,";"));
					check=(int)check;
					scdistance=(unsigned char)atof(strtok(p+14,"*"));
			    scdistance=(int)scdistance;
					
					i=number(scdistance);   //距离位数
					sum=XOR_Get(p+4,len-i);  //距离前的内容异或
					sum=(int)sum;
					check_sum=sum^scdistance;
					
					p +=4;
					if(check==check_sum) ret=1;
					else ret=0;
				}
				else{
					check = (unsigned char)atof(strtok(p+len+5, ";"));
					ret = XOR_Check(p+4, len, check);
					p += 4;
				}
				if(ret){//  校验正确，开始解析数据
					if(g_CarRunningMode == CONTROL_MODE)
					{
						if(strstr(p , "CS")){Steer(0, 0);}//停止
						else if(strstr(p, "CJ")){// 摇杆控制
							strtok(p, ",");
							fTemp1= atof(strtok(NULL,","));	// X轴坐标
							fTemp2 = atof(strtok(NULL,"*"));// Y轴坐标
							Steer(fTemp1 * 10, -fTemp2 * 10);
						}
						else if(strstr(p, "AX")){//加速
							if(v<8)  AX++;
							Steer(0,-(6+AX));
							v=6+AX;
						}
						else if(strstr(p, "BX")){// 减速
							if((v<8) && (v>3))  BX++;
							Steer(0,-(6-BX));
							v=6-BX;
						}
						else if(strstr(p, "CM")){//重力感应控制
							strtok(p, ",");
							fTemp1 = atof(strtok(NULL, ",")); 		// Pitch
	      						fTemp2 = atof(strtok(NULL, "*")); 	// Roll
	      						Steer(fTemp2 / 5, -fTemp1 / 2);
						}
						else if(strstr(p, "SC")){// 语音控制
							strtok(p, ",");
							fTemp1 = atof(strtok(NULL,","));	// direct
							fTemp2 = atof(strtok(NULL,","));// speed
							fTemp3 = atof(strtok(NULL,"*"));//distance
							ScDistance(fTemp1,fTemp2*10,fTemp3);
						}
						else if(strstr(p,"0L")){ //OLED页面1
					  GPIO_SetBits(GPIOB, GPIO_Pin_12);
						delay_ms(50);
						GPIO_ResetBits(GPIOB, GPIO_Pin_12);
						StatusFlag=0;
					  }
					  else if(strstr(p,"0E")){ //OLED页面2
            GPIO_SetBits(GPIOB, GPIO_Pin_12);
						delay_ms(50);
						GPIO_ResetBits(GPIOB, GPIO_Pin_12);
						StatusFlag=1;
					  }
					}
					
					if(strstr(p,"SNK")){// 设置为普通遥控模式
						g_CarRunningMode = CONTROL_MODE;
					}
					else if(strstr(p,"SUF")){// 设置为超声波跟随模式
						g_CarRunningMode = ULTRA_FOLLOW_MODE;
					}
					else if(strstr(p,"SUA")){// 设置为超声波避障模式
						g_CarRunningMode = ULTRA_AVOID_MODE;
						Steer(0, 5);
					}
					
					else if(strstr(p,"SAP")){// 设置角度环PID
						strtok(p, ",");
						g_tCarAnglePID.P = atof(strtok(NULL,","));
						g_tCarAnglePID.I = atof(strtok(NULL,","));
						g_tCarAnglePID.D = atof(strtok(NULL,"*"));
						PIDWrite(AnglePID);
						ResponsePID(AnglePID);
					}
					else if(strstr(p,"SSP")){// 设置速度环PID
						strtok(p, ",");
						g_tCarSpeedPID.P = atof(strtok(NULL,","));
						g_tCarSpeedPID.I = atof(strtok(NULL,","));
						g_tCarSpeedPID.D = atof(strtok(NULL,"*"));
						PIDWrite(SpeedPID);
						ResponsePID(SpeedPID);
					}
					else if(strstr(p, "GAP")){//查询角度环PID
						ResponsePID(AnglePID);
					}
					else if(strstr(p, "GSP")){//查询速度环PID
						ResponsePID(SpeedPID);
					}
					else if(strstr(p,"RA")){// 角度环pid恢复默认值
						PIDReset(AnglePID);
						ResponsePID(AnglePID);
					}
					else if(strstr(p,"RV")){//速度环pid恢复默认值
						PIDReset(SpeedPID);
						ResponsePID(SpeedPID);
					}
					else if(strstr(p,"RB")){//查询版本及开启状态上报信息
//						StatusFlag = 1;
						ResponseInfo();
					}
					else if(strstr(p,"RS")){//关闭状态上报
//						StatusFlag = 0;
					}
				}
			}
			
		}
		p++;
	}
	Uart3Index=0;
	memset(Uart3Buffer, 0, Uart3BufferSize);
}


