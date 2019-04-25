#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "main.h"

#define LF 0
#define RF 1
#define LB 2
#define RB 3

///************�궨���**************/
//#define LIFT_CALI_OUTPUT_MAX 1600//1700	//2018.3.25
//////////////////////////////////////

///************�г�����**************/
//#define LIFT_DISTANCE_FALL 30
//#define LIFT_DISTANCE_ISLAND 1170//1170//1200��ȡ����
//#define LIFT_DISTANCE_LIMIT 1400


//#define LIFT_POSITION_PID_P	55	//405�������PID
//#define LIFT_POSITION_PID_I 0.0
//#define LIFT_POSITION_PID_D	0.015f //	0.15f
//#define LIFT_POSITION_MAXOUT 9000	//λ�û����������ٶ�
//#define LIFT_POSITION_MAXINPUT 99999999
//#define LIFT_POSITION_PID_MER	180 //	20
//#define LIFT_POSITION_PID_DEAD	0	// 0
////�������ԣ�3510���ת��˲ʱֵ8500��һ��������8000

//#define LIFT_SPEED_PID_P	 5
//#define LIFT_SPEED_PID_I	 0.05f
//#define LIFT_SPEED_PID_D	 0.06f
//#define LIFT_SPEED_MAXOUT 9000	//�ٶȻ������������
//#define LIFT_SPEED_MAXINPUT 8400
//#define LIFT_SPEED_PID_MER 	6000
//#define LIFT_SPEED_PID_DEAD	 4
//#define LIFT_SPEED_PID_I_DEAD 	4
//#define LIFT_SPEED_PID_I_MAX 	3000/LIFT_SPEED_PID_I


////�������λ�û�PID����
//#define PID_LIFT_POSITION_DEFAULT \
//{\
//	LIFT_POSITION_PID_P,\
//	LIFT_POSITION_PID_I,\
//  LIFT_POSITION_PID_D,\
//	LIFT_POSITION_PID_MER,\
//	-LIFT_POSITION_MAXINPUT,\
//	LIFT_POSITION_MAXINPUT,\
//	-LIFT_POSITION_MAXOUT,\
//	LIFT_POSITION_MAXOUT,\
//	0,\
//	{0.0,0.0},\
//	0.0,\
//	0.0,\
//	0.0,\
//	0,\
//}\

////��������ٶȻ�PID����
//#define PID_LIFT_SPEED_DEFAULT \
//{\
//	LIFT_SPEED_PID_P,\
//	LIFT_SPEED_PID_I,\
//  LIFT_SPEED_PID_D,\
//	LIFT_SPEED_PID_MER,\
//	-LIFT_SPEED_MAXINPUT,\
//	LIFT_SPEED_MAXINPUT,\
//	-LIFT_SPEED_MAXOUT,\
//	LIFT_SPEED_MAXOUT,\
//	LIFT_SPEED_PID_I_MAX,\
//	{0.0,0.0},\
//	0.0,\
//	0.0,\
//	0.0,\
//	LIFT_SPEED_PID_DEAD,\
//}\



//typedef struct
//{
//	 int32_t lf_lift_fdbV; //201 
//	 int32_t rf_lift_fdbV; //202
//	 int32_t lb_lift_fdbV; //203
//	 int32_t rb_lift_fdbV; //204	  
//	
//   int32_t lf_lift_tarV;
//	 int32_t rf_lift_tarV;
//	 int32_t lb_lift_tarV;
//	 int32_t rb_lift_tarV;
//	  
//   int32_t lf_lift_fdbP;
//	 int32_t rf_lift_fdbP;
//	 int32_t lb_lift_fdbP;
//	 int32_t rb_lift_fdbP;
//	 
//	 int32_t lf_lift_tarP;
//	 int32_t rf_lift_tarP;
//	 int32_t lb_lift_tarP;
//	 int32_t rb_lift_tarP;
//	 
//	 float lf_lift_output;
//	 float rf_lift_output;	
//	 float lb_lift_output;
//	 float rb_lift_output;
//	
//}LIFT_DATA;	//CAN2

typedef struct
{
	u16 state;
}LO;

//extern float Chassis_GYRO[3];

void Control_Task(void);
void Work_State_Change(void);	//RC,PC�Ի���״̬�Ŀ���
void Work_State_Change_BackProtect(void);	//����ĳһ״̬�˳�ʱ��ȷ����״̬��һ���������ƶ���λ
void Work_Execute(void);	//����ִ��
void LED_Indicate(void);	//�ƹ�ָʾ

//void Lift_Task(void);

//u8 Lift_Cali(void);	//�°������궨
void RC_Calibration(void);
//void Lift_Calibration(void);	//��������ϵ�궨
void Motor_Send(void);
void Cali_Output_Limit(float cm_out,float * cali_out);

//void Entirety_PID(const LIFT_DATA * pliftdata,float send[]);	//����PID����		//2018.2.26DEBUG��
//void Chassis_Attitude_Angle_Convert(void);	//�ۺϵó�������̬
//void Lift_Cali_GYRO_Compensate(float cali_send[]);	//���������ǵĵ��̱궨�������

void Teleconltroller_Data_protect(void);	//ң���������Ա��� 

void KeyboardRetset(void);	//���ս���������⣬�ͽ��и�λ����
void Data_Init(void);	//�ں˸�λ����������

/***********************--�궨״̬--**********************/
typedef enum
{
	UP_STATE,	//��ʼ���Ϲ���
	WAIT_STATE,	//�м�ĵȴ��������
	DOWN_STATE,	//���±궨����
}LiftCaliState_e;


/***********************--����״̬--**********************/
typedef enum
{
    PREPARE_STATE,    //�ϵ���ʼ��״̬ 1s������
    CHECK_STATE,			//�Լ�״̬ �ڴ˽׶��ܹ�ͨ���ں�������������ⷢ��ʱ�����Զ��ں�����
		CALI_STATE,    		//У׼״̬�궨
		LOST_STATE,				//Ӳ��֡�ʶ�ʧ
    NORMAL_STATE,			//��������״̬	�����̸�����̨ģʽ
		WAIST_STATE,			//Ť��ģʽ
    STOP_STATE,       //ֹͣ�˶�״̬(��ң�������Ƶı���״̬)
		TAKEBULLET_STATE,	//ȡ��״̬
		PROTECT_STATE,		//����״̬���ڳ���ѭ��ʱִ�У�����⵽�������쳣���ߴ������쳣ʱ�����ģʽ
		ERROR_STATE,			//Ӳ������״̬�������������ڼ���������ʧЧ������ʧ���������޷����������
}WorkState_e;

extern WorkState_e workState;
void SetWorkState(WorkState_e state);
WorkState_e GetWorkState(void);

#endif
