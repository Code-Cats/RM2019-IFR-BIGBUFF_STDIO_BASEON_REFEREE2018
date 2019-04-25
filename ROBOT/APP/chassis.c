#include "chassis.h"
#include "math.h"

CHASSIS_DATA chassis_Data={0};

PID_GENERAL PID_Chassis_Speed[4]={PID_CHASSIS_SPEED_DEFAULT,PID_CHASSIS_SPEED_DEFAULT,PID_CHASSIS_SPEED_DEFAULT,PID_CHASSIS_SPEED_DEFAULT};
PID_GENERAL PID_Chassis_Follow=PID_CHASSIS_FOLLOW_DEFAULT;

FIRST_ORDER_FILTER Yaw_Follow_Filter=YAW_FOLLOW_FILTER_DEFAULT;

s16 Chassis_Vx=0;
s16 Chassis_Vy=0;
s16 Chassis_Vw=0;


/************************�ⲿ��������**************************/
extern RC_Ctl_t RC_Ctl;	//ң������
extern GYRO_DATA Gyro_Data;
extern YUN_MOTOR_DATA 	yunMotorData;
extern tPowerHeatData 	testPowerHeatData;      //ʵʱ������������
extern u32 time_1ms_count;
extern ViceControlDataTypeDef ViceControlData;

extern u8 Yun_WorkState_Turn180_statu;	//180��ת��λ��־λ������������
extern u8 Replenish_Bullet_Statu;
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
extern u8 IMU_Check_Useless_State;	//������ʧЧ���λ
/*************************************************************/
extern AutoAimBulletTypeDef AutoAimBulletData;

/***************������ؿ���λ���������������ã�*****************/
u8 Chassis_Follow_Statu=1;	//���̸����־λ�������˱�־λԭ���Ƿ�����ڴ�����ʧЧʱ�л����̶���״̬
float yaw_follow_error=0;	//�����Ʊ��븡��	//��������̨����Ť�����ֻ��õ�
/*************************************************************/

	s16 test_yaw_follow_error=0;
	u8 test_turn_flag=0;
#define WAIST_RANGE 800//721��780//750
#define K_SPEED 10
s32 t_Vw_PID=0;
s32 yaw_follow_tarP=YAW_INIT_DEFINE;
volatile float yaw_follow_real_error=0;	//����Ť������ͨ״̬�µ�90��

float t_Vy_k=0;
float t_Vx_k=0;
u8 Chassis_Control_RCorPC=RC_CONTROL;


void Remote_Task(void)
{
	if(IMU_Check_Useless_State==0)
	{
		Chassis_Control_External_Solution();	//������λ�û�
	}
	else
	{
		Chassis_Control_Inscribe_Solution();	//��������λ�÷������ˣ��Ͳ����ٶȻ����Ƶ���̨������Ӧ�ĵ��̷���
	}
}



/******************************************************/
void Chassis_Control_External_Solution(void)	//�����������ĵ��̽������
{
	static u8 chassis_follow_statu_last=0;	//��¼��һ��״̬��Ŀ�����������¼��̴���������ģʽ�ͼ��̿���ģʽ��Vw����������
	
	if(GetWorkState()==NORMAL_STATE)	//����PID��λ
	{
		for(int i=0;i<4;i++)
		{
			PID_Chassis_Speed[i].k_i=CHASSIS_SPEED_PID_I;
			PID_Chassis_Speed[i].i_sum_max=CHASSIS_SPEED_I_MAX;
		}
	}
	
	if(GetWorkState()!=PREPARE_STATE&&GetWorkState()!=CALI_STATE)	//ģʽ�л�
	{
		if(RC_Ctl.key.v_h!=0||RC_Ctl.key.v_l!=0)
		{
			Chassis_Control_RCorPC=PC_CONTROL;
		}
		else if(abs(RC_Ctl.rc.ch0-1024)>3||abs(RC_Ctl.rc.ch1-1024)>3||abs(RC_Ctl.rc.ch2-1024)>3)
		{
			Chassis_Control_RCorPC=RC_CONTROL;
		}
	}
	
	
	if(AutoAimBulletData.control_state==1)	//�Զ���λʱ�������ֶ�����
	{
		
	}
	else	//�ر��Զ���λ
	{
		if(Chassis_Control_RCorPC==RC_CONTROL)
		{
			RC_Control_Chassis();
		}
		else if(Chassis_Control_RCorPC==PC_CONTROL)
		{
			PC_Control_Chassis(&Chassis_Vx,&Chassis_Vy);
		}

	}
	
	
	
	static u8 turn_flag=0;
	if(GetWorkState()==WAIST_STATE)
	{
		
		s32 yaw_init_def=YAW_INIT_DEFINE;
		switch(turn_flag)
		{
			case 0:
			{
				YAW_INIT=yaw_init_def-WAIST_RANGE;
				test_yaw_follow_error=YAW_INIT-yunMotorData.yaw_fdbP;/////////////////////////////////////////
				test_turn_flag=0;///////////////////////////////////////
				if((YAW_INIT-yunMotorData.yaw_fdbP)>-85)	//�Ż�-����һ��0���� ��ֵ�Ϳ���
				{
					turn_flag=1;
				}
				break;
			}
			case 1:
			{
				YAW_INIT=yaw_init_def+WAIST_RANGE;
				test_yaw_follow_error=YAW_INIT-yunMotorData.yaw_fdbP;//////////////////////////////////////////
				test_turn_flag=0;//////////////////////////////////
				if((YAW_INIT-yunMotorData.yaw_fdbP)<85)	//�Ż�-С��һ��0��������ֵ
				{
					turn_flag=0;
				}
				break;
			}
		}
		
		PID_Chassis_Follow.k_p=CHASSIS_FOLLOW_PID_P/1.27f;
	}
	else
	{
		YAW_INIT=YAW_INIT_DEFINE;
		PID_Chassis_Follow.k_p=CHASSIS_FOLLOW_PID_P;
	}
	
	
	
//	yaw_follow_tarP=yunMotorData.yaw_fdbP;
	if(yunMotorData.yaw_tarP-Gyro_Data.angle[2]*10>1800)	//�����
	{
		yaw_follow_tarP=yunMotorData.yaw_fdbP+(yunMotorData.yaw_tarP-(Gyro_Data.angle[2]*10+3600))*8192/3600;
	}
	else if(yunMotorData.yaw_tarP-Gyro_Data.angle[2]*10<-1800)
	{
		yaw_follow_tarP=yunMotorData.yaw_fdbP+(yunMotorData.yaw_tarP-(Gyro_Data.angle[2]*10-3600))*8192/3600;
	}
	else
	{
		yaw_follow_tarP=yunMotorData.yaw_fdbP+(yunMotorData.yaw_tarP-Gyro_Data.angle[2]*10)*8192/3600;
	}
	
	
	
	if((GetWorkState()==NORMAL_STATE||GetWorkState()==WAIST_STATE)&&Replenish_Bullet_Statu==0)	//���̸����־λ�����	//ȡ��������������̨��
	{
		Chassis_Follow_Statu=1;
	}
	else if((GetWorkState()==TAKEBULLET_STATE)||Replenish_Bullet_Statu==1)	//ȡ��ת����ܿ�+����λ=1ʱ
	{
		Chassis_Follow_Statu=0;
	}
//	else
//	{
//		Chassis_Follow_Statu=0;
//	}
	
	
	if(chassis_follow_statu_last!=Chassis_Follow_Statu)
	{
		Chassis_Vw=0;	//��ֹVw����������
		Chassis_Control_RCorPC=RC_CONTROL;
	}
	
	chassis_follow_statu_last=Chassis_Follow_Statu;
	
	
	if(Chassis_Follow_Statu==1)	//���̸��沿�ֽ���
	{
//////		Chassis_Vx=RC_Ctl.rc.ch1-1024;
		
//		Chassis_Vw=RC_Ctl.rc.ch2-1024;
		if((abs(RC_Ctl.rc.ch2-1024)<40&&abs(YAW_INIT-yunMotorData.yaw_fdbP)<180)||GetWorkState()==TAKEBULLET_STATE)//�˴��������ڼ��ٶȹ���ʱ�������нϴ�����˲��õ�ת����ͨ���棬��ת�����ܸ���ģʽ	//�˴�Ӧ��fdb��ΪtarP
		{
			if(YAW_INIT-yunMotorData.yaw_fdbP>8192/2)	//��ͨ�����	
			{
				Chassis_Vw=-PID_ChassisFollow(YAW_INIT,yunMotorData.yaw_fdbP+8192,&PID_Chassis_Follow);	//������״����
				yaw_follow_error=YAW_INIT-(yunMotorData.yaw_fdbP+8192);
			}
			else if(YAW_INIT-yunMotorData.yaw_fdbP<-8192/2)
			{
				Chassis_Vw=-PID_ChassisFollow(YAW_INIT,yunMotorData.yaw_fdbP-8192,&PID_Chassis_Follow);	//������״����
				yaw_follow_error=YAW_INIT-(yunMotorData.yaw_fdbP-8192);
			}
			else
			{
				Chassis_Vw=-PID_ChassisFollow(YAW_INIT,yunMotorData.yaw_fdbP,&PID_Chassis_Follow);	//����״����
				yaw_follow_error=YAW_INIT-yunMotorData.yaw_fdbP;
			}
		}
		else
		{
			if(YAW_INIT-yaw_follow_tarP>8192/2)	//���ܸ����	
			{
				Chassis_Vw=-PID_ChassisFollow(YAW_INIT,yaw_follow_tarP+8192,&PID_Chassis_Follow);	//������״����
				yaw_follow_error=YAW_INIT-(yaw_follow_tarP+8192);
			}
			else if(YAW_INIT-yaw_follow_tarP<-8192/2)
			{
				Chassis_Vw=-PID_ChassisFollow(YAW_INIT,yaw_follow_tarP-8192,&PID_Chassis_Follow);	//������״����
				yaw_follow_error=YAW_INIT-(yaw_follow_tarP-8192);
			}
			else
			{
				Chassis_Vw=-PID_ChassisFollow(YAW_INIT,yaw_follow_tarP,&PID_Chassis_Follow);	//����״����
				yaw_follow_error=YAW_INIT-yaw_follow_tarP;
			}
		}
			


		Chassis_Vw=chassis_Vw_filter(Chassis_Vw);	//���ٶ�һ���˲�
		
		
//		Chassis_Vw=(s16)(FirstOrder_General((YAW_INIT-yunMotorData.yaw_fdbP),&Yaw_Follow_Filter)*0.43f);
//		Chassis_Vw=(s16)((YAW_INIT-yunMotorData.yaw_fdbP)*0.6f);	//YUN_INITΪĿ��λ�ã���ΪYAW_INIT-
	}
	
//////////////////	if(GetWorkState()==NORMAL_STATE||GetWorkState()==TAKEBULLET_STATE)	//ȡ��Ϊ��ʱ����
	yaw_follow_error=yaw_follow_error/8192.0f*2*PI;	//ÿ�ζ����㷽��yun.c����
//////////////////	if(GetWorkState()==NORMAL_STATE&&abs(YAW_INIT-yunMotorData.yaw_fdbP)>200)	//����Ư��	//���ֹ���Ʈ�ƻ�Ӱ��ת�򣬽������1��ת���������ﵽ���޴�������ʵ���ٶȱ���ƫ��Ԥ�ڣ�����һ��������������ֵ����8000ʱ��������ʹ����Ԥ�ڣ�������������Vy
//////////////////	{	//����ת���
//////////////////		s16 Vx_record=Chassis_Vx;
//////////////////		float yaw_follow_error_deal=0.0;	//5.21������Ż�	��ֹת�����ֱ��ͣ��
//////////////////		Chassis_Vx=0;
//////////////////		
//////////////////		yaw_follow_error_deal=yaw_follow_error*0.8f;	//5.21������Ż�	��ֹת�����ֱ��ͣ��
////////////////////		yaw_follow_error=yaw_follow_error/8192.0f*2*PI;
//////////////////		Chassis_Vx+=(s16)(Vx_record*(cos(yaw_follow_error_deal)));	//��ԭֻ�轫yaw_follow_error_deal����yaw_follow_error
//////////////////		Chassis_Vy+=(s16)(Vx_record*(sin(yaw_follow_error_deal))*1);
//////////////////	}
	
	
////////////////	if(GetWorkState()==WAIST_STATE)	//Ť��ǰ��	 ��������
////////////////	{
////////////////		s16 Vx_record=Chassis_Vx;
////////////////		if(YAW_INIT_DEFINE-yaw_follow_tarP>8192/2)	//��ֹ����
////////////////		{
////////////////			yaw_follow_real_error=YAW_INIT_DEFINE-(yaw_follow_tarP+8192);
////////////////		}
////////////////		else if(YAW_INIT_DEFINE-yaw_follow_tarP<-8192/2)
////////////////		{
////////////////			yaw_follow_real_error=YAW_INIT_DEFINE-(yaw_follow_tarP-8192);
////////////////		}
////////////////		else
////////////////		{
////////////////			yaw_follow_real_error=YAW_INIT_DEFINE-yaw_follow_tarP;
////////////////		}

////////////////		Chassis_Vx=0;
////////////////		yaw_follow_real_error=yaw_follow_real_error/8192.0f*2*PI;
////////////////		Chassis_Vx+=(s16)(Vx_record*(cos(yaw_follow_real_error)));
////////////////		Chassis_Vy+=(s16)(Vx_record*(sin(yaw_follow_real_error))*1);	
//////////////////		t_Vy_k=sin(yaw_follow_real_error);
//////////////////		t_Vx_k=cos(yaw_follow_real_error);
////////////////	}
	
	if(GetWorkState()==WAIST_STATE)	//Ť��ǰ��	 ������		//�ú���Ҫ��ÿ�δ���ǰ������Vx,Vy��ˢ��
	{
		s16 Vx_record=Chassis_Vx;
		s16 Vy_record=Chassis_Vy;
		if(YAW_INIT_DEFINE-yunMotorData.yaw_fdbP>8192/2)	//��ֹ����
		{
			yaw_follow_real_error=YAW_INIT_DEFINE-(yunMotorData.yaw_fdbP+8192);
		}
		else if(YAW_INIT_DEFINE-yunMotorData.yaw_fdbP<-8192/2)
		{
			yaw_follow_real_error=YAW_INIT_DEFINE-(yunMotorData.yaw_fdbP-8192);
		}
		else
		{
			yaw_follow_real_error=YAW_INIT_DEFINE-yunMotorData.yaw_fdbP;
		}
		Chassis_Vx=0;
		Chassis_Vy=0;
		yaw_follow_real_error=yaw_follow_real_error/8192.0f*2*PI;
		Chassis_Vx+=(s16)((Vx_record*cos(-yaw_follow_real_error)+Vy_record*cos(-yaw_follow_real_error+PI/2))*0.91f);
		Chassis_Vy+=(s16)((Vx_record*sin(-yaw_follow_real_error)+Vy_record*sin(-yaw_follow_real_error+PI/2))*1.09f);	
//		t_Vy_k=sin(-yaw_follow_real_error);
//		t_Vx_k=cos(-yaw_follow_real_error);
	}
	
	
//////////////////	if(GetWorkState()==WAIST_STATE&&KeyBoardData[KEY_CTRL].value==1	)	//��̨����ת��	//����Ť��ʱ��Ч
//////////////////	{	//�ȵ�CTRLд���˿���
//////////////////		float chassis_vw_record=Chassis_Vw;
//////////////////		Chassis_Vy-=(s16)(chassis_vw_record/1.7f);	//2
//////////////////		Chassis_Vx+=21;	//�Ժ��˵Ĳ���	//ÿ�ε�����ѹ��Ҫ���µ���		//˼�����������ﻹ������Ť��ǰ������֮ǰ����
//////////////////	}

//	Chassis_Vw=0;	//////////////////////////////��ʱ����
				
	chassis_Data.lf_wheel_tarV=(Chassis_Vx+Chassis_Vy+Chassis_Vw)*K_SPEED;
	chassis_Data.rf_wheel_tarV=(-Chassis_Vx+Chassis_Vy+Chassis_Vw)*K_SPEED;
	chassis_Data.lb_wheel_tarV=(Chassis_Vx-Chassis_Vy+Chassis_Vw)*K_SPEED;
	chassis_Data.rb_wheel_tarV=(-Chassis_Vx-Chassis_Vy+Chassis_Vw)*K_SPEED;
	
	
	Overall_Motion_Ratio_Protect(&chassis_Data);	//�����ٶȱ���
	

	
	chassis_Data.lf_wheel_output=PID_General(chassis_Data.lf_wheel_tarV,chassis_Data.lf_wheel_fdbV,&PID_Chassis_Speed[LF]);
	chassis_Data.rf_wheel_output=PID_General(chassis_Data.rf_wheel_tarV,chassis_Data.rf_wheel_fdbV,&PID_Chassis_Speed[RF]);
	chassis_Data.lb_wheel_output=PID_General(chassis_Data.lb_wheel_tarV,chassis_Data.lb_wheel_fdbV,&PID_Chassis_Speed[LB]);
	chassis_Data.rb_wheel_output=PID_General(chassis_Data.rb_wheel_tarV,chassis_Data.rb_wheel_fdbV,&PID_Chassis_Speed[RB]);

	
	if(GetWorkState()==NORMAL_STATE&&RC_Ctl.rc.switch_left==RC_SWITCH_UP)	//��չ���������PID������
	{
		Extended_Integral_PID(&chassis_Data);
	}
	
	
	{	//�������ƿ�
		float limit_k=Limit_Power(testPowerHeatData.chassisPower,testPowerHeatData.chassisPowerBuffer);	//testPowerHeatData.chassisPowerBuffer
		float output_limit_lf=chassis_Data.lf_wheel_output*limit_k;
		float output_limit_rf=chassis_Data.rf_wheel_output*limit_k;
		float output_limit_lb=chassis_Data.lb_wheel_output*limit_k;
		float output_limit_rb=chassis_Data.rb_wheel_output*limit_k;
		chassis_Data.lf_wheel_output=(s32)output_limit_lf;
		chassis_Data.rf_wheel_output=(s32)output_limit_rf;
		chassis_Data.lb_wheel_output=(s32)output_limit_lb;
		chassis_Data.rb_wheel_output=(s32)output_limit_rb;
		
	if(chassis_Data.lf_wheel_output>10000)	chassis_Data.lf_wheel_output=10000;
	if(chassis_Data.lf_wheel_output<-10000)	chassis_Data.lf_wheel_output=-10000;
	if(chassis_Data.rf_wheel_output>10000)	chassis_Data.rf_wheel_output=10000;
	if(chassis_Data.rf_wheel_output<-10000)	chassis_Data.rf_wheel_output=-10000;
	if(chassis_Data.lb_wheel_output>10000)	chassis_Data.lb_wheel_output=10000;
	if(chassis_Data.lb_wheel_output<-10000)	chassis_Data.lb_wheel_output=-10000;
	if(chassis_Data.rb_wheel_output>10000)	chassis_Data.rb_wheel_output=10000;
	if(chassis_Data.rb_wheel_output<-10000)	chassis_Data.rb_wheel_output=-10000;
	}
}
/******************************************************/



/******************************************************/	//������ʧЧʱ�ĵ��̸���Ƚ������
void Chassis_Control_Inscribe_Solution(void)	//������ʧЧ�ĵ��̽������	/////////////////////////////////////////�����棬Ť�����ܲ�ͬ
{
	static u8 chassis_follow_statu_last=0;	//��¼��һ��״̬��Ŀ�����������¼��̴���������ģʽ�ͼ��̿���ģʽ��Vw����������
	
	if(GetWorkState()==NORMAL_STATE)	//����PID��λ
	{
		for(int i=0;i<4;i++)
		{
			PID_Chassis_Speed[i].k_i=CHASSIS_SPEED_PID_I;
			PID_Chassis_Speed[i].i_sum_max=CHASSIS_SPEED_I_MAX;
		}
	}
	
	if(GetWorkState()!=PREPARE_STATE&&GetWorkState()!=CALI_STATE)	//ģʽ�л�
	{
		if(RC_Ctl.key.v_h!=0||RC_Ctl.key.v_l!=0)
		{
			Chassis_Control_RCorPC=PC_CONTROL;
		}
		else if(abs(RC_Ctl.rc.ch0-1024)>3||abs(RC_Ctl.rc.ch1-1024)>3||abs(RC_Ctl.rc.ch2-1024)>3)
		{
			Chassis_Control_RCorPC=RC_CONTROL;
		}
	}
	
	if(Chassis_Control_RCorPC==RC_CONTROL)
	{
		RC_Control_Chassis();
	}
	else if(Chassis_Control_RCorPC==PC_CONTROL)
	{
		PC_Control_Chassis(&Chassis_Vx,&Chassis_Vy);
	}
	
	
	
	static u8 turn_flag=0;
	if(GetWorkState()==WAIST_STATE)
	{
		
		s32 yaw_init_def=YAW_INIT_DEFINE;
		switch(turn_flag)
		{
			case 0:
			{
				YAW_INIT=yaw_init_def-WAIST_RANGE;
				if(abs(YAW_INIT-yunMotorData.yaw_fdbP)<55)
				{
					turn_flag=1;
				}
				break;
			}
			case 1:
			{
				YAW_INIT=yaw_init_def+WAIST_RANGE;
				if(abs(YAW_INIT-yunMotorData.yaw_fdbP)<55)
				{
					turn_flag=0;
				}
				break;
			}
		}
		
		PID_Chassis_Follow.k_p=CHASSIS_FOLLOW_PID_P/1;
	}
	else
	{
		YAW_INIT=YAW_INIT_DEFINE;
		PID_Chassis_Follow.k_p=CHASSIS_FOLLOW_PID_P;
	}
	
	
	
//////////////	yaw_follow_tarP=yunMotorData.yaw_fdbP;
////////////	if(yunMotorData.yaw_tarP-Gyro_Data.angle[2]*10>1800)	//�����
////////////	{
////////////		yaw_follow_tarP=yunMotorData.yaw_fdbP+(yunMotorData.yaw_tarP-(Gyro_Data.angle[2]*10+3600))*8192/3600;
////////////	}
////////////	else if(yunMotorData.yaw_tarP-Gyro_Data.angle[2]*10<-1800)
////////////	{
////////////		yaw_follow_tarP=yunMotorData.yaw_fdbP+(yunMotorData.yaw_tarP-(Gyro_Data.angle[2]*10-3600))*8192/3600;
////////////	}
////////////	else
////////////	{
////////////		yaw_follow_tarP=yunMotorData.yaw_fdbP+(yunMotorData.yaw_tarP-Gyro_Data.angle[2]*10)*8192/3600;
////////////	}
	
	
	
	if((GetWorkState()==NORMAL_STATE||GetWorkState()==WAIST_STATE)&&Replenish_Bullet_Statu==0)	//���̸����־λ�����	//ȡ��������������̨��
	{
		Chassis_Follow_Statu=1;
	}
	else if(GetWorkState()==TAKEBULLET_STATE||Replenish_Bullet_Statu==1)	//��������ʧЧ������ģʽ�����Ƕ��ܿ�
	{
		Chassis_Follow_Statu=1;
	}
//	else
//	{
//		Chassis_Follow_Statu=0;
//	}
	
	
	if(chassis_follow_statu_last!=Chassis_Follow_Statu)
	{
		Chassis_Vw=0;	//��ֹVw����������
		Chassis_Control_RCorPC=RC_CONTROL;
	}
	
	chassis_follow_statu_last=Chassis_Follow_Statu;
	
	
	if(Chassis_Follow_Statu==1)	//���̸��沿�ֽ���
	{
//////		Chassis_Vx=RC_Ctl.rc.ch1-1024;
		
//		Chassis_Vw=RC_Ctl.rc.ch2-1024;
		if(1)//�˴��������ڼ��ٶȹ���ʱ�������нϴ�����˲��õ�ת����ͨ���棬��ת�����ܸ���ģʽ	//�˴�Ӧ��fdb��ΪtarP
		{
			if(YAW_INIT-yunMotorData.yaw_fdbP>8192/2)	//��ͨ�����	
			{
				Chassis_Vw=-PID_ChassisFollow(YAW_INIT,yunMotorData.yaw_fdbP+8192,&PID_Chassis_Follow);	//������״����
				yaw_follow_error=YAW_INIT-(yunMotorData.yaw_fdbP+8192);
			}
			else if(YAW_INIT-yunMotorData.yaw_fdbP<-8192/2)
			{
				Chassis_Vw=-PID_ChassisFollow(YAW_INIT,yunMotorData.yaw_fdbP-8192,&PID_Chassis_Follow);	//������״����
				yaw_follow_error=YAW_INIT-(yunMotorData.yaw_fdbP-8192);
			}
			else
			{
				Chassis_Vw=-PID_ChassisFollow(YAW_INIT,yunMotorData.yaw_fdbP,&PID_Chassis_Follow);	//����״����
				yaw_follow_error=YAW_INIT-yunMotorData.yaw_fdbP;
			}
		}
//////////		else	//����4-5��ע�͵���˼�Ǻͱ�׼��������������
//////////		{
//////////			if(YAW_INIT-yaw_follow_tarP>8192/2)	//���ܸ����	
//////////			{
//////////				Chassis_Vw=PID_ChassisFollow(YAW_INIT,yaw_follow_tarP+8192,&PID_Chassis_Follow);	//������״����
//////////				yaw_follow_error=YAW_INIT-(yaw_follow_tarP+8192);
//////////			}
//////////			else if(YAW_INIT-yaw_follow_tarP<-8192/2)
//////////			{
//////////				Chassis_Vw=PID_ChassisFollow(YAW_INIT,yaw_follow_tarP-8192,&PID_Chassis_Follow);	//������״����
//////////				yaw_follow_error=YAW_INIT-(yaw_follow_tarP-8192);
//////////			}
//////////			else
//////////			{
//////////				Chassis_Vw=PID_ChassisFollow(YAW_INIT,yaw_follow_tarP,&PID_Chassis_Follow);	//����״����
//////////				yaw_follow_error=YAW_INIT-yaw_follow_tarP;
//////////			}
//////////		}
			


		Chassis_Vw=chassis_Vw_filter(Chassis_Vw);	//���ٶ�һ���˲�
		
		
//		Chassis_Vw=(s16)(FirstOrder_General((YAW_INIT-yunMotorData.yaw_fdbP),&Yaw_Follow_Filter)*0.43f);
//		Chassis_Vw=(s16)((YAW_INIT-yunMotorData.yaw_fdbP)*0.6f);	//YUN_INITΪĿ��λ�ã���ΪYAW_INIT-
	}
	
	if(GetWorkState()==NORMAL_STATE||GetWorkState()==TAKEBULLET_STATE)	//ȡ��Ϊ��ʱ����
	yaw_follow_error=yaw_follow_error/8192.0f*2*PI;	//ÿ�ζ����㷽��yun.c����
	if(GetWorkState()==NORMAL_STATE&&abs(YAW_INIT-yunMotorData.yaw_fdbP)>200)	//����Ư��	//���ֹ���Ʈ�ƻ�Ӱ��ת�򣬽������1��ת���������ﵽ���޴�������ʵ���ٶȱ���ƫ��Ԥ�ڣ�����һ��������������ֵ����8000ʱ��������ʹ����Ԥ�ڣ�������������Vy
	{	//����ת���
		s16 Vx_record=Chassis_Vx;
		float yaw_follow_error_deal=0.0;	//5.21������Ż�	��ֹת�����ֱ��ͣ��
		Chassis_Vx=0;
		
		yaw_follow_error_deal=yaw_follow_error*0.8f;	//5.21������Ż�	��ֹת�����ֱ��ͣ��
//		yaw_follow_error=yaw_follow_error/8192.0f*2*PI;
		Chassis_Vx+=(s16)(Vx_record*(cos(yaw_follow_error_deal)));	//��ԭֻ�轫yaw_follow_error_deal����yaw_follow_error
		Chassis_Vy+=(s16)(Vx_record*(sin(yaw_follow_error_deal))*1);
	}
	
	
	if(GetWorkState()==WAIST_STATE)	//Ť��ǰ��	//////////////////////////////////////////////////////////////////////////////////////������������������������OK������
	{
		s16 Vx_record=Chassis_Vx;
		if(YAW_INIT_DEFINE-yunMotorData.yaw_fdbP>8192/2)	//��ֹ����
		{
			yaw_follow_real_error=YAW_INIT_DEFINE-(yunMotorData.yaw_fdbP+8192);
		}
		else if(YAW_INIT_DEFINE-yunMotorData.yaw_fdbP<-8192/2)
		{
			yaw_follow_real_error=YAW_INIT_DEFINE-(yunMotorData.yaw_fdbP-8192);
		}
		else
		{
			yaw_follow_real_error=YAW_INIT_DEFINE-yunMotorData.yaw_fdbP;
		}

		Chassis_Vx=0;
		yaw_follow_real_error=yaw_follow_real_error/8192.0f*2*PI;
		Chassis_Vx+=(s16)(Vx_record*(cos(yaw_follow_real_error)));
		Chassis_Vy+=(s16)(Vx_record*(sin(yaw_follow_real_error))*1);	
//		t_Vy_k=sin(yaw_follow_real_error);
//		t_Vx_k=cos(yaw_follow_real_error);
	}
	
	
	
	if(GetWorkState()==WAIST_STATE&&KeyBoardData[KEY_CTRL].value==1	)	//��̨����ת��	//����Ť��ʱ��Ч
	{	//�ȵ�CTRLд���˿���
		float chassis_vw_record=Chassis_Vw;
		Chassis_Vy-=(s16)(chassis_vw_record/1.7f);	//2
		Chassis_Vx+=21;	//�Ժ��˵Ĳ���	//ÿ�ε�����ѹ��Ҫ���µ���		//˼�����������ﻹ������Ť��ǰ������֮ǰ����
	}

				
	chassis_Data.lf_wheel_tarV=(Chassis_Vx+Chassis_Vy+Chassis_Vw)*K_SPEED;
	chassis_Data.rf_wheel_tarV=(-Chassis_Vx+Chassis_Vy+Chassis_Vw)*K_SPEED;
	chassis_Data.lb_wheel_tarV=(Chassis_Vx-Chassis_Vy+Chassis_Vw)*K_SPEED;
	chassis_Data.rb_wheel_tarV=(-Chassis_Vx-Chassis_Vy+Chassis_Vw)*K_SPEED;
	
	
	Overall_Motion_Ratio_Protect(&chassis_Data);	//�����ٶȱ���
	

	
	chassis_Data.lf_wheel_output=PID_General(chassis_Data.lf_wheel_tarV,chassis_Data.lf_wheel_fdbV,&PID_Chassis_Speed[LF]);
	chassis_Data.rf_wheel_output=PID_General(chassis_Data.rf_wheel_tarV,chassis_Data.rf_wheel_fdbV,&PID_Chassis_Speed[RF]);
	chassis_Data.lb_wheel_output=PID_General(chassis_Data.lb_wheel_tarV,chassis_Data.lb_wheel_fdbV,&PID_Chassis_Speed[LB]);
	chassis_Data.rb_wheel_output=PID_General(chassis_Data.rb_wheel_tarV,chassis_Data.rb_wheel_fdbV,&PID_Chassis_Speed[RB]);

	
	if(GetWorkState()==NORMAL_STATE&&RC_Ctl.rc.switch_left==RC_SWITCH_UP)	//��չ���������PID������
	{
		Extended_Integral_PID(&chassis_Data);
	}
	
	
	{	//�������ƿ�
		float limit_k=Limit_Power(testPowerHeatData.chassisPower,testPowerHeatData.chassisPowerBuffer);	//testPowerHeatData.chassisPowerBuffer
		float output_limit_lf=chassis_Data.lf_wheel_output*limit_k;
		float output_limit_rf=chassis_Data.rf_wheel_output*limit_k;
		float output_limit_lb=chassis_Data.lb_wheel_output*limit_k;
		float output_limit_rb=chassis_Data.rb_wheel_output*limit_k;
		chassis_Data.lf_wheel_output=(s32)output_limit_lf;
		chassis_Data.rf_wheel_output=(s32)output_limit_rf;
		chassis_Data.lb_wheel_output=(s32)output_limit_lb;
		chassis_Data.rb_wheel_output=(s32)output_limit_rb;
	}
}




/******************************************************/



void RC_Control_Chassis(void)
{
	static s16 Chassis_Vx_last=0;
	static s16 Chassis_Vy_last=0;
	if((GetWorkState()==NORMAL_STATE||GetWorkState()==WAIST_STATE||GetWorkState()==TAKEBULLET_STATE)&&Replenish_Bullet_Statu==0)	//�������ͨģʽ�Ҳ���=0
	{
		if(time_1ms_count%1==0)
		{
			if(RC_Ctl.rc.ch1-1024-Chassis_Vx_last>1&&RC_Ctl.rc.ch1-1024>10)	//ֻ��ǰ������ʱ��Ч��
			{
				Chassis_Vx+=1;
			}
			else if(RC_Ctl.rc.ch1-1024-Chassis_Vx_last<-1&&RC_Ctl.rc.ch1-1024<-10)	//ֻ�ں��˼���ʱ��Ч
			{
				Chassis_Vx-=1;
			}
			else
			{
				Chassis_Vx=RC_Ctl.rc.ch1-1024;
			}
		}
//		Chassis_Vx=RC_Ctl.rc.ch1-1024;	//����Ϊб�º���
		Chassis_Vx_last=Chassis_Vx;
	}
	else if(Replenish_Bullet_Statu==1)	//ȡ��ʱ���򣬲���ʱ����		//GetWorkState()==TAKEBULLET_STATE||
	{
		if(time_1ms_count%1==0)
		{
			Chassis_Vx=-(RC_Ctl.rc.ch1-1024);
		}
//		Chassis_Vx=RC_Ctl.rc.ch1-1024;	//����Ϊб�º���
		Chassis_Vx_last=Chassis_Vx;
	}
	
	if(time_1ms_count%1==0)
	{
		if(Replenish_Bullet_Statu==0)	//ȡ��ʱ����	GetWorkState()!=TAKEBULLET_STATE&&
		{
			if(RC_Ctl.rc.ch0-1024-Chassis_Vy_last>1&&RC_Ctl.rc.ch0-1024>10)
			{
				Chassis_Vy+=1;
			}
			else if(RC_Ctl.rc.ch0-1024-Chassis_Vy_last<-1&&RC_Ctl.rc.ch0-1024<-10)	//ɲ����������
			{
				Chassis_Vy-=1;
			}
			else
			{
				Chassis_Vy=RC_Ctl.rc.ch0-1024;
			}
		}
		else	//ȡ������
		{
			Chassis_Vy=-(RC_Ctl.rc.ch0-1024);
		}

	}
	Chassis_Vy_last=Chassis_Vy;
	
	if((GetWorkState()==TAKEBULLET_STATE||Replenish_Bullet_Statu==1)&&IMU_Check_Useless_State==0)	//������������ʱȡ��ģʽ������̨�أ���������ʧЧ���ܿ�
	{
		Chassis_Vw=(RC_Ctl.rc.ch2-1024)*300/660;
	}
//	Chassis_Vy=RC_Ctl.rc.ch0-1024;
}

extern KeyBoardTypeDef KeyBoardData[KEY_NUMS];
void PC_Control_Chassis(s16 * chassis_vx,s16 * chassis_vy)	//1000Hz
{
	static s16 chassis_vx_record=0;
	static s16 chassis_vy_record=0;
	if(GetWorkState()==NORMAL_STATE||GetWorkState()==WAIST_STATE||GetWorkState()==TAKEBULLET_STATE)
	{
		if(time_1ms_count%2==0)
		{
			if(KeyBoardData[KEY_W].value!=0)
			{
				if(chassis_vx_record<660&&chassis_vx_record>=0)
				{
					chassis_vx_record++;
				}
				else if(chassis_vx_record<0)
				{
					chassis_vx_record=0;
				}
			}
			else if(KeyBoardData[KEY_S].value!=0)
			{
				if(chassis_vx_record>-660&&chassis_vx_record<=0)
				{
					chassis_vx_record--;
				}
				else if(chassis_vx_record>0)
				{
					chassis_vx_record=0;
				}
			}
			else
			{
				chassis_vx_record=0;
			}
			///////////////////////////////////////
			if(KeyBoardData[KEY_D].value!=0)
			{
				if(chassis_vy_record<660&&chassis_vy_record>=0)
				{
					chassis_vy_record++;
				}
				else if(*chassis_vy<0)
				{
					chassis_vy_record=0;
				}
			}
			else if(KeyBoardData[KEY_A].value!=0)
			{
				if(chassis_vy_record>-660&&chassis_vy_record<=0)
				{
					chassis_vy_record--;
				}
				else if(chassis_vy_record>0)
				{
					chassis_vy_record=0;
				}
			}
			else
			{
				chassis_vy_record=0;
			}
			*chassis_vx=chassis_vx_record;
			*chassis_vy=chassis_vy_record;
		}

	}
		
}


s16 chassis_Vw_filter(s16 now_V)
{
	static s32 Last_V=0;
	static s32 now_acc_V=0;
	static float a=0.25f;
	now_acc_V=(s32)(Last_V*(1-a)+now_V*a);
	Last_V=now_acc_V;
	return now_acc_V;
}


#define POWER_LIMIT_K 0.8f/50.0f	//�������ۿ�ʱ0.2��50ʱ��ʼ����
#define POWER_LIMIT_B	0.21f
u8 limit_power_statu=0;
extern u8 SuperC_Output_Enable;	//�����Ƿ��ܷŵ�
extern Error_check_t Error_Check;
#define POWERLIMIT 120 	//120w��������
#define POWERBUFFER 60	//60J���ʻ���
float Limit_Power(float power,float powerbuffer)	//Ӣ��120J�������ƣ�ֱ�����������
{
	float limit_k=1;
//	if(power>POWERLIMIT*0.6)
//	{
////		limit_k=3.0f*powerbuffer/200.0f+0.2f;	//0.4
////		limit_k=limit_k>1?1:limit_k;
////		limit_k=limit_k<0.1f?0.1f:limit_k;
//	}
//	limit_k=0.9;	//ȡ���������ƣ����κ�ȡ��
////////////	if(SuperC_Output_Enable==0)//���ݲ��ܷŵ�
////////////	{
////////////		limit_power_statu=1;
////////////		limit_k=3.0f*powerbuffer/200.0f+0.08f;	//0.4
////////////		limit_k=limit_k>1?1:limit_k;
////////////		limit_k=limit_k<0.1f?0.1f:limit_k;
////////////	}
////////////	else	//���ݿ��Էŵ磬�ſ�����
////////////	{
////////////		limit_power_statu=2;
////////////		limit_k=3.0f*powerbuffer/125.0f+0.1f;//+0.16f;//+0.25f;	//30j��ʼ����
////////////		limit_k=limit_k>1?1:limit_k;
////////////		limit_k=limit_k<0.1f?0.1f:limit_k;
////////////	}

	
	limit_k=POWER_LIMIT_K*powerbuffer+POWER_LIMIT_B;	//0.4
	limit_k=limit_k>1?1:limit_k;
	limit_k=limit_k<0.1f?0.1f:limit_k;
	
	if(Error_Check.statu[LOST_REFEREE]==1)	//����lost
	{
		limit_power_statu=3;
		limit_k=0.6;
	}
	
	return limit_k;
}


#define CHASSIS_SPEEDMAX 8500.0f
void Overall_Motion_Ratio_Protect(CHASSIS_DATA* chassis_data)	//�������ٱ�������,һ��Ҫ�����������ٽ��������
{
	s32 chassis_tarV_max=0;
	float chassis_protect_k=1;
	
	chassis_tarV_max=chassis_data->lf_wheel_tarV>chassis_data->rf_wheel_tarV?chassis_data->lf_wheel_tarV:chassis_data->rf_wheel_tarV;
	chassis_tarV_max=chassis_tarV_max>chassis_data->lb_wheel_tarV?chassis_tarV_max:chassis_data->lb_wheel_tarV;
	chassis_tarV_max=chassis_tarV_max>chassis_data->rb_wheel_tarV?chassis_tarV_max:chassis_data->rb_wheel_tarV;
	
	if(chassis_tarV_max>CHASSIS_SPEEDMAX)	//���������ϵ��Ϊ�˼��ټ���������С�������IF��
	{
		chassis_protect_k=CHASSIS_SPEEDMAX/chassis_tarV_max;
		if(chassis_protect_k>1)	chassis_protect_k=1;
		if(chassis_protect_k<0.2f)	chassis_protect_k=0.2f;	//����
		
		chassis_data->lf_wheel_tarV*=chassis_protect_k;
		chassis_data->rf_wheel_tarV*=chassis_protect_k;
		chassis_data->lb_wheel_tarV*=chassis_protect_k;
		chassis_data->rb_wheel_tarV*=chassis_protect_k;
	}
	
}



#define CHASSIS_INTEGRAL_PID_KP 3
#define CHASSIS_INTEGRAL_PID_KI 0.01f
#define CHASSIS_INTEGRAL_PID_I_SUM_LIM 1000
void Extended_Integral_PID(CHASSIS_DATA* chassis_data)	//��չ������PID�����������⶯������	2018.4.19
{
	float tarv_sum=abs(chassis_data->lf_wheel_tarV)+abs(chassis_data->rf_wheel_tarV)+abs(chassis_data->lb_wheel_tarV)+abs(chassis_data->rb_wheel_tarV);
	float fdbv_sum=abs(chassis_data->lf_wheel_fdbV)+abs(chassis_data->rf_wheel_fdbV)+abs(chassis_data->lb_wheel_fdbV)+abs(chassis_data->rb_wheel_fdbV);
	float expect[4]={0};
	float error[4]={0};
	static float inte[4];
	s32 output_compensation[4];
	
	if(abs(tarv_sum)<0.1f)	//�൱�ڱ�����Ϊ0
	{
		expect[LF]=0;
		expect[RF]=0;
		expect[LB]=0;
		expect[RB]=0;
	}
	else
	{
		expect[LF]=fdbv_sum*chassis_data->lf_wheel_tarV/tarv_sum;
		expect[RF]=fdbv_sum*chassis_data->rf_wheel_tarV/tarv_sum;
		expect[LB]=fdbv_sum*chassis_data->lb_wheel_tarV/tarv_sum;
		expect[RB]=fdbv_sum*chassis_data->rb_wheel_tarV/tarv_sum;
	}
	
	error[LF]=expect[LF]-chassis_data->lf_wheel_fdbV;
	error[RF]=expect[RF]-chassis_data->rf_wheel_fdbV;
	error[LB]=expect[LB]-chassis_data->lb_wheel_fdbV;
	error[RB]=expect[RB]-chassis_data->rb_wheel_fdbV;
	
	inte[LF]+=error[LF]*CHASSIS_INTEGRAL_PID_KI;
	inte[LF]+=error[LF]*CHASSIS_INTEGRAL_PID_KI;
	inte[LF]+=error[LF]*CHASSIS_INTEGRAL_PID_KI;
	inte[LF]+=error[LF]*CHASSIS_INTEGRAL_PID_KI;
	
	for(int id=0;id<4;id++)
	{
		inte[id]=inte[id]>CHASSIS_INTEGRAL_PID_I_SUM_LIM?CHASSIS_INTEGRAL_PID_I_SUM_LIM:inte[id];
		inte[id]=inte[id]<-CHASSIS_INTEGRAL_PID_I_SUM_LIM?-CHASSIS_INTEGRAL_PID_I_SUM_LIM:inte[id];
	}
	
	output_compensation[LF]=(s32)(error[LF]*CHASSIS_INTEGRAL_PID_KP+inte[LF]);
	output_compensation[RF]=(s32)(error[RF]*CHASSIS_INTEGRAL_PID_KP+inte[RF]);
	output_compensation[LB]=(s32)(error[LB]*CHASSIS_INTEGRAL_PID_KP+inte[LB]);
	output_compensation[RB]=(s32)(error[RB]*CHASSIS_INTEGRAL_PID_KP+inte[RB]);
	
	for(int id=0;id<4;id++)
	{
		output_compensation[id]=output_compensation[id]>4000?4000:output_compensation[id];
		output_compensation[id]=output_compensation[id]<-4000?-4000:output_compensation[id];
	}
	
	chassis_data->lf_wheel_output+=output_compensation[LF];
	chassis_data->rf_wheel_output+=output_compensation[RF];
	chassis_data->lb_wheel_output+=output_compensation[LB];
	chassis_data->rb_wheel_output+=output_compensation[RB];
}

