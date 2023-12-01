#include "stm32f10x.h"
#include "delay.h"
#include "gpio.h"
#include "moto.h"
#include "pwm.h"

/***********************************************
公司：轮趣科技（东莞）有限公司
品牌：WHEELTEC
官网：wheeltec.net
淘宝店铺：shop114407458.taobao.com 
速卖通: https://minibalance.aliexpress.com/store/4455017
版本：5.7
修改时间：2021-04-29

 
Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version:5.7
Update：2021-04-29

All rights reserved
***********************************************/



int main(void)
 {	
   SystemInit(); //配置系统时钟为72M   
	 delay_init();    //延时函数初始化
   Gpio_Init();    //初始化gpio口B pin_7/pin_8
   pwm_int();      //初始化pwm输出 72000 000 /7199+1=10000 
	
  while(1)
	{
	 moto(0);                //moto=0时正转
//	moto(1);                //moto=1时反转

	}
 }

