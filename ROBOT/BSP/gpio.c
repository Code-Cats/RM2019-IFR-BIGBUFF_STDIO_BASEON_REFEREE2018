#include "gpio.h"

/*************************
��������KEY_Configuration
�������ܣ�KEY��GPIO����
�������룺��
��������ֵ�� ��
���������õ����� PB0
*************************/
void KEY_Config(void)
{
   GPIO_InitTypeDef gpio;  
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
		
	 gpio.GPIO_Pin = GPIO_Pin_0 ;
	 gpio.GPIO_Mode = GPIO_Mode_IN;
	 gpio.GPIO_OType = GPIO_OType_OD;
	gpio.GPIO_PuPd = GPIO_PuPd_DOWN;
	 gpio.GPIO_Speed = GPIO_Speed_100MHz;
	 GPIO_Init(GPIOB,&gpio);
			
//	 GPIO_SetBits(GPIOB, GPIO_Pin_0);	//anjian
//	GPIO_ReadInputDataBit(GPIOB,0);
}

/*************************
��������LED_Configuration
�������ܣ�LED�Ƶ�GPIO����
�������룺��
��������ֵ�� ��
���������õ����� PC0��PC1
*************************/
void LED_Config(void)
{
   GPIO_InitTypeDef gpio;  
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
		
	 gpio.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0 ;
	 gpio.GPIO_Mode = GPIO_Mode_OUT;
	 gpio.GPIO_OType = GPIO_OType_PP;
	 gpio.GPIO_Speed = GPIO_Speed_100MHz;
	 GPIO_Init(GPIOC,&gpio);
			
	 GPIO_SetBits(GPIOC, GPIO_Pin_1);	//�� ��
	 GPIO_SetBits(GPIOC, GPIO_Pin_0);	//�� ��
}
/*************************
��������Laser_Config
�������ܣ������GPIO����
�������룺��
��������ֵ�� ��
���������õ����� PC5
*************************/
void Laser_Config(void)
{
   GPIO_InitTypeDef gpio;
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	 gpio.GPIO_Pin = GPIO_Pin_5;
	 gpio.GPIO_Mode = GPIO_Mode_OUT;
	 gpio.GPIO_OType = GPIO_OType_PP;
	 gpio.GPIO_Speed = GPIO_Speed_100MHz;
	 GPIO_Init(GPIOC, &gpio);
	 GPIO_ResetBits(GPIOC, GPIO_Pin_5);
}

