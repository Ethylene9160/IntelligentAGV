#include "timer.h"

//Input the capture flag for channel 1, 
//the capture flag for the higher bits, and the overflow flag for the lower 6 bits
//ͨ��1���벶���־������λ�������־����6λ�������־		
u8 TIM1CH1_CAPTURE_STA = 0;	
u16 TIM1CH1_CAPTURE_UPVAL;
u16 TIM1CH1_CAPTURE_DOWNVAL;

//Input the capture flag for channel 2, 
//the capture flag for the higher bits, and the overflow flag for the lower 6 bits
//ͨ��2���벶���־������λ�������־����6λ�������־	
u8 TIM1CH2_CAPTURE_STA = 0;		
u16 TIM1CH2_CAPTURE_UPVAL;
u16 TIM1CH2_CAPTURE_DOWNVAL;

//Input the capture flag for channel 3, 
//the capture flag for the higher bits, and the overflow flag for the lower 6 bits
//ͨ��3���벶���־������λ�������־����6λ�������־	
u8 TIM1CH3_CAPTURE_STA = 0;		
u16 TIM1CH3_CAPTURE_UPVAL;
u16 TIM1CH3_CAPTURE_DOWNVAL;

//Input the capture flag for channel 4, 
//the capture flag for the higher bits, and the overflow flag for the lower 6 bits
//ͨ��4���벶���־������λ�������־����6λ�������־
u8 TIM1CH4_CAPTURE_STA = 0;			
u16 TIM1CH4_CAPTURE_UPVAL;
u16 TIM1CH4_CAPTURE_DOWNVAL;

u32 TIM1_T1;
u32 TIM1_T2;
u32 TIM1_T3;
u32 TIM1_T4;

//Variables related to remote control acquisition of model aircraft
//��ģң�زɼ���ر���
int Remoter_Ch1=1500,Remoter_Ch2=1500,Remoter_Ch3=1500,Remoter_Ch4=1500;
//Model aircraft remote control receiver variable
//��ģң�ؽ��ձ���
int L_Remoter_Ch1=1500,L_Remoter_Ch2=1500,L_Remoter_Ch3=1500,L_Remoter_Ch4=1500;  

/**************************************************************************
Function: Model aircraft remote control initialization function, timer 1 input capture initialization
Input   : arr: Automatic reload value, psc: clock preset frequency
Output  : none
�������ܣ���ģң�س�ʼ����������ʱ��1���벶���ʼ��
��ڲ�����arr���Զ���װֵ��psc��ʱ��Ԥ��Ƶ�� 
�� �� ֵ����
**************************************************************************/ 
void TIM1_Cap_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//ʹ��PORTEʱ��	
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOE,&GPIO_InitStructure); 

	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);

	/*** Initialize timer 1 || ��ʼ����ʱ��1 ***/
	//Set the counter to automatically reload //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Period = arr; 
	//Pre-divider //Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 	
	//Set the clock split: TDTS = Tck_tim //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	//TIM up count mode //TIM���ϼ���ģʽ	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	//Initializes the timebase unit for TIMX based on the parameter specified in TIM_TimeBaseInitStruct
	//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 

	/*** ��ʼ��TIM1���벶�������ͨ��1 || Initialize TIM1 for the capture parameter, channel 1 ***/
	//Select input //ѡ������� 
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; 
  //Rising edge capture //�����ز���
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
	//Configure input frequency division, regardless of frequency //���������Ƶ,����Ƶ 
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	
  //IC1F=0000 Configure input filter //���������˲���
	TIM_ICInitStructure.TIM_ICFilter = 0x0F;	  
	TIM_ICInit(TIM1, &TIM_ICInitStructure);

	/*** ��ʼ��TIM1���벶�������ͨ��2 || Initialize TIM1 for the capture parameter, channel 2 ***/
	//CC1S=01 Select input //ѡ������� 
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	//Rising edge capture //�����ز���
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
	//Configure input frequency division, regardless of frequency //���������Ƶ,����Ƶ 
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  
	TIM_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲���
	TIM_ICInit(TIM1, &TIM_ICInitStructure);

	/*** ��ʼ��TIM1���벶�������ͨ��3 || Initialize TIM1 for the capture parameter, channel 3 ***/
	//Select input //ѡ������� 
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;   
	//Rising edge capture //�����ز���
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
	//Configure input frequency division, regardless of frequency //���������Ƶ,����Ƶ 
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  
	//IC1F=0000 Configure input filter //���������˲��������˲�  
	TIM_ICInitStructure.TIM_ICFilter = 0x00;	  
	TIM_ICInit(TIM1, &TIM_ICInitStructure);

	/*** ��ʼ��TIM1���벶�������ͨ��4 || Initialize TIM1 for the capture parameter, channel 4 ***/
	//Select input //ѡ������� 
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4; 
	//Rising edge capture //�����ز���
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
	//Configure input frequency division, regardless of frequency //���������Ƶ,����Ƶ 
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	   
	//IC1F=0000 Configure input filter //���������˲��������˲�  
	TIM_ICInitStructure.TIM_ICFilter = 0x00;	  
	TIM_ICInit(TIM1, &TIM_ICInitStructure);

  /*** interrupt packet initialization || �жϷ����ʼ�� ***/
  //TIM1 interrupts //TIM1�ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn; 
  //Preempt priority 0 //��ռ���ȼ�0��	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	//Level 0 from priority //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	//IRQ channels are enabled //IRQͨ����ʹ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	//Initializes the peripheral NVIC register according to the parameters specified in NVIC_InitStruct
	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	NVIC_Init(&NVIC_InitStructure);   
	
  //Allow CC1IE,CC2IE,CC3IE,CC4IE to catch interrupts, not allowed update_interrupts
  //����������жϣ�����CC1IE,CC2IE,CC3IE,CC4IE�����ж�	
	TIM_ITConfig(TIM1, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,	ENABLE);   
	//Advanced timer output must be enabled //�߼���ʱ���������ʹ�����	
	TIM_CtrlPWMOutputs(TIM1,ENABLE); 	
	//Enable timer //ʹ�ܶ�ʱ��
	TIM_Cmd(TIM1, ENABLE); 		
}
/**************************************************************************
Function: Model aircraft remote control receiving interrupt, namely timer 1 input capture interrupt
Input   : none
Output  : none
�������ܣ���ģң�ؽ����жϣ�����ʱ��1���벶���ж�
��ڲ�������
�� �� ֵ����
**************************************************************************/ 
void TIM1_CC_IRQHandler(void)
{
	//���Ӻ�ģңң��������Ҫ����ǰ���ˣ��ſ�����ʽ��ģ����С��
	//After connecting the remote controller of the model aircraft, 
	//you need to push down the forward lever to officially control the car of the model aircraft
  if(Remoter_Ch2>1600&&Remote_ON_Flag==0&&Deviation_Count>=CONTROL_DELAY)
  {
		//Model aircraft remote control mark position 1, other marks position 0
		//��ģң�ر�־λ��1��������־λ��0
		Remote_ON_Flag=1;
	  APP_ON_Flag=0;
		PS2_ON_Flag=0;
		CAN_ON_Flag=0;
		Usart_ON_Flag=0; 
	}
	 
	//Channel 1 //ͨ��һ
	if ((TIM1CH1_CAPTURE_STA & 0X80) == 0) 			
	{
		if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET) //A capture event occurred on channel 1 //ͨ��1���������¼�
		{
			TIM_ClearITPendingBit(TIM1, TIM_IT_CC1); //Clear the interrupt flag bit //����жϱ�־λ
			if (TIM1CH1_CAPTURE_STA & 0X40)	//A falling edge is caught //����һ���½���
			{
				TIM1CH1_CAPTURE_DOWNVAL = TIM_GetCapture1(TIM1); //Record the timer value at this point //��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM1CH1_CAPTURE_DOWNVAL < TIM1CH1_CAPTURE_UPVAL)
				{
					TIM1_T1 = 9999;
				}
				else
					TIM1_T1 = 0;
				Remoter_Ch1 = TIM1CH1_CAPTURE_DOWNVAL - TIM1CH1_CAPTURE_UPVAL + TIM1_T1;	//Time to get the total high level //�õ��ܵĸߵ�ƽ��ʱ��
				if(abs(Remoter_Ch1-L_Remoter_Ch1)>500) Remoter_Ch1=L_Remoter_Ch1; //Filter //�˲�
					 L_Remoter_Ch1=Remoter_Ch1;
				
				TIM1CH1_CAPTURE_STA = 0; //Capture flag bit to zero	//�����־λ����
				TIM_OC1PolarityConfig(TIM1, TIM_ICPolarity_Rising); //Set to rising edge capture //����Ϊ�����ز���		  
			}
			else 
			{
				//When the capture time occurs but not the falling edge, the first time the rising edge is captured, record the timer value at this time
				//��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
				TIM1CH1_CAPTURE_UPVAL = TIM_GetCapture1(TIM1); //Obtain rising edge data //��ȡ����������
				TIM1CH1_CAPTURE_STA |= 0X40; //The flag has been caught on the rising edge //����Ѳ���������
				TIM_OC1PolarityConfig(TIM1, TIM_ICPolarity_Falling); //Set to Falling Edge Capture //����Ϊ�½��ز���
			}
		}
	}
  //Channel 2 //ͨ����
	if ((TIM1CH2_CAPTURE_STA & 0X80) == 0)		
	{
		if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)	//A capture event occurred on channel 2 //ͨ��2���������¼�
		{
			TIM_ClearITPendingBit(TIM1, TIM_IT_CC2); //Clear the interrupt flag bit //����жϱ�־λ
			if (TIM1CH2_CAPTURE_STA & 0X40)	//A falling edge is caught //����һ���½���
			{
				TIM1CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM1); //Record the timer value at this point //��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM1CH2_CAPTURE_DOWNVAL < TIM1CH2_CAPTURE_UPVAL)
				{
					TIM1_T2 = 9999;
				}
				else
					TIM1_T2 = 0;
				Remoter_Ch2 = TIM1CH2_CAPTURE_DOWNVAL - TIM1CH2_CAPTURE_UPVAL + TIM1_T2; //Time to get the total high level //�õ��ܵĸߵ�ƽ��ʱ��
				if(abs(Remoter_Ch2-L_Remoter_Ch2)>500)Remoter_Ch2=L_Remoter_Ch2; //Filter //�˲�
				L_Remoter_Ch2=Remoter_Ch2;
				
				TIM1CH2_CAPTURE_STA = 0; //Capture flag bit to zero	//�����־λ����
				TIM_OC2PolarityConfig(TIM1, TIM_ICPolarity_Rising); //Set to rising edge capture //����Ϊ�����ز���		  
			}
			else 
			{
				//When the capture time occurs but not the falling edge, the first time the rising edge is captured, record the timer value at this time
				//��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
				TIM1CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM1); //Obtain rising edge data //��ȡ����������
				TIM1CH2_CAPTURE_STA |= 0X40; //The flag has been caught on the rising edge //����Ѳ���������
				TIM_OC2PolarityConfig(TIM1, TIM_ICPolarity_Falling); //Set to Falling Edge Capture //����Ϊ�½��ز���
			}
		}
	}
  //Channel 3 //ͨ����
	if ((TIM1CH3_CAPTURE_STA & 0X80) == 0)			
	{
		if (TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)	//A capture event occurred on channel 3 //ͨ��3���������¼�
		{
			TIM_ClearITPendingBit(TIM1, TIM_IT_CC3); //Clear the interrupt flag bit //����жϱ�־λ
			if (TIM1CH3_CAPTURE_STA & 0X40)	//A falling edge is caught //����һ���½���
			{
				TIM1CH3_CAPTURE_DOWNVAL = TIM_GetCapture3(TIM1); //Record the timer value at this point //��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM1CH3_CAPTURE_DOWNVAL < TIM1CH3_CAPTURE_UPVAL)
				{
					TIM1_T3 = 9999;
				}
				else
					TIM1_T3 = 0;
				Remoter_Ch3 = TIM1CH3_CAPTURE_DOWNVAL - TIM1CH3_CAPTURE_UPVAL + TIM1_T3; //Time to get the total high level //�õ��ܵĸߵ�ƽ��ʱ��
				if(abs(Remoter_Ch3-L_Remoter_Ch3)>500)Remoter_Ch3=L_Remoter_Ch3; //Filter //�˲�
									L_Remoter_Ch3=Remoter_Ch3;
				TIM1CH3_CAPTURE_STA = 0; //Capture flag bit to zero	//�����־λ����
				TIM_OC3PolarityConfig(TIM1, TIM_ICPolarity_Rising); //Set to rising edge capture //����Ϊ�����ز���		  
			}
			else 
			{
				//When the capture time occurs but not the falling edge, the first time the rising edge is captured, record the timer value at this time
				//��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
				TIM1CH3_CAPTURE_UPVAL = TIM_GetCapture3(TIM1); //Obtain rising edge data //��ȡ����������
				TIM1CH3_CAPTURE_STA |= 0X40; //The flag has been caught on the rising edge //����Ѳ���������
				TIM_OC3PolarityConfig(TIM1, TIM_ICPolarity_Falling); //Set to Falling Edge Capture //����Ϊ�½��ز���
			}
		}
	}

		//Channel 4 //ͨ����
		if ((TIM1CH4_CAPTURE_STA & 0X80) == 0)		
		{
			if (TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)	//A capture event occurred on channel 4 //ͨ��4���������¼�
			{
				TIM_ClearITPendingBit(TIM1, TIM_IT_CC4); //Clear the interrupt flag bit //����жϱ�־λ
				if (TIM1CH4_CAPTURE_STA & 0X40)	//A falling edge is caught //����һ���½���
				{
					TIM1CH4_CAPTURE_DOWNVAL = TIM_GetCapture4(TIM1); //Record the timer value at this point //��¼�´�ʱ�Ķ�ʱ������ֵ
					if (TIM1CH4_CAPTURE_DOWNVAL < TIM1CH4_CAPTURE_UPVAL)
					{
						TIM1_T4 = 9999;
					}
					else
						TIM1_T4 = 0;
					Remoter_Ch4 = TIM1CH4_CAPTURE_DOWNVAL - TIM1CH4_CAPTURE_UPVAL + TIM1_T4; //Time to get the total high level //�õ��ܵĸߵ�ƽ��ʱ��
					if(abs(Remoter_Ch4-L_Remoter_Ch4)>500)Remoter_Ch4=L_Remoter_Ch4; //Filter //�˲�
					L_Remoter_Ch4=Remoter_Ch4;				
					TIM1CH4_CAPTURE_STA = 0; //Capture flag bit to zero	//�����־λ����
					TIM_OC4PolarityConfig(TIM1, TIM_ICPolarity_Rising); //Set to rising edge capture //����Ϊ�����ز���		  
				}
				else 
				{
					//When the capture time occurs but not the falling edge, the first time the rising edge is captured, record the timer value at this time
				  //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
					TIM1CH4_CAPTURE_UPVAL = TIM_GetCapture4(TIM1); //Obtain rising edge data //��ȡ����������
					TIM1CH4_CAPTURE_STA |= 0X40; //The flag has been caught on the rising edge //����Ѳ���������
					TIM_OC4PolarityConfig(TIM1, TIM_ICPolarity_Falling); //Set to Falling Edge Capture //����Ϊ�½��ز���
				}
			}
		}
}
/**************************************************************************
Function: TIM1 Update Interrupt
Input   : none
Output  : none
�������ܣ���ʱ��1�����ж�
��ڲ�������
����  ֵ���� 
**************************************************************************/
void TIM1_UP_TIM10_IRQHandler(void) 
{ 
	//Clear the interrupt flag bit
	//����жϱ�־λ 
  TIM1->SR&=~(1<<0);	    
}

