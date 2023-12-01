#include "gpio.h"

void Gpio_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;            //����ṹ��GPIO_InitStructure
	
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // ʹ��PB�˿�ʱ��  
  GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_7| GPIO_Pin_8;	  //PB7 PB8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     	//���죬��������������  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO���ٶ�
  GPIO_Init(GPIOB, &GPIO_InitStructure);          //GBIOB��ʼ��  
	
}
