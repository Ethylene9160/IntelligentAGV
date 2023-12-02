#ifndef __ROBOTSELECTINIT_H
#define __ROBOTSELECTINIT_H
#include "sys.h"
#include "system.h"

//Parameter structure of robot
//机器人参数结构体
typedef struct  
{
  float WheelSpacing;      //Wheelspacing, Mec_Car is half wheelspacing //轮距 麦轮车为半轮距
  float AxleSpacing;       //Axlespacing, Mec_Car is half axlespacing //轴距 麦轮车为半轴距	
  int   GearRatio;         //Motor_gear_ratio //电机减速比
  int   EncoderAccuracy;   //Number_of_encoder_lines //编码器精度(编码器线数)
  float WheelDiameter;     //Diameter of driving wheel //主动轮直径	
  float OmniTurnRadiaus;   //Rotation radius of omnidirectional trolley //全向轮小车旋转半径
}Robot_Parament_InitTypeDef;

//The minimum turning radius of different Ackermann models is determined by the mechanical structure:
//the maximum Angle of the wheelbase, wheelbase and front wheel
//不同阿克曼车型的最小转弯半径，由机械结构决定：轮距、轴距、前轮最大转角
#define   SENIOR_AKM_MIN_TURN_RADIUS       0.750f 
#define   TOP_AKM_BS_MIN_TURN_RADIUS       1.400f 
#define   TOP_AKM_DL_MIN_TURN_RADIUS       1.200f 

//Axle_spacing //轴距
#define   SENIOR_AKM_axlespacing           0.322f 
#define   TOP_AKM_BS_axlespacing           0.590f 
#define   TOP_AKM_DL_axlespacing           0.530f 

//Wheel_spacing //轮距
#define   SENIOR_AKM_wheelspacing          0.322f  
#define   TOP_AKM_BS_wheelspacing          0.508f 
#define   TOP_AKM_DL_wheelspacing          0.585f 
#define   TOP_DIFF_wheelspacing            0.329f
#define   FOUR_WHEEL_DIFF_BS_wheelspacing  0.573f
#define   FOUR_WHEEL_DIFF_DL_wheelspacing  0.573f

//Motor_gear_ratio
//电机减速比
#define   MD36N_5_18  5.18
#define   MD36N_27    27
#define   MD36N_51    51
#define   MD36N_71    71
#define   MD60N_18    18
#define   MD60N_47    47

//Number_of_encoder_lines
//编码器精度
#define		Photoelectric_500 500
#define	  Hall_13 13

//Diameter of trolley tire
//小车轮胎直径
#define   SENIOR_AKM_Tyre_Diameter      0.125
#define   TOP_AKM_BS_WHEEL_Diameter     0.180
#define   TOP_AKM_DL_Tyre_Diameter      0.254
#define   TOP_DIFF_Tyre_Diameter        0.125
#define   FOUR_WHEEL_DIFF_Tyre_Diameter 0.215

//Mecanum wheel tire diameter series
//麦轮轮胎直径
#define		Mecanum_60  60
#define		Mecanum_75  75
#define		Mecanum_100 100
#define		Mecanum_127 127
#define		Mecanum_152 152
	   
//Omni wheel tire diameter series
//轮径全向轮直径系列
#define	  FullDirecion_75  75
#define	  FullDirecion_127 127
#define	  FullDirecion_152 152
#define	  FullDirecion_203 203
#define	  FullDirecion_217 217


//The encoder octave depends on the encoder initialization Settings
//编码器倍频数，取决于编码器初始化设置
#define   EncoderMultiples 4
//Encoder data reading frequency
//编码器数据读取频率
#define CONTROL_FREQUENCY 100

void Robot_Select(void);
void Robot_Init(float wheelspacing,float axlespacing,int gearratio,int Accuracy,float tyre_diameter) ;

#endif
