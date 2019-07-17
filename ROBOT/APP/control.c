#include "control.h"
#include "bigbuff_6623.h"
#include "hit_recognition.h"

#define PITCH 0
#define ROLL 1
#define YAW 2

extern u8 IMU_Check_Useless_State;	//������ʧЧ���λ

WorkState_e workState=PREPARE_STATE;


//extern KeyBoardTypeDef KeyBoardData[KEY_NUMS];
//extern RC_Ctl_t RC_Ctl;

extern YUN_MOTOR_DATA 	yunMotorData;

//extern s16 Chassis_Vx;
//extern s16 Chassis_Vy;
//extern s16 Chassis_Vw;


u32 time_1ms_count=0;

 u32 x=0;
void Control_Task(void)	//2ms
{
	time_1ms_count++;
	
	LED_Blink_Set(2,6);
	
	if(time_1ms_count%5==0&&time_1ms_count>1000)
	{
		SK6812_Run();	//ȫ��RGB���к���
	}
	
	Check_Task();
	
//	Work_State_Change();	//����״̬�л�
//	
//	Work_State_Change_BackProtect();	//����״̬�л�����
//	
//	Work_Execute();	//����״ִ̬��
	
	
	//LED_Indicate();
	
	if(time_1ms_count%BLINK_CYCLE==0)
	{
		LED_Blink_Run();
	}
	
	if(time_1ms_count>5000&&time_1ms_count%2000==0)
	{
		CAN_HeartBeat_SendMsg();
	}
	
	if(time_1ms_count>=20000)
				 {
					 if(time_1ms_count%2000==0)	//20s
					 {
						 //CAN_SetALLBlueTurnOFF_SendMsg();
						 x=1;
						 //CAN_SetAllRed2Blue_SendMsg();
					 }
//					 if((time_1ms_count+10000)%20000==0)
//					 {
//						 CAN_SetID3TurnON_SendMsg();
//						 x=2;
//					 }
				 }
	if(time_1ms_count%2==0)
	{
		Bigbuff_Task();
	}
	
	//Motor_Send();
	
}

/*************************************
RC��PC�Ի���״̬���л�
*************************************/
void Work_State_Change(void)
{
	static u8 Switch_Right_Last=0;
	static WorkState_e State_Record=CHECK_STATE;	
	State_Record=GetWorkState();
	
	
	switch (GetWorkState())	//2018.3.15
	{
		case CHECK_STATE:	//�Լ�ģʽ
		{	//���������ʼ���������Լ�ģʽ 
			
			break;
		}
		case PREPARE_STATE:	//Ԥ��ģʽ
		{	
			
			break;
		}
		case CALI_STATE:	//�궨ģʽ
		{
			
			break;
		}
		case NORMAL_STATE:	//��������ģʽ
		{
			
			break;
		}
		case WAIST_STATE:
		{
			
			break;
		}
		case TAKEBULLET_STATE:	//ȡ��ģʽ
		{
			
			break;
		}
		case ERROR_STATE:	//����ģʽ
		{
			
			break;
		}
		case LOST_STATE:	//����ģʽ
		{
			SetWorkState(CHECK_STATE);
			time_1ms_count=0;	//�����ʼ״̬�����Լ�
			break;
		}
		case STOP_STATE:	//ֹͣ״̬
		{
			
			break;
		}
		case PROTECT_STATE:	//���ұ���ģʽ
		{
			
			break;
		}
	}
	//Switch_Right_Last=RC_Ctl.rc.switch_right;
}



void Work_State_Change_BackProtect(void)	//����ĳһ״̬�˳�ʱ��ȷ����״̬��һ���������ƶ���λ	//�°�ȡ������Ҫ����Ϊ��д�˽ṹ��һ�ж������Զ�ȡ���ж���˳�����
{
	static WorkState_e State_Record=CHECK_STATE;
	
	if(State_Record==TAKEBULLET_STATE&&GetWorkState()!=TAKEBULLET_STATE)	//�˳�ȡ��ģʽ
	{

	}
	
	if(State_Record!=TAKEBULLET_STATE&&GetWorkState()==TAKEBULLET_STATE)
	{

	}
	State_Record=GetWorkState();
}


void Work_Execute(void)	//����ִ��2018.7.1
{
	switch (GetWorkState())	//2018.3.15	ִ�������
	{
		case CHECK_STATE:	//�Լ�ģʽ
		{	//���������ʼ���������Լ�ģʽ //��ʱ����ոտ�������ȴ�һ��ʱ��ȫ���Լ�δ��⵽�쳣��2-3���Լ촥���������ϣ�������Ϊʱ��������Ϊ��ʱ�������㣬���������ʱ����¼

			break;
		}
		case PREPARE_STATE:	//Ԥ��ģʽ
		{	//�ȴ�����״̬�ȶ��������ó�ֵ

			break;
		}
		case CALI_STATE:	//�궨ģʽ
		{

			break;
		}
		case NORMAL_STATE:	//��������ģʽ
		{

			break;
		}
		case WAIST_STATE:
		{

			break;
		}
		case TAKEBULLET_STATE:
		{

			break;
		}
		case ERROR_STATE:	//����ģʽ
		{
			
			break;
		}
		case LOST_STATE:	//����ģʽ
		{
			break;
		}
		case STOP_STATE:	//ֹͣ״̬
		{

			break;
		}
		case PROTECT_STATE:	//���ұ���ģʽ
		{
			break;
		}
	}
}





extern s16 t_error_record;
void LED_Indicate(void)
{
	if(time_1ms_count%BLINK_CYCLE==0)
	{
		switch (GetWorkState())	//2018.3.15
		{
			case CHECK_STATE:	//�Լ�ģʽ
			{	//���������ʼ���������Լ�ģʽ 
				LED_Blink_Set(10,10);
				break;
			}
			case PREPARE_STATE:	//Ԥ��ģʽ
			{	
				LED_Blink_Set(9,9);
				break;
			}
			case CALI_STATE:	//�궨ģʽ	�쿪����
			{
				LED_Blink_Set(9,9);
				break;
			}
			case NORMAL_STATE:	//��������ģʽ	�������
			{
				LED_Blink_Set(1,0);
				break;
			}
			case WAIST_STATE:
			{
				LED_Blink_Set(1,0);
				break;
			}
			case TAKEBULLET_STATE:
			{
				LED_Blink_Set(1,0);
				break;
			}
			case ERROR_STATE:	//����ģʽ
			{
				if(t_error_record==LOST_SM_DOWN)	//�²���
				{
					LED_Blink_Set(3,10);
				}
				else if(t_error_record==LOST_SM_UP)	//�ϲ���
				{
					LED_Blink_Set(4,10);
				}
				else if(t_error_record==LOST_CM1||t_error_record==LOST_CM2||t_error_record==LOST_CM3||t_error_record==LOST_CM4)	//���̵��
				{
					LED_Blink_Set(2,10);
				}
				else if(t_error_record==LOST_PITCH)	//��̨||t_error_record==LOST_YAW
				{
					LED_Blink_Set(5,10);
				}
				else if(t_error_record==LOST_YAW)	//LOST_BULLETROTATE1
				{
					LED_Blink_Set(1,10);
				}
				else if(t_error_record==LOST_IMU)
				{
					LED_Blink_Set(6,10);
				}
				break;
			}
			case LOST_STATE:	//����ģʽ
			{
				LED_Blink_Set(1,1);
				break;
			}
			case STOP_STATE:	//ֹͣ״̬	����
			{
				if(time_1ms_count%BLINK_INTERVAL==0)
				{
					LED_Blink_Set(0,10);
				}
				else if((time_1ms_count+BLINK_INTERVAL/2)%BLINK_INTERVAL==0)
				{
					LED_Blink_Set(10,0);
				}
				
				break;
			}
			case PROTECT_STATE:	//���ұ���ģʽ	˫��
			{
				LED_Blink_Set(1,1);
				break;
			}
		}
LED_Blink_Run();
	}
}


s16 t_yaw_send,t_pitch_send=0;

void Motor_Send(void)
{
	switch (GetWorkState())	//2018.3.15
	{	
		case CHECK_STATE:	//�Լ�ģʽ
		{	//���������ʼ���������Լ�ģʽ //��ʱ����ոտ�������ȴ�һ��ʱ��ȫ���Լ�δ��⵽�쳣��2-3���Լ촥���������ϣ�������Ϊʱ��������Ϊ��ʱ�������㣬���������ʱ����¼
			CAN1_Yun_SendMsg(0,0);	//CAN2	//yaw,pitch
			CAN2_Chassis_SendMsg(0,0,0,0);
			CAN2_Shoot_Bullet_SendMsg(0,0,0,0);//ȡ����ת��0���²������ϲ���
//			CAN2_Shoot_SendMsg((s16)shoot_Motor_Data_Down.output,0);	//�²������ϲ���
			break;
		}
		case PREPARE_STATE:	//Ԥ��ģʽ
		{	//�ȴ�����״̬�ȶ��������ó�ֵ
			CAN1_Yun_SendMsg(0,0);	//CAN2	//yaw,pitch
////////			CAN1_Yun_SendMsg(yunMotorData.yaw_output,yunMotorData.pitch_output);	//CAN2-1000	//ȡ����������
//CAN1_Yun_SendMsg(0,0);	//CAN2-1000	//ȡ����������
			//			CAN1_Yun_SendMsg(t_yaw_send,t_pitch_send);	//������ģʽ
//			CAN_Motor6623_calibration();
			//CAN1_Yun_SendMsg(yunMotorData.yaw_output+Yaw_output_offset(yunMotorData.yaw_fdbP),yunMotorData.pitch_output+Pitch_output_offset(yunMotorData.pitch_tarP));	//CAN2-1000
			CAN2_Chassis_SendMsg(0,0,0,0);
			//CAN2_Shoot_SendMsg(0,0);//�²������ϲ���
			CAN2_Shoot_Bullet_SendMsg(0,0,0,0);//ȡ����ת��0���²������ϲ���
			break;
		}
		case CALI_STATE:	//�궨ģʽ
		{


//			CAN2_Shoot_Bullet_SendMsg(0,0,0,0);//ȡ����ת��0���²������ϲ���
			break;
		}
		case NORMAL_STATE:	//��������ģʽ
		{

			break;
		}
		case WAIST_STATE:
		{

			break;
		}
		case ERROR_STATE:	//����ģʽ
		{

			break;
		}
		case STOP_STATE:	//ֹͣ״̬
		{
			CAN1_Yun_SendMsg(0,0);	//CAN2	//yaw,pitch
			CAN2_Chassis_SendMsg(0,0,0,0);
			CAN2_Shoot_Bullet_SendMsg(0,0,0,0);//ȡ����ת��0���²������ϲ���
			break;
		}
		case PROTECT_STATE:	//���ұ���ģʽ
		{
			CAN1_Yun_SendMsg(0,0);	//CAN2	//yaw,pitch
			CAN2_Chassis_SendMsg(0,0,0,0);
			CAN2_Shoot_Bullet_SendMsg(0,0,0,0);//ȡ����ת��0���²������ϲ���
			break;
		}
		case TAKEBULLET_STATE:
		{

			break;
		}
		default:
		{
			CAN1_Yun_SendMsg(0,0);	//CAN2	//yaw,pitch
			CAN2_Chassis_SendMsg(0,0,0,0);
			CAN2_Shoot_Bullet_SendMsg(0,0,0,0);//ȡ����ת��0���²������ϲ���
			break;
		}
	}
	
}




////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

/***********************--����״̬--**********************/
void SetWorkState(WorkState_e state)
{
    workState = state;
}


WorkState_e GetWorkState()
{
	return workState;
}


