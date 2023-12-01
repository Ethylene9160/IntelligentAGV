#include "pwm.h"



/**************************************************************************
函数功能：pwm初始化
入口参数：arr：设为一个时钟频率的最大值  psc： 预分频值
返回  值：无
**************************************************************************/

void	pwm_int(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;                 //定义结构体GPIO_InitStructure
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       //定义结构TIM_TimeBaseStructure
	 TIM_OCInitTypeDef  TIM_OCInitStructure;               //定义结构TIM_OCInitStructure

	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  //使能定时器4时钟
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能PB端口时钟

	 GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_6;          //PB6
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //IO口速度
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     	//复用模式输出
	 GPIO_Init(GPIOB, &GPIO_InitStructure);              //GBIOB初始化  
	
   TIM_TimeBaseStructure.TIM_Period = 7199;    //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
   TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置用来作为TIMx时钟频率除数的预分频值  不分频
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;  //设置时钟分割:TDTS = Tck_tim
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
   TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //PWMTIM脉冲宽度调制模式2
   TIM_OCInitStructure.TIM_Pulse  = 0;                 //输入脉冲捕获值
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //设置TIM输出比较极性为高
   TIM_OCInitStructure.TIM_OutputState=	TIM_OutputState_Enable;   //比较输出使能
   TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStructure中指定的参数初始化外设TIM4
   
   TIM_CtrlPWMOutputs(TIM4,ENABLE);                       //主输出使能
	 
   TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);	    //使能预装载寄存器
	
   TIM_ARRPreloadConfig(TIM4, ENABLE);      //使能自动装载允许位

   TIM_Cmd(TIM4, ENABLE);   //启动定时器TIM4

}





