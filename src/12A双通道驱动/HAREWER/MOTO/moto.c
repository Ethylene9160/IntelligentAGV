#include "moto.h"


/**************************************************************************
�������ܣ����������ת
��ڲ�����mode   mode=0ʱΪ��ת  mode=1ʱ��ת
����  ֵ����
**************************************************************************/


void moto(int mode)
{
	if(mode==1)
	{
	 GPIO_SetBits(GPIOB, GPIO_Pin_7);	 // �ߵ�ƽ
	 GPIO_ResetBits(GPIOB, GPIO_Pin_8);	 // �͵�ƽ
		
	 TIM_SetCompare1(TIM4,3000);   //����TIM4ͨ��1��ռ�ձ�  3000/7200
	}
	 if(mode==0)
	{
	 GPIO_SetBits(GPIOB, GPIO_Pin_8);	 // �ߵ�ƽ
	 GPIO_ResetBits(GPIOB, GPIO_Pin_7);	 // �͵�ƽ}
		
	 TIM_SetCompare1(TIM4,4000);   //����TIM4ͨ��1��ռ�ձ�  4000/7200
	 }
 
}

