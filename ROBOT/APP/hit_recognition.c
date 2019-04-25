#include "hit_recognition.h"

#define AIMORDATA_DEFAULT \
{\
	OFF_LINE,\
	0,\
	ArmorHit_CallBack,\
}

AimorDataTypeDef AimorData[5]={AIMORDATA_DEFAULT, AIMORDATA_DEFAULT, AIMORDATA_DEFAULT, AIMORDATA_DEFAULT, AIMORDATA_DEFAULT};
/****************************************************
name:CAN_GetTxMailboxesFreeLevel
function:查询是否有空闲邮箱
@param:CANx:CAN1 or CAN2
@return:1:free 0:buzy
description:
****************************************************/
u8 CAN_GetTxMailboxesFreeLevel(CAN_TypeDef* CANx)
{
//	uint8_t transmit_mailbox = 0;
	/* Check the parameters */
	assert_param(IS_CAN_ALL_PERIPH(CANx));
	/* Select one empty transmit mailbox */
	if ((CANx->TSR&CAN_TSR_TME0) == CAN_TSR_TME0)
	{
		//transmit_mailbox = 0;
		return 1;
	}
	else if ((CANx->TSR&CAN_TSR_TME1) == CAN_TSR_TME1)
	{
		//transmit_mailbox = 1;
		return 1;
	}
	else if ((CANx->TSR&CAN_TSR_TME2) == CAN_TSR_TME2)
	{
		//transmit_mailbox = 2;
		return 1;
	}
	else
	{
		//transmit_mailbox = CAN_TxStatus_NoMailBox;	//CAN_NO_MB
		return 0;
	}
}

/****************************************************
name:CAN_SendNormalMsg
function:CAN发送通用函数
@param:CANx:CAN1 or CAN2
@param:StdId:CAN StdID
@param:pdata:SendData Point
@param:length:SendData Length
@return:void
description:
****************************************************/
void CAN_SendNormalMsg(CAN_TypeDef* CANx, uint32_t StdId, u8* pdata, u8 length)
{
	CanTxMsg TxMessage;
	TxMessage.StdId = StdId;      //帧ID为传入参数的CAN_ID
    TxMessage.IDE = CAN_ID_STD;    //标准帧
    TxMessage.RTR = CAN_RTR_DATA;  //数据帧
    TxMessage.DLC = 0x08;          //帧长度初始化为8
	
	for(u8 Txcount=0;Txcount<length;)
	{
		u8 remain=length-Txcount;
		if(remain>=8)
		{
			TxMessage.DLC = 0x08;
			
		}
		else
		{
			TxMessage.DLC = remain;
		}
		
		for(int i=0;i<TxMessage.DLC;i++)
		{
			TxMessage.Data[i]=pdata[i+Txcount];
		}
		
		while(CAN_GetTxMailboxesFreeLevel(CANx)==0);
		CAN_Transmit(CANx,&TxMessage);
		
		Txcount+=TxMessage.DLC;
		
	}
}

/****************************************************
name:CAN_RefereeStart_SendMsg
function:发送裁判启动数据
@param:void
@return:void
description:将数据存入TxMessage结构体再由CAN_Transmit发送
****************************************************/
u8 RefereeStartData1[8]={0x5a, 0x00, 0x00, 0x33, 0x00, 0x01, 0x01, 0xff};
u8 RefereeStartData2[8]={0xff, 0x01, 0x00, 0x00, 0x15, 0x12, 0xfc, 0xea};
void CAN_RefereeStart_SendMsg(void)
{
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x0fe;      //帧ID为传入参数的CAN_ID
    TxMessage.IDE = CAN_ID_STD;    //标准帧
    TxMessage.RTR = CAN_RTR_DATA;  //数据帧
    TxMessage.DLC = 0x08;          //帧长度为8
    
	for(int i=0;i<TxMessage.DLC;i++)
	{
		TxMessage.Data[i]=RefereeStartData1[i];
	}
    CAN_Transmit(CAN1,&TxMessage);
	
	delay_us(10);
	
	for(int i=0;i<TxMessage.DLC;i++)
	{
		TxMessage.Data[i]=RefereeStartData2[i];
	}
    CAN_Transmit(CAN1,&TxMessage);
	
}


/****************************************************
name:CAN_HeartBeat_SendMsg
function:发送裁判心跳数据
@param:void
@return:void
description:将数据存入TxMessage结构体再由CAN_Transmit发送
****************************************************/
u8 RefereeHeartBeatData1[16]={0x5a, 0x00, 0x00, 0x0c, 0x00, 0x01, 0x01, 0xff,\
							0xff, 0x01, 0x00, 0x00, 0x15, 0xb8, 0x2f, 0x6f };
u8 RefereeHeartBeatData2[16]={0x5a, 0x00, 0x00, 0x56, 0x00, 0x01, 0x01, 0xff,\
							0xff, 0x01, 0x00, 0x00, 0x15, 0x7e, 0xdd, 0x67 };
u8 RefereeHeartBeatData3[16]={0x5a, 0x00, 0x00, 0xac, 0x00, 0x01, 0x01, 0xff,\
							0xff, 0x01, 0x00, 0x00, 0x15, 0x72, 0xe8, 0x76 };
void CAN_HeartBeat_SendMsg(void)
{	
//	CanTxMsg TxMessage;
//	TxMessage.StdId = 0xfe;      //帧ID为传入参数的CAN_ID
//    TxMessage.IDE = CAN_ID_STD;    //标准帧
//    TxMessage.RTR = CAN_RTR_DATA;  //数据帧
//    TxMessage.DLC = 0x08;          //帧长度为8
    
	static u8 heatcount=0;
	
	switch(heatcount)
	{
		case 0:
		{
//			for(int i=0;i<TxMessage.DLC;i++)
//			{
//				TxMessage.Data[i]=RefereeStartData2[i];
//			}
			CAN_SendNormalMsg(CAN1,0xfe,RefereeHeartBeatData1,16);
			heatcount++;
			break;
		}
		case 1:
		{
			CAN_SendNormalMsg(CAN1,0xfe,RefereeHeartBeatData2,16);
			heatcount++;
			break;
		}
		case 2:
		{
			CAN_SendNormalMsg(CAN1,0xfe,RefereeHeartBeatData3,16);
			heatcount=0;
			break;
		}
		default:
			break;
	}
}


u8 RefereeSetTurnONTESTData[2]={0x01 };//0xe8
void CAN_TurnONTEST_SendMsg(void)	//测试0x350包的作用
{
	CAN_SendNormalMsg(CAN1,0x350,RefereeSetTurnONTESTData,1);
}


/****************************************************
name:CAN_SetID0TurnON_SendMsg
function:发送裁判亮灯数据
@param:void
@return:void
description:将数据存入TxMessage结构体再由CAN_Transmit发送
****************************************************/
//u8 RefereeID0SetTurnONData[16]={0x5a, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02 ,\
//							0x00, 0x01, 0x00, 0x07, 0x15, 0x12, 0xb1, 0x4f };//0xe8	//只有接了大装甲才有
u8 RefereeID0SetTurnONData[16]={0x5a,0x00,0x00,0x05,0x00,0x01,0x01,0x02 ,\
							0x00,0x01,0x00,0x07,0x15,0xcf,0x1d,0x49 };//0xe8
void CAN_SetID0TurnON_SendMsg(void)	//开机可亮 关灯后不可亮
{
	CAN_SendNormalMsg(CAN1,0x140,RefereeID0SetTurnONData,16);
}

/****************************************************
name:CAN_SetID1TurnON_SendMsg
function:发送裁判亮灯数据
@param:void
@return:void
description:将数据存入TxMessage结构体再由CAN_Transmit发送
****************************************************/
u8 RefereeID1SetTurnONData[16]={0x5a, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02 ,\
							0x01, 0x01, 0x00, 0x07, 0x15, 0xdf, 0x73, 0x56 };//0xe8
void CAN_SetID1TurnON_SendMsg(void)	//开机可亮 关灯后不可亮
{
	CAN_SendNormalMsg(CAN1,0x141,RefereeID1SetTurnONData,16);
}

/****************************************************
name:CAN_SetID2TurnON_SendMsg
function:发送裁判亮灯数据
@param:void
@return:void
description:将数据存入TxMessage结构体再由CAN_Transmit发送
****************************************************/
u8 RefereeID2SetTurnONData[16]={0x5a, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02 ,\
							0x02, 0x01, 0x00, 0x07, 0x15, 0x91, 0x74, 0xf1 };//0xe8
void CAN_SetID2TurnON_SendMsg(void)	//开机可亮 关灯后不可亮
{
	CAN_SendNormalMsg(CAN1,0x142,RefereeID2SetTurnONData,16);
}

/****************************************************
name:RefereeID3SetTurnONData
function:发送裁判亮灯数据
@param:void
@return:void
description:将数据存入TxMessage结构体再由CAN_Transmit发送
****************************************************/
u8 RefereeID3SetTurnONData[16]={0x5a, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02 ,\
							0x03, 0x01, 0x00, 0x07, 0x15, 0x5c, 0xb6, 0xe8 };//0xe8
void CAN_SetID3TurnON_SendMsg(void)	//开机可亮 关灯后不可亮
{
	CAN_SendNormalMsg(CAN1,0x143,RefereeID3SetTurnONData,16);
}


void CAN_SetIDxTurnON_Task(AimorIDEnum id)
{
	switch(id)
	{
		case AIMORID_240:
		{
			CAN_SetID0TurnON_SendMsg();
			break;
		}
		case AIMORID_241:
		{
			CAN_SetID1TurnON_SendMsg();
			break;
		}
		case AIMORID_242:
		{
			CAN_SetID2TurnON_SendMsg();
			break;
		}
		case AIMORID_243:
		{
			CAN_SetID3TurnON_SendMsg();
			break;
		}
		case AIMORID_244:
		{
			break;
		}
		default:
			break;
	}
}


/****************************************************
name:RefereeSetRedToBlueData
function:发送裁判转灯数据
@param:void
@return:void
description:将数据存入TxMessage结构体再由CAN_Transmit发送
****************************************************/
u8 RefereeSetRedToBlueData[20]={0x5a, 0x04, 0x00, 0x04, 0x00, 0x01, 0x0b, 0x02 ,\
							0xff, 0x01, 0x00, 0x06, 0x10, 0x01, 0x00, 0x00,\
							0x00, 0x00, 0x2c, 0x53};
void CAN_SetAllRed2Blue_SendMsg(void)
{
	delay_us(50);
	CAN_SendNormalMsg(CAN1,0xfe,RefereeSetRedToBlueData,20);
}


/****************************************************
name:CAN_SetALLRedTurnOFF_SendMsg
function:发送裁判 Red Turn OFF灯数据
@param:void
@return:void
description:将数据存入TxMessage结构体再由CAN_Transmit发送
****************************************************/
u8 RefereeSetALLRedTurnOFFData[20]={0x5a, 0x04, 0x00, 0x86, 0x01, 0x01, 0x01, 0x02 ,\
							0xff, 0x01, 0x00, 0x06, 0x10, 0xf9, 0x01, 0x00,\
							0x00, 0x00, 0x8d, 0x53};
void CAN_SetALLRedTurnOFF_SendMsg(void)
{
	CAN_SendNormalMsg(CAN1,0xfe,RefereeSetALLRedTurnOFFData,20);
}

/****************************************************
name:CAN_SetALLBlueTurnOFF_SendMsg
function:发送裁判Blue Turn OFF灯数据
@param:void
@return:void
description:将数据存入TxMessage结构体再由CAN_Transmit发送
****************************************************/
u8 RefereeSetALLBlueTurnOFFData[20]={0x5a, 0x04, 0x00, 0x8a, 0x01, 0x01, 0x0b, 0x02 ,\
							0xff, 0x01, 0x00, 0x06, 0x10, 0x47, 0x01, 0x00,\
							0x00, 0x00, 0x2d, 0x1d};
void CAN_SetALLBlueTurnOFF_SendMsg(void)
{
	CAN_SendNormalMsg(CAN1,0xfe,RefereeSetALLBlueTurnOFFData,20);
}


void ALLAutoTurnON(void)
{
	for(AimorIDEnum id=AIMORID_240;id<AIMORID_NUMS;id++)
	{
		switch(AimorData[id].state)	//可以尝试先灭所有的再亮指定的
		{
			case OFF_LINE:
			{
				//CAN_RefereeStart_SendMsg();
				break;
			}
			case ON_WAIT_SET:
			{
				CAN_SetIDxTurnON_Task(id);
				delay_ms(20);
				AimorData[id].state=ON_LINE;
				break;
			}
			case ON_LINE:
			{
				//CAN_SetAllRed2Blue_SendMsg();
				//CAN_HeartBeat_SendMsg();
				//delay_ms(200);

				break;
			}
		}

	}
	
}

/****************************************************
name:ArmorHit_CallBack
function:装甲板被击打时产生的回调
@param:id:装甲板ID最低字节
@param:type:受打击的类型
@return:void
description:击打回调函数，其中处理击打数据
****************************************************/
__weak void ArmorHit_CallBack(AimorIDEnum id,AimorHitTypeEnum type)
{
	
}

void Armor_Data_Analysis(AimorIDEnum id,u8* pdata,u8 length)	//仅适用于装甲板ID:240-244
{
	if(length==8)
	{
		if(*pdata==0x5a&&*(pdata+1)==0x00&&*(pdata+2)==0x00)	//模块请求通信数据
		{
			AimorData[id].state=ON_WAIT_SET;
		}
		
		if(*pdata==0x5a&&*(pdata+1)==0x09&&*(pdata+2)==0x00)	//伤害数据
		{
			AimorData[id].numOfhits++;
			AimorData[id].hitcallback(id,HIT_BULLET17);
		}
	}
}

/**************** **************************
函数名：CAN1_Hit_Analysis
函数功能：对装甲板数据进行解析
          得到击打数据
函数参数：无
函数返回值：无
函数描述：无
*******************************************/
u32 hit_count=0;
void CAN1_Hit_Analysis(CanRxMsg *rx_message)
{		
	//CAN_Receive(CAN1, CAN_FIFO0, rx_message);//读取数据  后移至bsp层
	
	switch(rx_message->StdId)
	{
		case 0x150:	//弹丸击打数据
		{
			hit_count++;  
			break;
		}
		case 0x240:	//id=1装甲数据
		{
//			if(AimorData[0].state==OFF_LINE)
//			{
//				AimorData[0].state=ON_WAIT_SET;
//			}
//			else if(AimorData[0].state==ON_WAIT_SET)
//			{
//				//AimorData[0].state=ON_LINE;
//			}
			Armor_Data_Analysis(AIMORID_240,rx_message->Data,rx_message->DLC);
			break;
		}
		case 0x241:	//id=2装甲数据
		{
//			if(AimorData[1].state==OFF_LINE)
//			{
//				AimorData[1].state=ON_WAIT_SET;
//			}
//			else if(AimorData[1].state==ON_WAIT_SET)
//			{
//				//AimorData[1].state=ON_LINE;
//			}
			Armor_Data_Analysis(AIMORID_241,rx_message->Data,rx_message->DLC);
			break;
		}
		case 0x242:	//id=3装甲数据
		{
//			if(AimorData[2].state==OFF_LINE)
//			{
//				AimorData[2].state=ON_WAIT_SET;
//			}
//			else if(AimorData[2].state==ON_WAIT_SET)
//			{
//				//AimorData[2].state=ON_LINE;
//			}
			Armor_Data_Analysis(AIMORID_242,rx_message->Data,rx_message->DLC);
			break;
		}
		case 0x243:	//id=4装甲数据
		{
			
//			if(AimorData[3].state==OFF_LINE)
//			{
//				AimorData[3].state=ON_WAIT_SET;
//			}
//			else if(AimorData[3].state==ON_WAIT_SET)
//			{
//				//AimorData[3].state=ON_LINE;
//			}
			Armor_Data_Analysis(AIMORID_243,rx_message->Data,rx_message->DLC);
			break;
		}
		case 0x244:	//id=5？装甲数据
		{
//			if(AimorData[4].state==OFF_LINE)
//			{
//				AimorData[4].state=ON_WAIT_SET;
//			}
//			else if(AimorData[4].state==ON_WAIT_SET)
//			{
//				//AimorData[4].state=ON_LINE;
//			}
			Armor_Data_Analysis(AIMORID_244,rx_message->Data,rx_message->DLC);
			break;
		}
		default:
		break;
	}
}


///****************************************************
//name:CAN_HeartBeat_SendMsg
//function:发送裁判心跳数据
//@param:void
//@return:void
//description:将数据存入TxMessage结构体再由CAN_Transmit发送
//****************************************************/
//void CAN_HeartBeat_SendMsg(void)
//{	
//	CanTxMsg TxMessage;
//	TxMessage.StdId = 0x200;      //帧ID为传入参数的CAN_ID
//    TxMessage.IDE = CAN_ID_STD;    //标准帧
//    TxMessage.RTR = CAN_RTR_DATA;  //数据帧
//    TxMessage.DLC = 0x08;          //帧长度为8
//    
//	TxMessage.Data={0};
//		 
//    CAN_Transmit(CAN1,&TxMessage);
//}

