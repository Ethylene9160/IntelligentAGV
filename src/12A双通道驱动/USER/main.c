#include "stm32f10x.h"
#include "delay.h"
#include "gpio.h"
#include "moto.h"
#include "pwm.h"

/***********************************************
��˾����Ȥ�Ƽ�����ݸ�����޹�˾
Ʒ�ƣ�WHEELTEC
������wheeltec.net
�Ա����̣�shop114407458.taobao.com 
����ͨ: https://minibalance.aliexpress.com/store/4455017
�汾��5.7
�޸�ʱ�䣺2021-04-29

 
Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version:5.7
Update��2021-04-29

All rights reserved
***********************************************/



int main(void)
 {	
   SystemInit(); //����ϵͳʱ��Ϊ72M   
	 delay_init();    //��ʱ������ʼ��
   Gpio_Init();    //��ʼ��gpio��B pin_7/pin_8
   pwm_int();      //��ʼ��pwm��� 72000 000 /7199+1=10000 
	
  while(1)
	{
	 moto(0);                //moto=0ʱ��ת
//	moto(1);                //moto=1ʱ��ת

	}
 }

