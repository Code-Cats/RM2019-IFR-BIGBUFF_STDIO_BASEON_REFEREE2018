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
function:��ʱ�ص�������ÿ1ms����һ��
@param:void
@return:void
description:�����洦��ʱ����ע�⴦������ʱ�䲻�ܴ���1ms������ʹ��Delay�����������ж�
****************************************************/
void Timer_1ms_CallBack(void)
{
	//ϵͳȫ��ʱ��time_1ms_count �����ԼӴ��������ټ�
	BigBuff_Control_Tack();
}

