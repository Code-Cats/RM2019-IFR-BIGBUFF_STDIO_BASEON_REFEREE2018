#ifndef __HEARTBEAT_H__
#define __HEARTBEAT_H__
#include "main.h"

void TIM6_Configuration(void);
void TIM6_Start(void);

void Timer_1ms_CallBack(void);

extern int speed;

#endif
