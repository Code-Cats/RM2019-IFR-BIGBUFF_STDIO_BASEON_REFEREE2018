#include "control.h"
#include "bigbuff_6623.h"
#include "hit_recognition.h"

#define PITCH 0
#define ROLL 1
#define YAW 2

extern u8 IMU_Check_Useless_State;	//陀螺仪失效检测位

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
		SK6812_Run();	//全彩RGB运行函数
	}
	
	Check_Task();
	
//	Work_State_Change();	//工作状态切换
//	
//	Work_State_Change_BackProtect();	//工作状态切换保护
//	
//	Work_Execute();	//工作状态执行
	
	
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
RC或PC对机器状态的切换
*************************************/
void Work_State_Change(void)
{
	static u8 Switch_Right_Last=0;
	static WorkState_e State_Record=CHECK_STATE;	
	State_Record=GetWorkState();
	
	
	switch (GetWorkState())	//2018.3.15
	{
		case CHECK_STATE:	//自检模式
		{	//板载外设初始化后便进入自检模式 
			
			break;
		}
		case PREPARE_STATE:	//预备模式
		{	
			
			break;
		}
		case CALI_STATE:	//标定模式
		{
			
			break;
		}
		case NORMAL_STATE:	//正常操作模式
		{
			
			break;
		}
		case WAIST_STATE:
		{
			
			break;
		}
		case TAKEBULLET_STATE:	//取弹模式
		{
			
			break;
		}
		case ERROR_STATE:	//错误模式
		{
			
			break;
		}
		case LOST_STATE:	//错误模式
		{
			SetWorkState(CHECK_STATE);
			time_1ms_count=0;	//进入初始状态重新自检
			break;
		}
		case STOP_STATE:	//停止状态
		{
			
			break;
		}
		case PROTECT_STATE:	//自我保护模式
		{
			
			break;
		}
	}
	//Switch_Right_Last=RC_Ctl.rc.switch_right;
}



void Work_State_Change_BackProtect(void)	//当从某一状态退出时，确保该状态的一切遗留控制都归位	//新版取弹不需要，因为改写了结构，一切动作在自动取弹中都会顺延完成
{
	static WorkState_e State_Record=CHECK_STATE;
	
	if(State_Record==TAKEBULLET_STATE&&GetWorkState()!=TAKEBULLET_STATE)	//退出取弹模式
	{

	}
	
	if(State_Record!=TAKEBULLET_STATE&&GetWorkState()==TAKEBULLET_STATE)
	{

	}
	State_Record=GetWorkState();
}


void Work_Execute(void)	//工作执行2018.7.1
{
	switch (GetWorkState())	//2018.3.15	执行任务块
	{
		case CHECK_STATE:	//自检模式
		{	//板载外设初始化后便进入自检模式 //此时外设刚刚开启，需等待一段时间全局自检未检测到异常（2-3个自检触发周期以上），又因为时间计算起点为定时器启动点，故无需进行时间差记录

			break;
		}
		case PREPARE_STATE:	//预备模式
		{	//等待车身状态稳定，并设置初值

			break;
		}
		case CALI_STATE:	//标定模式
		{

			break;
		}
		case NORMAL_STATE:	//正常操作模式
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
		case ERROR_STATE:	//错误模式
		{
			
			break;
		}
		case LOST_STATE:	//错误模式
		{
			break;
		}
		case STOP_STATE:	//停止状态
		{

			break;
		}
		case PROTECT_STATE:	//自我保护模式
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
			case CHECK_STATE:	//自检模式
			{	//板载外设初始化后便进入自检模式 
				LED_Blink_Set(10,10);
				break;
			}
			case PREPARE_STATE:	//预备模式
			{	
				LED_Blink_Set(9,9);
				break;
			}
			case CALI_STATE:	//标定模式	红开绿闪
			{
				LED_Blink_Set(9,9);
				break;
			}
			case NORMAL_STATE:	//正常操作模式	红关绿闪
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
			case ERROR_STATE:	//错误模式
			{
				if(t_error_record==LOST_SM_DOWN)	//下拨弹
				{
					LED_Blink_Set(3,10);
				}
				else if(t_error_record==LOST_SM_UP)	//上拨弹
				{
					LED_Blink_Set(4,10);
				}
				else if(t_error_record==LOST_CM1||t_error_record==LOST_CM2||t_error_record==LOST_CM3||t_error_record==LOST_CM4)	//底盘电机
				{
					LED_Blink_Set(2,10);
				}
				else if(t_error_record==LOST_PITCH)	//云台||t_error_record==LOST_YAW
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
			case LOST_STATE:	//错误模式
			{
				LED_Blink_Set(1,1);
				break;
			}
			case STOP_STATE:	//停止状态	红闪
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
			case PROTECT_STATE:	//自我保护模式	双闪
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
		case CHECK_STATE:	//自检模式
		{	//板载外设初始化后便进入自检模式 //此时外设刚刚开启，需等待一段时间全局自检未检测到异常（2-3个自检触发周期以上），又因为时间计算起点为定时器启动点，故无需进行时间差记录
			CAN1_Yun_SendMsg(0,0);	//CAN2	//yaw,pitch
			CAN2_Chassis_SendMsg(0,0,0,0);
			CAN2_Shoot_Bullet_SendMsg(0,0,0,0);//取弹旋转、0、下拨弹、上拨弹
//			CAN2_Shoot_SendMsg((s16)shoot_Motor_Data_Down.output,0);	//下拨弹、上拨弹
			break;
		}
		case PREPARE_STATE:	//预备模式
		{	//等待车身状态稳定，并设置初值
			CAN1_Yun_SendMsg(0,0);	//CAN2	//yaw,pitch
////////			CAN1_Yun_SendMsg(yunMotorData.yaw_output,yunMotorData.pitch_output);	//CAN2-1000	//取消反馈补偿
//CAN1_Yun_SendMsg(0,0);	//CAN2-1000	//取消反馈补偿
			//			CAN1_Yun_SendMsg(t_yaw_send,t_pitch_send);	//调试用模式
//			CAN_Motor6623_calibration();
			//CAN1_Yun_SendMsg(yunMotorData.yaw_output+Yaw_output_offset(yunMotorData.yaw_fdbP),yunMotorData.pitch_output+Pitch_output_offset(yunMotorData.pitch_tarP));	//CAN2-1000
			CAN2_Chassis_SendMsg(0,0,0,0);
			//CAN2_Shoot_SendMsg(0,0);//下拨弹、上拨弹
			CAN2_Shoot_Bullet_SendMsg(0,0,0,0);//取弹旋转、0、下拨弹、上拨弹
			break;
		}
		case CALI_STATE:	//标定模式
		{


//			CAN2_Shoot_Bullet_SendMsg(0,0,0,0);//取弹旋转、0、下拨弹、上拨弹
			break;
		}
		case NORMAL_STATE:	//正常操作模式
		{

			break;
		}
		case WAIST_STATE:
		{

			break;
		}
		case ERROR_STATE:	//错误模式
		{

			break;
		}
		case STOP_STATE:	//停止状态
		{
			CAN1_Yun_SendMsg(0,0);	//CAN2	//yaw,pitch
			CAN2_Chassis_SendMsg(0,0,0,0);
			CAN2_Shoot_Bullet_SendMsg(0,0,0,0);//取弹旋转、0、下拨弹、上拨弹
			break;
		}
		case PROTECT_STATE:	//自我保护模式
		{
			CAN1_Yun_SendMsg(0,0);	//CAN2	//yaw,pitch
			CAN2_Chassis_SendMsg(0,0,0,0);
			CAN2_Shoot_Bullet_SendMsg(0,0,0,0);//取弹旋转、0、下拨弹、上拨弹
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
			CAN2_Shoot_Bullet_SendMsg(0,0,0,0);//取弹旋转、0、下拨弹、上拨弹
			break;
		}
	}
	
}




////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

/***********************--工作状态--**********************/
void SetWorkState(WorkState_e state)
{
    workState = state;
}


WorkState_e GetWorkState()
{
	return workState;
}


