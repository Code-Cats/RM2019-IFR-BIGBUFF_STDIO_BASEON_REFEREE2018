#include "gpioctrl.h"

extern u32 time_1ms_count;

//#define BLINK_CYCLE 150
//#define BLINK_INTERVAL 2500

u8 Frequency_Green,Frequency_Red=0;
void LED_Blink_Run(void)	//LED��˸���к���10h����Ƶ��
{
	if(Frequency_Green<10&&Frequency_Green>0)
	{
		if(time_1ms_count%BLINK_INTERVAL<=2*Frequency_Green*BLINK_CYCLE)
		{
			if(time_1ms_count%BLINK_INTERVAL==0)
			{
				GREEN_LED_OFF();	//ÿ��ѭ����ʼ����Ϩ��
			}
			else if(time_1ms_count%BLINK_CYCLE==0)
			{
				GREEN_LED_TOGGLE();
			}
		}
	}
	else if(Frequency_Green>=10)	//����
	{
		GREEN_LED_ON();
	}
	else
	{
		GREEN_LED_OFF();
	}
	
	if(Frequency_Red<10&&Frequency_Red>0)
	{
		if(time_1ms_count%BLINK_INTERVAL<=2*Frequency_Red*BLINK_CYCLE)
		{
			if(time_1ms_count%BLINK_INTERVAL==0)
			{
				RED_LED_OFF();	//ÿ��ѭ����ʼ����Ϩ��
			}
			else if(time_1ms_count%BLINK_CYCLE==0)
			{
				RED_LED_TOGGLE();
			}
		}
	}
	else if(Frequency_Red>=10)	//����
	{
		RED_LED_ON();
	}
	else
	{
		RED_LED_OFF();
	}
	
}

void LED_Blink_Set(u8 frequency_green,u8 frequency_red)	//2s����˸����,���9
{
//	Frequency_Green=frequency_green<0?0:frequency_green;
//	Frequency_Red=frequency_red<0?0:frequency_red;
	Frequency_Green=frequency_green>10?10:frequency_green;
	Frequency_Red=frequency_red>10?10:frequency_red;
}
