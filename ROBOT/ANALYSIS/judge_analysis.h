#ifndef __JUDGE_ANALYSIS_H_
#define __JUDGE_ANALYSIS_H_

#include "bsp.h"

typedef struct
{
	uint16_t bullet_num;	//������	ֱ�ӷ��������ж����Լ�
	s16 heat;
	u16 maxheat;
}RobotHeatDataSimuTypeDef;	//������������-�������ٸ���

void BulletNum_Calculate(void);
void Heat_MAX_COOL_calc(int* maxheat,float* coolheat,int maxhp);	//���ݲ������Ѫ���Ƴ��ȼ����Ƴ��������޼���ȴֵ

void Heat_Simulating(u16 heating,u8 level);	//��������//����Ƶ��1-10HZ
void BulletNum_Simu_ADD(void);
#endif
