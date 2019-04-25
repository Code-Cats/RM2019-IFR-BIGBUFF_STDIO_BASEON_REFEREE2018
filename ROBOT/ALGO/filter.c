#include "filter.h"




/*********************************************
�������ƣ�FirstOrder_General
�������ܣ�ͨ��һ���ͺ��˲��㷨
������������ǰֵ��cur_val
��������ֵ�����ֵ��output
**********************************************/
float FirstOrder_General(const s32 cur_val,FIRST_ORDER_FILTER*filter)
{
	if(abs(cur_val)>filter->tiny)	//��������
	{
		filter->Data_Save[1]=cur_val;
	}
	else
	{
		filter->Data_Save[1]=0;
	}
	
	if(abs(filter->Data_Save[1])>=abs(filter->Data_Save[0]))	//��һ�εĴ�
	{
		if(filter->up_flag==1)
		{
			filter->output=(1-filter->ratio)*filter->Data_Save[0]+filter->ratio*filter->Data_Save[1];
		}
		else
		{
			filter->output=filter->Data_Save[1];
		}
	}
	else
	{
		if(filter->down_flag==1)
		{
			filter->output=(1-filter->ratio)*filter->Data_Save[0]+filter->ratio*filter->Data_Save[1];
		}
		else
		{
			filter->output=filter->Data_Save[1];
		}
	}
	
	filter->Data_Save[0]=filter->output;

	return filter->output;
}



///**************************************************************
//�������ƣ�Filter_firstOrder
//�������ܣ�����һ���ͺ��˲�
//          ����Ծ�źű�Ϊб�����ź�
//������������ǰ��������ֵ��input
//          �˲��ṹ�������FIRST_ORDER_FILTER
//��������ֵ�����������ֵ��output
//��������¼��ǰֵ����һ��ֵ������Ȩ�صõ����ֵ
//      ��ѡ�������½��Ƿ���Ҫ�˲�
//***************************************************************/
//float Filter_firstOrder(int16_t input,FIRST_ORDER_FILTER *ray)
//{
//	  //�������ݴ�С����
//		if(input>ray->input_max){input=ray->input_max;}
//		if(input<ray->input_min){input=ray->input_min;}
//    ray->Data_Save[1]=input;
//	  //�����Ƿ���Ҫ�˲� Flag==1 ��Ҫ ��֮��
//		if(abs(ray->Data_Save[1])>=abs(ray->Data_Save[0]))
//		{
//			 if(ray->up_flag==1)
//			 {
//			     ray->Data_Save[1]=ray->Data_Save[0] * ray->former + ray->Data_Save[1] * ray->later;
//			 }
//		}
//	  else
//	  //�½��Ƿ���Ҫ�˲� Flag==1 ��Ҫ ��֮��
//		{
//			 if(ray->down_flag==1)
//			 {
//				  ray->Data_Save[1]=ray->Data_Save[0] * ray->former + ray->Data_Save[1] * ray->later;
//				//�½�ʱ��Сֵ����
//				  if((ray->Data_Save[1] >=- ray->back)&&(ray->Data_Save[1] <= ray->back))
//				  ray->Data_Save[1]=0;	
//			 }
//	  }
//		//������ݴ�С����
//		if(ray->Data_Save[1]>ray->output_max)  {ray->Data_Save[1]=ray->output_max;}
//		if(ray->Data_Save[1]<ray->output_min)  {ray->Data_Save[1]=ray->output_min;}
//		
//		ray->output=ray->Data_Save[1];
//    ray->Data_Save[0]=ray->Data_Save[1];
//		return ray->output;
//}

/*************************************
��������:Filter_MPU6050_Accel
�������ܣ���MPU6050�����˲�
����������MPU6050��ǰ����ֵ��Accel
         Ȩ�ز�����param
��������ֵ����
��������
**************************************/
float	Filter_MPU6050_Accel(float Accel,float param)
{			
		static float accel[2]= {0.0,0.0};
		static float a_rpm=0;
		float output_accel;
		accel[0]=accel[1];
		accel[1]=Accel;
		a_rpm=accel[0]*(1-param)+accel[1]*param;
		output_accel=((60*abs(a_rpm)*9.8f*19)/(13.24f*PI*0.1510f));
    return output_accel;
}


