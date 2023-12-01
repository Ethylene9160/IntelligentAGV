#include "pwm.h"



/**************************************************************************
�������ܣ�pwm��ʼ��
��ڲ�����arr����Ϊһ��ʱ��Ƶ�ʵ����ֵ  psc�� Ԥ��Ƶֵ
����  ֵ����
**************************************************************************/

void	pwm_int(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;                 //����ṹ��GPIO_InitStructure
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       //����ṹTIM_TimeBaseStructure
	 TIM_OCInitTypeDef  TIM_OCInitStructure;               //����ṹTIM_OCInitStructure

	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  //ʹ�ܶ�ʱ��4ʱ��
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PB�˿�ʱ��

	 GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_6;          //PB6
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //IO���ٶ�
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     	//����ģʽ���
	 GPIO_Init(GPIOB, &GPIO_InitStructure);              //GBIOB��ʼ��  
	
   TIM_TimeBaseStructure.TIM_Period = 7199;    //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
   TIM_TimeBaseStructure.TIM_Prescaler = 0; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;  //����ʱ�ӷָ�:TDTS = Tck_tim
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
   TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //PWMTIM�����ȵ���ģʽ2
   TIM_OCInitStructure.TIM_Pulse  = 0;                 //�������岶��ֵ
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //����TIM����Ƚϼ���Ϊ��
   TIM_OCInitStructure.TIM_OutputState=	TIM_OutputState_Enable;   //�Ƚ����ʹ��
   TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStructure��ָ���Ĳ�����ʼ������TIM4
   
   TIM_CtrlPWMOutputs(TIM4,ENABLE);                       //�����ʹ��
	 
   TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);	    //ʹ��Ԥװ�ؼĴ���
	
   TIM_ARRPreloadConfig(TIM4, ENABLE);      //ʹ���Զ�װ������λ

   TIM_Cmd(TIM4, ENABLE);   //������ʱ��TIM4

}





