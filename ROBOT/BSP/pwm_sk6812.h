#ifndef __PWM_SK6812_H__
#define __PWM_SK6812_H__

#include "sys.h"
#include <stdbool.h>
#include "delay.h"

#define SK6812_GREEN 0
#define SK6812_RED 1
#define SK6812_BLUE 2

#define PWM2_1_DMA_ENAB 0
#define PWM2_2_DMA_ENAB 1
#define PWM2_3_DMA_ENAB 1
#define PWM2_4_DMA_ENAB 0
#define PWM3_1_DMA_ENAB 1
#define PWM3_2_DMA_ENAB 1
#define PWM3_3_DMA_ENAB 1
#define PWM3_4_DMA_ENAB 0

void Sk6812_Init(void);	//总初始化

void PWM2_Init(void);
void PWM3_Init(void);

#if PWM2_1_DMA_ENAB==1
void PWM2_1_DMA_Init(void);
#endif

#if PWM2_2_DMA_ENAB==1
void PWM2_2_DMA_Init(void);
#endif

#if PWM2_3_DMA_ENAB==1
void PWM2_3_DMA_Init(void);
#endif

#if PWM2_4_DMA_ENAB==1
void PWM2_4_DMA_Init(void);
#endif


#if PWM3_1_DMA_ENAB==1
void PWM3_1_DMA_Init(void);
#endif

#if PWM3_2_DMA_ENAB==1
void PWM3_2_DMA_Init(void);
#endif

#if PWM3_3_DMA_ENAB==1
void PWM3_3_DMA_Init(void);
#endif

#if PWM3_4_DMA_ENAB==1
void PWM3_4_DMA_Init(void);
#endif

//bool PWM2_1_DMA_Enable(void);	//DMA1_S5C3
//bool PWM2_2_DMA_Enable(void);	//DMA1_S6C3

bool PAGE1_UpdateColor(u8 colors[][3],u16 led_nums);	//GRB 高位先发	//PWM3_1
bool PAGE2_UpdateColor(u8 colors[][3],u16 led_nums);	//GRB 高位先发  //PWM3_2
bool PAGE3_UpdateColor(u8 colors[][3],u16 led_nums);	//GRB 高位先发  //PWM3_3
bool PAGE4_UpdateColor(u8 colors[][3],u16 led_nums);	//GRB 高位先发  //PWM3_4
bool PAGE5_UpdateColor(u8 colors[][3],u16 led_nums);	//GRB 高位先发  //PWM2_3

#endif
