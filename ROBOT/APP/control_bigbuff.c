#include "control_bigbuff.h"


u8 viceboard_num = 0;

const u8 viceboard_seqience[24][4]={
							{4, 3, 2, 1},
							{4, 3, 1, 2},
							{4, 2, 3, 1},
							{4, 2, 1, 3},
							{4, 1, 3, 2},
							{4, 1, 2, 3},
							{3, 4, 2, 1},
							{3, 4, 1, 2},
							{3, 2, 4, 1},
							{3, 2, 1, 4},
							{3, 2, 4, 1},
							{3, 2, 1, 4},
							{2, 3, 4, 1},
							{2, 3, 1, 4},
							{2, 4, 3, 1},
							{2, 4, 1, 3},
							{2, 1, 3, 4},
							{2, 1, 4, 3},
							{1, 3, 2, 4},
							{1, 3, 4, 2},
							{1, 2, 3, 4},
							{1, 2, 4, 3},
							{1, 4, 3, 2},
							{1, 4, 2, 3},

};

u8 alreadly_count = 0;
u8 plate_sequence = 0;
u8 ready_flag = 1;

void Work_Normal(void)
{
	extern u32 time_1ms_count;
	static u8 select_viceboard_flag = 1;
	static u16 all_viceboard_shoot = 0;
	static u16 wait_shoot_time = 0;
		
	if(ready_flag)
	{
		if(alreadly_count<4)
		{
			if(select_viceboard_flag)
			{
				plate_sequence = time_1ms_count % 24;
				select_viceboard_flag = 0;

			}
			else
			{
			
				viceboard_num = viceboard_seqience[plate_sequence][alreadly_count];

				switch(viceboard_num)
				{
					case 1:PLATE1_LED1_ON; break;
					case 2:PLATE2_LED1_ON; break;
					case 3:PLATE3_LED1_ON; break;
					case 4:PLATE4_LED1_ON; break;
					default: break;
				}
				wait_shoot_time = 0;
				ready_flag = 0;
			}
		}
		else
		{
			all_viceboard_shoot++;		
			if(all_viceboard_shoot > 2000)
			{
				all_viceboard_shoot = 0;
				
				PLATE_LED_ALL_OFF;
				select_viceboard_flag = 1;
				alreadly_count = 0;
				wait_shoot_time = 0;
				
				
			}
			else
			{
				if(all_viceboard_shoot%500==0)
				{
					PLATE_LED_ALL_TOGGLE;
				}
			}
				
		}
		
	}
	else
	{
		if(wait_shoot_time <2500)
			wait_shoot_time++;
		else
		{
			wait_shoot_time = 0;
			ready_flag = 1;		
			select_viceboard_flag = 1;
			alreadly_count = 0;

			PLATE_LED_ALL_OFF;
		
		}
	}
			
		
}


void BigBuff_Control_Tack(void)
{

	Work_Normal();
}
