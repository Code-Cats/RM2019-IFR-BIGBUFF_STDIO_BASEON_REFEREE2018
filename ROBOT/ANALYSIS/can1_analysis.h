#ifndef __CAN1_ANALYSIS__
#define __CAN1_ANALYSIS__

#include "main.h"


typedef struct
{
	s16 real[8];
//	s16 sum;
	s16 calc;	//	����һƽ���ĽǶ���ֵ
	s16 calc_diff;
	s16 calc_last;	//��һ�μ���ĽǶ�ֵ
	s16 turns;	//�������νǶ�ֵ֮��ó���Ȧ�����ֵ
}LIFT_POSITION_ENCODER;	//CAN1


void CAN1_Feedback_Analysis(CanRxMsg *rx_message);
//void CAN1_Lift_SendMsg(int motor_201,int motor_202,int motor_203,int motor_204);
void CAN1_Yun_SendMsg(int16_t motor_205,int16_t motor_206);	//yaw  pitch
void CAN2_Shoot_Bullet_SendMsg(int16_t motor_201,int16_t motor_202,int16_t motor_203,int16_t motor_204);
void CAN_Motor6623_calibration(void);

void Position_Data_deal_DIV8(s32 * value,LIFT_POSITION_ENCODER *Receive,CanRxMsg * msg);	//�ֱ���ת��1/8Ȧ
void Position_Data_deal_DIV81(s32 * value,LIFT_POSITION_ENCODER *Receive,CanRxMsg * msg);	//�ֱ���ת��1/81Ȧ
void Position_To_Turns(LIFT_POSITION_ENCODER *Receive);	//����6�����������㣬��е�Ƕȹ�8192����λ���������ֲ�ֵΪ6826
void Speed_Data_deal(s32 * fdbV,CanRxMsg * msg);

s16 Motor_6623_GetSpeed(s16 pos_now);	//����6623λ�ý����ٶ�

#endif

