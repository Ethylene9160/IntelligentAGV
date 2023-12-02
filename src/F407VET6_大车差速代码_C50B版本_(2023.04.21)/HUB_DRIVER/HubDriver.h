#ifndef __HUBDRIVER_H
#define __HUBDRIVER_H	 

#include "sys.h"
#include "system.h"
/**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/


#define Synchronization  0
#define Asynchronization 1
#define Relate           1
#define Absolute         0

//Diameter of trolley tire
//无刷电机轮胎直径
#define HUB_130_Diameter  0.13
#define HUB_200_Diameter  0.20

#define HUB_wheelspacing  0.2272
extern float left_rpm, right_rpm;
extern float Left_encoder_rpm, Right_encoder_rpm;
	
void HUB_init(void);

unsigned short ModbusCRC16_Check(u8 array[], int BitNumber);

void Enable(void);
void Set_stop(void);

void hub_485_velocity_init(void);
void hub_485_position_init(void);
void hub_485_torque_init(void);

void V_mode_set(void);
void T_mode_set(void);
void Relate_position_mode_set(int relate);

void Left_acc_time(int time);
void Left_dec_time(int time);
void Right_acc_time(int time);
void Right_dec_time(int time);

void Asyn_ctrl_set(int asyn);

void SetMaxRpm_left_Right(int Left, int Right);

void Left_Torque_mAs(int mAs);
void Right_Torque_mAs(int mAs);

void hub_485_Rpm(int R_L, int rpm);
void hub_485_Position(int R_L, long int position);
void hub_485_Torque(int R_L, int torque);
void hub_485_Syn_Rpm(int rpm);
void hub_485_Syn_Position(long int position);
void hub_485_Syn_Torque(int torque);

#endif





















