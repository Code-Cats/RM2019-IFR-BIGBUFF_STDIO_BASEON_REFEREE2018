#include "control_bigbuff.h"
#include "time.h"


extern u32 time_1ms_count;

u8 bigbuff_lightstate=0;
u8 bigbuff_armorstate[5]={1,0,0,0,0};	//0未激活  1待激活 2已激活
u8 bigbuff_activate_flag=0;
u32 bigbuff_timeout=0;

u8 changecolor_flag=0xe0;;//0xe0
extern u8 BIGBUFF_COLOR;

void Bigbuff_Failed_Deal(void)	//大风车击打失败处理函数
{
	static int last_armorid=0;
	srand(time_1ms_count);
	for(int i=0;i<5;i++)
	{
		bigbuff_armorstate[i]=0;
	}
	bigbuff_timeout=0;
	int armor_id=rand()%5;
	while(armor_id==last_armorid)
	{
		armor_id=rand()%5;
	}
	bigbuff_armorstate[armor_id]=1;
}

void Bigbuff_HitSucced_Deal(void)	//从待激活到已激活放到回调里处理
{
	bigbuff_timeout=0; 
	srand(time_1ms_count);
	int armor_id=rand()%5;
	
	if(bigbuff_armorstate[0]==2&&\
	bigbuff_armorstate[1]==2&&\
	bigbuff_armorstate[2]==2&&\
	bigbuff_armorstate[3]==2&&\
	bigbuff_armorstate[4]==2)
	{
		bigbuff_activate_flag=1;
	}
	else
	{
		while(bigbuff_armorstate[armor_id]!=0)
		{
			armor_id=rand()%5;
		}
		bigbuff_armorstate[armor_id]=1;
	}
	
}

//定时器 1000HZ
void BigBuff_Control_Tack(void)
{
	bigbuff_timeout++;
	
	if(bigbuff_activate_flag==1)
	{
		static u8 succsedlightstate=0;
		if(bigbuff_timeout>4000)	//超时处理
		{
			Bigbuff_Failed_Deal();
			bigbuff_activate_flag=0;
			changecolor_flag=0xe0;
		}
		
		if(time_1ms_count%100==0)
		{
			succsedlightstate=!succsedlightstate;
		}
		
		for(int i=0;i<5;i++)
		{
			bigbuff_armorstate[i]=2*succsedlightstate;
		}
	}
	else
	{
		if(bigbuff_timeout>1500)	//超时处理
		{
			Bigbuff_Failed_Deal();
		}
	}
}


//打击回调
void ArmorHit_Deal(AimorIDEnum id)
{
	bigbuff_timeout=0;
	if(bigbuff_activate_flag==0)
	{
		switch(id)
		{
			case AIMORID_240:
			{
				if(bigbuff_armorstate[0]!=1)
				{
					Bigbuff_Failed_Deal();
				}
				else
				{
					bigbuff_armorstate[0]=2;
					Bigbuff_HitSucced_Deal();
				}
				break;
			}
			case AIMORID_241:
			{
				if(bigbuff_armorstate[1]!=1)
				{
					Bigbuff_Failed_Deal();
				}
				else
				{
					bigbuff_armorstate[1]=2;
					Bigbuff_HitSucced_Deal();
				}
				break;
			}
			case AIMORID_242:
			{
				if(bigbuff_armorstate[2]!=1)
				{
					Bigbuff_Failed_Deal();
				}
				else
				{
					bigbuff_armorstate[2]=2;
					Bigbuff_HitSucced_Deal();
				}
				break;
			}
			case AIMORID_243:
			{
				if(bigbuff_armorstate[3]!=1)
				{
					Bigbuff_Failed_Deal();
				}
				else
				{
					bigbuff_armorstate[3]=2;
					Bigbuff_HitSucced_Deal();
				}
				break;
			}
			case AIMORID_244:
			{
				if(bigbuff_armorstate[4]!=1)
				{
					Bigbuff_Failed_Deal();
				}
				else
				{
					bigbuff_armorstate[4]=2;
					Bigbuff_HitSucced_Deal();
				}
				break;
			}
			default:
			{
				
				break;
			}
		}

	}
	else	//已激活待切换颜色
	{
		changecolor_flag|=0x01<<id;//记录激活后敲打了哪些装甲板
		if(changecolor_flag==0xf5)
		{
			BIGBUFF_COLOR=CYAN;
		}
		else if(changecolor_flag==0xea)
		{
			BIGBUFF_COLOR=ORANGE;
		}
	}
}
