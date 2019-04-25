#include "yun.h"
//#include "usart1_remote_analysis.h"
//#include "vision.h"
s32 YAW_INIT=YAW_INIT_DEFINE;
/*
����ṹ��yaw���ݶ������ٶȻ�//���ڼƻ��������������λ�û�����ѡ��
pitch��λ�û�

����������Ϣ�����������Ϊ
yaw�ٶȣ�����λ��
pitchλ��


*/

#define YUN_PITCH_TAKEBULLET	3795

YUN_MOTOR_DATA 			yunMotorData=YUN_MOTOR_DATA_DEFAULT;
YUN_DATA          	yunData=YUN_DATA_DEFAULT;
/**************************************************************************************************************************************
PID_GENERAL          PID_PITCH_POSITION=PID_PITCH_POSITION_DEFAULT;
PID_GENERAL          PID_PITCH_SPEED=PID_PITCH_SPEED_DEFAULT;
PID_GENERAL          PID_YAW_POSITION=PID_YAW_POSITION_DEFAULT;
PID_GENERAL          PID_YAW_SPEED=PID_YAW_SPEED_DEFAULT;

extern  FIRST_ORDER_FILTER   FILTER_MOUSE_YAW;
extern  FIRST_ORDER_FILTER   FILTER_WAIST_YAW;
extern  MPU6050_REAL_DATA    MPU6050_Real_Data;
extern	RC_Ctl_t RC_Ctl;
extern KeyBoardTypeDef KeyBoardData[KEY_NUMS];
extern GYRO_DATA Gyro_Data;
extern u8 Chassis_Follow_Statu;	//���̸����־λ
extern volatile float yaw_follow_real_error;	//Ť��ʱ�ĵ��̸���ƫ��
extern float yaw_follow_error;	//��ͨʱ�ĵ��̸������

extern VisionDataTypeDef	VisionData;

extern u8 Replenish_Bullet_Statu;	//����״̬λ
***************************************************************************************************************************************



extern u32 time_1ms_count;
s32 t_pitch____=0;
s32 t_yaw___=0;
void Yun_Task(void)	//��̨�������� 
{
	if(IMU_Check_Useless_State==0)
	{
		Yun_Control_External_Solution();	//������λ�û�
	}
	else
	{
		Yun_Control_Inscribe_Solution();	//��������λ�÷������ˣ��Ͳ����ٶȻ�����
	}
}

u8 Yun_WorkState_Turn180_statu=0;	//180��ת��λ��־λ������������
u8 Yun_Control_RCorPC=RC_CONTROL;
u8 yun_control_pcorrc_last=RC_CONTROL;	//��¼��һ�ο���ģʽ���������л�ʱ��ĳЩ���ݽ��д���	//����ʱ�ܹ����⣬���ļܹ����Բ��øñ���
void Yun_Control_External_Solution(void)	//���÷�������
{
	static WorkState_e State_Record=CHECK_STATE;
	
	if(GetWorkState()!=PREPARE_STATE&&GetWorkState()!=CALI_STATE)	//ģʽ�л�
	{
		if(RC_Ctl.mouse.press_l==1||RC_Ctl.mouse.press_r==1||RC_Ctl.mouse.x>1||RC_Ctl.mouse.y>1)
		{
			Yun_Control_RCorPC=PC_CONTROL;
		}
		else if(abs(RC_Ctl.rc.ch2-1024)>3||abs(RC_Ctl.rc.ch3-1024)>3)
		{
			Yun_Control_RCorPC=RC_CONTROL;
		}
	}
	
//	Yun_WorkState_Turn_Task();	//�°�����ȡ��//ȡ��ʱ��̨ת���־λ
	
//	if(VisionData.vision_control_state==0)	//�����Ӿ����źŲ��ܿ�	//���ڵײ�PC_Control
//	{
		if(GetWorkState()==NORMAL_STATE||GetWorkState()==WAIST_STATE)	//��������ģʽ���ܿ�	//ȡ���ܿ�Ϊ��ʱ���룬֮���Դ������Զ�����	//ȡ���ܿ���ȡ������̨�������
		{
			if(Yun_Control_RCorPC==PC_CONTROL)
			{	//PC��������
				PC_Control_Yun(&yunMotorData.yaw_tarP,&yunMotorData.pitch_tarP);
			}
			else if(Yun_Control_RCorPC==RC_CONTROL)
			{	//RC��������
				RC_Control_Yun(&yunMotorData.yaw_tarP,&yunMotorData.pitch_tarP);
			}
		}

//	}
//	else
//	{ 
//		
//	}
	
	
	yun_control_pcorrc_last=Yun_Control_RCorPC;
	
	if(GetWorkState()==TAKEBULLET_STATE)	//�°�ȡ������ת180//ȡ��ģʽ���Ѿ�ת��180��һֱУ׼		//����ȡ��180��
	{
		yunMotorData.pitch_tarP=YUN_PITCH_TAKEBULLET;	//
		yunMotorData.yaw_tarP=(Gyro_Data.angle[YAW]*10+(YAW_INIT-yunMotorData.yaw_fdbP)*3600/8192);	//�����Ŵ�10������Ŀ��λ����Ϊ�е�
	}
	else if(GetWorkState()==NORMAL_STATE&&Replenish_Bullet_Statu==1)
	{
		yunMotorData.yaw_tarP=(Gyro_Data.angle[YAW]*10+(YAW_INIT-yunMotorData.yaw_fdbP)*3600/8192);	//�����Ŵ�10������Ŀ��λ����Ϊ�е�
	}
	
	if(GetWorkState()==STOP_STATE||GetWorkState()==LOST_STATE||GetWorkState()==PROTECT_STATE||GetWorkState()==CALI_STATE)	//ֹͣģʽ�·�ֹ���Ư��
	{
		yunMotorData.yaw_tarP=(Gyro_Data.angle[YAW]*10+(YAW_INIT-yunMotorData.yaw_fdbP)*3600/8192);	//�����Ŵ�10������Ŀ��λ����Ϊ�е�
	}

	if(State_Record==TAKEBULLET_STATE&&GetWorkState()!=TAKEBULLET_STATE)
	{
		yunMotorData.pitch_tarP=PITCH_INIT;
	}
	/////////////////////
//	if(KeyBoardData[KEY_F].value==1&&time_1ms_count%10==0)
	if(time_1ms_count%10==0)		//�������һ��ˢ�¿���=λ�����ã����򴮿����жϻ�����
	{
		Vision_Task(&yunMotorData.yaw_tarP,&yunMotorData.pitch_tarP);
	}
	
	if(VisionData.vision_control_state==1)	//������ģʽ������PID
	{
		PID_PITCH_POSITION.k_p=PITCH_POSITION_PID_P*1.2f;
		PID_PITCH_POSITION.k_i=PITCH_POSITION_PID_I*1.5f;
		PID_PITCH_POSITION.i_sum_max=PITCH_POSITION_PID_I_MAX*2;
	}
	else
	{
		PID_PITCH_POSITION.k_p=PITCH_POSITION_PID_P;
		PID_PITCH_POSITION.k_i=PITCH_POSITION_PID_I;
		PID_PITCH_POSITION.i_sum_max=PITCH_POSITION_PID_I_MAX;
	}
	////////////////////////////
	
	yunMotorData.pitch_tarV=PID_General(yunMotorData.pitch_tarP,(yunMotorData.pitch_fdbP),&PID_PITCH_POSITION);
		
	if(yunMotorData.yaw_tarP-Gyro_Data.angle[YAW]*10>1800)	//�����
	{
		yunMotorData.yaw_tarV=PID_General(yunMotorData.yaw_tarP,Gyro_Data.angle[YAW]*10+3600,&PID_YAW_POSITION);
	}
	else if(yunMotorData.yaw_tarP-Gyro_Data.angle[YAW]*10<-1800)
	{
		yunMotorData.yaw_tarV=PID_General(yunMotorData.yaw_tarP,Gyro_Data.angle[YAW]*10-3600,&PID_YAW_POSITION);
	}
	else
	{
		yunMotorData.yaw_tarV=PID_General(yunMotorData.yaw_tarP,Gyro_Data.angle[YAW]*10,&PID_YAW_POSITION);
	}
	
//	if(KeyBoardData[KEY_SHIFT].value==1)	//�������汾��չ��̨��Χ
//	{
//		Yun_Pitch_Extension(yunMotorData.pitch_tarP);
//	}
	
	
	yunMotorData.pitch_output=-PID_General(yunMotorData.pitch_tarV,(Gyro_Data.angvel[PITCH]),&PID_PITCH_SPEED);
	yunMotorData.yaw_output=-PID_General(yunMotorData.yaw_tarV,(Gyro_Data.angvel[YAW]),&PID_YAW_SPEED);	//�������������������
	
	State_Record=GetWorkState();
}




void RC_Control_Yun(float * yaw_tarp,float * pitch_tarp)	//1000Hz
{
	if(VisionData.vision_control_state==0)	//û���Ӿ�����ʱ�ſɿ�
	{
		if(time_1ms_count%15==0)	//66.67hz
		{
			yunMotorData.yaw_tarP+=((RC_Ctl.rc.ch2-1024)*27.0/660.0);	//35.0/660.0 ͼ����ʱ���� ��С
			yunMotorData.yaw_tarP=yunMotorData.yaw_tarP>1800?yunMotorData.yaw_tarP-3600:yunMotorData.yaw_tarP;	//�����
			yunMotorData.yaw_tarP=yunMotorData.yaw_tarP<-1800?yunMotorData.yaw_tarP+3600:yunMotorData.yaw_tarP;	//�����
		}
		
		yunMotorData.pitch_tarP=((RC_Ctl.rc.ch3-1024)*460.0/660.0)+PITCH_INIT;	//-50����Ϊ������ˮƽʱ��̨����
	}
}

u32 test_turn_count=0;
//#define PITCH_INIT         3098	//2018.7.10
#define YUN_DOWN_VALUELIMIT 2765	//������λ
#define YUN_UP_VALUELIMIT 3650	//������λ
#define YUN_UP_DISLIMIT 552	//�����Ļ��Χ��UPΪ��
#define YUN_DOWN_DISLIMIT 333	//�����Ļ��Χ��DOWNΪ��
//u8 keyQ_last,keyE_last=0;	//��ʱ����
#define YUN_UPMAX_EXTENSION (YUN_UPMAX+200)	//�����Ļ��Χ��UPΪ��
#define YUN_DOWNMAX_EXTENSION (YUN_DOWNMAX+200)	//�����Ļ��Χ��DOWNΪ��
extern KeyBoardTypeDef KeyBoardData[KEY_NUMS];
void PC_Control_Yun(float * yaw_tarp,float * pitch_tarp)	//1000Hz	
{
	static float yaw_tarp_float=0;	//��PID����Ϊ����ʱΪ�������΢Сֵ�ĸ���ֵ�ܱ���ȡ����������float,֮��ת��floatPID���÷���ʧȥ���ã�����ÿ�ν���ú���ʱ����ͬ���������Ӱ��
	static float pitch_tarp_float=PITCH_INIT;
	static u8 start_state=0;	//��ʼλ��

	static u8 keyQ_last,keyE_last=0;	//��ʱ����
	
	if(yun_control_pcorrc_last==RC_CONTROL&&Yun_Control_RCorPC==PC_CONTROL)
	{
		yaw_tarp_float=(float)*yaw_tarp;
	}
	
	if(start_state==0)	//��ʼʱ��¼
	{
		yaw_tarp_float=(float)*yaw_tarp;
		start_state=1;
	}
	else	//�ǳ�ʼ��¼��
	{
		yaw_tarp_float=(float)*yaw_tarp;	//ͬ������
		pitch_tarp_float=(float)*pitch_tarp;
	}
	
	
	if(VisionData.vision_control_state==0)	//û���Ӿ�����ʱ�ſɿ�
	{
		if(keyQ_last==0&&KeyBoardData[KEY_Q].value==1&&abs(yunMotorData.yaw_tarP-Gyro_Data.angle[YAW]*10)<50)	//abs(yaw_follow_error)<PI/7	&&abs(yunMotorData.yaw_tarP-Gyro_Data.angle[YAW]*10)<50
		{
			test_turn_count++;
			yaw_tarp_float-=900;
			yaw_tarp_float=yaw_tarp_float>1800?yaw_tarp_float-3600:yaw_tarp_float;	//�����
			yaw_tarp_float=yaw_tarp_float<-1800?yaw_tarp_float+3600:yaw_tarp_float;	//�����
		}
		keyQ_last=KeyBoardData[KEY_Q].value;
		
		if(keyE_last==0&&KeyBoardData[KEY_E].value==1&&abs(yunMotorData.yaw_tarP-Gyro_Data.angle[YAW]*10)<50)	//abs(yaw_follow_error)<PI/7	&&abs(yunMotorData.yaw_tarP-Gyro_Data.angle[YAW]*10)<50
		{
			test_turn_count++;
			yaw_tarp_float+=900;
			yaw_tarp_float=yaw_tarp_float>1800?yaw_tarp_float-3600:yaw_tarp_float;	//�����
			yaw_tarp_float=yaw_tarp_float<-1800?yaw_tarp_float+3600:yaw_tarp_float;	//�����
		}
		keyE_last=KeyBoardData[KEY_E].value;
		
		if(time_1ms_count%10==0)
		{
			yaw_tarp_float+=RC_Ctl.mouse.x*11.0f/40.0f;	//15.0f/40.0f,ͼ����ʱ���󣬸�С
			pitch_tarp_float-=RC_Ctl.mouse.y*2.0f/3.0f;	//2/4
			
			yaw_tarp_float=yaw_tarp_float>1800?yaw_tarp_float-3600:yaw_tarp_float;	//�����
			yaw_tarp_float=yaw_tarp_float<-1800?yaw_tarp_float+3600:yaw_tarp_float;	//�����
			
//			if(KeyBoardData[KEY_SHIFT].value!=1)
//			{
				pitch_tarp_float=pitch_tarp_float>(PITCH_INIT+YUN_UP_DISLIMIT)?(PITCH_INIT+YUN_UP_DISLIMIT):pitch_tarp_float;	//�����г�
				pitch_tarp_float=pitch_tarp_float<(PITCH_INIT-YUN_DOWN_DISLIMIT)?(PITCH_INIT-YUN_DOWN_DISLIMIT):pitch_tarp_float;	//�����г�
//			}
//			else	//shiftģʽ
//			{
//				pitch_tarp_float=pitch_tarp_float>(PITCH_INIT+YUN_DOWNMAX_EXTENSION)?(PITCH_INIT+YUN_DOWNMAX_EXTENSION):pitch_tarp_float;	//�����г�
//				pitch_tarp_float=pitch_tarp_float<(PITCH_INIT-YUN_UPMAX_EXTENSION)?(PITCH_INIT-YUN_UPMAX_EXTENSION):pitch_tarp_float;	//�����г�
//			}
			
			
		}
		*yaw_tarp=yaw_tarp_float;
		*pitch_tarp=pitch_tarp_float;
	}
	else
	{
		yaw_tarp_float=(float)*yaw_tarp;
		pitch_tarp_float=(float)*pitch_tarp;
	}
	

}




void Yun_Control_Inscribe_Solution(void)	//�������Ǳ���ʱ���ٶȷ�������
{
	if(GetWorkState()!=PREPARE_STATE&&GetWorkState()!=CALI_STATE)	//ģʽ�л�
	{
		if(RC_Ctl.mouse.press_l==1||RC_Ctl.mouse.press_r==1||RC_Ctl.mouse.x>1||RC_Ctl.mouse.y>1)
		{
			Yun_Control_RCorPC=PC_CONTROL;
		}
		else if(abs(RC_Ctl.rc.ch2-1024)>3||abs(RC_Ctl.rc.ch3-1024)>3)
		{
			Yun_Control_RCorPC=RC_CONTROL;
		}
	}
	
	
	if(Yun_Control_RCorPC==PC_CONTROL)
	{	//PC���Ʒ���
		static float pitch_tarp_float=PITCH_INIT;
		
		if(time_1ms_count%10==0)
		{
			pitch_tarp_float+=RC_Ctl.mouse.y*2.0f/4.0f;	//2/4
			
//			if(KeyBoardData[KEY_SHIFT].value!=1)
//			{
				pitch_tarp_float=pitch_tarp_float>(PITCH_INIT+YUN_UP_DISLIMIT)?(PITCH_INIT+YUN_UP_DISLIMIT):pitch_tarp_float;	//�����г�
				pitch_tarp_float=pitch_tarp_float<(PITCH_INIT-YUN_DOWN_DISLIMIT)?(PITCH_INIT-YUN_DOWN_DISLIMIT):pitch_tarp_float;	//�����г�
//			}
//			else	//shiftģʽ
//			{
//				pitch_tarp_float=pitch_tarp_float>(PITCH_INIT+YUN_DOWNMAX_EXTENSION)?(PITCH_INIT+YUN_DOWNMAX_EXTENSION):pitch_tarp_float;	//�����г�
//				pitch_tarp_float=pitch_tarp_float<(PITCH_INIT-YUN_UPMAX_EXTENSION)?(PITCH_INIT-YUN_UPMAX_EXTENSION):pitch_tarp_float;	//�����г�
//			}
			
			yunMotorData.pitch_tarP=pitch_tarp_float;	//��ֵλ��
		}
		
		yunMotorData.yaw_tarV=RC_Ctl.mouse.x*5;	//��ֵ�ٶ�
	}
	
	
	if(Yun_Control_RCorPC==RC_CONTROL)
	{	//RC���Ʒ���
		if(time_1ms_count%15==0)	//66.67hz
		{
			yunMotorData.yaw_tarV=((RC_Ctl.rc.ch2-1024)*300.0/660.0);
		}
		
		yunMotorData.pitch_tarP=((RC_Ctl.rc.ch3-1024)*460.0/660.0)+PITCH_INIT;	//-50����Ϊ������ˮƽʱ��̨����
	}
	
	
//	Yun_WorkState_Turn_Task();	//ȡ��ʱ��̨ת���־λ	//�ڲ��ж���������״̬������
	
	yunMotorData.pitch_tarV=PID_General(yunMotorData.pitch_tarP,(yunMotorData.pitch_fdbP),&PID_PITCH_POSITION);	//pitchλ�û�
	
//	if(KeyBoardData[KEY_SHIFT].value==1)	//�������汾��չ��̨��Χ
//	{
//		Yun_Pitch_Extension(yunMotorData.pitch_tarP);	//pitch��չ
//	}
	
	yunMotorData.pitch_output=-PID_General(yunMotorData.pitch_tarV,(Gyro_Data.angvel[PITCH]),&PID_PITCH_SPEED);
	yunMotorData.yaw_output=-PID_General(yunMotorData.yaw_tarV,(Gyro_Data.angvel[YAW]),&PID_YAW_SPEED);	//�������������������
}



/********************************�ϰ�Ӣ��*******************************
extern LIFT_DATA lift_Data;
*******************************
��̨�������ͬ�����ǣ���Ϊ������Ϊ��
********************************
#define PITCH_EXTENSION_UP 550
#define PITCH_EXTENSION_DOWN	550
#define PITCH_EXTENSION_TRIGGER_UP	620	//ʵ�ʻ��Χ��860
#define PITCH_EXTENSION_TRIGGER_DOWN	450	//ʵ�ʻ��Χ��650
void Yun_Pitch_Extension(float pitch_tar)	//pitch����չ��Χ
{
	if(pitch_tar>PITCH_INIT+PITCH_EXTENSION_TRIGGER_DOWN)	//���´���,����������
	{
		lift_Data.lf_lift_tarP=LIFT_DISTANCE_FALL;
		lift_Data.rf_lift_tarP=LIFT_DISTANCE_FALL;
		lift_Data.lb_lift_tarP=PITCH_EXTENSION_DOWN;
		lift_Data.rb_lift_tarP=PITCH_EXTENSION_DOWN;
	}
	else if(pitch_tar<PITCH_INIT-PITCH_EXTENSION_TRIGGER_UP)	//���ϴ���
	{
		lift_Data.lf_lift_tarP=PITCH_EXTENSION_UP;
		lift_Data.rf_lift_tarP=PITCH_EXTENSION_UP;
		lift_Data.lb_lift_tarP=LIFT_DISTANCE_FALL;
		lift_Data.rb_lift_tarP=LIFT_DISTANCE_FALL;
	}
}
**********************************************************************

//u8 Yun_WorkState_Turn180_statu=0;	//180��ת��λ��־λ������������
void Yun_WorkState_Turn_Task(void)	//ģʽ�л�ʱ��̨ת������	//��ת����ɱ�־λΪ1ʱ�л�����̨�������
{
	static WorkState_e State_Record=CHECK_STATE;
	static u32 time_start_record=0;
	
	if(IMU_Check_Useless_State==0)	//����������
	{
		if(State_Record!=TAKEBULLET_STATE&&GetWorkState()==TAKEBULLET_STATE)
		{
			time_start_record=time_1ms_count;
			Yun_WorkState_Turn180_statu=0;
			yunMotorData.yaw_tarP-=1800;
			yunMotorData.yaw_tarP=yunMotorData.yaw_tarP>1800?yunMotorData.yaw_tarP-3600:yunMotorData.yaw_tarP;	//�����
			yunMotorData.yaw_tarP=yunMotorData.yaw_tarP<-1800?yunMotorData.yaw_tarP+3600:yunMotorData.yaw_tarP;	//�����
		}
		else if(State_Record==TAKEBULLET_STATE&&GetWorkState()!=TAKEBULLET_STATE)
		{
			Yun_WorkState_Turn180_statu=0;
		}
		
		if(abs(yunMotorData.yaw_tarP-Gyro_Data.angle[2]*10)<2&&time_1ms_count-time_start_record>1500&&time_start_record!=0)	//2�ȷ�Χ��Ϊ��λ
		{
			Yun_WorkState_Turn180_statu=1;	//��Ϊ1
			time_start_record=0;	//5.21����
		}
	}
	else	//�������쳣
	{
		if(State_Record!=TAKEBULLET_STATE&&GetWorkState()==TAKEBULLET_STATE)
		{
			time_start_record=time_1ms_count;
			yunMotorData.yaw_tarV=260;
			Yun_WorkState_Turn180_statu=0;
		}
		else if(State_Record==TAKEBULLET_STATE&&GetWorkState()!=TAKEBULLET_STATE)
		{
			Yun_WorkState_Turn180_statu=0;
		}
		
		if(time_start_record!=0&&time_1ms_count-time_start_record<900)	//�������
		{
			yunMotorData.yaw_tarV=260;
		}
		
		if(time_1ms_count-time_start_record>900&&time_start_record!=0)	//2�ȷ�Χ��Ϊ��λ
		{
			yunMotorData.yaw_tarV=0;
			Yun_WorkState_Turn180_statu=1;	//��Ϊ1
			time_start_record=0;
		}
		
	}
	
	State_Record=GetWorkState();
}

float yaw_move_optimize_PC(s16 mouse_x,s16 mouse_y)	//����괦��
{
	return 0;
}

//s16 __t_yaw_offset=0;
//	void __yun_yaw_offset(void)
//	{
//	static s32 Last_V_yaw=0;
//	static float a=0.05f;
//	__t_yaw_offset=(s32)(Last_V_yaw*(1-a)+yunMotorData.yaw_output*a);
//	Last_V_yaw=__t_yaw_offset;
//	}

s32 yun_pitch_tarV(s32 now_V)
{
	static s32 Last_V=0;
	static s32 now_acc_V=0;
	static float a=0.97f;
	now_acc_V=(s32)(Last_V*(1-a)+now_V*a);
	Last_V=now_acc_V;
	return now_acc_V;
}



***************************************
�������ƣ�Yaw_Angle_Calculate
�������ܣ�ͨ����ǰ��е�Ƕ�����ֵ��е�ǶȱȽϵõ�ʵ�ʲ��
������������ǰ��е�Ƕȣ�src_angle
          ��ֵ��е�Ƕȣ�Init
��������ֵ��ʵ�ʲ�ǣ�output
��������
****************************************
float Yaw_Angle_Calculate(int16_t src_angle,int16_t Init)
{
    float output=-(float)(src_angle-Init)/8192*2*PI;	
	  return output;
}
//��¼����ֵ����
#define YAW_OFFSET_COUNT 11
const s32 YAW_OFFSET_VALUE[YAW_OFFSET_COUNT][2]=\
{\
	{5310,-620},\
	{5200,-530},\
	{5100,-475},\
	{5000,-390},\
	{4900,-110},\
	{4800,70},\
	{4700,280},\
	{4600,400},\
	{4500,470},\
	{4400,530},\
	{4300,573},\
};	//3.6���

//const s32 YAW_OFFSET_VALUE[YAW_OFFSET_COUNT][2]=\
{\
	{5310,-1000},\
	{5180,-800},\
	{5000,-700},\
	{4910,-600},\
	{4800,-350},\
	{4710,0},\
	{4600,20},\
	{4500,240},\
	{4450,420},\
};	//��

#define PITCH_OFFSET_COUNT 12
const s32 PITCH_OFFSET_VALUE[PITCH_OFFSET_COUNT][2]=\
{\
	{6600,-3500},\
	{6500,-2800},\
	{6400,-2500},\
	{6300,-2300},\
	{6200,-1920},\
	{6100,-1465},\
	{6000,-1300},\
	{5900,-1186},\
	{5800,-1200},\
	{5700,-1000},\
	{5600,-900},\
	{5500,-800},\
};	//ԭ�棬����
//const s32 PITCH_OFFSET_VALUE[PITCH_OFFSET_COUNT][2]=\
{\
	{6600,-3000},\
	{6500,-2400},\
	{6400,-2100},\
	{6300,-1900},\
	{6200,-1750},\
	{6100,-1400},\
	{6000,-1200},\
	{5900,-1186},\
	{5800,-1100},\
	{5700,-1000},\
	{5600,-900},\
	{5500,-800},\
};

s32 Yaw_output_offset(s32 yaw_fbdP)	//�˷���̨yaw������������ǶԳ��ԵĲ��� //��Ȼyaw��̨����������������������Ϊ�����ﲢ��Ϊ��̨�������Ӧ���õ�ǰ����λ��������
{
	s32 offset=0;
	int i=0;
	
	yaw_fbdP=yaw_fbdP>YAW_OFFSET_VALUE[0][0]?YAW_OFFSET_VALUE[0][0]:yaw_fbdP;
	yaw_fbdP=yaw_fbdP<YAW_OFFSET_VALUE[YAW_OFFSET_COUNT-1][0]?YAW_OFFSET_VALUE[YAW_OFFSET_COUNT-1][0]:yaw_fbdP;
	
	for(i=0;i<YAW_OFFSET_COUNT;i++)	//��������Ѱ��λ��
	{
		if(yaw_fbdP>=YAW_OFFSET_VALUE[i][0]) break;
	}
	
	i=i>YAW_OFFSET_COUNT-2?YAW_OFFSET_COUNT-2:i;	//���Ƶ������ڶ���Ԫ�ص�λ�ã�������һ����������Խ��
	
	offset=YAW_OFFSET_VALUE[i][1]+(YAW_OFFSET_VALUE[i+1][1]-YAW_OFFSET_VALUE[i][1])*(YAW_OFFSET_VALUE[i][0]-yaw_fbdP)/(YAW_OFFSET_VALUE[i][0]-YAW_OFFSET_VALUE[i+1][0]);
	return offset;
}

s16 Pitch_output_offset(s32 pitch_tarP)	//�˷���̨pitch������������ǶԳ��ԵĲ���	//��Ϊ��̨pitch���������������������ⲿ���������ֻ����һ����Сֵ�����ʲ���tarP��Ϊ�������տ�����߼�ӷ�Ӧ�ٶ�
{
	s16 offset=0;
//	int i=0;
//	
//	pitch_tarP=pitch_tarP>PITCH_OFFSET_VALUE[0][0]?PITCH_OFFSET_VALUE[0][0]:pitch_tarP;
//	pitch_tarP=pitch_tarP<PITCH_OFFSET_VALUE[PITCH_OFFSET_COUNT-1][0]?PITCH_OFFSET_VALUE[PITCH_OFFSET_COUNT-1][0]:pitch_tarP;
//	
//	for(i=0;i<PITCH_OFFSET_COUNT;i++)	//��������Ѱ��λ��
//	{
//		if(pitch_tarP>=PITCH_OFFSET_VALUE[i][0]) break;
//	}
//	
//	i=i>PITCH_OFFSET_COUNT-2?PITCH_OFFSET_COUNT-2:i;	//���Ƶ������ڶ���Ԫ�ص�λ�ã�������һ����������Խ��
//	
//	offset=PITCH_OFFSET_VALUE[i][1]+(PITCH_OFFSET_VALUE[i+1][1]-PITCH_OFFSET_VALUE[i][1])*(PITCH_OFFSET_VALUE[i][0]-pitch_tarP)/(PITCH_OFFSET_VALUE[i][0]-PITCH_OFFSET_VALUE[i+1][0]);
	return offset;
}

*****/


