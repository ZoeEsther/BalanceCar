#ifndef __CONTROL_H
#define __CONTROL_H


typedef struct _PID_t{
	float P;
	float I;
	float D;
}PID_t;

#define ACCEL_SENSITIVITY  16384.0
#define GYRO_SENSITIVITY 16.4
#define M_PI 3.14159265359
#define    GYRO_OFFSET      0          //���������ƫ��ֵ
//#define 	 GRAVITY_OFFSET   (g_iGravity_Offset)      			//
#define    CAR_ZERO_ANGLE   (2)
#define CAR_ANGLE_SET 0
#define CAR_ANGLE_SPEED_SET 0
/******�ٶȿ�����غ궨��******/
#define CAR_POSITION_SET      0
#define CAR_SPEED_SET         g_iCarSpeedSet
#define MOTOR_LEFT_SPEED_POSITIVE  (g_fLeftMotorOut >0)
#define MOTOR_RIGHT_SPEED_POSITIVE (g_fRightMotorOut>0)
#define SPEED_CONTROL_PERIOD	 25	    //�ٶȻ���������
#define CAR_SPEED_CONSTANT		(1000.0/(float)SPEED_CONTROL_PERIOD/(float)OPTICAL_ENCODE_CONSTANT)
#define CAR_POSITION_MAX	700
#define CAR_POSITION_MIN	(-900)
/******���������غ궨��******/
#define MOTOR_OUT_DEAD_VAL       0	   //����ֵ8
#define MOTOR_OUT_MAX           999	   //ռ�ձ������ֵ
#define MOTOR_OUT_MIN         (-999)   //ռ�ձȸ����ֵ


extern short  g_s16LeftMotorPulse;
extern short  g_s16RightMotorPulse;

extern int  g_s32LeftMotorPulseSigma;
extern int  g_s32RightMotorPulseSigma;

extern int  g_iTim4Encoder;//��ʱ���TIM4��ʱ���������ı���������  ����
extern int  g_iTim2Encoder;//��ʱ���TIM2��ʱ���������ı���������  ����

extern int  g_iGravity_Offset;

extern float g_fCarAngle;         		
extern float g_fGyroAngleSpeed;		   			
extern float g_fGravityAngle;			

extern PID_t g_tCarAnglePID;
extern PID_t g_tCarSpeedPID;

extern float g_fBluetoothSpeed;
extern float g_fBluetoothDirection;
extern unsigned char g_u8MainEventCount;
extern unsigned char g_u8SpeedControlCount;
extern unsigned char g_u8SpeedControlPeriod;
extern unsigned char g_u8DirectionControlPeriod;
extern unsigned char g_u8DirectionControlCount;

//extern unsigned char g_StatysFlag;



void CarUpstandInit(void);                       //
void MotorManage(void);                          //�������
void AngleControl(void)	 ;                       //          �ǶȻ�����
void MotorOutput(void);                          //������
void SpeedControl(void);                         //          �ٶȻ�����
void BluetoothControl(void)	;                    //
void GetMotorPulse(void);                        // 
void AngleCalculate(void);                       //          �ǶȻ�����
void SpeedControlOutput(void);                   //          �ٶȻ��������
void DirectionControlOutput(void);               //
void DirectionControl(void);                     //
void Steer(float direct, float speed);           //ң���ٶȺͷ�����
void UltraControl(int mode);                     //���������桢����
void ScDistance(float direct, float speed,float length);//��������ʱ���ٶȣ����򣬾��봦��

//void TailingControl(void);                       //�����߼��

int GetTim4Encoder(void);                          //���ֵ�����������
int GetTim2Encoder(void);

#endif
