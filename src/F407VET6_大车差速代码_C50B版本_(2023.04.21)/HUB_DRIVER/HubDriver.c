#include "HubDriver.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
float left_rpm, right_rpm;
float Left_encoder_rpm, Right_encoder_rpm;
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ���ˢ���������ʼ��
��ڲ�������
����  ֵ����
**************************************************************************/
void HUB_init(void)
{	
	//delay_xms(1500);
	//hub_485_velocity_init();
  //hub_485_position_init();
	//hub_485_torque_init();
	hub_CAN_velocity_init();
	//hub_CAN_position_init();
	//hub_CAN_torque_init();
	//hub_CAN_Rpm(1, 50);
	hub_CAN_Encoder_init();
	delay_xms(500);
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ�ʹ�ܵ��
��ڲ�������
����  ֵ����
**************************************************************************/
void Enable(void)
{
	unsigned char i = 0;
  u8 enable[8]= {0x01, 0x06, 0x20, 0x0e, 0x00, 0x08, 0xe2, 0x0f};
	
  delay_ms(5);	
	for(i=0; i<8; i++)
	{
		usart3_send(enable[i]);
	}	 	
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ���ˢ���ֹͣ
��ڲ�������
����  ֵ����
**************************************************************************/
void Set_stop(void)
{	
  unsigned char i = 0;
  u8 set_stop[8]= {0x01, 0x06, 0x20, 0x0e, 0x00, 0x07, 0xa2, 0x0b};

	delay_ms(5);
	for(i=0; i<8; i++)
	{
		usart3_send(set_stop[i]);
	}	
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ�CRCУ��
��ڲ�������
����  ֵ����
**************************************************************************/
unsigned short ModbusCRC16_Check(u8 array[], int BitNumber)
{
	unsigned short tmp = 0xffff;
//    unsigned short ret1 = 0;
	int n=0, i=0;

	for(n = 0; n < (BitNumber-2); n++){/*�˴���6 -- ҪУ���λ��Ϊ6��*/
			tmp = array[n] ^ tmp;
			for(i = 0;i < 8;i++){  /*�˴���8 -- ָÿһ��char������8bit��ÿbit��Ҫ����*/
					if(tmp & 0x01){
							tmp = tmp >> 1;
							tmp = tmp ^ 0xa001;
					}   
					else{
							tmp = tmp >> 1;
					}   
			}   
	}   
//    /*CRCУ����ֵ*/
//    printf("%X\n",tmp);
//    /*��CRCУ��ĸߵ�λ�Ի�λ��*/
//    ret1 = tmp >> 8;
//    ret1 = ret1 | (tmp << 8); 
//    printf("ret: %X\n",ret1);
	
	return tmp;
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ�����ٶ�ģʽ��ʼ����30ms
��ڲ�������
����  ֵ����
**************************************************************************/
void hub_485_velocity_init(void)
{
  V_mode_set();
	Left_acc_time(500);
	Right_acc_time(500);
  Left_dec_time(500);
	Right_dec_time(500);
  Enable();
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ�����첽λ��ģʽ��ʼ��
��ڲ�������
����  ֵ����
**************************************************************************/
void hub_485_position_init(void)
{
	Asyn_ctrl_set(Asynchronization); //�����첽
	Relate_position_mode_set(Absolute);
	Left_acc_time(500);
	Right_acc_time(500);
  Left_dec_time(500);
	Right_dec_time(500);
  SetMaxRpm_left_Right(50, 50);
	Enable();
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ����ת��ģʽ��ʼ��
��ڲ�������
����  ֵ����
**************************************************************************/
void hub_485_torque_init(void)
{
  T_mode_set();
  Left_Torque_mAs(500);
  Right_Torque_mAs(500);
	Enable();
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ������ٶ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void V_mode_set(void)
{
	unsigned char i = 0;
  u8 v_mode_set[8]= {0x01, 0x06, 0x20, 0x0d, 0x00, 0x03, 0x53, 0xc8};

	delay_ms(5);
	for(i=0; i<8; i++)
	{
		usart3_send(v_mode_set[i]);
	}	
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ�����ת��ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void T_mode_set(void)
{
	unsigned char i = 0;
  u8 t_mode_set[8]= {0x01, 0x06, 0x20, 0x0d, 0x00, 0x04, 0x12, 0x0a};

	delay_ms(5);
	for(i=0; i<8; i++)
	{
		usart3_send(t_mode_set[i]);
	}	
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ��������/����λ��ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Relate_position_mode_set(int relate)
{
	unsigned char i = 0;
  unsigned short check;
	int ArraySize;
	
  u8 p_relate_mode_set[8]= {0x01, 0x06, 0x20, 0x0d, 0x00, 0x01, 0xd2, 0x09};
  //u8 p_abs_mode_set[8]=    {0x01, 0x06, 0x20, 0x0d, 0x00, 0x02, 0x92, 0x08};
	
	if(relate==1)
		p_relate_mode_set[5]=1;
	else
		p_relate_mode_set[5]=2;
	
	//CRCУ��
	ArraySize=sizeof(p_relate_mode_set);	
	check=ModbusCRC16_Check(p_relate_mode_set, ArraySize);
	p_relate_mode_set[6]=check;
	p_relate_mode_set[7]=check>>8;
	
	//usart1_send(p_relate_mode_set[6]);
	//usart1_send(p_relate_mode_set[7]);
  delay_ms(30);
	for(i=0; i<8; i++)
	{
		usart3_send(p_relate_mode_set[i]);
	} 	
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ���������S�ͼ���ʱ��,Ĭ��500ms
��ڲ�����time������ʱ�䣬��λms
����  ֵ����
**************************************************************************/
void Left_acc_time(int time)
{
	unsigned char i = 0;
	unsigned short check;
	int ArraySize;
	
  u8 left_acc_time[8]= {0x01, 0x06, 0x20, 0x80, 0x01, 0xf4, 0x83, 0xf5};

	if(time>0)
		left_acc_time[4]=time>>8,
	  left_acc_time[5]=time;
	
	//CRCУ��
	ArraySize=sizeof(left_acc_time);	
	check=ModbusCRC16_Check(left_acc_time, ArraySize);
	left_acc_time[6]=check;
	left_acc_time[7]=check>>8;
	
	delay_ms(30);
	for(i=0; i<8; i++)
	{
		usart3_send(left_acc_time[i]);
	}	 	
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ������ҵ��S�ͼ���ʱ��,Ĭ��500ms
��ڲ�����time������ʱ�䣬��λms
����  ֵ����
**************************************************************************/
void Right_acc_time(int time)
{
	unsigned char i = 0;
	unsigned short check;
	int ArraySize;
	
  u8 right_acc_time[8]=      {0x01, 0x06, 0x20, 0x81, 0x01, 0xf4, 0xd2, 0x35};

	if(time>0)
		right_acc_time[4]=time>>8,
	  right_acc_time[5]=time;
	
	//CRCУ��
	ArraySize=sizeof(right_acc_time);	
	check=ModbusCRC16_Check(right_acc_time, ArraySize);
	right_acc_time[6]=check;
	right_acc_time[7]=check>>8;
	
	delay_ms(30);
	for(i=0; i<8; i++)
	{
		usart3_send(right_acc_time[i]);
	}	 	
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ���������S�ͼ���ʱ��,Ĭ��500ms
��ڲ�����time������ʱ�䣬��λms
����  ֵ����
**************************************************************************/
void Left_dec_time(int time)
{
	unsigned char i = 0;
	unsigned short check;
	int ArraySize;
	
  u8 left_dec_time[8]= {0x01, 0x06, 0x20, 0x82, 0x01, 0xf4, 0x22, 0x35};

	if(time>0)
		left_dec_time[4]=time>>8,
	  left_dec_time[5]=time;
	
	//CRCУ��
	ArraySize=sizeof(left_dec_time);	
	check=ModbusCRC16_Check(left_dec_time, ArraySize);
	left_dec_time[6]=check;
	left_dec_time[7]=check>>8;
	
  delay_ms(30);	
	for(i=0; i<8; i++)
	{
		usart3_send(left_dec_time[i]);
	}	 	
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ������ҵ��S�ͼ���ʱ��,Ĭ��500ms
��ڲ�����time������ʱ�䣬��λms
����  ֵ����
**************************************************************************/
void Right_dec_time(int time)
{
	unsigned char i = 0;
	unsigned short check;
	int ArraySize;
	
  u8 right_dec_time[8]= {0x01, 0x06, 0x20, 0x83, 0x01, 0xf4, 0x73, 0xf5};

	if(time>0)
		right_dec_time[4]=time>>8,
	  right_dec_time[5]=time;
	
	//CRCУ��
	ArraySize=sizeof(right_dec_time);	
	check=ModbusCRC16_Check(right_dec_time, ArraySize);
	right_dec_time[6]=check;
	right_dec_time[7]=check>>8;
	
  delay_ms(30);	
	for(i=0; i<8; i++)
	{
		usart3_send(right_dec_time[i]);
	}	 	
}

/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ�����λ��ģʽ�첽/ͬ������
��ڲ�������
����  ֵ����
**************************************************************************/
void Asyn_ctrl_set(int asyn)
{
	unsigned char i = 0;
	unsigned short check;
	int ArraySize;

  u8 asyn_ctrl_set[8]= {0x01, 0x06, 0x20, 0x0f, 0x00, 0x00, 0xb2, 0x09};
  //u8 syn_ctrl_set[8]=  {0x01, 0x06, 0x20, 0x0f, 0x00, 0x01, 0x73, 0xc9};
	
	if(asyn==1)
		asyn_ctrl_set[5]=0;
	else
		asyn_ctrl_set[5]=1;
	
	//CRCУ��
	ArraySize=sizeof(asyn_ctrl_set);	
	check=ModbusCRC16_Check(asyn_ctrl_set, ArraySize);
	asyn_ctrl_set[6]=check;
	asyn_ctrl_set[7]=check>>8;
	
  delay_ms(30);
	for(i=0; i<8; i++)
	{
		usart3_send(asyn_ctrl_set[i]);
	} 	
}

/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ��������ҵ�����ת�٣�Ĭ��50rpm
��ڲ�����time������ʱ�䣬��λms
����  ֵ����
**************************************************************************/
void SetMaxRpm_left_Right(int Left, int Right)
{
	unsigned char i = 0;
	unsigned short check;
	int ArraySize;
	
  u8 set_left_max[8]=  {0x01, 0x06, 0x20, 0x8e, 0x00, 0x32, 0x63, 0xf4};
  u8 set_right_max[8]= {0x01, 0x06, 0x20, 0x8f, 0x00, 0x32, 0x32, 0x34};

	if(Left>0)
	  set_left_max [4]=Left>>8,
	  set_left_max [5]=Left;
	if(Right>0)
	  set_right_max[4]=Right>>8,
	  set_right_max[5]=Right;
	
	//CRCУ��
	ArraySize=sizeof(set_left_max);	
	check=ModbusCRC16_Check(set_left_max, ArraySize);
	set_left_max[6]=check;
	set_left_max[7]=check>>8;
	ArraySize=sizeof(set_right_max);	
	check=ModbusCRC16_Check(set_right_max, ArraySize);
	set_right_max[6]=check;
	set_right_max[7]=check>>8;
	
	delay_ms(30);
	for(i=0; i<8; i++)
	{
		usart3_send(set_left_max[i]);
	}	 	
	delay_ms(30);
	for(i=0; i<8; i++)
	{
		usart3_send(set_right_max[i]);
	}	 
}

/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ���������ת��б��,Ĭ��500mA/s
��ڲ�����mAs��ת��б�ʣ���λmA/s
����  ֵ����
**************************************************************************/
void Left_Torque_mAs(int mAs)
{
	unsigned char i = 0;
	unsigned short check;
	int ArraySize;
	
  u8 left_torque[8]= {0x01, 0x06, 0x20, 0x86, 0x01, 0xf4, 0x63, 0xf4};

	if(mAs>0)
		left_torque[4]=mAs>>8,
	  left_torque[5]=mAs;
	
		//CRCУ��
	ArraySize=sizeof(left_torque);	
	check=ModbusCRC16_Check(left_torque, ArraySize);
	left_torque[6]=check;
	left_torque[7]=check>>8;
	
  delay_ms(30);	
	for(i=0; i<8; i++)
	{
		usart3_send(left_torque[i]);
	}	 	
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ������ҵ��ת��б��,Ĭ��500mA/s
��ڲ�����mAs��ת��б�ʣ���λmA/s
����  ֵ����
**************************************************************************/
void Right_Torque_mAs(int mAs)
{
	unsigned char i = 0;
	unsigned short check;
	int ArraySize;
	
  u8 right_torque[8]= {0x01, 0x06, 0x20, 0x86, 0x01, 0xf4, 0x63, 0xf4};

	if(mAs>0)
		right_torque[4]=mAs>>8,
	  right_torque[5]=mAs;
	
	//CRCУ��
	ArraySize=sizeof(right_torque);	
	check=ModbusCRC16_Check(right_torque, ArraySize);
	right_torque[6]=check;
	right_torque[7]=check>>8;
	
  delay_ms(30);	
	for(i=0; i<8; i++)
	{
		usart3_send(right_torque[i]);
	}	 	
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ���������ٶ�
��ڲ�������20480
����  ֵ����
**************************************************************************/
void hub_485_Rpm(int R_L, int rpm)
{
	unsigned char i = 0;
	unsigned short check;
	int ArraySize;
	
  u8 set_rpm[8]= {0x01, 0x06, 0x20, 0x88, 0x00, 0x00, 0x00, 0x00};
	
	delay_ms(30);
	
	//�������������ҵ��
	if     (R_L==0) set_rpm[3]=0x88; //����
	else if(R_L==1) set_rpm[3]=0x89; //�ҵ��
	
	//����ת���뷽��
	if(rpm>=0)
	{
		set_rpm[4]=rpm>>8;
		set_rpm[5]=rpm;
	}
	else
	{
		set_rpm[4]=(0xffff+rpm+1)>>8;
		set_rpm[5]=(0xffff+rpm+1);
	}
	
	//CRCУ��
	ArraySize=sizeof(set_rpm);	
	check=ModbusCRC16_Check(set_rpm, ArraySize);
	set_rpm[6]=check;
	set_rpm[7]=check>>8;

	//485��������
	for(i=0; i<8; i++)
	{
		usart3_send(set_rpm[i]);
	}
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ��������ͬ���ٶ�
��ڲ�������20480
����  ֵ����
**************************************************************************/
void hub_485_Syn_Rpm(int rpm)
{
	unsigned char i = 0;
	unsigned short check;
	int ArraySize;
	
  u8 set_rpm[13]= {0x01, 0x10, 0x20, 0x88, 0x00, 0x02, 0x04, 0x00, 0x64, 0x00, 0x64, 0x23, 0x9c};
	
	delay_ms(30);
	
	
	//����ת���뷽��
	if(rpm>=0)
	{
		set_rpm[7] =rpm>>8;
		set_rpm[8] =rpm;
		set_rpm[9] =rpm>>8;
		set_rpm[10]=rpm;
	}
	else
	{
		set_rpm[7] =(0xffff+rpm+1)>>8;
		set_rpm[8] =(0xffff+rpm+1);
		set_rpm[9] =(0xffff+rpm+1)>>8;
		set_rpm[10]=(0xffff+rpm+1);
	}
	
	//CRCУ��
	ArraySize=sizeof(set_rpm);	
	check=ModbusCRC16_Check(set_rpm, ArraySize);
	set_rpm[11]=check;
	set_rpm[12]=check>>8;

	//485��������
	for(i=0; i<13; i++)
	{
		usart3_send(set_rpm[i]);
	}
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ��������λ��
��ڲ�������
����  ֵ����
**************************************************************************/
void hub_485_Position(int R_L, long int position)
{
	unsigned char i = 0;
	unsigned short check;
	int ArraySize;
	u8 set_position[13]=   {0x01, 0x10, 0x20, 0x8c, 0x00, 0x02, 0x04, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00};
  u8 set_left_start[8]=  {0x01, 0x06, 0x20, 0x0e, 0x00, 0x11, 0x23, 0xc5};
  u8 set_right_start[8]= {0x01, 0x06, 0x20, 0x0e, 0x00, 0x12, 0x63, 0xc4};

	delay_ms(30);
	
	//�������������ҵ��
	if     (R_L==0) set_position[3]=0x8a; //����
	else if(R_L==1) set_position[3]=0x8c; //�ҵ��
	
	if(position>=0)
	{
		set_position[7] = position>>24;
		set_position[8] = position>>16;
		set_position[9] = position>>8;
		set_position[10]= position;
	}
	else
	{
		set_position[7] =(0xffffffff+position+1)>>24;
		set_position[8] =(0xffffffff+position+1)>>16;
		set_position[9] =(0xffffffff+position+1)>>8;
		set_position[10]=(0xffffffff+position+1);
	}
	
  //CRCУ��
	ArraySize=sizeof(set_position);	
	check=ModbusCRC16_Check(set_position, ArraySize);
	set_position[11]=check;
	set_position[12]=check>>8;
	
	//����λ��
	for(i=0; i<13; i++)
	{
		usart3_send(set_position[i]);
	}	
	delay_ms(5);
	//��������/�ҵ��
	if(R_L==0)	
	{
		for(i=0; i<8; i++)
		{
			usart3_send(set_left_start[i]);
		}	
	}
	else
	{
		for(i=0; i<8; i++)
		{
			usart3_send(set_right_start[i]);
		}	
	}
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ��������ͬ��λ��
��ڲ�������
����  ֵ����
**************************************************************************/
void hub_485_Syn_Position(long int position)
{
	unsigned char i = 0;
	unsigned short check;
	int ArraySize;
	u8 set_position[17]= {0x01, 0x10, 0x20, 0x8a, 0x00, 0x04, 0x08, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x50, 0x00, 0xe3, 0x2c};
  u8 start       [8] = {0x01, 0x06, 0x20, 0x0e, 0x00, 0x10, 0xe2, 0x05};

	delay_ms(30);
	
	if(position>=0)
	{
		set_position[7]  = position>>24;
		set_position[8]  = position>>16;
		set_position[9]  = position>>8;
		set_position[10] = position;
		set_position[11] = position>>24;
		set_position[12] = position>>16;
		set_position[13] = position>>8;
		set_position[14] = position;
	}
	else
	{
		set_position[7]  = (0xffffffff+position+1)>>24;
		set_position[8]  = (0xffffffff+position+1)>>16;
		set_position[9]  = (0xffffffff+position+1)>>8;
		set_position[10] = (0xffffffff+position+1);
		set_position[11] = (0xffffffff+position+1)>>24;
		set_position[12] = (0xffffffff+position+1)>>16;
		set_position[13] = (0xffffffff+position+1)>>8;
		set_position[14] = (0xffffffff+position+1);
	}
	
  //CRCУ��
	ArraySize=sizeof(set_position);	
	check=ModbusCRC16_Check(set_position, ArraySize);
	set_position[15]=check;
	set_position[16]=check>>8;
	
	//����λ��
	for(i=0; i<17; i++)
	{
		usart3_send(set_position[i]);
	}	
	delay_ms(30);

	for(i=0; i<8; i++)
	{
		usart3_send(start[i]);
	}	
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ��������ת��
��ڲ�������2000ma
����  ֵ����
**************************************************************************/
void hub_485_Torque(int R_L, int torque)
{
	unsigned char i = 0;
	unsigned short check;
	int ArraySize;
	
  u8 set_torque[8]= {0x01, 0x06, 0x20, 0x90, 0x00, 0x00, 0x00, 0x00};
	
	delay_ms(30);
	
	//�������������ҵ��
	if     (R_L==0) set_torque[3]=0x90; //����
	else if(R_L==1) set_torque[3]=0x91; //�ҵ��
	
	//����ת���뷽��
	if(torque>=0)
	{
		set_torque[4]=torque>>8;
		set_torque[5]=torque;
	}
	else
	{
		set_torque[4]=(0xffff+torque+1)>>8;
		set_torque[5]=(0xffff+torque+1);
	}
	
	//CRCУ��
	ArraySize=sizeof(set_torque);	
	check=ModbusCRC16_Check(set_torque, ArraySize);
	set_torque[6]=check;
	set_torque[7]=check>>8;

	//485��������
	for(i=0; i<8; i++)
	{
		usart3_send(set_torque[i]);
	}
}
/**************************************************************************
Function: 
Input   : none
Output  : none
�������ܣ��������ͬ��ת��
��ڲ�������2000ma
����  ֵ����
**************************************************************************/
void hub_485_Syn_Torque(int torque)
{
	unsigned char i = 0;
	unsigned short check;
	int ArraySize;
	
  u8 set_torque[13]= {0x01, 0x10, 0x20, 0x90, 0x00, 0x02, 0x04, 0x07, 0xd0, 0x07, 0xd0, 0x60, 0x23};
	
	delay_ms(30);
	
	//����ת���뷽��
	if(torque>=0)
	{
		set_torque[7] =torque>>8;
		set_torque[8] =torque;
		set_torque[9] =torque>>8;
		set_torque[10]=torque;
	}
	else
	{
		set_torque[7] =(0xffff+torque+1)>>8;
		set_torque[8] =(0xffff+torque+1);
		set_torque[9] =(0xffff+torque+1)>>8;
		set_torque[10]=(0xffff+torque+1);
	}
	
	//CRCУ��
	ArraySize=sizeof(set_torque);	
	check=ModbusCRC16_Check(set_torque, ArraySize);
	set_torque[11]=check;
	set_torque[12]=check>>8;

	//485��������
	for(i=0; i<13; i++)
	{
		usart3_send(set_torque[i]);
	}
}







