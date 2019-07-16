#include "led_control.h"
#include "pwm_sk6812.h"

#include "gpio.h"

void SK6812_BIGBUFF_Set(void);

extern u32 time_1ms_count;

//#define BLINK_CYCLE 150
//#define BLINK_INTERVAL 2500

u8 Frequency_Green,Frequency_Red=0;
void LED_Blink_Run(void)	//LED闪烁运行函数10h处理频率
{
	if(Frequency_Green<10&&Frequency_Green>0)
	{
		if(time_1ms_count%BLINK_INTERVAL<=2*Frequency_Green*BLINK_CYCLE)
		{
			if(time_1ms_count%BLINK_INTERVAL==0)
			{
				GREEN_LED_OFF();	//每次循环开始将灯熄灭
			}
			else if(time_1ms_count%BLINK_CYCLE==0)
			{
				GREEN_LED_TOGGLE();
			}
		}
	}
	else if(Frequency_Green>=10)	//常亮
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
				RED_LED_OFF();	//每次循环开始将灯熄灭
			}
			else if(time_1ms_count%BLINK_CYCLE==0)
			{
				RED_LED_TOGGLE();
			}
		}
	}
	else if(Frequency_Red>=10)	//常亮
	{
		RED_LED_ON();
	}
	else
	{
		RED_LED_OFF();
	}
	
}

void LED_Blink_Set(u8 frequency_green,u8 frequency_red)	//2s内闪烁次数,最高9
{
//	Frequency_Green=frequency_green<0?0:frequency_green;
//	Frequency_Red=frequency_red<0?0:frequency_red;
	Frequency_Green=frequency_green>10?10:frequency_green;
	Frequency_Red=frequency_red>10?10:frequency_red;
}

u8 led_start2normal=0;

u8 SK6812Colors[350][3]={0};
#define MAX_LI 80
u32 led_lightnums=350;
void SK6812_Run(void)
{

////////	if(led_start2normal==0)
////////	{
////////		SK6812_SetStart();
////////		//SK6812_SetError();
////////	}
////////	else
////////	{
////////		SK6812_SetNormal();
////////	}
	
	SK6812_BIGBUFF_Set();
	//led_lightnums++;
	//if(led_lightnums>1000) led_lightnums=0;
	PAGE1_UpdateColor(SK6812Colors,1);//OK
	PAGE2_UpdateColor(SK6812Colors,2);//XX
	PAGE3_UpdateColor(SK6812Colors,3);//OK
	PAGE4_UpdateColor(SK6812Colors,4);	//OK右上
	PAGE5_UpdateColor(SK6812Colors,5);
	//PWM2_1_DMA_Enable();
	
}

u8 rgbstate=0;
void SK6812_SetNormal(void)
{
	static u8 krg,kgb,kbr=0;
	
	switch(rgbstate)
	{
		case 0:
		{
			krg++;
			if(krg>=MAX_LI)
			{
				rgbstate=1;
				krg=MAX_LI;
				kgb=0;
				kbr=0;
			}
			SK6812Colors[0][SK6812_RED]=(u8)(MAX_LI-krg);
			SK6812Colors[0][SK6812_GREEN]=(u8)(krg);
			SK6812Colors[0][SK6812_BLUE]=0;
			break;
		}
		case 1:
		{
			kgb++;
			if(kgb>=MAX_LI)
			{
				rgbstate=2;
				krg=0;
				kgb=MAX_LI;
				kbr=0;
			}
			SK6812Colors[0][SK6812_RED]=0;
			SK6812Colors[0][SK6812_GREEN]=(u8)(MAX_LI-kgb);
			SK6812Colors[0][SK6812_BLUE]=(u8)(kgb);
			break;
		}
		case 2:
		{
			kbr++;
			if(kbr>=MAX_LI)
			{
				rgbstate=0;
				krg=0;
				kgb=0;
				kbr=MAX_LI;
			}
			SK6812Colors[0][SK6812_RED]=(u8)(kbr);
			SK6812Colors[0][SK6812_GREEN]=0;
			SK6812Colors[0][SK6812_BLUE]=(u8)(MAX_LI-kbr);
			break;
		}
	}
	
	static u8 ledLightstate[5]={0,0,0,0,1};
	static float kk[5]={0,0.1f,0.2f,0.3f,0.4f};
	
	for(int i=0;i<5;i++)
	{
		switch(ledLightstate[i])
		{
			case 0:
			{
				kk[i]+=0.01f;
				if(kk[i]>=0.4f)
				{
					kk[i]=0.4f;
					ledLightstate[i]=1;
				}
				break;
			}
			case 1:
			{
				kk[i]-=0.01f;
				if(kk[i]<=0)
				{
					kk[i]=0;
					ledLightstate[i]=0;
				}
				break;
			}
		}
		
		
	}
	
	
	SK6812Colors[1][SK6812_GREEN]=SK6812Colors[0][SK6812_RED];
	SK6812Colors[1][SK6812_BLUE]=SK6812Colors[0][SK6812_GREEN];
	SK6812Colors[1][SK6812_RED]=SK6812Colors[0][SK6812_BLUE];
//	for(int j=0;j<3;j++)
//	{
//		SK6812Colors[0][j]*=kk[0];
//		SK6812Colors[1][j]*=1-kk[0];
//	}
	
	SK6812Colors[2][SK6812_GREEN]=SK6812Colors[0][SK6812_GREEN];
	SK6812Colors[2][SK6812_BLUE]=SK6812Colors[0][SK6812_RED];
	SK6812Colors[2][SK6812_RED]=SK6812Colors[0][SK6812_BLUE];
	
	SK6812Colors[3][SK6812_GREEN]=SK6812Colors[0][SK6812_BLUE];
	SK6812Colors[3][SK6812_BLUE]=SK6812Colors[0][SK6812_GREEN];
	SK6812Colors[3][SK6812_RED]=SK6812Colors[0][SK6812_RED];
	
	SK6812Colors[4][SK6812_GREEN]=SK6812Colors[0][SK6812_GREEN];
	SK6812Colors[4][SK6812_BLUE]=SK6812Colors[0][SK6812_BLUE];
	SK6812Colors[4][SK6812_RED]=SK6812Colors[0][SK6812_RED];
	
	for(int i=5;i<1000;i++)//////////////////////////////////////////////////
	{
		SK6812Colors[i][SK6812_GREEN]=SK6812Colors[0][SK6812_GREEN];
		SK6812Colors[i][SK6812_BLUE]=SK6812Colors[0][SK6812_BLUE];
		SK6812Colors[i][SK6812_RED]=SK6812Colors[0][SK6812_RED];
	}
	
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<3;j++)
		{
			SK6812Colors[i][j]*=kk[i];
			//SK6812Colors[1][j]*=1-kk[0];
		}
	}
}

void _rol_(u8 *x)
{
	u8 t=*x>>7&0x01;
	*x=*x<<1;
	*x |= t;
}

void _ror_(u8 *x)
{
	u8 t=*x&0x01;
	*x=*x>>1;
	*x |= t<<7;
}


void SK6812_SetStart(void)	//调用周期100ms
{
	/**********************************************
	static u8 ledLightstate[5]={0,0,0,1,1};
	static float kk[5]={0,0.2f,0.4f,0.2f,0.0f};
	
	for(int i=0;i<5;i++)
	{
		switch(ledLightstate[i])
		{
			case 0:
			{
				kk[i]+=0.01f;
				if(kk[i]>=0.4f)
				{
					kk[i]=0.4f;
					ledLightstate[i]=1;
				}
				break;
			}
			case 1:
			{
				kk[i]-=0.01f;
				if(kk[i]<=0)
				{
					kk[i]=0;
					ledLightstate[i]=0;
				}
				break;
			}
		}
	}
	
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<3;j++)
		{
			SK6812Colors[i][j]=40*kk[i];
			//SK6812Colors[1][j]*=1-kk[0];
		}
	}
	***************************************************/
	
	static u8 ledstate=1;
	static u8 ledchangecnt=0;
	static u8 ledstartnum=0;
	if(time_1ms_count%100==0)
	{
		for(int i=0;i<5;i++)
		{
			for(int j=0;j<3;j++)
			{
				SK6812Colors[i][j]=100*((ledstate>>i)&0x01);
			}
		}
		_ror_(&ledstate);
		ledchangecnt++;
		if(ledchangecnt%8==0&&ledstartnum<=5)
		{
			ledstartnum++;
		}
		
		for(int i=0;i<ledstartnum;i++)
		{
			for(int j=0;j<3;j++)
			{
				SK6812Colors[i][j]=100;
			}
		}
//		if(ledstartnum==1)
//		{
//			for(int i=0;i<1;i++)
//			{
//				for(int j=0;j<3;j++)
//				{
//					SK6812Colors[i][j]=100;
//				}
//			}
//		}
//		else if(ledstartnum==2)
//		{
//			
//		}
//		else if(ledstartnum==3)
//		{
//			
//		}
//		else if(ledstartnum==4)
//		{
//			
//		}
//		else if(ledstartnum>=5)
//		{
//			
//		}
	}
	
}


void SK6812_SetError(void)
{
	static u8 errorledstate=0;
	
	if(time_1ms_count%500==0)
	{
		for(int i=0;i<5;i++)
		{
//			for(int j=0;j<3;j++)
//			{
				SK6812Colors[i][SK6812_RED]=100*errorledstate;
				SK6812Colors[i][SK6812_GREEN]=0;
				SK6812Colors[i][SK6812_BLUE]=0;
//			}
		}
		errorledstate=!errorledstate;
	}
	
}

#define PAGE1_ARMORSTART 93
#define PAGE1_ARMOREND 207
#define PAGE1_ALLEND 284
#define PAGE2_ARMORSTART 94
#define PAGE2_ARMOREND 211
#define PAGE2_ALLEND 287
#define PAGE3_ARMORSTART 94
#define PAGE3_ARMOREND 205
#define PAGE3_ALLEND 284
#define PAGE4_ARMORSTART 94
#define PAGE4_ARMOREND 206
#define PAGE4_ALLEND 284
#define PAGE5_ARMORSTART 91
#define PAGE5_ARMOREND 208
#define PAGE5_ALLEND 286

#define CYAN 0
#define ORANGE 1
#define BIGBUFF_ORANGE_R 0x7F
#define BIGBUFF_ORANGE_G 0x20
#define BIGBUFF_ORANGE_B 0x00
#define BIGBUFF_CYAN_R 0x00
#define BIGBUFF_CYAN_G 0x60
#define BIGBUFF_CYAN_B 0x43
u8 BIGBUFF_COLOR=CYAN;
void SK6812_BIGBUFF_Set(void)
{
	
	static u8 key_last=0;
	
	if(KEY_PB0==1&&key_last==0)
	{
		BIGBUFF_COLOR=!BIGBUFF_COLOR;
	}
	key_last=KEY_PB0;
	
	if(BIGBUFF_COLOR==CYAN)
	{
		for(int i=0;i<350;i++)//////////////////////////////////////////////////
		{
			if(i>PAGE3_ARMORSTART&&i<=PAGE3_ARMOREND)
			{
				SK6812Colors[i][SK6812_GREEN]=BIGBUFF_CYAN_G;
				SK6812Colors[i][SK6812_BLUE]=BIGBUFF_CYAN_B;
				SK6812Colors[i][SK6812_RED]=BIGBUFF_CYAN_R;
			}
			else if(i>PAGE3_ALLEND)
			{
				SK6812Colors[i][SK6812_GREEN]=BIGBUFF_CYAN_G;
				SK6812Colors[i][SK6812_BLUE]=BIGBUFF_CYAN_B;
				SK6812Colors[i][SK6812_RED]=BIGBUFF_CYAN_R;
			}
			else
			{
//				SK6812Colors[i][SK6812_GREEN]=0;
//				SK6812Colors[i][SK6812_BLUE]=0;
//				SK6812Colors[i][SK6812_RED]=0;
				SK6812Colors[i][SK6812_GREEN]=BIGBUFF_CYAN_G;
				SK6812Colors[i][SK6812_BLUE]=BIGBUFF_CYAN_B;
				SK6812Colors[i][SK6812_RED]=BIGBUFF_CYAN_R;
			}
			
		}
	}
	else
	{
		for(int i=0;i<350;i++)//////////////////////////////////////////////////
		{
			//if(i>PAGE2_ARMORSTART&&i<=PAGE2_ARMOREND)
			{
				SK6812Colors[i][SK6812_GREEN]=BIGBUFF_ORANGE_G;
				SK6812Colors[i][SK6812_BLUE]=BIGBUFF_ORANGE_B;
				SK6812Colors[i][SK6812_RED]=BIGBUFF_ORANGE_R;
			}
//			else
//			{
//				SK6812Colors[i][SK6812_GREEN]=0;
//				SK6812Colors[i][SK6812_BLUE]=0;
//				SK6812Colors[i][SK6812_RED]=0;
//			}
			
		}
	}
	
//	for(int i=0;i<300;i++)//////////////////////////////////////////////////
//		{
//			if(i>PAGE2_ARMORSTART&&i<=PAGE2_ARMOREND)
//			{
//				if(i<PAGE2_ARMORSTART+35||i>PAGE2_ARMORSTART+95)
//				{
//					SK6812Colors[i][SK6812_GREEN]=BIGBUFF_ORANGE_G;
//					SK6812Colors[i][SK6812_BLUE]=BIGBUFF_ORANGE_B;
//					SK6812Colors[i][SK6812_RED]=BIGBUFF_ORANGE_R;
//				}
//				else
//				{
//					SK6812Colors[i][SK6812_GREEN]=BIGBUFF_CYAN_G;
//					SK6812Colors[i][SK6812_BLUE]=BIGBUFF_CYAN_B;
//					SK6812Colors[i][SK6812_RED]=BIGBUFF_CYAN_R;
//				}
//				
//			}
//			else
//			{
//				SK6812Colors[i][SK6812_GREEN]=0;
//				SK6812Colors[i][SK6812_BLUE]=0;
//				SK6812Colors[i][SK6812_RED]=0;
//			}
//			
//		}
	
}
