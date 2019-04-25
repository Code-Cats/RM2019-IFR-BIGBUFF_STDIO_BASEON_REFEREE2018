#include "judge_analysis.h"

//extern tGameRobotState         testGameRobotState;      //����������״̬
/**************************************************
�������ƣ�BulletNum_Calculate
�������ܣ������ӵ����������ӵ���Ŀ
�����������
��������ֵ����
���������ݲ���ϵͳ��ǹ���������Ƶ�ʺ���ȴֵ�ɼ��������٣�
      100ms֮�ڿ��ܴ���ü���������Ӧ�����ɼ��������ӵ���Ŀ
***************************************************/
u8 Robot_Level=1;


int realBulletNum=0;	//������
int speedCnt=0;
float speedReal=16;	//����
float realBulletFreq=0.0;

int maxHeat=80;	//�������
float coolHeat=20;	//ÿ����ȴ
/***************************************************************888	
void BulletNum_Calculate(void)
{
	static u8 secondSpeed[2]={0,0};
    speedCnt++;
	if(speedCnt==20)
	{ 
		  speedCnt=0;
		
		Heat_MAX_COOL_calc(&maxHeat,&coolHeat,testGameRobotState.maxHP);
		
		  secondSpeed[0]=secondSpeed[1];
		  secondSpeed[1]=testPowerHeatData.shooterHeat1;  //��¼��0ms�͵�100ms��ǹ������	//Heat1Ϊ���ӵ�
		  if(secondSpeed[1]-secondSpeed[0]>0)             //�жϷ�������ӵ�
		  {	
			    speedReal=secondSpeed[1]-secondSpeed[0];//+coolHeat; //���ݲ����������㷽ʽ��Ӽ�����ӵ��ٶ�
				realBulletFreq=speedReal/40;              //��¼�ӵ����������100ms�ڿ��ܴ���ü����ӵ�
				if(abs(realBulletFreq-1)<0.5f) {speedReal=speedReal;realBulletNum=realBulletNum+1;}
				else if(abs(realBulletFreq-2)<0.5f) {speedReal=speedReal/2;realBulletNum=realBulletNum+2;}
				else if(abs(realBulletFreq-3)<0.5f) {speedReal=speedReal/3;realBulletNum=realBulletNum+3;}
				else if(abs(realBulletFreq-4)<0.5f) {speedReal=speedReal/4;realBulletNum=realBulletNum+4;}
				else if(abs(realBulletFreq-5)<0.5f) {speedReal=speedReal/5;realBulletNum=realBulletNum+5;}
			    else if(abs(realBulletFreq-6)<0.5f) {speedReal=speedReal/5;realBulletNum=realBulletNum+6;}
			    else if(abs(realBulletFreq-7)<0.5f) {speedReal=speedReal/5;realBulletNum=realBulletNum+7;}
			    }
		  }
         // if(PWM5==MAGAZINE_OPEN)  realBulletNum=0;
}


void Heat_MAX_COOL_calc(int* maxheat,float* coolheat,int maxhp)	//���ݲ������Ѫ���Ƴ��ȼ����Ƴ��������޼���ȴֵ
{
	if(maxhp==1500)
	{
		*maxheat=80;
		*coolheat=20;
		Robot_Level=1;
	}
	else if(maxhp==2500)
	{
		*maxheat=160;
		*coolheat=40;
		Robot_Level=2;
	}
	else if(maxhp==3500)
	{
		*maxheat=320;
		*coolheat=80;
		Robot_Level=3;
	}
	else
	{
		*maxheat=80;
		*coolheat=20;
		Robot_Level=1;
	}
}


extern u32 time_1ms_count;

RobotHeatDataSimuTypeDef RobotHeatDataSimu42={0};	//�߶���
const u16 MaxHeat[3]={80,120,200};	//�����ȼ��µ��������
const u16 CoolHeat[3]={20,40,60};	//�����ȼ��µ���ȴֵ
	
void Heat_Simulating(u16 heating,u8 level)	//��������//����Ƶ��1-10HZ
{
	static u16 bulletnum_last=0;
	
	if(RobotHeatDataSimu42.bullet_num!=bulletnum_last)	//�Ҳ���Ϊ10ms�����������
	{
		RobotHeatDataSimu42.heat+=40;
	}
	if(level>0&&level<4)
	{
		RobotHeatDataSimu42.maxheat=MaxHeat[level-1];
	}
	else
	{
		RobotHeatDataSimu42.maxheat=MaxHeat[0];
	}
	
	
	if(time_1ms_count%100==0)	//����Ƶ��10HZ
	{
		switch(level)
		{
			case 1:
			{
				if(RobotHeatDataSimu42.heat>2*MaxHeat[level-1])
				{
					RobotHeatDataSimu42.heat=2*MaxHeat[level-1];
				}
				
				RobotHeatDataSimu42.heat-=CoolHeat[level-1]/10;
				
				RobotHeatDataSimu42.heat=RobotHeatDataSimu42.heat<0?0:RobotHeatDataSimu42.heat;
				break;
			}
			case 2:
			{
				if(RobotHeatDataSimu42.heat>2*MaxHeat[level-1])
				{
					RobotHeatDataSimu42.heat=2*MaxHeat[level-1];
				}
				
				RobotHeatDataSimu42.heat-=CoolHeat[level-1]/10;
				
				RobotHeatDataSimu42.heat=RobotHeatDataSimu42.heat<0?0:RobotHeatDataSimu42.heat;
				break;
			}
			case 3:
			{
				if(RobotHeatDataSimu42.heat>2*MaxHeat[level-1])
				{
					RobotHeatDataSimu42.heat=2*MaxHeat[level-1];
				}
				
				RobotHeatDataSimu42.heat-=CoolHeat[level-1]/10;
				
				RobotHeatDataSimu42.heat=RobotHeatDataSimu42.heat<0?0:RobotHeatDataSimu42.heat;
				break;
			}
			default:
			{
				if(RobotHeatDataSimu42.heat>2*MaxHeat[0])
				{
					RobotHeatDataSimu42.heat=2*MaxHeat[0];
				}
				
				RobotHeatDataSimu42.heat-=CoolHeat[0]/10;
				
				RobotHeatDataSimu42.heat=RobotHeatDataSimu42.heat<0?0:RobotHeatDataSimu42.heat;
				break;
			}
		}
	}
	
	bulletnum_last=RobotHeatDataSimu42.bullet_num;
}


void BulletNum_Simu_ADD(void)
{
	RobotHeatDataSimu42.bullet_num++;
}
***********************************************************/

