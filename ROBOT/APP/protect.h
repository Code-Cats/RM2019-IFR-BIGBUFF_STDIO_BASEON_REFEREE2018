#ifndef __PROTECT_H
#define __PROTECT_H
#include "stm32f4xx.h"

enum LOST_TYPES
{
	LOST_IMU,
	LOST_DBUS,
	LOST_VICEBOARD,
	LOST_REFEREE,
	LOST_VISION,
	LOST_CM1,
	LOST_CM2,
	LOST_CM3,
	LOST_CM4,
	LOST_YAW,
	LOST_PITCH,
	LOST_BULLETROTATE1,	//ȡ��һ�ŵ����CAN1 ID201	��
	LOST_BULLETROTATE2,	//ȡ�����ŵ����CAN1 ID202	��
	LOST_SM_DOWN,
	LOST_SM_UP,
	LOST_TYPE_NUM,
};


typedef struct
{
	const u16 cycle[LOST_TYPE_NUM];
	u16 count[LOST_TYPE_NUM];
	u8 statu[LOST_TYPE_NUM];
}Error_check_t;

extern Error_check_t Error_Check;

#define LOST_CYCLE \
{\
	6,\
	17,\
	4,\
	10,\
	10,\
	4,\
	4,\
	4,\
	4,\
	4,\
	4,\
	4,\
	4,\
	4,\
	4,\
}

void LostCountAdd(u16* lostcount);
void LostCountFeed(u16* lostcoun);
	u8 LostCountCheck(u16 lostcoun,u8* statu,const u16 cycle);

void Check_Task(void);	//control.c
void IMU_Check_Useless(void);	//�����Ǽ��ʧЧ

extern Error_check_t Error_Check;
extern u8 IMU_Check_Useless_State;

#endif
