#ifndef __LED_H
#define __LED_H
#include "sys.h"
#include "system.h"

#define LED_TASK_PRIO		3     //Task priority //任务优先级
#define LED_STK_SIZE 		128   //Task stack size //任务堆栈大小


/*--------Buzzer control pin--------*/
#define Buzzer_PORT GPIOD
#define Buzzer_PIN GPIO_Pin_11
#define Buzzer PDout(11)
/*----------------------------------*/

/*--------LED control pin--------*/
#define LED_PORT GPIOD
#define LED_R_PIN GPIO_Pin_14
#define LED_G_PIN GPIO_Pin_13
#define LED_B_PIN GPIO_Pin_15
#define LED_R PDout(14) 
#define LED_G PDout(13) 
#define LED_B PDout(15) 
/*----------------------------------*/

void LED_Init(void);  
void Buzzer_Init(void); 
void Led_Flash(u16 time);
void led_task(void *pvParameters);
extern int Led_Count;
#endif
