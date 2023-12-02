#ifndef __BALANCE_H
#define __BALANCE_H			  	 
#include "sys.h"
#include "system.h"
#define BALANCE_TASK_PRIO		4     //Task priority //任务优先级
#define BALANCE_STK_SIZE 		512   //Task stack size //任务堆栈大小

//Parameter of kinematics analysis of omnidirectional trolley
//全向轮小车运动学分析参数
extern int SLIDE_POSITION,SERVO_BIAS;
extern float Velocity_Smoother_Rate, Angle_Smoother_Rate, Velocity_Smoother_Rate_Z;
extern int Servo_max, Servo_min;
#define FILTERING_TIMES 20

extern int A,B,C,DD;
extern u8 command_lost_count;//串口、CAN控制命令丢失时间计数，丢失1秒后停止控制
void Balance_task(void *pvParameters);
void Limit_Pwm(int amplitude);
float target_limit_float(float insert,float low,float high);
int target_limit_int(int insert,int low,int high);
u8 Turn_Off( int voltage);
u32 myabs(long int a);
float float_abs(float insert);
int Incremental_PI_A (float Encoder,float Target);
int Incremental_PI_B (float Encoder,float Target);
int Incremental_PI_C (float Encoder,float Target);
int Incremental_PI_D (float Encoder,float Target);

void Get_RC(void);
void PS2_control(void);
void Remote_Control(void);
void Set_Pwm(int motor_a,int motor_b,int servo);
void Drive_Motor(float Vx,float Vz);
void Key(void);
void Get_Velocity_Form_Encoder(void);
void robot_mode_check(void);
int Incremental_SERVO (float SlidePosition,float SlideTarget);

void Smooth_control(float vx, float step);
#endif  

