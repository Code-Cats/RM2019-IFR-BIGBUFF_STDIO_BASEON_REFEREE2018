#include "pid.h"



/****************************************
�������ƣ�PID_General
�������ܣ�ʹ���ض�����ȶ��ĴﵽĿ��ֵ
����������Ŀ��ֵ��target
          ��ǰֵ��current
					PID�ṹ�������PID_GENERAL
��������ֵ�����:output
������ͨ��PID,��ͳPID
*****************************************/
float PID_General(float target,float current,PID_GENERAL *pid)
{
	  //Ŀ�����ݴ�С����
		if(target>pid->input_max){target=pid->input_max;}
		if(target<pid->input_min){target=pid->input_min;}
    
    pid->Data_Save[0] = pid->Data_Save[1];
	  pid->Data_Save[1] = target - current;
		
		if(target - current>pid->input_maxerror)pid->Data_Save[1]=pid->input_maxerror;
		else if(target - current<-pid->input_maxerror)pid->Data_Save[1]=-pid->input_maxerror;
		
		if(abs(target - current)<=pid->tiny)pid->Data_Save[1]=0;
		
		pid->inte = 0.999f*pid->inte+pid->Data_Save[1];
		pid->dire = pid->k_d * (pid->Data_Save[1] - pid->Data_Save[0]);

		if(pid->inte>pid->i_sum_max){pid->inte=pid->i_sum_max;}
		if(pid->inte<-pid->i_sum_max){pid->inte=-pid->i_sum_max;}
//pid->t_error=(s16)(pid->Data_Save[1]*pid->k_p);
//pid->t_dire=(s16)pid->dire;
//pid->t_inte=(s16)(pid->inte * pid->k_i);
		pid->output = pid->Data_Save[1] * pid->k_p + pid->inte * pid->k_i + pid->dire; 
	
		//������ݴ�С����
		if(pid->output>pid->output_max){pid->output=pid->output_max;}
		if(pid->output<pid->output_min){pid->output=pid->output_min;}
		return pid->output;
}


float PID_ChassisFollow_Variable_kp(float error)	//���̵��8192Ϊ360�ȣ�15Ϊ341��
{
	float kp=1;
	if(abs(error)<400)
	{
		kp=abs(error)/400+0.4f;
		kp=kp>1?1:kp;
		kp=kp<0?0:kp;
	}
	return kp;
}

/****************************************
�������ƣ�PID_ChassisFollow
�������ܣ�ʹ���ض�����ȶ��ĴﵽĿ��ֵ
����������Ŀ��ֵ��target
          ��ǰֵ��current
					PID�ṹ�������PID_ChassisFollow
��������ֵ�����:output
���������̸����õ�PID
*****************************************/
float PID_ChassisFollow(float target,float current,PID_GENERAL *pid)
{
//float kp_vari=1;
	
	  //Ŀ�����ݴ�С����
		if(target>pid->input_max){target=pid->input_max;}
		if(target<pid->input_min){target=pid->input_min;}
    
    pid->Data_Save[0] = pid->Data_Save[1];
	  pid->Data_Save[1] = target - current;
		
		if(target - current>pid->input_maxerror)pid->Data_Save[1]=pid->input_maxerror;
		else if(target - current<-pid->input_maxerror)pid->Data_Save[1]=-pid->input_maxerror;
		
		if(abs(target - current)<=pid->tiny)pid->Data_Save[1]=0;
		
//		if(pid->Data_Save[1]<250)
//		{
//			kp_vari=1-0.5f*(250-pid->Data_Save[1])/250;
//		}
		
		
		
		pid->inte = 0.999f*pid->inte+pid->Data_Save[1];
		pid->dire = pid->k_d * (pid->Data_Save[1] - pid->Data_Save[0]);

		if(pid->inte>pid->i_sum_max){pid->inte=pid->i_sum_max;}
		if(pid->inte<-pid->i_sum_max){pid->inte=-pid->i_sum_max;}
//pid->t_error=(s16)(pid->Data_Save[1]*pid->k_p);
//pid->t_dire=(s16)pid->dire;
//pid->t_inte=(s16)(pid->inte * pid->k_i);
		pid->output = pid->Data_Save[1] * pid->k_p *PID_ChassisFollow_Variable_kp(pid->Data_Save[1]) + pid->inte * pid->k_i + pid->dire; 
	
		//������ݴ�С����
		if(pid->output>pid->output_max){pid->output=pid->output_max;}
		if(pid->output<pid->output_min){pid->output=pid->output_min;}
		return pid->output;
}



float PID_Robust(float target,float current,float differential,PID_GENERAL *pid)
{
	//Ŀ�����ݴ�С����
		if(target>pid->input_max){target=pid->input_max;}
		if(target<pid->input_min){target=pid->input_min;}
    
    pid->Data_Save[0] = pid->Data_Save[1];
	  pid->Data_Save[1] = target - current;
		
		if(target - current>pid->input_maxerror)pid->Data_Save[1]=pid->input_maxerror;
		else if(target - current<-pid->input_maxerror)pid->Data_Save[1]=-pid->input_maxerror;
		
		if(abs(target - current)<=pid->tiny)pid->Data_Save[1]=0;
		
		pid->inte = 0.999f*pid->inte+pid->Data_Save[1];
		pid->dire = pid->k_d * differential;

		if(pid->inte>pid->i_sum_max){pid->inte=pid->i_sum_max;}
		if(pid->inte<-pid->i_sum_max){pid->inte=-pid->i_sum_max;}
//pid->t_error=(s16)(pid->Data_Save[1]*pid->k_p);
//pid->t_dire=(s16)pid->dire;
//pid->t_inte=(s16)(pid->inte * pid->k_i);
		pid->output = pid->Data_Save[1] * pid->k_p + pid->inte * pid->k_i + pid->dire; 
	
		//������ݴ�С����
		if(pid->output>pid->output_max){pid->output=pid->output_max;}
		if(pid->output<pid->output_min){pid->output=pid->output_min;}
		return pid->output;
}

