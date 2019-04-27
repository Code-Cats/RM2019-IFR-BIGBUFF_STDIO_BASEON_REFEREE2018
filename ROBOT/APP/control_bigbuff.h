#ifndef __CONTROL_BIGBUFF_H
#define __CONTROL_BIGBUFF_H

#include "main.h"

#define PLATE1_LED1_ON											GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define PLATE1_LED1_OFF											GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#define PLATE1_LED2_ON											GPIO_SetBits(GPIOA, GPIO_Pin_0)
#define PLATE1_LED2_OFF											GPIO_ResetBits(GPIOA, GPIO_Pin_0)

#define PLATE2_LED1_ON											GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define PLATE2_LED1_OFF											GPIO_ResetBits(GPIOA, GPIO_Pin_7)
#define PLATE2_LED2_ON											GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define PLATE2_LED2_OFF											GPIO_ResetBits(GPIOA, GPIO_Pin_1)

#define PLATE3_LED1_ON											GPIO_SetBits(GPIOC, GPIO_Pin_8)
#define PLATE3_LED1_OFF											GPIO_ResetBits(GPIOC, GPIO_Pin_8)
#define PLATE3_LED2_ON											GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define PLATE3_LED2_OFF											GPIO_ResetBits(GPIOB, GPIO_Pin_10)

#define PLATE4_LED1_ON											GPIO_SetBits(GPIOC, GPIO_Pin_9)
#define PLATE4_LED1_OFF											GPIO_ResetBits(GPIOC, GPIO_Pin_9)
#define PLATE4_LED2_ON											GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define PLATE4_LED2_OFF											GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define PLATE5_LED1_ON											GPIO_SetBits(GPIOC, GPIO_Pin_2)
#define PLATE5_LED1_OFF											GPIO_ResetBits(GPIOC, GPIO_Pin_2)
#define PLATE5_LED2_ON											GPIO_SetBits(GPIOC, GPIO_Pin_3)
#define PLATE5_LED2_OFF											GPIO_ResetBits(GPIOC, GPIO_Pin_3)

#define PLATE_LED_ALL_OFF										GPIO_ResetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7);GPIO_ResetBits(GPIOB, GPIO_Pin_10|GPIO_Pin_11);GPIO_ResetBits(GPIOC, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_8|GPIO_Pin_9)
#define PLATE_LED_ALL_ON										GPIO_SetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7);GPIO_SetBits(GPIOB, GPIO_Pin_10|GPIO_Pin_11);GPIO_SetBits(GPIOC, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_8|GPIO_Pin_9)
#define PLATE_LED_ALL_TOGGLE									GPIO_ToggleBits(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7);GPIO_ToggleBits(GPIOB, GPIO_Pin_10|GPIO_Pin_11);GPIO_ToggleBits(GPIOC, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_8|GPIO_Pin_9)

void BigBuff_Control_Tack(void);

#endif
