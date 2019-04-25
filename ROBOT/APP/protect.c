#include "protect.h"
#include "control.h"
/*
���ļ���;���ṩ�����������㷨���������л����Լ�����ʵʱ״̬��⣬����״̬�л�
Ԥ���幦�ܣ�
1.��̨��̬��������̬����Դ�����㷨
2.�����������⼰��Ч��ֹ
3.�����ģ�����߼����
4.�����������
5.����...
����һ����¼֡�ʡ����ڵ����㣿
*/

#define LOST_THRESHOLD 10

Error_check_t Error_Check={LOST_CYCLE,{0},{0}};

u8 Error_check_workstate=1;

s16 t_error_record=0;
void LostCountAdd(u16* lostcount)	//����ı�
{
	if(*lostcount<0xFFFE)
	(*lostcount)++;
}

void LostCountFeed(u16* lostcount)	//����ı�
{
	*lostcount=0;
}

u8 LostCountCheck(u16 lostcount,u8* statu,const u16 cycle)	//����ı�
{
	if(lostcount>LOST_THRESHOLD*cycle)
		*statu=1;
	else
		*statu=0;
	return *statu;
}

s32 test_error_Satrt=0;
void Check_Task(void)
{
	for(int i=0;i<LOST_TYPE_NUM;i++)	//����ı�
	{
		LostCountAdd(&Error_Check.count[i]);
		LostCountCheck(Error_Check.count[i],&Error_Check.statu[i],Error_Check.cycle[i]);
	}
	
	
	if(Error_check_workstate==1)	//����״̬
	{
		if(Error_Check.statu[LOST_IMU]==1)
		{
			test_error_Satrt=1;
			t_error_record=LOST_IMU;
			SetWorkState(ERROR_STATE);
		}
//		
		for(int i=5;i<LOST_TYPE_NUM-1;i++)	//����ȿظ���Ҫ
		{
			if(Error_Check.statu[i]==1)	//&&i!=LOST_BULLETROTATE1
			{
				test_error_Satrt=-1;
				t_error_record=i;
				SetWorkState(ERROR_STATE);
			}
				
		}

	}
	
	
	if(Error_Check.statu[LOST_DBUS]==1)
	{
		if(GetWorkState()==CHECK_STATE||GetWorkState()==PREPARE_STATE||GetWorkState()==CALI_STATE)
		{
			SetWorkState(LOST_STATE);	//����ʱû��ң���źŵ�ѡ��
		}
		else
		{
			SetWorkState(PROTECT_STATE);
		}
		
	}

}




