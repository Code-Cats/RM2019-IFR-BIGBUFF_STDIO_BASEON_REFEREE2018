#include "main.h"
#include "math.h"
#include "hit_recognition.h"
#include "control_bigbuff.h"

extern u32 time_1ms_count;

u32 while_time_record=0;
int main(void)
{
	SetWorkState(CHECK_STATE);
//	Data_Init();
	delay_ms(500);
	BSP_Init();
	delay_ms(500);
	CAN_RefereeStart_SendMsg();
	while(1)
	 {
		 //PLATE_LED_ALL_ON;
		 //delay_ms(1);
		 ALLAutoTurnON();
		 if(time_1ms_count-while_time_record>=400)
		 {
			 CAN_HeartBeat_SendMsg();
			 while_time_record=time_1ms_count;
		 }
	 }
}

/****************************************************
name:ArmorHit_CallBack
function:装甲板被击打时产生的回调
@param:id:装甲板ID最低字节
@param:type:受打击的类型枚举：   HIT_OTHER,//撞击伤害
								HIT_BULLET17,//17mm伤害
								HIT_BULLET42,//42mm伤害
@return:void
description:击打回调函数，其中处理击打数据
****************************************************/
void ArmorHit_CallBack(AimorIDEnum id,AimorHitTypeEnum type)
{
	extern u8 viceboard_num;
	extern u8 ready_flag;
	extern u8 alreadly_count;
	
	if((id+1) == viceboard_num)
	{
		switch(viceboard_num)
		{
			case 1:PLATE1_LED2_ON; break;
			case 2:PLATE2_LED2_ON; break;
			case 3:PLATE3_LED2_ON; break;
			case 4:PLATE4_LED2_ON; break;
			case 5:PLATE5_LED2_ON; break;
			default: break;
		}
		
		alreadly_count++;
		
		ready_flag = 1;
	}
}

/****************************************************
name:Timer_1ms_CallBack
function:定时回调函数，每1ms调用一次
@param:void
@return:void
description:在里面处理定时任务，注意处理任务时间不能大于1ms，不能使用Delay，否侧进错误中断
****************************************************/
void Timer_1ms_CallBack(void)
{
	//系统全局时间time_1ms_count 已做自加处理，无需再加
	BigBuff_Control_Tack();
}

