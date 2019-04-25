#include "bigbuff_6623.h"
#include "pid.h"
#include "gpio.h"

extern YUN_MOTOR_DATA	yunMotorData;	//云台挂载在CAN1上，因为CAN2预留了6pin接口，云台不需要该接口，为不浪费，故接CAN1

PID_GENERAL          PID_BIGBUFF_SPEED=PID_BIGBUFF_SPEED_DEFAULT;

s16 test_send=0;
float fdb_v_firstordor=0;
u8 key_now=0;
s8 bigbuff_roaite=0;
u32 bigbuff_roaite_count=0;
void Bigbuff_Task(void)
{
	static u8 key_last=0;
	fdb_v_firstordor=fdb_v_firstordor*0.7f+yunMotorData.yaw_fdbV*0.3f;
	key_now=KEY_PB0;
	if(key_now==1&&key_last==0)
	{
		if(bigbuff_roaite==0)
		{
			bigbuff_roaite=1;
		}
		else if(bigbuff_roaite==1||bigbuff_roaite==-1)
		{
			bigbuff_roaite=0;
		}
		else
		{
			bigbuff_roaite=0;
		}
	}
	key_last=key_now;
	
	if(bigbuff_roaite==1||bigbuff_roaite==-1)
	{
		
		bigbuff_roaite_count++;
		if(bigbuff_roaite_count>=1000*60)
		{
			bigbuff_roaite*=-1;
			bigbuff_roaite_count=0;
		}
		
	}
	else
	{
		bigbuff_roaite_count=0;
	}
	
	yunMotorData.yaw_tarV=bigbuff_roaite*30;
	yunMotorData.yaw_output=PID_General(yunMotorData.yaw_tarV,fdb_v_firstordor,&PID_BIGBUFF_SPEED);	//采用外界陀螺仪做反馈
	
	CAN1_Yun_SendMsg((s16)yunMotorData.yaw_output,0);
}




