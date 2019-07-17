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
void SK6812_Run(void)	//200hz
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
	
	//led_lightnums++;
	//if(led_lightnums>1000) led_lightnums=0;
	static u16 light_state=1;
	if(time_1ms_count%100==0)
	{
		light_state<<=1;
		if(light_state==(0x0001<<10))
		{
			light_state=1;
		}
		//SK6812_BIGBUFF_Set();
	
		//PAGE1_UpdateColor(SK6812Colors,1);//OK
		//PAGE2_UpdateColor(SK6812Colors,2);//XX
		//PAGE3_UpdateColor(SK6812Colors,3);//OK
		//PAGE4_UpdateColor(SK6812Colors,4);	//OK右上
		//PAGE5_UpdateColor(SK6812Colors,5);
		
		//PWM3_1_DMA_Enable();
		//PWM3_2_DMA_Enable();
		//PWM3_3_DMA_Enable();
		//PWM2_2_DMA_Enable();
		//PWM2_3_DMA_Enable();
	}
	
	static u8 segcolor_r[4][3]=\
		{\
			{BIGBUFF_CYAN_G,BIGBUFF_CYAN_R,BIGBUFF_CYAN_B},\
			{0,0,0},\
			{0,0,0},\
			{BIGBUFF_CYAN_G,BIGBUFF_CYAN_R,BIGBUFF_CYAN_B},\
		};
	static u8 segcolor_l[4][3]=\
		{\
			{0,0,0},\
			{0,0,0},\
			{BIGBUFF_CYAN_G,BIGBUFF_CYAN_R,BIGBUFF_CYAN_B},\
			{BIGBUFF_CYAN_G,BIGBUFF_CYAN_R,BIGBUFF_CYAN_B},\
		};
	static u8 segcolor_no[4][3]=\
		{\
			{0,0,0},\
			{0,0,0},\
			{0,0,0},\
			{BIGBUFF_CYAN_G,BIGBUFF_CYAN_R,BIGBUFF_CYAN_B},\
		};
	
	if((time_1ms_count-15)%100==0)
	{
		static u16 segnode[4]={0,PAGE1_ARMORSTART,PAGE1_ARMOREND,PAGE1_ALLEND};
		if((light_state&0x0001<<0)!=0)
		{
			SK6812_Draw_ColorSegmentation(SK6812Colors,segnode,segcolor_l,4,false,1);
		}
		else if((light_state&0x0001<<1)!=0)
		{
			SK6812_Draw_ColorSegmentation(SK6812Colors,segnode,segcolor_r,4,false,1);
		}
		else
		{
			SK6812_Draw_ColorSegmentation(SK6812Colors,segnode,segcolor_no,4,false,1);
		}
		PAGE1_UpdateColor(SK6812Colors,290);//OK
		PWM3_1_DMA_Enable();
	}
	else if((time_1ms_count-30)%100==0)
	{
		//PAGE2_UpdateColor(SK6812Colors,2);//XX
		static u16 segnode[4]={0,PAGE2_ARMORSTART,PAGE2_ARMOREND,PAGE2_ALLEND};
		if((light_state&0x0001<<2)!=0)
		{
			SK6812_Draw_ColorSegmentation(SK6812Colors,segnode,segcolor_l,4,false,1);
		}
		else if((light_state&0x0001<<3)!=0)
		{
			SK6812_Draw_ColorSegmentation(SK6812Colors,segnode,segcolor_r,4,false,1);
		}
		else
		{
			SK6812_Draw_ColorSegmentation(SK6812Colors,segnode,segcolor_no,4,false,1);
		}
		
		PAGE2_UpdateColor(SK6812Colors,287);
		PWM3_2_DMA_Enable();
	}
	else if((time_1ms_count-45)%100==0)
	{
		static u16 segnode[4]={0,PAGE3_ARMORSTART,PAGE3_ARMOREND,PAGE3_ALLEND};
		if((light_state&0x0001<<4)!=0)
		{
			SK6812_Draw_ColorSegmentation(SK6812Colors,segnode,segcolor_l,4,false,1);
		}
		else if((light_state&0x0001<<5)!=0)
		{
			SK6812_Draw_ColorSegmentation(SK6812Colors,segnode,segcolor_r,4,false,1);
		}
		else
		{
			SK6812_Draw_ColorSegmentation(SK6812Colors,segnode,segcolor_no,4,false,1);
		}
		PAGE3_UpdateColor(SK6812Colors,350);//OK
		PWM3_3_DMA_Enable();
	}
	else if((time_1ms_count-60)%100==0)
	{
		static u16 segnode[4]={0,PAGE4_ARMORSTART,PAGE4_ARMOREND,PAGE4_ALLEND};
		if((light_state&0x0001<<6)!=0)
		{
			SK6812_Draw_ColorSegmentation(SK6812Colors,segnode,segcolor_l,4,false,1);
		}
		else if((light_state&0x0001<<7)!=0)
		{
			SK6812_Draw_ColorSegmentation(SK6812Colors,segnode,segcolor_r,4,false,1);
		}
		else
		{
			SK6812_Draw_ColorSegmentation(SK6812Colors,segnode,segcolor_no,4,false,1);
		}
		PAGE4_UpdateColor(SK6812Colors,290);	//
		PWM2_2_DMA_Enable();
	}
	else if((time_1ms_count-75)%100==0)
	{
		static u16 segnode[4]={0,PAGE5_ARMORSTART,PAGE5_ARMOREND,PAGE5_ALLEND};
		if((light_state&0x0001<<8)!=0)
		{
			SK6812_Draw_ColorSegmentation(SK6812Colors,segnode,segcolor_l,4,false,1);
		}
		else if((light_state&0x0001<<9)!=0)
		{
			SK6812_Draw_ColorSegmentation(SK6812Colors,segnode,segcolor_r,4,false,1);
		}
		else
		{
			SK6812_Draw_ColorSegmentation(SK6812Colors,segnode,segcolor_no,4,false,1);
		}
		PAGE5_UpdateColor(SK6812Colors,290);
		PWM2_3_DMA_Enable();
	}

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

///@author YuXin
///@brief draw color in different segment
///@parm allcolors:所有颜色数组
///@parm seg_node:分段节点坐标 向前包含关系 即当前节点为上一段颜色内
///@parm seg_color:段颜色
///@parm seg_nums:节点数
///@parm smooth_flag:颜色平滑flag
///@parm smooth_factor:平滑系数，前后差值最大值
void SK6812_Draw_ColorSegmentation(u8 allcolors[][3],u16 seg_node[],u8 seg_color[][3],u16 seg_nums,bool smooth_flag,u8 smooth_factor)
{//平滑功能必须全部绘制出来后再修改
	int segi=0;
	
	for(segi=0;segi<seg_nums;segi++)	//直接赋值
	{
		int i=0;
		if(segi<seg_nums-1)
		{
			i=seg_node[segi];
			while(i!=seg_node[segi+1])	//向前包含关系 即当前节点为上一段颜色内
			{
				i++;
				i=i>=290?0:i;
				allcolors[i][0]=seg_color[segi][0];
				allcolors[i][1]=seg_color[segi][1];
				allcolors[i][2]=seg_color[segi][2];
			}
		}
		else
		{
			i=seg_node[segi];
			while(i!=seg_node[0])	//向前包含关系 即当前节点为上一段颜色内
			{
				i++;
				i=i>=290?0:i;
				allcolors[i][0]=seg_color[segi][0];
				allcolors[i][1]=seg_color[segi][1];
				allcolors[i][2]=seg_color[segi][2];
			}
		}
		
	}
	
	if(smooth_flag==true)	//平滑使能
	{
		int i=0;
		i=seg_node[0];
		u16 seg_end=seg_node[0]>0?seg_node[0]-1:289;
		while(i!=seg_end)	//向前包含关系 即当前节点为上一段颜色内
		{	//若采用while循环则无法得知当前i属于seg_node第几段，则采用对比前后位value的方法，当坡度小于间距 则会出现达不到最大值效果
			i=i>=290?0:i;
			
			if(i==seg_end)
			{
				break;
			}				
			
			///////////////////////////////////////////////////////////////////////////////
			if(i!=0&&i!=289)
			{
				if(allcolors[i-1][0]==allcolors[i+1][0])	//比较上一颗值与这一颗值判断当前是否需要滤波   && allcolors[i][0]!=seg_color[segi][0]
				{
					allcolors[i][0]=allcolors[i+1][0];
				}   
				else if(allcolors[i-1][0]>allcolors[i+1][0])
				{
					allcolors[i][0]=allcolors[i-1][0]-smooth_factor;
				}
				else if(allcolors[i-1][0]<allcolors[i+1][0])
				{
					allcolors[i][0]=allcolors[i-1][0]+smooth_factor;
				}
				if(abs(allcolors[i][0]-allcolors[i+1][0])<smooth_factor)
				{
					allcolors[i][0]=allcolors[i+1][0];
				}
					//没有用for因为这样会节约一点资源
				if(allcolors[i-1][1]==allcolors[i+1][1])
				{
					allcolors[i][1]=allcolors[i+1][1];
				}
				else if(allcolors[i-1][1]>allcolors[i+1][1])
				{
					allcolors[i][1]=allcolors[i-1][1]-smooth_factor;
				}
				else if(allcolors[i-1][1]<allcolors[i+1][1])
				{
					allcolors[i][1]=allcolors[i-1][1]+smooth_factor;
				}
				if(abs(allcolors[i][1]-allcolors[i+1][1])<smooth_factor)
				{
					allcolors[i][1]=allcolors[i+1][1];
				}
				
					
				if(allcolors[i-1][2]==allcolors[i+1][2])
				{
					allcolors[i][2]=allcolors[i+1][2];
				}
				else if(allcolors[i-1][2]>allcolors[i+1][2])
				{
					allcolors[i][2]=allcolors[i-1][2]-smooth_factor;
				}
				else if(allcolors[i-1][2]<allcolors[i+1][2])
				{
					allcolors[i][2]=allcolors[i-1][2]+smooth_factor;
				}
				if(abs(allcolors[i][2]-allcolors[i+1][2])<smooth_factor)
				{
					allcolors[i][2]=allcolors[i+1][2];
				}
			}
			else if(i==0)	//i=0;需做特殊处理
			{
				if(allcolors[289][0]==allcolors[i+1][0])	//比较上一颗值与这一颗值判断当前是否需要滤波   && allcolors[i][0]!=seg_color[segi][0]
				{
					allcolors[i][0]=allcolors[i+1][0];
				}   
				else if(allcolors[289][0]>allcolors[i+1][0])
				{
					allcolors[i][0]=allcolors[289][0]-smooth_factor;
				}
				else if(allcolors[289][0]<allcolors[i+1][0])
				{
					allcolors[i][0]=allcolors[289][0]+smooth_factor;
				}
				if(abs(allcolors[i][0]-allcolors[i+1][0])<smooth_factor)
				{
					allcolors[i][0]=allcolors[i+1][0];
				}
					//没有用for因为这样会节约一点资源
				if(allcolors[289][1]==allcolors[i+1][1])
				{
					allcolors[i][1]=allcolors[i+1][1];
				}
				else if(allcolors[289][1]>allcolors[i+1][1])
				{
					allcolors[i][1]=allcolors[289][1]-smooth_factor;
				}
				else if(allcolors[289][1]<allcolors[i+1][1])
				{
					allcolors[i][1]=allcolors[289][1]+smooth_factor;
				}
				if(abs(allcolors[i][1]-allcolors[i+1][1])<smooth_factor)
				{
					allcolors[i][1]=allcolors[i+1][1];
				}
				
					
				if(allcolors[289][2]==allcolors[i+1][2])
				{
					allcolors[i][2]=allcolors[i+1][2];
				}
				else if(allcolors[289][2]>allcolors[i+1][2])
				{
					allcolors[i][2]=allcolors[289][2]-smooth_factor;
				}
				else if(allcolors[289][2]<allcolors[i+1][2])
				{
					allcolors[i][2]=allcolors[289][2]+smooth_factor;
				}
				if(abs(allcolors[i][2]-allcolors[i+1][2])<smooth_factor)
				{
					allcolors[i][2]=allcolors[i+1][2];
				}
			}
			else if(i==289)
			{
				
			}

			///////////////////////////////////////////////////////////////////////////
			
			i++;	//不包含最后一个
		}
	}
//	for(int i=0;i<350;i++)
//	{
//		if(smooth_flag==0||i==0)	//第一颗颜色无比较对象
//		{
//			allcolors[i][0]=seg_color[segi][0];
//			allcolors[i][1]=seg_color[segi][1];
//			allcolors[i][2]=seg_color[segi][2];
//		}
//		else
//		{
//			if(allcolors[i-1][0]==seg_color[segi][0])	//比较上一颗值与这一颗值判断当前是否需要滤波
//			{
//				//do nothing
//			}   
//			else if(allcolors[i-1][0]>seg_color[segi][0])
//			{
//				allcolors[i][0]=allcolors[i-1][0]-smooth_factor;
//			}
//			else if(allcolors[i-1][0]<seg_color[segi][0])
//			{
//				allcolors[i][0]=allcolors[i-1][0]+smooth_factor;
//			}
//			if(abs(allcolors[i][0]-seg_color[segi][0])<smooth_factor)
//			{
//				allcolors[i][0]=seg_color[segi][0];
//			}
//				//没有用for因为这样会节约一点资源
//			if(allcolors[i][1]==seg_color[segi][1])
//			{
//				
//			}
//			else if(allcolors[i-1][1]>seg_color[segi][1])
//			{
//				allcolors[i][1]=allcolors[i-1][1]-smooth_factor;
//			}
//			else if(allcolors[i-1][1]<seg_color[segi][1])
//			{
//				allcolors[i][1]=allcolors[i-1][1]+smooth_factor;
//			}
//			if(abs(allcolors[i][1]-seg_color[segi][1])<smooth_factor)
//			{
//				allcolors[i][1]=seg_color[segi][1];
//			}
//			
//				
//			if(allcolors[i][2]==seg_color[segi][2])
//			{
//				
//			}
//			else if(allcolors[i-1][2]>seg_color[segi][2])
//			{
//				allcolors[i][2]=allcolors[i-1][2]-smooth_factor;
//			}
//			else if(allcolors[i-1][2]<seg_color[segi][2])
//			{
//				allcolors[i][2]=allcolors[i-1][2]+smooth_factor;
//			}
//			if(abs(allcolors[i][2]-seg_color[segi][2])<smooth_factor)
//			{
//				allcolors[i][2]=seg_color[segi][2];
//			}
//			
//		}
//		
//		
//		if(seg_node[segi]==i) segi++;
//	}
}
