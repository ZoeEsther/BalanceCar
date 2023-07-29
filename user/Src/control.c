/**********************************************************************

�� �� ��: 	  control.c
������:  ��һ��
�ա���:   2020.05.05 
��  Ҫ: 	������Ʋ������ٶȿ��Ʋ��������ٶȺ��ٶȻ�PID���Ʋ������������Ʋ�������ȫ�ֱ�����
          ���ת���쳣��⺯��  AbnormalSpinDetect
					С���ŵؼ��  LandingDetect
					���ʹ��/ʧ�ܿ���   MotorManage
					���ת�ټ�������ƺ���      SetMotorVoltageAndDirection
					����������       MotorOutput
					�ǶȻ����㺯����AngleCalculate�����ǶȻ����ƺ��� (AngleControl)
					�ٶȻ����ƺ�����SpeedControl�����ٶȻ��������������SpeedControlOutput��
					���̹�һ������Scale��
					ң���ٶȼ�����������Steer��
					����������/���� �� UltraControl��
***********************************************************************/
#include "math.h"
#include "stdio.h"
#include "control.h"
#include "debug.H"
#include "MPU6050.H"
#include "communicate.h"
#include "bsp.h"
#include "ultrasonic.h"
#include "common.h"
#include "stm32f10x_gpio.h"
#include "sys.h"
#include "syn6288.h" 

//unsigned char g_StatusFlag;  //ҳ���л��õ��Ĳ���

unsigned char g_u8MainEventCount;
unsigned char g_u8SpeedControlCount;
unsigned char g_u8SpeedControlPeriod;
unsigned char g_u8DirectionControlPeriod;
unsigned char g_u8DirectionControlCount;

unsigned char g_cMotorDisable = 0;//ֵ����0ʱ�������ת��������ֹͣת��


int g_iGravity_Offset = 0;

/******������Ʋ���******/
float g_fSpeedControlOut;
float g_fSpeedControlOutOld;
float g_fSpeedControlOutNew;
float g_fAngleControlOut;
float g_fLeftMotorOut;
float g_fRightMotorOut;

/******�ٶȿ��Ʋ���******/

short  g_s16LeftMotorPulse;
short  g_s16RightMotorPulse;

int  g_s32LeftMotorPulseOld;
int  g_s32RightMotorPulseOld;
int  g_s32LeftMotorPulseSigma;
int  g_s32RightMotorPulseSigma;

int  g_iTim4Encoder;//��ʱ���TIM4��ʱ���������ı���������  ����
int  g_iTim2Encoder;//��ʱ���TIM2��ʱ���������ı���������  ����

float g_fCarSpeed;
float g_iCarSpeedSet;
float g_fCarSpeedOld;
float g_fCarPosition;

/*-----�ǶȻ����ٶȻ�PID���Ʋ���-----*/
PID_t g_tCarAnglePID={18.51, 0, 29.22};	//
PID_t g_tCarSpeedPID={14.83, 0.11, 0};	//
/******�������Ʋ���******/
float g_fBluetoothSpeed;
float g_fBluetoothDirection;
float g_fBluetoothDirectionOld;
float g_fBluetoothDirectionNew;
float g_fBluetoothDirectionOut;

float g_fCarAngle;         	//
float g_fGyroAngleSpeed;		//     			
float g_fGravityAngle;			//


int g_iLeftTurnRoundCnt = 0;
int g_iRightTurnRoundCnt = 0;

static int AbnormalSpinFlag = 0;
/***************************************************************
** ��������: CarUpstandInit
** ��������: ȫ�ֱ�����ʼ������
** ������:  ��һ��
** �ա���:   2020.05.05 
***************************************************************/
void CarUpstandInit(void)
{
	//g_iAccelInputVoltage_X_Axis = g_iGyroInputVoltage_Y_Axis = 0;
	g_s16LeftMotorPulse = g_s16RightMotorPulse = 0;
	g_s32LeftMotorPulseOld = g_s32RightMotorPulseOld = 0;
	g_s32LeftMotorPulseSigma = g_s32RightMotorPulseSigma = 0;

	g_fCarSpeed = g_fCarSpeedOld = 0;
	g_fCarPosition = 0;
	g_fCarAngle    = 0;
	g_fGyroAngleSpeed = 0;
	g_fGravityAngle   = 0;

	g_fAngleControlOut = g_fSpeedControlOut = g_fBluetoothDirectionOut = 0;
	g_fLeftMotorOut    = g_fRightMotorOut   = 0;
	g_fBluetoothSpeed  = g_fBluetoothDirection = 0;
	g_fBluetoothDirectionNew = g_fBluetoothDirectionOld = 0;

  g_u8MainEventCount=0;
	g_u8SpeedControlCount=0;
 	g_u8SpeedControlPeriod=0;
}


/***************************************************************
** ��������: AbnormalSpinDetect
** ��������: ���ת���쳣���   
** ������:  ��һ��
** �ա���:   2020.05.05  
***************************************************************/

void AbnormalSpinDetect(short leftSpeed,short rightSpeed)
{
	static unsigned short count = 0;
	
	//�ٶ�����Ϊ0ʱ��⣬���򲻼��
	if(g_iCarSpeedSet==0)
	{
		if(((leftSpeed>30)&&(rightSpeed>30)&&(g_fCarAngle > -30) && (g_fCarAngle < 30))
			||((leftSpeed<-30)&&(rightSpeed<-30))&&(g_fCarAngle > -30) && (g_fCarAngle < 30))
		{// ���ҵ��ת�ٴ���30��������ͬ������ʱ�䳬��250ms���ҳ���ǶȲ�����30�ȣ����ж�Ϊ���տ�ת
			count++;
			if(count>50){
				count = 0;
				AbnormalSpinFlag = 1;
			}
		}
		else{
			count = 0;
		}
	}
	else{
		count = 0;
	}
}

/***************************************************************
** ��������: LandingDetect
** ��������: С���ŵؼ��    
** ������:  ��һ��
** �ա���:   2020.05.05
***************************************************************/
void LandingDetect(void)
{
	static float lastCarAngle = 0;
	static unsigned short count = 0,count1 = 0;
	
	if(AbnormalSpinFlag == 0)return;
	
	// С���Ƕ�5��~-5���������
	if((g_fCarAngle > -5) && (g_fCarAngle < 5))
	{
		count1++;
		if(count1 >= 50)
		{//ÿ��250ms�ж�һ��С���Ƕȱ仯�����仯��С��0.8������-0.8���ж�ΪС����ֹ
			count1 = 0;
			if(((g_fCarAngle - lastCarAngle) < 0.8) && ((g_fCarAngle - lastCarAngle) > -0.8))
			{
				count++;
				if(count >= 4){
					count = 0;
					count1 = 0;
					g_fCarPosition = 0;
					AbnormalSpinFlag = 0;
				}
			}
			else{
				count = 0;
			}
			lastCarAngle = g_fCarAngle;
		}
	}
	else
	{
		count1 = 0;
		count = 0;
	}
}

/***************************************************************
** ��������: MotorManage
** ��������: ���ʹ��/ʧ�ܿ���  
** ������:  ��һ��
** �ա���:   2020.05.05   
***************************************************************/
void MotorManage(void)
{

	AbnormalSpinDetect(g_s16LeftMotorPulse, g_s16RightMotorPulse);
		
	LandingDetect();
	
	if(AbnormalSpinFlag)
	{	
		g_cMotorDisable |= (0x01<<1);
	}
	else
	{
		g_cMotorDisable &= ~(0x01<<1);
	}
	
	if(g_fCarAngle > 30 || g_fCarAngle < (-30))
	{
		g_cMotorDisable |= (0x01<<2);
	}
	else
	{
		g_cMotorDisable &= ~(0x01<<2);
	}
	
}

/***************************************************************
** ��������: SetMotorVoltageAndDirection
** ��������: ���ת�ټ�������ƺ���         
** ������:  ��һ��
** �ա���:   2020.05.05  
***************************************************************/
void SetMotorVoltageAndDirection(int i16LeftVoltage,int i16RightVoltage)
{
	  if(i16LeftVoltage<0)
    {	
			GPIO_SetBits(GPIOA, GPIO_Pin_3 );				    
      GPIO_ResetBits(GPIOA, GPIO_Pin_4 );
      i16LeftVoltage = (-i16LeftVoltage);
    }
    else 
    {	
      GPIO_SetBits(GPIOA, GPIO_Pin_4 );				    
      GPIO_ResetBits(GPIOA, GPIO_Pin_3 ); 
    }

    if(i16RightVoltage<0)
    {	
     	GPIO_SetBits(GPIOB, GPIO_Pin_0 );				    
      GPIO_ResetBits(GPIOB, GPIO_Pin_1 );
      i16RightVoltage = (-i16RightVoltage);
    }
    else
    {
			GPIO_SetBits(GPIOB, GPIO_Pin_1 );				    
			GPIO_ResetBits(GPIOB, GPIO_Pin_0 );	      
    }

	if(i16RightVoltage > MOTOR_OUT_MAX)  
	{
		i16RightVoltage = MOTOR_OUT_MAX;
	}
	if(i16LeftVoltage > MOTOR_OUT_MAX)
	{
	   i16LeftVoltage = MOTOR_OUT_MAX;
	}  
	
	if(g_cMotorDisable)
	{
		TIM_SetCompare1(TIM3,0);
		TIM_SetCompare2(TIM3,0); 
	}
	else
	{
		TIM_SetCompare1(TIM3,i16RightVoltage);
		TIM_SetCompare2(TIM3,i16LeftVoltage);
	}
}


/***************************************************************
** ��������: MotorOutput
** ��������: ������������ֱ�����ơ��ٶȿ��ơ�������Ƶ���������е���,���������������������������������
** ������:  ��һ��
** �ա���:   2020.05.05
***************************************************************/
void MotorOutput(void)
{
	g_fLeftMotorOut  = g_fAngleControlOut - g_fSpeedControlOut - g_fBluetoothDirection ;	
	g_fRightMotorOut = g_fAngleControlOut - g_fSpeedControlOut + g_fBluetoothDirection ;


	/*������������*/
	if((int)g_fLeftMotorOut>0)       g_fLeftMotorOut  += MOTOR_OUT_DEAD_VAL;
	else if((int)g_fLeftMotorOut<0)  g_fLeftMotorOut  -= MOTOR_OUT_DEAD_VAL;
	if((int)g_fRightMotorOut>0)      g_fRightMotorOut += MOTOR_OUT_DEAD_VAL;
	else if((int)g_fRightMotorOut<0) g_fRightMotorOut -= MOTOR_OUT_DEAD_VAL;

	/*������ʹ�����ֹ����PWM��Χ*/			
	if((int)g_fLeftMotorOut  > MOTOR_OUT_MAX)	g_fLeftMotorOut  = MOTOR_OUT_MAX;
	if((int)g_fLeftMotorOut  < MOTOR_OUT_MIN)	g_fLeftMotorOut  = MOTOR_OUT_MIN;
	if((int)g_fRightMotorOut > MOTOR_OUT_MAX)	g_fRightMotorOut = MOTOR_OUT_MAX;
	if((int)g_fRightMotorOut < MOTOR_OUT_MIN)	g_fRightMotorOut = MOTOR_OUT_MIN;
	
    SetMotorVoltageAndDirection((int)g_fLeftMotorOut,(int)g_fRightMotorOut);
}



void GetMotorPulse(void)  //�ɼ�����ٶ�����
{ 	
  g_s16LeftMotorPulse = TIM_GetCounter(TIM2);     
  g_s16RightMotorPulse= -TIM_GetCounter(TIM4);
  TIM2->CNT = 0;
  TIM4->CNT = 0;   //����

  g_s32LeftMotorPulseSigma +=  g_s16LeftMotorPulse;
  g_s32RightMotorPulseSigma += g_s16RightMotorPulse; 
	
	g_iLeftTurnRoundCnt -= g_s16LeftMotorPulse;
	g_iRightTurnRoundCnt -= g_s16RightMotorPulse;

}

/***************************************************************
** ������:  ��һ��
** �ա���:   2020.05.05 
** ��������: AngleCalculate
** ��������: �ǶȻ����㺯��          
***************************************************************/
void AngleCalculate(void)
{
	//-------���ٶ�--------------------------
	//����Ϊ��2gʱ�������ȣ�16384 LSB/g
    g_fGravityAngle = atan2(g_fAccel_y/16384.0,g_fAccel_z/16384.0) * 180.0 / 3.14;
	  g_fGravityAngle = g_fGravityAngle - g_iGravity_Offset;

	//-------���ٶ�-------------------------
	//��ΧΪ2000deg/sʱ�������ϵ��16.4 LSB/(deg/s)
	g_fGyro_x  = g_fGyro_x / 16.4;  //������ٶ�ֵ			   
	g_fGyroAngleSpeed = g_fGyro_x;	
	
	//-------�����˲�---------------
	g_fCarAngle = 0.98 * (g_fCarAngle + g_fGyroAngleSpeed * 0.005) + 0.02 *	g_fGravityAngle;
}
/***************************************************************
** ������:  ��һ��
** �ա���:   2020.05.05
** ��������: AngleControl
** ��������: �ǶȻ����ƺ���            

***************************************************************/
void AngleControl(void)	 
{
	g_fAngleControlOut =  (CAR_ANGLE_SET-g_fCarAngle) * (-g_tCarAnglePID.P) *5 + \
	(CAR_ANGLE_SPEED_SET-g_fGyroAngleSpeed) * (-g_tCarAnglePID.D /10);
}



/***************************************************************
** ��������: SpeedControl
** ��������: �ٶȻ����ƺ���  
** ������:  ��һ��
** �ա���:   2020.05.05
***************************************************************/

void SpeedControl(void)
{
  	float fP,fI;   	
	float fDelta;
	
	
	g_fCarSpeed = (g_s32LeftMotorPulseSigma  + g_s32RightMotorPulseSigma ) * 0.5 ;
  g_s32LeftMotorPulseSigma = g_s32RightMotorPulseSigma = 0;	  //ȫ�ֱ��� ע�⼰ʱ����
    	
	g_fCarSpeed = 0.7 * g_fCarSpeedOld + 0.3 * g_fCarSpeed ;//��ͨ�˲���ʹ�ٶȸ�ƽ��
	g_fCarSpeedOld = g_fCarSpeed;

	fDelta = CAR_SPEED_SET;
	fDelta -= g_fCarSpeed;   
	
	fP = fDelta * (g_tCarSpeedPID.P);
  fI = fDelta * (g_tCarSpeedPID.I/10.0);

	g_fCarPosition += fI;
	g_fCarPosition += g_fBluetoothSpeed;	  
	
//������������
	if((s16)g_fCarPosition > CAR_POSITION_MAX)    g_fCarPosition = CAR_POSITION_MAX;
	if((s16)g_fCarPosition < CAR_POSITION_MIN)    g_fCarPosition = CAR_POSITION_MIN;
	
	g_fSpeedControlOutOld = g_fSpeedControlOutNew;
  g_fSpeedControlOutNew = fP + g_fCarPosition;
}
/***************************************************************
** ��������: SpeedControlOutput
** ��������: �ٶȻ������������-�ֶಽ��αƽ���������������ܽ���ֱ�����ĸ��Ž��͡�
** ������:  ��һ��
** �ա���:   2020.05.05
***************************************************************/
void SpeedControlOutput(void)
{
  float fValue;
  fValue = g_fSpeedControlOutNew - g_fSpeedControlOutOld ;
  g_fSpeedControlOut = fValue * (g_u8SpeedControlPeriod + 1) / SPEED_CONTROL_PERIOD + g_fSpeedControlOutOld; 
}


/***************************************************************
** ��������: Scale
** ��������: ���̹�һ������
** ������:  ��һ��
** �ա���:   2020.05.05
***************************************************************/
float Scale(float input, float inputMin, float inputMax, float outputMin, float outputMax) { 
  float output;
  if (inputMin < inputMax)
    output = (input - inputMin) / ((inputMax - inputMin) / (outputMax - outputMin));
  else
    output = (inputMin - input) / ((inputMin - inputMax) / (outputMax - outputMin));
  if (output > outputMax)
    output = outputMax;
  else if (output < outputMin)
    output = outputMin;
  return output;
}

/***************************************************************
** ��������: Steer
** ��������: ң���ٶȼ���������
** ������:  ��һ��
** �ա���:   2020.05.05
***************************************************************/
void Steer(float direct, float speed)
{
	if(direct > 0)
		g_fBluetoothDirection = Scale(direct, 0, 10, 0, 400);
	else
		g_fBluetoothDirection = -Scale(direct, 0, -10, 0, 400);

	if(speed > 0)
		g_iCarSpeedSet = Scale(speed, 0, 10, 0, 70);
	else
		g_iCarSpeedSet = -Scale(speed, 0, -10, 0, 70);

}
/***************************************************************
** ��������: GetTim4Encoder, GetTim2Encoder
** ��������: ��ȡTIM4/TIM2��ʱ���������ı���������
** ������:  ��һ��
** �ա���:   2020.06.05
***************************************************************/
int GetTim4Encoder(void) //��
{
	delay_ms(30);
	g_iTim4Encoder=(short)(TIM_GetCounter(TIM4));
	TIM4->CNT = 0;
	return g_iTim4Encoder;
 
}
int GetTim2Encoder(void)
{
	delay_ms(30);
	g_iTim2Encoder=(short)(TIM_GetCounter(TIM2));
	TIM2->CNT = 0;
	return g_iTim2Encoder;
	
}

/***************************************************************
** ��������: ScDistance
** ��������: �����������߾���
** ������:  ��һ����������
** �ա���:   2020.06.05
***************************************************************/
void ScDistance(float direct, float speed,float length)
{
	int delaytime;

	 
	if(direct==1 && speed==0 && length==1)  //��ת 
    {
			StatusFlag=3;
			
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
			delay_ms(50);
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
			
      Play_Sound("�յ���������ת");
			StatusFlag=8;
			
			Steer(3,0);
			delay_ms(2000);
			Steer(0,0);

	  }   
   if(direct==-1 && speed==0 && length==-1)//��ת
		{ 
			StatusFlag=2;
		
      GPIO_SetBits(GPIOB, GPIO_Pin_12);
			delay_ms(50);
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
			
			Play_Sound("�յ���������ת");
			StatusFlag=8;
			
			Steer(-3,0);
			delay_ms(2000);
			Steer(0,0);

		}
		if(speed==6) //ֱ��
	  {
			StatusFlag=4;
			
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
			delay_ms(50);
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
			
			Play_Sound("�յ���������ǰ��");
			StatusFlag=8;
			
			Steer(0,-6);
			delaytime=length*20;
			delay_ms(delaytime);
			Steer(0,0);

		}
		 if(speed==-6) //����
	  {
			StatusFlag=5;
			
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
			delay_ms(50);
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
				
			Play_Sound("�յ������ڵ���");
			StatusFlag=8;
			
			Steer(0, 6);
			delaytime=length*20;
			delay_ms(delaytime);
			Steer(0,0);
    }
	
}


/***************************************************************
** ����  ��: ��һ��
** �ա�  ��:2020.05.05
** ��������: UltraControl
** ��������: ����������/����        
**********************************************/
void UltraControl(int mode)
{
	if(mode == 0)
	{
		if((Distance >= 0) && (Distance<= 12))
		{//����С��12cm�����
			Steer(0, 4);
		}
		else if((Distance> 18) && (Distance<= 30))	
		{//�������18cm��С��30��ǰ��
			Steer(0, -4);
		}
		else
			Steer(0, 0);
	}
	else if(mode == 1)
	{
		if((Distance >= 0) && (Distance<= 20))
		{//��ת750�����������ת��Ƕ�ԼΪ90��
			Steer(5, 0);
			g_iLeftTurnRoundCnt = 750;
			g_iRightTurnRoundCnt = -750;
		}
		if((g_iLeftTurnRoundCnt < 0)&&(g_iRightTurnRoundCnt > 0))
		{
			Steer(0, -4);
		}
	}
}


