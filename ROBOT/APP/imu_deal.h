#ifndef __IMU_DEAL_H_
#define __IMU_DEAL_H_

#include "uart4_gyro.h"
#include "stm32f4xx.h"

u8 Imu_Cali_State(void);
void Imu_Offset(float angvel[]);	//�������ǽ��ٶȸ���Ƶ��һ��
#endif
