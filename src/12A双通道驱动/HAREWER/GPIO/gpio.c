#include "gpio.h"

void Gpio_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;            //定义结构体GPIO_InitStructure
	
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // 使能PB端口时钟  
  GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_7| GPIO_Pin_8;	  //PB7 PB8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     	//推挽，增大电流输出能力  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO口速度
  GPIO_Init(GPIOB, &GPIO_InitStructure);          //GBIOB初始化  
	
}
