#ifndef __HUBDRIVERCAN_H
#define __HUBDRIVERCAN_H	 

#include "sys.h"
#include "system.h"

extern int acc_time;
extern int BrushlessEnable;

void hub_CAN_velocity_init(void);
void hub_CAN_position_init(void);
void hub_CAN_torque_init(void);

void CAN_setBaudrate(int Baudrate);
void CAN_Write_EEPROM(void);

void CAN_Stop(void);
void CAN_Enable(void);

void CAN_Asyn_ctrl_set(int asyn);

void CAN_V_mode_set(void);
void CAN_P_mode_set(void);
void CAN_T_mode_set(void);

void CAN_acc_time(int time);
void CAN_acc_time_left(int time);
void CAN_dec_time_left(int time);
void CAN_acc_time_right(int time);
void CAN_dec_time_right(int time);
	
void CAN_SetMaxRpm_left_Right(int Left, int Right);

void CAN_Left_Torque_mAs(int mAs);
void CAN_Right_Torque_mAs(int mAs);

void hub_CAN_Rpm(int R_L, int rpm);
//void hub_CAN_Syn_Rpm(int rpm);
void hub_CAN_Syn_Rpm(int left_rpm, int right_rpm);
void hub_CAN_Rel_Position(int R_L, long int position);
void hub_CAN_Abs_Position(int R_L, long int position);
void hub_CAN_Torque(int R_L, int torque);
void hub_CAN_Syn_Torque(int torque);

void hub_CAN_Encoder_init(void);
#endif

