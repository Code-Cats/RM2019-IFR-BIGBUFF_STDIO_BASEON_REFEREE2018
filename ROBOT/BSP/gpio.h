#ifndef __GPIO_H__
#define __GPIO_H__
#include "main.h"

void LED_Config(void);
void Laser_Config(void);

void KEY_Config(void);

//void Plate_GPIO_Init(void);

#define KEY_PB0 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)
#endif

