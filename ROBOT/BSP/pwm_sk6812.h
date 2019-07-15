#ifndef __PWM_SK6812_H__
#define __PWM_SK6812_H__

#include "sys.h"
#include <stdbool.h>
#include "delay.h"

#define SK6812_GREEN 0
#define SK6812_RED 1
#define SK6812_BLUE 2

void Sk6812_Init(void);	//总初始化

void PWM2_Init(void);
void PWM2_1_DMA_Init(void);
void PWM2_2_DMA_Init(void);
bool PWM2_1_DMA_Enable(void);	//DMA1_S5C3
bool PWM2_2_DMA_Enable(void);	//DMA1_S6C3

bool PAGE1_UpdateColor(u8 colors[][3],u16 led_nums);	//GRB 高位先发	//PWM2_1
bool PAGE2_UpdateColor(u8 colors[][3],u16 led_nums);	//GRB 高位先发  //PWM2_2


#endif
