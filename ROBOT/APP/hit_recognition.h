#ifndef __HIT_RECOGNITION_H
#define __HIT_RECOGNITION_H

#include "bsp.h"

typedef enum
{
	AIMORID_240,
	AIMORID_241,
	AIMORID_242,
	AIMORID_243,
	AIMORID_244,
	AIMORID_NUMS
}AimorIDEnum;

typedef enum
{
	HIT_OTHER,
	HIT_BULLET17 ,
	HIT_BULLET42,
}AimorHitTypeEnum;

typedef enum
{
	OFF_LINE,
	ON_WAIT_SET,
	ON_LINE,
}AimorStateEnum;

typedef struct
{
	AimorStateEnum state;
	u16 numOfhits;
	void(*hitcallback)(AimorIDEnum,AimorHitTypeEnum);
}AimorDataTypeDef;

extern AimorDataTypeDef AimorData[AIMORID_NUMS];

void CAN_RefereeStart_SendMsg(void);

void CAN_HeartBeat_SendMsg(void);

void CAN_SetID0TurnON_SendMsg(void);	//开机可亮 关灯后不可亮
void CAN_SetID1TurnON_SendMsg(void);	//开机可亮 关灯后不可亮
void CAN_SetID2TurnON_SendMsg(void);	//开机可亮 关灯后不可亮
void CAN_SetID3TurnON_SendMsg(void);
void CAN_SetIDxTurnON_Task(AimorIDEnum id);
void ALLAutoTurnON(void);

void CAN_TurnONTEST_SendMsg(void);	//测试0x350包的作用

void CAN_SetAllRed2Blue_SendMsg(void);

void CAN_SetALLRedTurnOFF_SendMsg(void);
void CAN_SetALLBlueTurnOFF_SendMsg(void);

void CAN1_Hit_Analysis(CanRxMsg *rx_message);

void ArmorHit_CallBack(AimorIDEnum id,AimorHitTypeEnum type);
#endif
