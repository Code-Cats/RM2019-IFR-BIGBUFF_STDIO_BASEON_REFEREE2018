#include "main.h"
#include "math.h"
#include "hit_recognition.h"

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
	
}

