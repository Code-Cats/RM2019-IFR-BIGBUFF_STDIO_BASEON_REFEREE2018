#include "main.h"
#include "math.h"
#include "hit_recognition.h"

#define PITCH 0	//移到上面去了
#define ROLL 1
#define YAW 2


extern u32 time_1ms_count;
extern RobotHeatDataSimuTypeDef RobotHeatDataSimu42;

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
		 //delay_ms(200);
		 //delay_ms(1);
		 ALLAutoTurnON();
		 if(time_1ms_count%400==0)
		 {
			 CAN_HeartBeat_SendMsg();
		 }
	 }
}


void ArmorHit_CallBack(AimorIDEnum id,AimorHitTypeEnum type)
{
 
}


//////////////////////////////////////////////////////////////////
//??????,??printf??,??????use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//??????????                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//??_sys_exit()??????????    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//???fputc?? 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//????,??????   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
