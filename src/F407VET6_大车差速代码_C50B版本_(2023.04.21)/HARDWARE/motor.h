
#ifndef __MOTOR_H
#define __MOTOR_H

#include "system.h"

/*--------Motor_A control pins--------*/
#define PWM_PORTA GPIOC			 //PWMA
#define PWM_PIN_A GPIO_Pin_9//PWMA
#define PWMA 	  TIM8->CCR4	 //PWMA

#define IN1_PORTA GPIOB			 //AIN1
#define IN1_PIN_A GPIO_Pin_13 //AIN1
#define AIN1 	  PBout(13)		 //AIN1

#define IN2_PORTA GPIOB			 //AIN2
#define IN2_PIN_A GPIO_Pin_12 //AIN2
#define AIN2 	  PBout(12)		 //AIN2
/*------------------------------------*/

/*--------Motor_B control pins--------*/
#define PWM_PORTB GPIOC			 //PWMB
#define PWM_PIN_B GPIO_Pin_8 //PWMB
#define PWMB 	  TIM8->CCR3	 //PWMB

#define IN1_PORTB GPIOC			 //BIN1
#define IN1_PIN_B GPIO_Pin_0 //BIN1
#define BIN1 	  PCout(0)		 //BIN1

#define IN2_PORTB GPIOB			 //BIN2
#define IN2_PIN_B GPIO_Pin_14 //BIN2
#define BIN2 	  PBout(14)		 //BIN2
/*------------------------------------*/

/*--------Motor_C control pins--------*/
#define PWM_PORTC GPIOC			 //PWMC
#define PWM_PIN_C GPIO_Pin_7 //PWMC
#define PWMC 	  TIM8->CCR2	 //PWMC

#define IN1_PORTC GPIOD			  //CIN1
#define IN1_PIN_C GPIO_Pin_10	//CIN1
#define CIN1 	  PDout(10)		  //CIN1

#define IN2_PORTC GPIOD			 //CIN2
#define IN2_PIN_C GPIO_Pin_12 //CIN2
#define CIN2 	  PDout(12)		 //CIN2
/*------------------------------------*/

/*--------Motor_D control pins--------*/
#define PWM_PORTD GPIOC			 //PWMD
#define PWM_PIN_D GPIO_Pin_6 //PWMD
#define PWMD 	  TIM8->CCR1	 //PWMD

#define IN1_PORTD GPIOA			  //DIN1
#define IN1_PIN_D GPIO_Pin_5	//DIN1
#define DIN1 	  PAout(5)		  //DIN1

#define IN2_PORTD GPIOA			  //DIN2
#define IN2_PIN_D GPIO_Pin_4	//DIN2
#define DIN2 	  PAout(4)		  //DIN2



/*------------------------------------*/
#define EN     PEin(4)  
#define Servo_PWM  TIM9->CCR1
#define SERVO_INIT 1500  //Servo zero point //¶æ»úÁãµã

void Enable_Pin(void);
void MiniBalance_PWM_Init(u16 arr,u16 psc);
void MiniBalance_Motor_Init(void);
void Servo_PWM_Init(u16 arr,u16 psc);
#endif
