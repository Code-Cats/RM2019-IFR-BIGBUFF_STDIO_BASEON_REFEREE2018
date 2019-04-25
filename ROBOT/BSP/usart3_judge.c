#include "usart3_judge.h"

extern u16 FRICTION_SHOOT;	//�Զ�����

//extern OS_EVENT * judgeMsg;
char g_judgeFlag = 0;
char g_judgeLost = 0;
uint8_t   _USART3_DMA_RX_BUF[2][BSP_USART3_DMA_RX_BUF_LEN];
u8              JudgeSendBuff[22];

char            this_dma_type=0;
uint16_t        testcmdId;
uint16_t        testcmdIdCnt;
uint16_t        recordData;

float realShootSpeed = 27.0;
int16_t shootedBulletNum;
int16_t drop_rate_blood=0;
JUDGEMENT_DATA judgementData={0};

/*����Ƶ�ʲ��Դ���
int16_t judgetimer = 1;
int     judgeT = 0;
*/ 

tFrameHeader            testFrameHeader;
tGameRobotState         testGameRobotState;      //����������״̬
tRobotHurt              testRobotHurt;          //�������˺�����
tShootData              testShootData;          //ʵʱ�������
tPowerHeatData 				  testPowerHeatData;      //ʵʱ������������
tRfidDetect             testRfidDetect;          //���ؽ�������
tGameResult             testGameResult;          //����ʤ������
tGetBuff                testGetBuff;             //Buff��ȡ����
tGameRobotPos           testGameRobotPos;        //������λ�ó�����Ϣ
tShowData               testShowData;            //�Զ�������
JudgementSendData       testJudgementSendData=JUDGEMENTSENDDATA_DEFAULT;

void USART3_Configuration(uint32_t baud_rate)
{

    GPIO_InitTypeDef gpio;
	  USART_InitTypeDef usart;
	  NVIC_InitTypeDef nvic;
    DMA_InitTypeDef dma;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //ʹ��USART3��GPIOBʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);   //ʹ��DMA����
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //ʹ��USART3ʱ��
	
	  USART_DeInit(USART3);//��λ����3
    
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 

    gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB,&gpio);
    
		nvic.NVIC_IRQChannel = USART3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&nvic);
    
    USART_StructInit(&usart);
    usart.USART_BaudRate = baud_rate;
    usart.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
    usart.USART_StopBits = USART_StopBits_1;  //һ��ֹͣλ
    usart.USART_Parity = USART_Parity_No;   //����żУ��λ
    usart.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;  //�շ�ģʽ
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //��Ӳ������������
		
    USART_Init(USART3, &usart);  //��ʼ������3
		USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);        //���������ж�
		USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);   //ʹ�ܴ���1 DMA����
		USART_Cmd(USART3, ENABLE);                 //ʹ�ܴ��� 
		
		//��Ӧ��DMA����
	  DMA_DeInit(DMA1_Stream1);
   // DMA_StructInit(&dma);
    dma.DMA_Channel = DMA_Channel_4;
    dma.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);  //DMA����ADC����ַ
    dma.DMA_Memory0BaseAddr = (uint32_t)&_USART3_DMA_RX_BUF[0][0]; //DMA�ڴ����ַ
    dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
    dma.DMA_BufferSize = BSP_USART3_DMA_RX_BUF_LEN;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dma.DMA_Mode = DMA_Mode_Normal;      //��������������ģʽ
    dma.DMA_Priority = DMA_Priority_Medium;   //DMAͨ�� xӵ�������ȼ� 
    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;   //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream1, &dma);
		
		
		    //����Memory1,Memory0�ǵ�һ��ʹ�õ�Memory
    DMA_DoubleBufferModeConfig(DMA1_Stream1, (uint32_t)&_USART3_DMA_RX_BUF[1][0], DMA_Memory_0);   //first used memory configuration
    DMA_DoubleBufferModeCmd(DMA1_Stream1, ENABLE);
    
    DMA_Cmd(DMA1_Stream1, ENABLE);
		
   
		
		
    
}
		
		


const uint8_t CRC8_INIT = 0xff;
const uint8_t CRC8_TAB[256] =
{
  0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
  0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
  0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
  0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
  0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
  0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
  0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
  0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b, 0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
  0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
  0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
  0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
  0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
  0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
  0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
  0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
  0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
};




/*
** Descriptions: CRC8 checksum function
** Input: Data to check,Stream length, initialized checksum
** Output: CRC checksum
*/
uint8_t Get_CRC8_Check_Sum(uint8_t *pchMessage,uint16_t dwLength,uint8_t ucCRC8)
{
  uint8_t ucIndex;
  while (dwLength--)
  {
    ucIndex = ucCRC8^(*pchMessage++);
    ucCRC8 = CRC8_TAB[ucIndex];
  }
  return(ucCRC8);
}


/*
** Descriptions: CRC8 Verify function
** Input: Data to Verify,Stream length = Data + checksum
** Output: True or False (CRC Verify Result)
*/
uint16_t Verify_CRC8_Check_Sum(uint8_t *pchMessage, uint16_t dwLength)
{
  uint8_t ucExpected = 0;
  if ((pchMessage == 0) || (dwLength <= 2)) return 0;
  ucExpected = Get_CRC8_Check_Sum (pchMessage, dwLength-1, CRC8_INIT);
  return ( ucExpected == pchMessage[dwLength-1] );
}


/*
** Descriptions: append CRC8 to the end of data
** Input: Data to CRC and append,Stream length = Data + checksum
** Output: True or False (CRC Verify Result)
*/
void Append_CRC8_Check_Sum(uint8_t *pchMessage, uint16_t dwLength)
{
  uint8_t ucCRC = 0;
  if ((pchMessage == 0) || (dwLength <= 2)) return;
  ucCRC = Get_CRC8_Check_Sum ( (uint8_t *)pchMessage, dwLength-1, CRC8_INIT);
  pchMessage[dwLength-1] = ucCRC;
}


/*
** Descriptions: CRC16 checksum function
** Input: Data to check,Stream length, initialized checksum
** Output: CRC checksum
*/
uint16_t CRC_INIT = 0xffff;
const uint16_t wCRC_Table[256] =
{
  0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
  0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
  0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
  0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
  0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
  0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
  0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
  0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
  0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
  0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
  0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
  0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
  0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
  0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
  0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
  0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
  0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
  0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
  0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
  0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
  0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
  0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
  0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
  0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
  0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
  0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
  0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
  0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
  0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
  0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
  0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
  0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC)
{
  uint8_t chData;
  if (pchMessage == NULL)
  {
    return 0xFFFF;
  }
  while(dwLength--)
  {
    chData = *pchMessage++;
    (wCRC) = ((uint16_t)(wCRC) >> 8) ^ wCRC_Table[((uint16_t)(wCRC) ^(uint16_t)(chData)) & 0x00ff];
  }
  return wCRC;
}


/*
** Descriptions: CRC16 Verify function
** Input: Data to Verify,Stream length = Data + checksum
** Output: True or False (CRC Verify Result)
*/
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength)
{
  uint16_t wExpected = 0;
  if ((pchMessage == NULL) || (dwLength <= 2))
  {
    return 0;
  }
  wExpected = Get_CRC16_Check_Sum ( pchMessage, dwLength - 2, CRC_INIT);
  return ((wExpected & 0xff) == pchMessage[dwLength - 2] && ((wExpected >> 8) & 0xff)== pchMessage[dwLength - 1]);
}


/*
** Descriptions: append CRC16 to the end of data
** Input: Data to CRC and append,Stream length = Data + checksum
** Output: True or False (CRC Verify Result)
*/
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength)
{
  uint16_t wCRC = 0;
  if ((pchMessage == NULL) || (dwLength <= 2))
  {
    return;
  }
  wCRC = Get_CRC16_Check_Sum ( (uint8_t*)pchMessage, dwLength-2, CRC_INIT );
  pchMessage[dwLength-2] = (uint8_t)(wCRC & 0x00ff);
  pchMessage[dwLength-1] = (uint8_t)((wCRC >> 8)& 0x00ff);
}

void USART3_IRQHandler(void)
{
	 static uint32_t this_frame_rx_len = 0;
  
	 if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	 {
		  (void)USART3->SR;
		  (void)USART3->DR;
			if(DMA_GetCurrentMemoryTarget(DMA1_Stream1) == 0)
		  {
			   this_dma_type=0;
			   DMA_Cmd(DMA1_Stream1, DISABLE);
			   this_frame_rx_len = BSP_USART3_DMA_RX_BUF_LEN - DMA_GetCurrDataCounter(DMA1_Stream1);
			   DMA1_Stream1->NDTR = (uint16_t)BSP_USART3_DMA_RX_BUF_LEN;     //??relocate the dma memory pointer to the beginning position
			   DMA1_Stream1->CR |= (uint32_t)(DMA_SxCR_CT);                  //enable the current selected memory is Memory 1
			   DMA_Cmd(DMA1_Stream1, ENABLE);
				
				 judgementDataHandler();
		  }
			else 
		  {
			   this_dma_type=1;
			   DMA_Cmd(DMA1_Stream1, DISABLE);
			   this_frame_rx_len = BSP_USART3_DMA_RX_BUF_LEN - DMA_GetCurrDataCounter(DMA1_Stream1);
			   DMA1_Stream1->NDTR = (uint16_t)BSP_USART3_DMA_RX_BUF_LEN;      //??relocate the dma memory pointer to the beginning position
			   DMA1_Stream1->CR &= ~(uint32_t)(DMA_SxCR_CT);                  //enable the current selected memory is Memory 0
			   DMA_Cmd(DMA1_Stream1, ENABLE);
				
				 judgementDataHandler();
		  }
	 }

}


void judgementDataHandler(void)
{
  uint8_t cnt = 5;
	
/**�������ݷ���Ƶ�ʲ��Դ���**/
//	static int32_t error[2] = {0};
//	if(judgementData.flag==0)
//	{
//	  judgementData.flag=1;
//	}
//	
//	if(judgetimer == 1)
//		error[0] = get_heartbeat();
//	
//	error[1] = get_heartbeat();
//	
//	if((error[1] - error[0]) > 1000) 
//	{
//		judgeT = judgetimer;
//		judgetimer = 0;
//	}
/***************************/
	
	LostCountFeed(&Error_Check.count[LOST_REFEREE]);//////////////////////////////////////
	
	memcpy(&testcmdId, (_USART3_DMA_RX_BUF[this_dma_type]+cnt), sizeof(testcmdId));  //04
		recordData=cnt+2;
	
	if(testcmdId == PowerHeatDataId)
	{
		/*�������Դ���
		judgetimer++;
		*/
		memcpy(&testPowerHeatData, (_USART3_DMA_RX_BUF[this_dma_type]+recordData),20);		
	}
	
	if(testcmdId == ShootDataId)
	{
		memcpy(&testShootData, (_USART3_DMA_RX_BUF[this_dma_type]+recordData),6);
		////////////////////////////////////////////////////////////////shootedBulletNum++;
		////////////////////////////////////////////////////////////////BulletNum_Simu_ADD();	//////////////////////////////////////////////////////////////////

		///////////Friction_Adjust_DependOn_fdbV(&FRICTION_SHOOT,testShootData.bulletSpeed);
	}
	
	if(testcmdId == GetBuffId)	////////////////////////////////////////////
	{
		//memcpy(&testShootData, (_USART3_DMA_RX_BUF[this_dma_type]+recordData),6);
		testGetBuff.myselfBigBuff   = (_USART3_DMA_RX_BUF[this_dma_type][recordData] & 0x10) >> 4;
    testGetBuff.enemyBigBuff    = (_USART3_DMA_RX_BUF[this_dma_type][recordData] & 0x20) >> 5;
    testGetBuff.myselfSmallBuff = (_USART3_DMA_RX_BUF[this_dma_type][recordData] & 0x40) >> 6;
    testGetBuff.enemySmallBuff =  (_USART3_DMA_RX_BUF[this_dma_type][recordData] & 0x80) >> 7; 		
	}
	
	if(testcmdId == RobotHurtId )
	{
		testRobotHurt.armorType = _USART3_DMA_RX_BUF[this_dma_type][recordData] & 0x0f;
		testRobotHurt.hurtType  = (_USART3_DMA_RX_BUF[this_dma_type][recordData] >> 4) & 0x0f;
	}
	
	memcpy(&testcmdId, (_USART3_DMA_RX_BUF[this_dma_type]+cnt+29), sizeof(testcmdId)); //01
		recordData=cnt+29+2;
	
	if(testcmdId == GameRobotPosId)
		memcpy(&testGameRobotPos, (_USART3_DMA_RX_BUF[this_dma_type]+recordData), 16);
	
	memcpy(&testcmdId, (_USART3_DMA_RX_BUF[this_dma_type]+cnt+54), sizeof(testcmdId)); //01
		recordData=cnt+54+2;
	
	if(testcmdId == GameRobotStateId)
		memcpy(&testGameRobotState, (_USART3_DMA_RX_BUF[this_dma_type]+recordData), 8);
}

union var
{
  char c[4];
	float f;
};
union var Data_A;
union var Data_B;
union var Data_C;
union var Data_D;
void Judgement_DataSend(float a,float b,float c,uint8_t d)
{
	  int i=0;
		testJudgementSendData.cSeq++;
		testJudgementSendData.Data_A=a;
		testJudgementSendData.Data_B=b;
		testJudgementSendData.Data_C=c;
		testJudgementSendData.Data_D=d;
	
		JudgeSendBuff[0]=0x00A5;
		JudgeSendBuff[1]=testJudgementSendData.dataLenth;
		JudgeSendBuff[2]=testJudgementSendData.dataLenth>>8;
		JudgeSendBuff[3]=testJudgementSendData.cRC8;
		Append_CRC8_Check_Sum(JudgeSendBuff,5);
		JudgeSendBuff[5]=StudentSend;
		JudgeSendBuff[6]=StudentSend>>8;
		
		Data_A.f=testJudgementSendData.Data_A;	
		JudgeSendBuff[7]=0x0001;
		JudgeSendBuff[8]=0x0000;
		JudgeSendBuff[9]=0x0000;
		JudgeSendBuff[10]=0x0000;

		Data_B.f=testJudgementSendData.Data_B;			
		JudgeSendBuff[11]=0x0002;
		JudgeSendBuff[12]=0x0000;
		JudgeSendBuff[13]=0x0000;
		JudgeSendBuff[14]=0x0000;

		Data_C.f=testJudgementSendData.Data_C;		
		JudgeSendBuff[15]=0x0003;
		JudgeSendBuff[16]=0x0000;
		JudgeSendBuff[17]=0x0000;
		JudgeSendBuff[18]=0x0000;
		
		Data_D.f=testJudgementSendData.Data_D;	
		JudgeSendBuff[19]=0x0004;
		Append_CRC16_Check_Sum(JudgeSendBuff,22);
		for(i=0;i<22;i++)
		{
    USART_SendData(USART3, (uint8_t)JudgeSendBuff[i]);
		while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);			
		}
}

u8 Guiding_Lights_Data=0;	//ָʾ��
uint8_t Judagement_Send_Guiding_lights(u8 stateA, u8 stateB, u8 stateC, u8 stateD, u8 stateE, u8 stateF)
{
    static uint8_t output=0;
    output=stateA|(stateB<<1)|(stateC<<2)|(stateD<<3)|(stateE<<4)|(stateF<<5);
    return output;
}

/*
���:���з�����ʵʱǹ������
     ˫Ŀ������Ŀ�����
     ����õ���Ŀ���ٶȣ�����һ���������ɣ�
����:���Ƶ��
*/
