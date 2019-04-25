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
function:װ�װ屻����ʱ�����Ļص�
@param:id:װ�װ�ID����ֽ�
@param:type:�ܴ��������ö�٣�   HIT_OTHER,//ײ���˺�
								HIT_BULLET17,//17mm�˺�
								HIT_BULLET42,//42mm�˺�
@return:void
description:����ص����������д����������
****************************************************/
void ArmorHit_CallBack(AimorIDEnum id,AimorHitTypeEnum type)
{
	
}

/****************************************************
name:Timer_1ms_CallBack
function:��ʱ�ص�������ÿ1ms����һ��
@param:void
@return:void
description:�����洦��ʱ����ע�⴦������ʱ�䲻�ܴ���1ms������ʹ��Delay�����������ж�
****************************************************/
void Timer_1ms_CallBack(void)
{
	//ϵͳȫ��ʱ��time_1ms_count �����ԼӴ��������ټ�
	
}

