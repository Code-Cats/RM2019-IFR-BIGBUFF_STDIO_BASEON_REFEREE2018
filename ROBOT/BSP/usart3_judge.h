
#ifndef __USART_JUDGE_H__
#define __USART_JUDGE_H__


#include "bsp.h"

#define JUDGEMENTSENDDATA_DEFAULT {0,SOF_FIXED,13,0,0,0,0,0,0} 
#define  SOF_FIXED                          0xA5    //�̶�֡ͷ�ֽ�
#define  FRAMEHEADER_SIZE                   0x04      //֡ͷ����
#define  FRAMEHEADER_LEN                    0x05    //֡ͷ����
#define  BSP_USART3_DMA_RX_BUF_LEN          100u  
#define  RC_JUDGEMENT_LENGTH                44u

#define  RESTART      1
#define  NO_RESTART   0

#define  ON           1
#define  OFF          0

#define  HURT         					 1
#define  SMALLAMROR_FRAME_CHANGE 2
#define  BIGARMOR_FRAME_CHANGE   2
#define  BIGARMOR_LOST           3

#define  GENERAL	    			   0
#define  LITTLE_ADVANTAGE		   1
#define  LARGE_ADVANTAGE	  	 2
#define  LITTLE_DISADVANTAGE   3
#define  LARGE_DISADVANTAGE		 4

#define  START        0
#define  PROCESSING   1
#define  END          2

#define  LEVEL_0    0
#define  LEVEL_1    1
#define  LEVEL_2    2
#define  LEVEL_4    4
#define  LEVEL_5    10
#define  LEVEL_BIG  20

typedef enum{
  GameRobotStateId  = 0x0001, //����������״̬         8 Byte           10HZ
  RobotHurtId       = 0x0002,  //�������˺�����        1 Byte            ---
  ShootDataId       = 0x0003, //ʵʱ�������           6 Byte            ---
	PowerHeatDataId   = 0x0004, //ʵʱ������������      20 Byte            50HZ
	RfidDetectId      = 0x0005, //���ؽ�������           2 Byte            10HZ
	GameResultId      = 0x0006, //����ʤ������           2 Byte            ---
	GetBuffId         = 0x0007, //Buff��ȡ����           3 Byte            ---
	GameRobotPosId    = 0x0008,	//������λ�ó�����Ϣ    16 Byte            50HZ
	StudentSend       = 0x0100, //ѧ���Զ�������
}comIdType;


typedef __packed struct
{
  uint8_t   sOF;           //165
  uint16_t  dataLenth;     //20
	uint8_t   cSeq;
  uint8_t   cRC8;
}tFrameHeader;

typedef __packed struct 
{ 
   uint16_t  stageRemainTime;  //��ǰ�׶�ʣ��ʱ��
   uint8_t   gameProgress;     //��ǰ�����׶�  
   uint8_t   robotLevel;       //�����˵�ǰ�ȼ�
   uint16_t   remainHP;         //�����˵�ǰѪ��
   uint16_t   maxHP;            //��������Ѫ��
}tGameRobotState; 

typedef __packed struct
{
  uint8_t armorType:4;    /*0-3bits: ���仯����Ϊװ���˺�ʱ����ʶװ�� ID
                        0x00: 0 ��װ���� ��ǰ��
                        0x01�� 1 ��װ���� ����
                        0x02�� 2 ��װ���� ����
                        0x03�� 3 ��װ���� �� �ң�
                        0x04: 4 ��װ���� ���� 1��
                        0x05: 5 ��װ���棨 �� 2��*/
  
  uint8_t hurtType:4;   /*4-7bits: Ѫ���仯����
                        0x0: װ���˺����ܵ�������    
                        0x1: ģ�����*/
}tRobotHurt;

typedef __packed struct
{
  uint8_t bulletType;        //��������   1-> 17mm���� ��2-> 42mm����
  uint8_t bulletFreq;       //������Ƶ�� ��/s��
  float   bulletSpeed;     //��������(m/s Ӣ�ۻ�����)
}tShootData;

typedef __packed struct  
{  
   float chassisVolt;        //���������ѹ
   float chassisCurrent;     //�����������
   float chassisPower;       //�����������
   float chassisPowerBuffer; //���̹��ʻ���
	 uint16_t shooterHeat0;    //17mmǹ������
	 uint16_t shooterHeat1;    //42mmǹ������
}tPowerHeatData;

typedef __packed struct 
{
  uint8_t cardType;  //������   0�������ӳɿ�  1�������ӳɿ�  2���췽��Ѫ�� 3��������Ѫ�� 4���췽���������ؿ� 5���������������ؿ�
	uint8_t cardIdx;   //�������ţ����������ֲ�ͬ����
}tRfidDetect;

typedef __packed struct
{
  uint8_t winner;
}tGameResult;

typedef __packed struct
{
  uint8_t enemySmallBuff;
	uint8_t enemyBigBuff;
	uint8_t myselfSmallBuff;
	uint8_t myselfBigBuff;
}tGetBuff;

typedef __packed struct
{
  float x;   //X����
	float y;   //Y����
	float z;   //Z����
	float yaw; //ǹ�ڳ���Ƕ�ֵ����λ����
}tGameRobotPos;

typedef __packed struct
{
  float data1;    //�Զ�������1
	float data2;    //�Զ�������2
	float data3;    //�Զ�������3
	uint8_t mask;   //�Զ�������4
}tShowData;

typedef union{
	
	u8		U8[4];
	float F32;
}Send2PcDataConvertUnion;

typedef __packed struct
{
  char flag; 
  uint8_t   sOF;
  uint16_t  dataLenth;
	uint8_t   cSeq;
  uint8_t   cRC8;	
  float Data_A; 
	float Data_B;
	float Data_C;
	uint8_t Data_D;
}JudgementSendData;

typedef struct  __JUDGEMENT_DATA__
{
  char flag;	
  char shootflag;	

}JUDGEMENT_DATA;
extern u8 JudgeSendBuff[22];
void Judgement_DataSend(float a,float b,float c,uint8_t d);
uint8_t Judagement_Send_Guiding_lights(u8 stateA, u8 stateB, u8 stateC, u8 stateD, u8 stateE, u8 stateF);
//void Judagement_Send_Change_hero(float *a,float *b,float *c,uint8_t *d);
extern uint8_t judgementBuf[];
extern JUDGEMENT_DATA judgementData;
extern tFrameHeader            testFrameHeader;
extern tGameRobotState         testGameRobotState;      //����������״̬
extern tRobotHurt              testRobotHurt;          //�������˺�����
extern tShootData              testShootData;          //ʵʱ�������
extern tPowerHeatData 				  testPowerHeatData;      //ʵʱ������������
extern tRfidDetect             testRfidDetect;          //���ؽ�������
extern tGameResult             testGameResult;          //����ʤ������
extern tGetBuff                testGetBuff;             //Buff��ȡ����
extern tGameRobotPos           testGameRobotPos;        //������λ�ó�����Ϣ
extern tShowData               testShowData;            //�Զ�������
extern JudgementSendData     testJudgementSendData;

extern uint16_t  testcmdId;
extern int16_t testNum;
extern JUDGEMENT_DATA Judgement_Data;
//void judgementDataHandler(void);
void USART3_Configuration(uint32_t baud_rate);
void USART3_IRQHandler(void);
void Frameheader_Prcess(uint8_t *pData);
void BinocularVisionDataPrcess(uint8_t *pData);
float get_speed(void);
void judgementDataHandler(void);
#endif
