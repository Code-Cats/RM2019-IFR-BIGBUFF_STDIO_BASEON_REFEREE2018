#ifndef __YUN_H__

#define __YUN_H__

#include "main.h"

#define  YAW_INIT_DEFINE 3525//3160
//s32 YAW_INIT=YAW_INIT_DEFINE;
extern s32 YAW_INIT;
#define PITCH_INIT         3045//3098
#define PITCH_GYRO_INIT 3045//3098	//��̨����������Ӧ���λ�ã������Ǹ���λ����Ҫ�ص�	//������̫С��˵�����̫����Ҫ��

#define 		Yaw_MAX 			   YAW_INIT+1000
#define 		Yaw_MIN				   YAW_INIT-1000
#define 		Pitch_MAX 		   PITCH_INIT+250
#define 		Pitch_MIN 		   PITCH_INIT-600

#define RC_CONTROL 0	//ң��������ģʽ
#define PC_CONTROL 1	//���̲���ģʽ


#define YUN_MOTOR_DATA_DEFAULT \
{\
	0,\
	0,\
	0,\
	PITCH_INIT,\
	0,\
	0,\
	0,\
	0,\
	YAW_INIT_DEFINE,\
	0,\
}


#define YUN_DATA_DEFAULT  {0};


/*PID����-���������*/
#define PITCH_POSITION_PID_P 0.80//0.83
#define PITCH_POSITION_PID_I 0.008f
#define PITCH_POSITION_PID_D 1.1f	//1.2
#define PITCH_POSITION_PID_MER 400	//���ƫ������
#define PITCH_POSITION_PID_MAXINPUT 8190	//�����������
#define PITCH_POSITION_PID_MAXOUTPUT 320	//������
#define PITCH_POSITION_PID_I_MAX 22/PITCH_POSITION_PID_I	//I�ۼ�����
#define PITCH_POSITION_PID_DEAD 0	//��������3
//������¼��
#define PITCH_SPEED_PID_P 25
#define PITCH_SPEED_PID_I 0.01f//0.014	//0.026
#define PITCH_SPEED_PID_D 1	//0.08
#define PITCH_SPEED_PID_MER 260	//���ƫ������240
#define PITCH_SPEED_PID_MAXINPUT 320	//�����������
#define PITCH_SPEED_PID_MAXOUTPUT 6000	//����7000�����ת
#define PITCH_SPEED_PID_I_MAX 600/PITCH_SPEED_PID_I	//I�ۼ�����1200
#define PITCH_SPEED_PID_DEAD 0	//��������3

///*PID����-����������*/
//#define PITCH_POSITION_PID_P 0.92	//0.95
//#define PITCH_POSITION_PID_I 0
//#define PITCH_POSITION_PID_D 2	//1.2
//#define PITCH_POSITION_PID_MER 250	//���ƫ������
//#define PITCH_POSITION_PID_MAXINPUT 6700	//�����������
//#define PITCH_POSITION_PID_MAXOUTPUT 250	//������
//#define PITCH_POSITION_PID_I_MAX 0	//I�ۼ�����
//#define PITCH_POSITION_PID_DEAD 1	//��������3
////������¼��
//#define PITCH_SPEED_PID_P 17.5
//#define PITCH_SPEED_PID_I 0.028	//0.03
//#define PITCH_SPEED_PID_D 2	//0.08
//#define PITCH_SPEED_PID_MER 240	//���ƫ������300
//#define PITCH_SPEED_PID_MAXINPUT 240	//�����������
//#define PITCH_SPEED_PID_MAXOUTPUT 5300	//������5500
//#define PITCH_SPEED_PID_I_MAX 450/PITCH_SPEED_PID_I	//I�ۼ�����1200
//#define PITCH_SPEED_PID_DEAD 1	//��������3


#define YAW_SPEED_PID_P 45//44//45	//30  
#define YAW_SPEED_PID_I 0.025//0.03	//0.02
#define YAW_SPEED_PID_D 1
#define YAW_SPEED_PID_MER 260	//���ƫ������
#define YAW_SPEED_PID_MAXINPUT 320	//�����������
#define YAW_SPEED_PID_MAXOUTPUT 7000	//������
#define YAW_SPEED_PID_I_MAX 1500/YAW_SPEED_PID_I	//I�ۼ�����
#define YAW_SPEED_PID_DEAD 0	//��������

#define YAW_POSITION_PID_P 1.3f//1.4//1.2  
#define YAW_POSITION_PID_I 0.004f
#define YAW_POSITION_PID_D 1	//����ʵ��۲����ݷ���D��������tarV�������󣬵�D���ڼ������γ����²���Ч�ã����÷�����D	//ԭֵ6
#define YAW_POSITION_PID_MER 280	//���ƫ������
#define YAW_POSITION_PID_MAXINPUT 5350	//�����������
#define YAW_POSITION_PID_MAXOUTPUT 300	//������
#define YAW_POSITION_PID_I_MAX 15/YAW_POSITION_PID_I	//I�ۼ�����
#define YAW_POSITION_PID_DEAD 0	//��������

//PITCH���λ�û�����
#define PID_PITCH_POSITION_DEFAULT \
{\
	PITCH_POSITION_PID_P,\
	PITCH_POSITION_PID_I,\
  PITCH_POSITION_PID_D,\
	PITCH_POSITION_PID_MER,\
	-PITCH_POSITION_PID_MAXINPUT,\
	PITCH_POSITION_PID_MAXINPUT,\
	-PITCH_POSITION_PID_MAXOUTPUT,\
	PITCH_POSITION_PID_MAXOUTPUT,\
	PITCH_POSITION_PID_I_MAX,\
	{0.0,0.0},\
	0.0,\
	0.0,\
	0.0,\
	PITCH_POSITION_PID_DEAD,\
}\

//PITCH����ٶȻ�����
#define PID_PITCH_SPEED_DEFAULT \
{\
	PITCH_SPEED_PID_P,\
	PITCH_SPEED_PID_I,\
  PITCH_SPEED_PID_D,\
	PITCH_SPEED_PID_MER,\
	-PITCH_SPEED_PID_MAXINPUT,\
	PITCH_SPEED_PID_MAXINPUT,\
	-PITCH_SPEED_PID_MAXOUTPUT,\
	PITCH_SPEED_PID_MAXOUTPUT,\
	PITCH_SPEED_PID_I_MAX,\
	{0.0,0.0},\
	0.0,\
	0.0,\
	0.0,\
	PITCH_SPEED_PID_DEAD,\
}\

//YAW����ٶȻ�����
#define PID_YAW_SPEED_DEFAULT \
{\
	YAW_SPEED_PID_P,\
	YAW_SPEED_PID_I,\
  YAW_SPEED_PID_D,\
	YAW_SPEED_PID_MER,\
	-YAW_SPEED_PID_MAXINPUT,\
	YAW_SPEED_PID_MAXINPUT,\
	-YAW_SPEED_PID_MAXOUTPUT,\
	YAW_SPEED_PID_MAXOUTPUT,\
	YAW_SPEED_PID_I_MAX,\
	{0.0,0.0},\
	0.0,\
	0.0,\
	0.0,\
	YAW_SPEED_PID_DEAD,\
}\

//YAW���λ�û�����
#define PID_YAW_POSITION_DEFAULT \
{\
	YAW_POSITION_PID_P,\
	YAW_POSITION_PID_I,\
  YAW_POSITION_PID_D,\
	YAW_POSITION_PID_MER,\
	-YAW_POSITION_PID_MAXINPUT,\
	YAW_POSITION_PID_MAXINPUT,\
	-YAW_POSITION_PID_MAXOUTPUT,\
	YAW_POSITION_PID_MAXOUTPUT,\
	YAW_POSITION_PID_I_MAX,\
	{0.0,0.0},\
	0.0,\
	0.0,\
	0.0,\
	YAW_POSITION_PID_DEAD,\
}\

typedef struct  __YUN_MOTOR_DATA__
{
		int32_t   pitch_fdbP;  //��������
		int32_t   pitch_fdbV;
		int32_t   pitch_tarV;
		float   pitch_tarP;	//�����ԽǶ�Ϊ��λ
    int32_t   pitch_output;

		int32_t   yaw_fdbP;  //��������
		int32_t   yaw_fdbV;
		int32_t   yaw_tarV;
		float   yaw_tarP;
    int32_t   yaw_output;
}YUN_MOTOR_DATA;

typedef struct  __YUN_DATA__
{
	float pitch_move;
	float yaw_move;

	char Turo_State;
	char Fire_State;
	char Right_State;
	char Left_State;
	int16_t value_mouse_X;
	int16_t value_mouse_Y;
}YUN_DATA;

s32 yun_pitch_tarV(s32 now_V);

float Yaw_Angle_Calculate(int16_t src_angle,int16_t Init);
void Yun_Init(void);
s32 Yaw_output_offset(s32 yaw_fbdP);	//�˷���̨yaw������������ǶԳ��ԵĲ��� //��Ȼyaw��̨����������������������Ϊ�����ﲢ��Ϊ��̨�������Ӧ���õ�ǰ����λ��������
s16 Pitch_output_offset(s32 pitch_tarP);	//�˷���̨pitch������������ǶԳ��ԵĲ���	//��Ϊ��̨pitch���������������������ⲿ���������ֻ����һ����Сֵ�����ʲ���tarP��Ϊ�������տ�����߼�ӷ�Ӧ�ٶ�

void Yun_Task(void);	//��̨�������� 
void Yun_Control_External_Solution(void);	//���÷�������
void Yun_Control_Inscribe_Solution(void);	//�ڽӷ�������
//void Yun_WorkState_Turn_Task(void);	//ģʽ�л�ʱ��̨ת������

s32 t_yun_yaw_tarV(s32 now_V);	//���ٶȵ�һ���ͺ��˲�

void __yun_yaw_offset(void);

void RC_Control_Yun(float * yaw_tarp,float * pitch_tarp);	//1000Hz
void PC_Control_Yun(float * yaw_tarp,float * pitch_tarp);	//1000Hz


#endif

