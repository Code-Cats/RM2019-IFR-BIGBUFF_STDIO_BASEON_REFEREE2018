#include "control_bigbuff.h"
#include "time.h"

u8 viceboard_num = 0;

const u8 viceboard_seqience[120][5]={
							{5, 4, 3, 2, 1},
							{5, 4, 3, 1, 2},
							{5, 4, 1, 2, 3},
							{5, 4, 2, 3, 1},
							{5, 4, 1, 3, 2},
							{5, 4, 2, 1, 3},							
							{5, 1, 3, 2, 4},
							{5, 2, 3, 4, 1},
							{5, 3, 4, 2, 1},							
							{5, 3, 4, 1, 2},
							{5, 1, 3, 4, 2},
							{5, 2, 3, 1, 4},							
							{5, 1, 4, 2, 3},
							{5, 2, 1, 4, 3},
							{5, 3, 1, 2, 4},						
							{5, 2, 4, 3, 1},
							{5, 3, 2, 4, 1},
							{5, 1, 2, 3, 4},							
							{5, 1, 4, 3, 2},
							{5, 3, 1, 4, 2},
							{5, 2, 1, 3, 4},							
							{5, 2, 4, 1, 3},
							{5, 1, 2, 4, 3},
							{5, 3, 2, 1, 4},
							
							{4, 5, 3, 2, 1},
							{4, 5, 3, 1, 2},
							{4, 5, 1, 2, 3},
							{4, 5, 2, 3, 1},
							{4, 5, 1, 3, 2},
							{4, 5, 2, 1, 3},							
							{4, 3, 5, 2, 1},
							{4, 2, 3, 5, 1},
							{4, 1, 3, 2, 5},						
							{4, 3, 5, 1, 2},
							{4, 1, 3, 5, 2},
							{4, 2, 3, 1, 5},							
							{4, 1, 5, 2, 3},
							{4, 2, 1, 5, 3},
							{4, 3, 1, 2, 5},							
							{4, 2, 5, 3, 1},
							{4, 3, 2, 5, 1},
							{4, 1, 2, 3, 5},							
							{4, 1, 5, 3, 2},
							{4, 3, 1, 5, 2},
							{4, 2, 1, 3, 5},							
							{4, 2, 5, 1, 3},
							{4, 1, 2, 5, 3},
							{4, 3, 2, 1, 5},
							
							{3, 5, 4, 2, 1},
							{3, 5, 4, 1, 2},
							{3, 5, 1, 2, 4},
							{3, 5, 2, 4, 1},
							{3, 5, 1, 4, 2},
							{3, 5, 2, 1, 4},							
							{3, 4, 5, 2, 1},
							{3, 2, 4, 5, 1},
							{3, 1, 4, 2, 5},						
							{3, 4, 5, 1, 2},
							{3, 1, 4, 5, 2},
							{3, 2, 4, 1, 5},							
							{3, 1, 5, 2, 4},
							{3, 2, 1, 5, 4},
							{3, 4, 1, 2, 5},							
							{3, 2, 5, 4, 1},
							{3, 4, 2, 5, 1},
							{3, 1, 2, 4, 5},							
							{3, 1, 5, 4, 2},
							{3, 4, 1, 5, 2},
							{3, 2, 1, 4, 5},							
							{3, 2, 5, 1, 4},
							{3, 1, 2, 5, 4},
							{3, 4, 2, 1, 5},
							
							{2, 5, 4, 3, 1},
							{2, 5, 4, 1, 3},
							{2, 5, 1, 3, 4},
							{2, 5, 3, 4, 1},
							{2, 5, 1, 4, 3},
							{2, 5, 3, 1, 4},							
							{2, 4, 5, 3, 1},
							{2, 3, 4, 5, 1},
							{2, 1, 4, 3, 5},						
							{2, 4, 5, 1, 3},
							{2, 1, 4, 5, 3},
							{2, 3, 4, 1, 5},							
							{2, 1, 5, 3, 4},
							{2, 3, 1, 5, 4},
							{2, 4, 1, 3, 5},							
							{2, 3, 5, 4, 1},
							{2, 4, 3, 5, 1},
							{2, 1, 3, 4, 5},							
							{2, 1, 5, 4, 3},
							{2, 4, 1, 5, 3},
							{2, 3, 1, 4, 5},							
							{2, 3, 5, 1, 4},
							{2, 1, 3, 5, 4},
							{2, 4, 3, 1, 5},
							
							{1, 5, 4, 3, 2},
							{1, 5, 4, 2, 3},
							{1, 5, 2, 3, 4},
							{1, 5, 3, 4, 2},
							{1, 5, 2, 4, 3},
							{1, 5, 3, 2, 4},							
							{1, 4, 5, 3, 2},
							{1, 3, 4, 5, 2},
							{1, 2, 4, 3, 5},						
							{1, 4, 5, 2, 3},
							{1, 2, 4, 5, 3},
							{1, 3, 4, 2, 5},							
							{1, 2, 5, 3, 4},
							{1, 3, 2, 5, 4},
							{1, 4, 2, 3, 5},							
							{1, 3, 5, 4, 2},
							{1, 4, 3, 5, 2},
							{1, 2, 3, 4, 5},							
							{1, 2, 5, 4, 3},
							{1, 4, 2, 5, 3},
							{1, 3, 2, 4, 5},							
							{1, 3, 5, 2, 4},
							{1, 2, 3, 5, 4},
							{1, 4, 3, 2, 5}
};

u8 alreadly_count = 0;
u8 plate_sequence = 0;
u8 ready_flag = 1;
u8 last_number = 0;
void Work_Normal(void)
{
	extern u32 time_1ms_count;
	static u8 select_viceboard_flag = 1;
	static u16 all_viceboard_shoot = 0;
	static u16 wait_shoot_time = 0;
		
	if(ready_flag)
	{
		if(alreadly_count<5)
		{
			if(select_viceboard_flag)
			{
				plate_sequence = rand()%120;//time_1ms_count % 120;
				while(last_number == viceboard_seqience[plate_sequence][0])
				{
					plate_sequence = rand()%120;
				}
				last_number = viceboard_seqience[plate_sequence][0];
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
					case 5:PLATE5_LED1_ON; break;
					default: break;
				}
				wait_shoot_time = 0;
				ready_flag = 0;
			}
		}
		else
		{
			all_viceboard_shoot++;		
			if(all_viceboard_shoot > 3500)
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
