#ifndef __CONTROL_BIGBUFF_H
#define __CONTROL_BIGBUFF_H

#include "main.h"

#define digitalHi(p,i)											GPIO_SetBits(p, i)
#define digitalLo(p,i)											GPIO_ResetBits(p, i)
#define digitalToggle(p,i)										GPIO_ToggleBits(p, i)

#define PLATE1_LED1_ON											digitalHi(GPIOA, GPIO_Pin_6)
#define PLATE1_LED1_OFF											digitalLo(GPIOA, GPIO_Pin_6)
#define PLATE1_LED2_ON											digitalHi(GPIOA, GPIO_Pin_0)
#define PLATE1_LED2_OFF											digitalLo(GPIOA, GPIO_Pin_0)

#define PLATE2_LED1_ON											digitalHi(GPIOA, GPIO_Pin_7)
#define PLATE2_LED1_OFF											digitalLo(GPIOA, GPIO_Pin_7)
#define PLATE2_LED2_ON											digitalHi(GPIOA, GPIO_Pin_1)
#define PLATE2_LED2_OFF											digitalLo(GPIOA, GPIO_Pin_1)

#define PLATE3_LED1_ON											digitalHi(GPIOC, GPIO_Pin_8)
#define PLATE3_LED1_OFF											digitalLo(GPIOC, GPIO_Pin_8)
#define PLATE3_LED2_ON											digitalHi(GPIOB, GPIO_Pin_10)
#define PLATE3_LED2_OFF											digitalLo(GPIOB, GPIO_Pin_10)

#define PLATE4_LED1_ON											digitalHi(GPIOC, GPIO_Pin_9)
#define PLATE4_LED1_OFF											digitalLo(GPIOC, GPIO_Pin_9)
#define PLATE4_LED2_ON											digitalHi(GPIOB, GPIO_Pin_11)
#define PLATE4_LED2_OFF											digitalLo(GPIOB, GPIO_Pin_11)

#define PLATE_LED_ALL_OFF										digitalLo(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7);digitalLo(GPIOB, GPIO_Pin_10|GPIO_Pin_11);digitalLo(GPIOC, GPIO_Pin_8|GPIO_Pin_9)
#define PLATE_LED_ALL_ON										digitalHi(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7);digitalHi(GPIOB, GPIO_Pin_10|GPIO_Pin_11);digitalHi(GPIOC, GPIO_Pin_8|GPIO_Pin_9)
#define PLATE_LED_ALL_TOGGLE									digitalToggle(GPIOA, GPIO_Pin_0)

void BigBuff_Control_Tack(void);

#endif
