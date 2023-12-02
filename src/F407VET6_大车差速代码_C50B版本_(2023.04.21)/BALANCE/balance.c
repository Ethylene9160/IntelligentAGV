#include "balance.h"

//Whether the robot model is incorrectly marked
//�������ͺ��Ƿ�����־λ
int robot_mode_check_flag=0;

//The current position of the slide represents the current rotation Angle of the front wheel, 
//which is specially designed for Ackerman car
//���쵱ǰλ�ã�����ǰ�ֵ�ǰת�ǽǶȣ����䰢����С��ר��
int SLIDE_POSITION=0;

//The target position of the slide track, representing the front wheel target rotation Angle, 
//is specially equipped with Ackerman car
//����Ŀ��λ�ã�����ǰ��Ŀ��ת�ǽǶȣ����䰢����С��ר��
float Slide_target=0;

//Front wheel Angle zero
//ǰ��ת�����
int SERVO_BIAS;

//Maximum and minimum PWM values of steering gear
//���PWMֵ���ֵ����Сֵ
int Servo_max=2000, Servo_min=900;

//Front wheel Angle steering speed
//ǰ��ת��ת���ٶ�
float Angle_Smoother_Rate=20;

//Forward and backward velocity acceleration
//ǰ�������ٶȼ��ٶ�
float Velocity_Smoother_Rate=0.02;

//Time variable //��ʱ����
int Time_count=0;

u8 command_lost_count=0;//���ڡ�CAN�������ʧʱ���������ʧһ���ֹͣ����

/**************************************************************************
Function: The inverse kinematics solution is used to calculate the target speed of each wheel according to the target speed of three axes
Input   : X and Y, Z axis direction of the target movement speed
Output  : none
�������ܣ��˶�ѧ��⣬��������Ŀ���ٶȼ��������Ŀ��ת��
��ڲ�����X��Y��Z�᷽���Ŀ���˶��ٶ�
����  ֵ����
**************************************************************************/
void Drive_Motor(float Vx,float Vz)
{   
		//Wheel target speed limit //����Ŀ���ٶ��޷�
	  float amplitude=3.5;
	 
	  #if Akm_Car
	
		if(Car_Mode==0||Car_Mode==1||Car_Mode==7||Car_Mode==8) //SENIOR_AKM - ���䰢����   ����ר��Car_Mode==8
		{
			//Ackerman car specific related variables //������С��ר����ر���
			float R, Ratio=636.56, AngleR, Angle_Servo;
			
			// For Ackerman small car, Vz represents the front wheel steering Angle
			//���ڰ�����С��Vz������ǰ��ת��Ƕ�
			AngleR=Vz;
			R=Axle_spacing/tan(AngleR)-0.5f*Wheel_spacing;
			
			// Front wheel steering Angle limit (front wheel steering Angle controlled by steering engine), unit: rad
			//ǰ��ת��Ƕ��޷�(�������ǰ��ת��Ƕ�)����λ��rad
			AngleR=target_limit_float(AngleR,-0.50f,0.34f);
			
			//Inverse kinematics //�˶�ѧ���
			if(AngleR!=0)
			{
				MOTOR_A.Target = Vx*(R-0.5f*Wheel_spacing)/R;
				MOTOR_B.Target = Vx*(R+0.5f*Wheel_spacing)/R;
			}
			else 
			{
				MOTOR_A.Target = Vx;
				MOTOR_B.Target = Vx;
			}
			// The PWM value of the servo controls the steering Angle of the front wheel
			//���PWMֵ���������ǰ��ת��Ƕ�
			Angle_Servo    =  -0.2137f*pow(AngleR, 2) + 1.439f*AngleR + 0.009599f;
			Servo=SERVO_INIT + SERVO_BIAS + Angle_Servo*Ratio;
			
			//Wheel (motor) target speed limit //����(���)Ŀ���ٶ��޷�
			MOTOR_A.Target=target_limit_float(MOTOR_A.Target, -amplitude,amplitude); 
			MOTOR_B.Target=target_limit_float(MOTOR_B.Target, -amplitude,amplitude); 
			Servo=target_limit_int(Servo, Servo_min, Servo_max);	//Servo PWM value limit //���PWMֵ�޷�
		}
	  
		else if(Car_Mode==2||Car_Mode==3) //TOP_AKM_BS - ���䰢����С����ʽ����
		{
			//Ackerman car specific related variables //������С��ר����ر���
			int TargetServo;	
			float R, AngleR;
			
			// For Ackerman small car, Vz represents the front wheel steering Angle
			//���ڰ�����С��Vz������ǰ��ת��Ƕ�
			AngleR=Vz;
			R=Axle_spacing/tan(AngleR)-0.5f*Wheel_spacing;
			// Front wheel steering Angle limit (front wheel steering Angle controlled by steering engine), unit: rad
			//ǰ��ת��Ƕ��޷�(�������ǰ��ת��Ƕ�)����λ��rad
			AngleR=target_limit_float(AngleR,-0.47f,0.34f);
			
			//The trolley accelerates larger, adding smoothing processing
			//��С�����ٶȽϴ����ƽ������
			if(APP_ON_Flag==1||PS2_ON_Flag==1||Remote_ON_Flag==1||CAN_ON_Flag==1||Usart_ON_Flag==1)
			{
				//Smoothing the input speed //�������ٶȽ���ƽ������
			  Smooth_control(Vx, 0.04); 
        //Get the smoothed data //��ȡƽ������������					
	      Vx=smooth_control.VX;   
			}
			
      //Inverse kinematics //�˶�ѧ���
			if(AngleR!=0)
			{
				MOTOR_A.Target = Vx*(R-0.5f*Wheel_spacing)/R;
				MOTOR_B.Target = Vx*(R+0.5f*Wheel_spacing)/R;		
			}
			else 
			{
				MOTOR_A.Target = Vx;
				MOTOR_B.Target = Vx;
			}
			Slide_target= -2450*pow(AngleR, 3) + 1290*pow(AngleR, 2) + 3242*AngleR - 228.6f;
			Slide_target= Slide_target + SERVO_BIAS;		
		  
			//Closed-loop feedback control of the front wheel Angle
			//�ջ���������ǰ��ת��
			TargetServo=Incremental_SERVO(SLIDE_POSITION, Slide_target);
			Servo=Smooth_steering(Servo, TargetServo, Angle_Smoother_Rate);
			
			//Wheel (motor) target speed limit //����(���)Ŀ���ٶ��޷�
			MOTOR_A.Target=target_limit_float(MOTOR_A.Target, -amplitude,amplitude);
			MOTOR_B.Target=target_limit_float(MOTOR_B.Target, -amplitude,amplitude);
			//Servo PWM value limit //���PWMֵ�޷�
			Servo=target_limit_int(Servo, Servo_min, Servo_max);
		}
		
		else if(Car_Mode==4||Car_Mode==5) //TOP_AKM_DL - ���䰢����С����������
		{
			//Ackerman car specific related variables //������С��ר����ر���
			int TargetServo;	
			float R, AngleR;
			
			// For Ackerman small car, Vz represents the front wheel steering Angle
			//���ڰ�����С��Vz������ǰ��ת��Ƕ�
			AngleR=Vz;
			R=Axle_spacing/tan(AngleR)-0.5f*Wheel_spacing;
			// Front wheel steering Angle limit (front wheel steering Angle controlled by steering engine), unit: rad
			//ǰ��ת��Ƕ��޷�(�������ǰ��ת��Ƕ�)����λ��rad
			AngleR=target_limit_float(AngleR, -0.48f, 0.32f);
					
			//The trolley accelerates larger, adding smoothing processing
			//��С�����ٶȽϴ����ƽ������
			if((APP_ON_Flag==1||PS2_ON_Flag==1||Remote_ON_Flag==1||CAN_ON_Flag==1||Usart_ON_Flag==1)&&(Car_Mode==5))
			{
			 Smooth_control(Vx, Velocity_Smoother_Rate); //�������ٶȽ���ƽ������	    
	     Vx=smooth_control.VX;   //��ȡƽ������������
			}			
			
			//Inverse kinematics //�˶�ѧ���
			if(AngleR!=0)
			{
				MOTOR_A.Target = Vx*(R-0.5f*Wheel_spacing)/R;
				MOTOR_B.Target = Vx*(R+0.5f*Wheel_spacing)/R;
			}
			else
			{
				MOTOR_A.Target = Vx;
				MOTOR_B.Target = Vx;
			}
					
      Slide_target= 4515*pow(AngleR, 3) - 174.2f*pow(AngleR, 2) - 4323*AngleR + 42.01f;
			Slide_target= Slide_target + SERVO_BIAS;		
			
			//Closed-loop feedback control of the front wheel Angle
			//�ջ���������ǰ��ת��
			TargetServo=Incremental_SERVO(-SLIDE_POSITION, Slide_target);
			Servo=TargetServo;
			//Servo=Smooth_steering(Servo, TargetServo, Angle_Smoother_Rate);
		
			//Wheel (motor) target speed limit //����(���)Ŀ���ٶ��޷�
			MOTOR_A.Target=target_limit_float(MOTOR_A.Target, -amplitude,amplitude);
			MOTOR_B.Target=target_limit_float(MOTOR_B.Target, -amplitude,amplitude);
			//Servo PWM value limit //���PWMֵ�޷�
			Servo=target_limit_int(Servo, Servo_min, Servo_max);		
		}
		
		#elif Diff_Car
		
		//Inverse kinematics //�˶�ѧ���
		MOTOR_A.Target  = Vx - Vz * Wheel_spacing / 2.0f; 
		MOTOR_B.Target =  Vx + Vz * Wheel_spacing / 2.0f; 
		
		//Wheel (motor) target speed limit //����(���)Ŀ���ٶ��޷�
		MOTOR_A.Target=target_limit_float( MOTOR_A.Target,-amplitude,amplitude);
	  MOTOR_B.Target=target_limit_float( MOTOR_B.Target,-amplitude,amplitude);
		
		#endif
}
/**************************************************************************
Function: FreerTOS task, core motion control task
Input   : none
Output  : none
�������ܣ�FreeRTOS���񣬺����˶���������
��ڲ�������
����  ֵ����
**************************************************************************/
void Balance_task(void *pvParameters)
{ 
	  u32 lastWakeTime = getSysTickCnt();
    while(1)
    {	
			// This task runs at a frequency of 100Hz (10ms control once)
			//��������100Hz��Ƶ�����У�10ms����һ�Σ�
			vTaskDelayUntil(&lastWakeTime, F2T(RATE_100_HZ));
			
			//Time count is no longer needed after 30 seconds
			//ʱ�������30�������Ҫ
			if(Time_count<3000)Time_count++;
			//and convert to transposition international units
			//��ȡ���������ݣ�������ʵʱ�ٶȣ���ת��λ���ʵ�λ
			Get_Velocity_Form_Encoder();
			
			#if Akm_Car
			//Gets the position of the slide, representing the front wheel rotation Angle
			//��ȡ����λ��,����ǰ��ת�ǽǶ�
			SLIDE_POSITION=Get_Adc(WEITIAO)-2048;
			SLIDE_POSITION=Mean_Filter(SLIDE_POSITION);
			#endif
			
			//Do not enter the control before the end of self-check to prevent the PID control from starting integration
			//�Լ����ǰ��������ƣ���ֹPID���ƿ�ʼ����
			if(Time_count>CONTROL_DELAY+150) 
			{
//				command_lost_count++;//���ڡ�CAN�������ʧʱ���������ʧ1���ֹͣ����
//				if(command_lost_count>RATE_100_HZ&&APP_ON_Flag==0&&Remote_ON_Flag==0&&PS2_ON_Flag==0)
//					Move_X=0,Move_Y=0,Move_Z=0;
				
				if      (APP_ON_Flag)      Get_RC();         //Handle the APP remote commands //����APPң������
				else if (Remote_ON_Flag)   Remote_Control(); //Handle model aircraft remote commands //����ģң������
				else if (PS2_ON_Flag)      PS2_control();    //Handle PS2 controller commands //����PS2�ֱ���������
				
				//CAN, Usart 1, Usart 3 control can directly get the 2 axis target speed, 
				//without additional processing
				//CAN������1������3(ROS)����ֱ�ӵõ�2��Ŀ���ٶȣ�������⴦��
				else                      Drive_Motor(Move_X,Move_Z);  //CAN������1������3(ROS)����
			}

				//�������ǳ�ʼ����ɺ�,���������ͺ��Ƿ�ѡ�����
				//When the gyroscope is initialized, check whether the robot model is selected incorrectly
				if(CONTROL_DELAY<Time_count && Time_count<CONTROL_DELAY+100) //Advance 1 seconds to test //ǰ��1����в���
				{
					Drive_Motor(0.02, 0); 
					robot_mode_check(); //Detection function //��⺯��
				}
				else if(CONTROL_DELAY+100<Time_count && Time_count<CONTROL_DELAY+150) Drive_Motor(0, 0); //The stop forward control is completed //������ֹͣǰ������
				
				//If there is no abnormity in the battery voltage, and the enable switch is in the ON position,
				//and the software failure flag is 0, or the model detection marker is 0
				//�����ص�ѹ�������쳣������ʹ�ܿ�����ON��λ���������ʧ�ܱ�־λΪ0�������ͺż���־λΪ0
				if(Turn_Off(Voltage)==0&&robot_mode_check_flag==0)
				{ 		
					 //Speed closed-loop control to calculate the PWM value of each motor, 
					 //PWM represents the actual wheel speed					 
					 //�ٶȱջ����Ƽ�������PWMֵ��PWM������ʵ��ת��					 
					 MOTOR_A.Motor_Pwm=Incremental_PI_A(MOTOR_A.Encoder, MOTOR_A.Target);
					 MOTOR_B.Motor_Pwm=Incremental_PI_B(MOTOR_B.Encoder, MOTOR_B.Target);
					 
					 #if Akm_Car
					 //Set different PWM control polarity according to different car models
					 //���ݲ�ͬС���ͺ����ò�ͬ��PWM���Ƽ���
					 if      (Car_Mode==0)	Set_Pwm( MOTOR_A.Motor_Pwm, MOTOR_B.Motor_Pwm,Servo); //MD36
					 else if (Car_Mode==1)	Set_Pwm( MOTOR_A.Motor_Pwm, MOTOR_B.Motor_Pwm,Servo); //MD36
					 else if (Car_Mode==2)	Set_Pwm(-MOTOR_A.Motor_Pwm,-MOTOR_B.Motor_Pwm,Servo); //MD60
					 else if (Car_Mode==3)	Set_Pwm(-MOTOR_A.Motor_Pwm,-MOTOR_B.Motor_Pwm,Servo); //MD60
					 else if (Car_Mode==4)	Set_Pwm(-MOTOR_A.Motor_Pwm,-MOTOR_B.Motor_Pwm,Servo); //MD60
					 else if (Car_Mode==5)	Set_Pwm(-MOTOR_A.Motor_Pwm,-MOTOR_B.Motor_Pwm,Servo); //MD60
					 else if (Car_Mode==6)	Set_Pwm( MOTOR_A.Motor_Pwm, MOTOR_B.Motor_Pwm,Servo); //MD36
					 	else if (Car_Mode==7)	Set_Pwm(-MOTOR_A.Motor_Pwm,-MOTOR_B.Motor_Pwm,Servo); //����ר��
						else if (Car_Mode==8)	Set_Pwm(-MOTOR_A.Motor_Pwm,-MOTOR_B.Motor_Pwm,Servo); //����ר��
					 #elif Diff_Car
					 //Set different PWM control polarity according to different car models
					 //���ݲ�ͬС���ͺ����ò�ͬ��PWM���Ƽ���
								if (Car_Mode==0)	Set_Pwm( MOTOR_A.Motor_Pwm, MOTOR_B.Motor_Pwm, 0); //MD36
					 else if (Car_Mode==1)	Set_Pwm( MOTOR_A.Motor_Pwm, MOTOR_B.Motor_Pwm, 0); //MD36
					 else if (Car_Mode==2)	Set_Pwm(-MOTOR_A.Motor_Pwm,-MOTOR_B.Motor_Pwm, 0); //MD60
					 else if (Car_Mode==3)	Set_Pwm(-MOTOR_A.Motor_Pwm,-MOTOR_B.Motor_Pwm, 0); //MD60
					 else if (Car_Mode==4)	Set_Pwm(-MOTOR_A.Motor_Pwm,-MOTOR_B.Motor_Pwm, 0); //MD60
					 else if (Car_Mode==5)	Set_Pwm(-MOTOR_A.Motor_Pwm,-MOTOR_B.Motor_Pwm, 0); //MD60
					 
					 #endif
					
				}
				//If Turn_Off(Voltage) returns to 1, or the model detection marker is 1, the car is not allowed to move, and the PWM value is set to 0
				//���Turn_Off(Voltage)����ֵΪ1�������ͺż���־λΪ1�����������С�������˶���PWMֵ����Ϊ0
				else	Set_Pwm(0,0,0);

				
			      //Click the user button to update the gyroscope zero
			//�����û������������������
				Key();
    }	
}
/**************************************************************************
Function: Assign a value to the PWM register to control wheel speed and direction
Input   : PWM
Output  : none
�������ܣ���ֵ��PWM�Ĵ��������Ƴ���ת���뷽��
��ڲ�����PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motor_a,int motor_b,int servo)
{  
	  //Forward and reverse control of motor
	  //�������ת����
		if(motor_a<0)			AIN2=0,		AIN1=1;   
		else 	            AIN2=1,		AIN1=0;
    //Motor speed control 
	  //���ת�ٿ���	
	 TIM_SetCompare4(TIM8,myabs(motor_a));
	
		if(motor_b>0)			BIN2=0,		BIN1=1;   
		else 	            BIN2=1,			BIN1=0;
	  //Motor speed control 
	  //���ת�ٿ���
		TIM_SetCompare3(TIM8,myabs(motor_b));
		Servo_PWM =servo; 
}

/**************************************************************************
Function: Limit PWM value
Input   : Value
Output  : none
�������ܣ�����PWMֵ 
��ڲ�������ֵ
����  ֵ����
**************************************************************************/
void Limit_Pwm(int amplitude)
{	
	    MOTOR_A.Motor_Pwm=target_limit_float(MOTOR_A.Motor_Pwm,-amplitude,amplitude);
	    MOTOR_B.Motor_Pwm=target_limit_float(MOTOR_B.Motor_Pwm,-amplitude,amplitude);
		  MOTOR_C.Motor_Pwm=target_limit_float(MOTOR_C.Motor_Pwm,-amplitude,amplitude);
	    MOTOR_D.Motor_Pwm=target_limit_float(MOTOR_D.Motor_Pwm,-amplitude,amplitude);
}	    
/**************************************************************************
Function: Limiting function
Input   : Value
Output  : none
�������ܣ��޷�����
��ڲ�������ֵ
����  ֵ����
**************************************************************************/
float target_limit_float(float insert,float low,float high)
{
    if (insert < low)
        return low;
    else if (insert > high)
        return high;
    else
        return insert;	
}
int target_limit_int(int insert,int low,int high)
{
    if (insert < low)
        return low;
    else if (insert > high)
        return high;
    else
        return insert;	
}
/**************************************************************************
Function: Check the battery voltage, enable switch status, software failure flag status
Input   : Voltage
Output  : Whether control is allowed, 1: not allowed, 0 allowed
�������ܣ�����ص�ѹ��ʹ�ܿ���״̬�����ʧ�ܱ�־λ״̬
��ڲ�������ѹ
����  ֵ���Ƿ�������ƣ�1��������0����
**************************************************************************/
u8 Turn_Off( int voltage)
{
		u8 temp;
	  //static int stop_count, enable_count;
		if(voltage<20||EN==0||Flag_Stop==1)
		{	                                                
			temp=1;      
			PWMA=0;
			PWMB=0;
			PWMC=0;
			PWMD=0;
			AIN1=0;AIN2=0;
			BIN1=0;BIN2=0;
			CIN1=0;CIN2=0;
			DIN1=0;DIN2=0;					
		}
		else
			temp=0;
		return temp;			
}
/**************************************************************************
Function: Calculate absolute value
Input   : long int
Output  : unsigned int
�������ܣ������ֵ
��ڲ�����long int
����  ֵ��unsigned int
**************************************************************************/
u32 myabs(long int a)
{ 		   
	  u32 temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
Function: Floating-point data calculates the absolute value
Input   : float
Output  : The absolute value of the input number
�������ܣ����������ݼ������ֵ
��ڲ�����������
����  ֵ���������ľ���ֵ
**************************************************************************/
float float_abs(float insert)
{
	if(insert>=0) return insert;
	else return -insert;
}
/**************************************************************************
Function: Incremental PI controller
Input   : Encoder measured value (actual speed), target speed
Output  : Motor PWM
According to the incremental discrete PID formula
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k) represents the current deviation
e(k-1) is the last deviation and so on
PWM stands for incremental output
In our speed control closed loop system, only PI control is used
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)

�������ܣ�����ʽPI������
��ڲ���������������ֵ(ʵ���ٶ�)��Ŀ���ٶ�
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_A (float Encoder,float Target)
{
	 static float Bias,Pwm,Last_bias;
	 Bias=Target-Encoder; //Calculate the deviation //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias; 
	 if(Pwm>16800)Pwm=16800;
	 if(Pwm<-16800)Pwm=-16800;
	 Last_bias=Bias; //Save the last deviation //������һ��ƫ�� 
	 return Pwm; 
}
int Incremental_PI_B (float Encoder,float Target)
{
	 static float Bias,Pwm,Last_bias;
	 Bias=Target-Encoder; //Calculate the deviation //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias; 
	 if(Pwm>16800)Pwm=16800;
	 if(Pwm<-16800)Pwm=-16800;
	 Last_bias=Bias; //Save the last deviation //������һ��ƫ�� 
	 return Pwm; 
}
int Incremental_SERVO (float SlidePosition,float SlideTarget)
{
	 static float Bias,Pwm=1700,Last_bias;
	 Bias=SlideTarget-SlidePosition; //Calculate the deviation //����ƫ��
	 if(Car_Mode==2||Car_Mode==3)
	 {
		 //The mechanical structure determines that the right turn stroke is longer, so the steering speed should be accelerated
		 //��е�ṹ������ת�г̱Ƚϳ�����ʱ�ӿ�ת���ٶ�
	   if(SlideTarget<0||SlidePosition<-200) Pwm+=0.004*1.6*(Bias-Last_bias)+0.009*1.6*Bias; 
	   else Pwm+=0.004f*(Bias-Last_bias)+0.009f*Bias;   
	 }
	 if(Car_Mode==4||Car_Mode==5)
	 {
		 //The mechanical structure determines that the right turn stroke is longer, so the steering speed should be accelerated
		 //��е�ṹ������ת�г̱Ƚϳ�����ʱ�ӿ�ת���ٶ�
	   if(SlideTarget>0||SlidePosition<200) Pwm+=0.004*1.6*(Bias-Last_bias)+0.009*1.6*Bias;
	   else Pwm+=0.004f*(Bias-Last_bias)+0.009f*Bias;   
	 }
   if(Pwm>Servo_max)Pwm=Servo_max;
	 if(Pwm<Servo_min)Pwm=Servo_min;
	 Last_bias=Bias; //Save the last deviation //������һ��ƫ�� 
	 return Pwm;
}

/**************************************************************************
Function: Processes the command sent by APP through usart 2
Input   : none
Output  : none
�������ܣ���APPͨ������2���͹�����������д���
��ڲ�������
����  ֵ����
**************************************************************************/
void Get_RC(void)
{
	 switch(Flag_Direction) //Handle direction control commands //�������������
   { 
			case 1:      Move_X=+RC_Velocity;  	 Move_Z=0;         break;
		  case 2:      Move_X=+RC_Velocity;  	 Move_Z=-PI/4;   	 break;
			case 3:      Move_X=0;      				 Move_Z=-PI/4;   	 break;
		 
			#if Akm_Car                          //AKM car Z stands for front wheel steering Angle //Akm��Z����ǰ��ת���
			case 4:      Move_X=-RC_Velocity;  	 Move_Z=-PI/4;     break;
			#elif Diff_Car                       //Differential car Z stands for clockwise(<0) and counterclockwise(>0) rotation //���ٳ�Z����˳(<0)��(>0)ʱ����ת
			case 4:      Move_X=-RC_Velocity;  	 Move_Z=+PI/4;     break;
			#endif
		 
			case 5:      Move_X=-RC_Velocity;  	 Move_Z=0;         break;
		 
		  #if Akm_Car                          //AKM car Z stands for front wheel steering Angle //Akm��Z����ǰ��ת���
			case 6:      Move_X=-RC_Velocity;  	 Move_Z=+PI/4;    break;
		  #elif Diff_Car                       //Differential car Z stands for clockwise(<0) and counterclockwise(>0) rotation //���ٳ�Z����˳(<0)��(>0)ʱ����ת
		  case 6:      Move_X=-RC_Velocity;  	 Move_Z=-PI/4;    break;
		  #endif
		 
			case 7:      Move_X=0;     	 			 	 Move_Z=+PI/4;    break;
			case 8:      Move_X=+RC_Velocity; 	 Move_Z=+PI/4;    break; 
			default:     Move_X=0;               Move_Z=0;        break;
   }
	
	 //AKM car Z stands for front wheel steering Angle 
	 //Akm��Z����ǰ��ת���
	 #if Akm_Car 
	 //Different Ackerman cars have different maximum steering angles
	 //��ͬ������С�������ת��ǲ�һ��
	 if     (Car_Mode==2||Car_Mode==3) Move_Z=Move_Z*2/3;
	 else if(Car_Mode==4||Car_Mode==5) Move_Z=Move_Z/2;
	 else if(Car_Mode==6)              Move_Z=Move_Z*0.4f;
	 else Move_Z=Move_Z/2;
	 
	 //Differential car Z stands for clockwise(<0) and counterclockwise(>0) rotation 
	 //���ٳ�Z����˳(<0)��(>0)ʱ����ת
	 #elif Diff_Car
	 //The greater the forward speed, the greater the rotation speed
	 //ǰ���ٶ�Խ����ת�ٶ�Խ��
	 Move_Z=Move_Z*RC_Velocity/500; 
	 #endif

	 //Unit conversion, mm/s -> m/s
   //��λת����mm/s -> m/s	
	 Move_X=Move_X/1000;
	 
	 //Control target value is obtained and kinematics analysis is performed
	 //�õ�����Ŀ��ֵ�������˶�ѧ����
	 Drive_Motor(Move_X,Move_Z);
}
/**************************************************************************
Function: Handle PS2 controller control commands
Input   : none
Output  : none
�������ܣ���PS2�ֱ�����������д���
��ڲ�������
����  ֵ����
**************************************************************************/
void PS2_control(void)
{
   	int LX,LY,RY;
		int Yuzhi=20; //Threshold to ignore small movements of the joystick //��ֵ������ҡ��С���ȶ���
			
	  //128 is the median.The definition of X and Y in the PS2 coordinate system is different from that in the ROS coordinate system
	  //128Ϊ��ֵ��PS2����ϵ��ROS����ϵ��X��Y�Ķ��岻һ��
		LY=-(PS2_LX-128);
		LX=-(PS2_LY-128);
		RY=-(PS2_RX-128);

	  //Ignore small movements of the joystick //����ҡ��С���ȶ���
		if(LX>-Yuzhi&&LX<Yuzhi)LX=0;
		if(LY>-Yuzhi&&LY<Yuzhi)LY=0;
		if(RY>-Yuzhi&&RY<Yuzhi)RY=0;

	  if     (PS2_KEY==11) RC_Velocity+=5;  //To accelerate//����
		else if(PS2_KEY==9)	 RC_Velocity-=5;  //To slow down //����
	
		if(RC_Velocity<0)    RC_Velocity=0;
	
	  //Handle PS2 controller control commands
	  //��PS2�ֱ�����������д���
		Move_X=LX;
		Move_Z=RY;
	  Move_X=Move_X*RC_Velocity/128;
		Move_Z=Move_Z*(PI/4)/128;
		
	  //AKM car Z stands for front wheel steering Angle 
	  //Akm��Z����ǰ��ת���
		#if Akm_Car
	  //Different Ackerman cars have different maximum steering angles
	  //��ͬ������С�������ת��ǲ�һ��
	  if     (Car_Mode==2||Car_Mode==3) Move_Z=Move_Z*2/3;
		else if(Car_Mode==4||Car_Mode==5) Move_Z=Move_Z/2;
		else if(Car_Mode==6)              Move_Z=Move_Z*0.4f;
		else Move_Z=Move_Z/2;
	  #endif
		
		//Differential car Z stands for clockwise(<0) and counterclockwise(>0) rotation 
	  //���ٳ�Z����˳(<0)��(>0)ʱ����ת
		#if Diff_Car 
		//The greater the forward speed, the greater the rotation speed
	  //ǰ���ٶ�Խ����ת�ٶ�Խ��
		if(Move_X<0)Move_Z=-Move_Z*(RC_Velocity/500);
		#endif

	  //Unit conversion, mm/s -> m/s
    //��λת����mm/s -> m/s
		Move_X=Move_X/1000;

	  //Control target value is obtained and kinematics analysis is performed
	  //�õ�����Ŀ��ֵ�������˶�ѧ����
		Drive_Motor(Move_X,Move_Z);		
} 

/**************************************************************************
Function: The remote control command of model aircraft is processed
Input   : none
Output  : none
�������ܣ��Ժ�ģң�ؿ���������д���
��ڲ�������
����  ֵ����
**************************************************************************/
void Remote_Control(void)
{
	  //Data within 1 second after entering the model control mode will not be processed
	  //�Խ��뺽ģ����ģʽ��1���ڵ����ݲ�����
    static u8 thrice=100;
    int Yuzhi=100; //Threshold to ignore small movements of the joystick //��ֵ������ҡ��С���ȶ���
	
	  //limiter //�޷�
    int LX,LY,RY,RX,Remote_RCvelocity; 					//
		Remoter_Ch1=target_limit_int(Remoter_Ch1,1000,2000);
		Remoter_Ch2=target_limit_int(Remoter_Ch2,1000,2000);
		Remoter_Ch3=target_limit_int(Remoter_Ch3,1000,2000);
		Remoter_Ch4=target_limit_int(Remoter_Ch4,1000,2000);

		//Front and back direction of left rocker. Control forward and backward.
	  //��ҡ��ǰ���򡣿���ǰ�����ˡ�
    LX=Remoter_Ch2-1500;
	  //The channel is not currently in use
	  //��ͨ����ʱû��ʹ�õ�
    LY=Remoter_Ch4-1500;
	
		  //Front and back direction of right rocker. Throttle/acceleration/deceleration.
		//��ҡ��ǰ��������/�Ӽ��١�
	  RX=Remoter_Ch3-1500;											//
	  //Right stick left and right. To control the rotation. 
		//��ҡ�����ҷ��򡣿�����ת��
    RY=-(Remoter_Ch1-1500);//��ת

    if(LX>-Yuzhi&&LX<Yuzhi)LX=0;
    if(LY>-Yuzhi&&LY<Yuzhi)LY=0;
		if(RX>-Yuzhi&&RX<Yuzhi)RX=0;							//
    if(RY>-Yuzhi&&RY<Yuzhi)RY=0;
		
		
		//Throttle related //�������
		Remote_RCvelocity=RC_Velocity+RX;				//
	  if(Remote_RCvelocity<0)Remote_RCvelocity=0;					//
			
		//The remote control command of model aircraft is processed
		//�Ժ�ģң�ؿ���������д���
		Move_X=LX;
		Move_Z=RY;
		Move_X=Move_X*Remote_RCvelocity/500;					//
		Move_Z=Move_Z*(PI/4)/500;
		
	  //AKM car Z stands for front wheel steering Angle 
	  //Akm��Z����ǰ��ת���
		#if Akm_Car
	  //Different Ackerman cars have different maximum steering angles
	  //��ͬ������С�������ת��ǲ�һ��
	  if     (Car_Mode==2||Car_Mode==3) Move_Z=Move_Z*2/3;
		else if(Car_Mode==4||Car_Mode==5) Move_Z=Move_Z/2;
		else if(Car_Mode==6)              Move_Z=Move_Z*0.4f;
		else Move_Z=Move_Z/2;
	  #endif
		
		//Differential car Z stands for clockwise(<0) and counterclockwise(>0) rotation 
	  //���ٳ�Z����˳(<0)��(>0)ʱ����ת
		#if Diff_Car 
		//The greater the forward speed, the greater the rotation speed
	  //ǰ���ٶ�Խ����ת�ٶ�Խ��
		if(Move_X<0)Move_Z=-Move_Z*(RC_Velocity/500);
		#endif
			 
	  //Unit conversion, mm/s -> m/s
    //��λת����mm/s -> m/s
		Move_X=Move_X/1000;

	  //Data within 1 second after entering the model control mode will not be processed
	  //�Խ��뺽ģ����ģʽ��1���ڵ����ݲ�����
    if(thrice>0) Move_X=0,Move_Z=0,thrice--;

		//Control target value is obtained and kinematics analysis is performed
	  //�õ�����Ŀ��ֵ�������˶�ѧ����
		Drive_Motor(Move_X,Move_Z);
}
/**************************************************************************
Function: Click the user button to update gyroscope zero
Input   : none
Output  : none
�������ܣ������û������������������
��ڲ�������
����  ֵ����
**************************************************************************/
void Key(void)
{	
	u8 tmp;
	tmp=click(); 
	if(tmp==1)memcpy(Deviation_gyro,Original_gyro,sizeof(gyro));
}
/**************************************************************************
Function: Read the encoder value and calculate the wheel speed, unit m/s
Input   : none
Output  : none
�������ܣ���ȡ��������ֵ�����㳵���ٶȣ���λm/s
��ڲ�������
����  ֵ����
**************************************************************************/
void Get_Velocity_Form_Encoder(void)
{
	//Retrieves the original data of the encoder
	//��ȡ��������ԭʼ����
	float Encoder_A_pr,Encoder_B_pr; 
	Encoder_A_pr= Read_Encoder(2);  
	Encoder_B_pr=-Read_Encoder(3);

  //The encoder converts the raw data to wheel speed in m/s
	//������ԭʼ����ת��Ϊ�����ٶȣ���λm/s
	MOTOR_A.Encoder= Encoder_A_pr*CONTROL_FREQUENCY/Encoder_precision*Wheel_perimeter;
	MOTOR_B.Encoder= Encoder_B_pr*CONTROL_FREQUENCY/Encoder_precision*Wheel_perimeter;
}
/**************************************************************************
Function: Smoothing the target velocity
Input   : Target velocity
Output  : none
�������ܣ���Ŀ���ٶ���ƽ������
��ڲ�����Ŀ���ٶ�
����  ֵ����
**************************************************************************/
void Smooth_control(float vx, float step)
{
	if     (vx>smooth_control.VX) smooth_control.VX+=step;
	else if(vx<smooth_control.VX) smooth_control.VX-=step;
	else                          smooth_control.VX =vx;

	if(vx==0&&smooth_control.VX<(2.5f*step)&&smooth_control.VX>(-2.5f*step)) smooth_control.VX=0;
}

/**************************************************************************
Function: Prevent the potentiometer to choose the wrong mode, resulting in initialization error caused by the motor spinning.
Input   : none
Output  : none
�������ܣ���ֹ��λ��ѡ��ģʽ�����³�ʼ���������������ת��
��ڲ�������
����  ֵ����
**************************************************************************/
void robot_mode_check(void)
{
	static u8 error=0;
	if(myabs(MOTOR_A.Motor_Pwm)>2000||myabs(MOTOR_B.Motor_Pwm)>2000||myabs(MOTOR_C.Motor_Pwm)>2000||myabs(MOTOR_D.Motor_Pwm)>2000)   error++;
	else error=0;
	
	//If the output is close to full amplitude for 6 times in a row, it is judged that the motor rotates wildly and makes the motor incapacitated
	//�������6�νӽ�����������ж�Ϊ�����ת���õ��ʧ��	
	if(error>15) EN=0,Flag_Stop=1,robot_mode_check_flag=1;  //�������ʮ�νӽ�����������ж�Ϊ�����ת���õ��ʧ��	
}



