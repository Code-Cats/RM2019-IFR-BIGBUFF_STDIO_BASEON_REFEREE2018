#include "pwm_sk6812.h"

//共用参数
#define SK6812BIT_HIGH 3
#define SK6812BIT_LOW 1
const u8 SK6812BitDef[2] ={SK6812BIT_LOW,SK6812BIT_HIGH};
#define SK6812_NUMS 350
#define SK6812_SIZE 24*SK6812_NUMS+1	//最后一bit为reset电平	//还有另一种实现思路是使能DMA传输完成中断 并在该中断中将CCR寄存器置0

void Sk6812_Init(void)
{
	PWM2_Init();
	PWM3_Init();
	
	//PWM2_1_DMA_Init();
	PWM2_2_DMA_Init();
	PWM2_3_DMA_Init();
	//PWM2_4_DMA_Init();
	TIM_Cmd(TIM2, ENABLE);  /* 使能TIM2 */
	
	PWM3_1_DMA_Init();
	PWM3_2_DMA_Init();
	PWM3_3_DMA_Init();
	//PWM3_4_DMA_Init();
	TIM_Cmd(TIM3, ENABLE);  /* 使能TIM3 */
}

void PWM2_Init(void)
{
	GPIO_InitTypeDef          gpio;
	TIM_TimeBaseInitTypeDef   tim;
	TIM_OCInitTypeDef         oc;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB ,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   

	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	//CH1,2
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&gpio);

	gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;	//CH3,4
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&gpio);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2);//定时器2 通道1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM2);//定时器2 通道2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);//定时器2 通道3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_TIM2);//定时器2 通道4

	/* TIM2 */
	tim.TIM_Prescaler = 19-1;	//18OK
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Period = 1*5;   //4->1us	//0.5HIGH 0.75LOW 0码； 0.75HIGH 0.5LOW 1码
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2,&tim);	//初始化时间基数单位

	oc.TIM_OCMode = TIM_OCMode_PWM2;
	oc.TIM_OutputState = TIM_OutputState_Enable;
	oc.TIM_OutputNState = TIM_OutputState_Disable;
	oc.TIM_Pulse = 0;
	oc.TIM_OCPolarity = TIM_OCPolarity_Low;
	oc.TIM_OCNPolarity = TIM_OCPolarity_High;
	oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
	oc.TIM_OCNIdleState = TIM_OCIdleState_Set;

	TIM_OC1Init(TIM2,&oc);//定时器5 通道1
	TIM_OC2Init(TIM2,&oc);//定时器5 通道2
	TIM_OC3Init(TIM2,&oc);//定时器5 通道3
	TIM_OC4Init(TIM2,&oc);//定时器5 通道4

	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM2,ENABLE);

	TIM_Cmd(TIM2,DISABLE);
}

#if PWM2_1_DMA_ENAB==1
u32 Pwm2_1_DMABuffer[SK6812_SIZE]={0x1,0x2,0x3,0x4};
void PWM2_1_DMA_Init(void)
{
    DMA_InitTypeDef dma;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);   //使能DMA传输
    delay_ms(5);

	DMA_Cmd(DMA1_Stream5, DISABLE);
	DMA_DeInit(DMA1_Stream5);
    dma.DMA_Channel = DMA_Channel_3;
    dma.DMA_PeripheralBaseAddr = (uint32_t)(&TIM2->CCR1);  /* DMA外设基地址 *///DMA外设TIM5-CCR3地址/
    dma.DMA_Memory0BaseAddr = (uint32_t)Pwm2_1_DMABuffer; ///* DMA内存基地址 */DMA内存基地址/
    dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;/* 数据传输方向，从内存读取发送到外设 */
    dma.DMA_BufferSize = SK6812_SIZE;/* DMA通道的DMA缓存的大小 */
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/* 外设地址寄存器不变 */
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;/* 内存地址寄存器递增 */
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;/* 数据宽度为8位 *///  
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;/* 数据宽度为8位 *///  DMA_MemoryDataSize_Word  DMA_MemoryDataSize_HalfWord  DMA_MemoryDataSize_Byte
    dma.DMA_Mode = DMA_Mode_Normal;      ///* 工作在正常模式 */工作在正常缓存模式
    dma.DMA_Priority = DMA_Priority_Medium;   //DMA通道 x拥有中优先级 
    dma.DMA_FIFOMode = DMA_FIFOMode_Enable;   //DMA通道x没有设置为内存到内存传输
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;//DMA_FIFOThreshold_HalfFull;//DMA_FIFOThreshold_1QuarterFull;
    dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		
	    /* 配置DMA */
    DMA_Init(DMA1_Stream5, &dma);
    /*使能TIM的DMA接口 */
	//TIM_SelectCCDMA(TIM5,ENABLE);
	//TIM_DMAConfig(TIM5, TIM_DMABase_CCR3, TIM_DMABurstLength_16Bytes);
	
    //TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE);	/* 如果是要调节占空比就把这行去掉注释，注释另一行，再修改相应通道 */
	TIM_DMACmd(TIM2, TIM_DMA_CC1, ENABLE);	/* 如果是要调节频率就把这行去掉注释，注释另一行，再修改相应通道 */

    DMA_Cmd(DMA1_Stream5, DISABLE);	 /*不使能DMA */  
	////TIM_Cmd(TIM2, ENABLE);  /* 使能TIM5 */ //4个通道只使能一次
}

u32 Pwm2_1_Dmasetcounter=0;
bool PWM2_1_DMA_Enable(void)	//DMA1_S5C3
{
	static u8 start_flag=0;	//否则第一次无法启动
	if(DMA_GetFlagStatus(DMA1_Stream5,DMA_FLAG_TCIF5)!= RESET || start_flag!=1)	//传输完成标志，若在传输过程中再次设置将会打乱时序
	{
		start_flag=1;
		DMA_ClearFlag(DMA1_Stream5,DMA_IT_TCIF5);
		DMA_Cmd(DMA1_Stream5, DISABLE );
		DMA_SetCurrDataCounter(DMA1_Stream5,Pwm2_1_Dmasetcounter);	//SK6812_SIZE

		DMA_Cmd(DMA1_Stream5, ENABLE);
		//TIM_Cmd(TIM5, ENABLE);  /* 使能TIM3 */
		TIM2->EGR |= 0x00000001;	/* 由于最后一次ARR值为0，这是为了停止定时器对io口的操作，但是不要忽略了一点：CNT并没有停止计数，而且是不会再停下来，如果没有手动操作的话，所以需要在每次dma使能时加上一句，将EGR里的UG位置1，清零计数器 */
		return true;
	}
	else
	{
		return false;
	}
}
#endif

#if PWM2_2_DMA_ENAB==1
u32 Pwm2_2_DMABuffer[SK6812_SIZE]={0};
void PWM2_2_DMA_Init(void)
{
    DMA_InitTypeDef dma;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);   //使能DMA传输
    delay_ms(1);

	DMA_Cmd(DMA1_Stream6, DISABLE);
	DMA_DeInit(DMA1_Stream6);
    dma.DMA_Channel = DMA_Channel_3;
    dma.DMA_PeripheralBaseAddr = (uint32_t)(&TIM2->CCR2);  /* DMA外设基地址 *///DMA外设TIM5-CCR3地址/
    dma.DMA_Memory0BaseAddr = (uint32_t)Pwm2_2_DMABuffer; ///* DMA内存基地址 */DMA内存基地址/
    dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;/* 数据传输方向，从内存读取发送到外设 */
    dma.DMA_BufferSize = SK6812_SIZE;/* DMA通道的DMA缓存的大小 */
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/* 外设地址寄存器不变 */
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;/* 内存地址寄存器递增 */
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;/* 数据宽度为32位 */
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;/* 数据宽度为32位 */
    dma.DMA_Mode = DMA_Mode_Normal;      ///* 工作在正常模式 */工作在正常缓存模式
    dma.DMA_Priority = DMA_Priority_Medium;   //DMA通道 x拥有中优先级 
    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;   //DMA通道x没有设置为内存到内存传输
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//DMA_FIFOThreshold_HalfFull;//DMA_FIFOThreshold_1QuarterFull;
    dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		
	    /* 配置DMA */
    DMA_Init(DMA1_Stream6, &dma);

    /*使能TIM的DMA接口 */
	//TIM_SelectCCDMA(TIM5,ENABLE);
	//TIM_DMAConfig(TIM5, TIM_DMABase_CCR3, TIM_DMABurstLength_16Bytes);
	
    //TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE);	/* 如果是要调节占空比就把这行去掉注释，注释另一行，再修改相应通道 */
	TIM_DMACmd(TIM2, TIM_DMA_CC2, ENABLE);	/* 如果是要调节频率就把这行去掉注释，注释另一行，再修改相应通道 */

    DMA_Cmd(DMA1_Stream6, DISABLE);	 /*不使能DMA */  
	////TIM_Cmd(TIM2, ENABLE);  /* 使能TIM5 */	//4个通道最后只能使能一次
}

u32 Pwm2_2_Dmasetcounter=0;
bool PWM2_2_DMA_Enable(void)	//DMA1_S6C3
{
	static u8 start_flag=0;	//否则第一次无法启动
	if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!= RESET || start_flag!=1)	//传输完成标志，若在传输过程中再次设置将会打乱时序
	{
		start_flag=1;
		DMA_ClearFlag(DMA1_Stream6,DMA_IT_TCIF6);
		DMA_Cmd(DMA1_Stream6, DISABLE );
		DMA_SetCurrDataCounter(DMA1_Stream6,Pwm2_2_Dmasetcounter);	//SK6812_SIZE

		DMA_Cmd(DMA1_Stream6, ENABLE);
		//TIM_Cmd(TIM5, ENABLE);  /* 使能TIM3 */
		TIM2->EGR |= 0x00000001;	/* 由于最后一次ARR值为0，这是为了停止定时器对io口的操作，但是不要忽略了一点：CNT并没有停止计数，而且是不会再停下来，如果没有手动操作的话，所以需要在每次dma使能时加上一句，将EGR里的UG位置1，清零计数器 */
		return true;
	}
	else
	{
		return false;
	}
}
#endif

#if PWM2_3_DMA_ENAB==1
u32 Pwm2_3_DMABuffer[SK6812_SIZE]={0};
void PWM2_3_DMA_Init(void)
{
    DMA_InitTypeDef dma;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);   //使能DMA传输
    delay_ms(1);

	DMA_Cmd(DMA1_Stream1, DISABLE);
	DMA_DeInit(DMA1_Stream1);
    dma.DMA_Channel = DMA_Channel_3;
    dma.DMA_PeripheralBaseAddr = (uint32_t)(&TIM2->CCR3);  /* DMA外设基地址 *///DMA外设TIM2-CCR3地址/
    dma.DMA_Memory0BaseAddr = (uint32_t)Pwm2_3_DMABuffer; ///* DMA内存基地址 */DMA内存基地址/
    dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;/* 数据传输方向，从内存读取发送到外设 */
    dma.DMA_BufferSize = SK6812_SIZE;/* DMA通道的DMA缓存的大小 */
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/* 外设地址寄存器不变 */
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;/* 内存地址寄存器递增 */
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;/* 数据宽度为32位 */
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;/* 数据宽度为32位 */
    dma.DMA_Mode = DMA_Mode_Normal;      ///* 工作在正常模式 */工作在正常缓存模式
    dma.DMA_Priority = DMA_Priority_Medium;   //DMA通道 x拥有中优先级 
    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;   //DMA通道x没有设置为内存到内存传输
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//DMA_FIFOThreshold_HalfFull;//DMA_FIFOThreshold_1QuarterFull;
    dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		
	    /* 配置DMA */
    DMA_Init(DMA1_Stream1, &dma);
    /*使能TIM的DMA接口 */
	//TIM_SelectCCDMA(TIM5,ENABLE);
	//TIM_DMAConfig(TIM5, TIM_DMABase_CCR3, TIM_DMABurstLength_16Bytes);
	
    //TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE);	/* 如果是要调节占空比就把这行去掉注释，注释另一行，再修改相应通道 */
	TIM_DMACmd(TIM2, TIM_DMA_CC3, ENABLE);	/* 如果是要调节频率就把这行去掉注释，注释另一行，再修改相应通道 */

    DMA_Cmd(DMA1_Stream1, DISABLE);	 /*不使能DMA */  
	////TIM_Cmd(TIM2, ENABLE);  /* 使能TIM5 */	//4个通道最后只能使能一次
}

u32 Pwm2_3_Dmasetcounter=0;
bool PWM2_3_DMA_Enable(void)	//DMA1_S6C3
{
	static u8 start_flag=0;	//否则第一次无法启动
	if(DMA_GetFlagStatus(DMA1_Stream1,DMA_FLAG_TCIF1)!= RESET || start_flag!=1)	//传输完成标志，若在传输过程中再次设置将会打乱时序
	{
		start_flag=1;
		DMA_ClearFlag(DMA1_Stream1,DMA_IT_TCIF1);
		DMA_Cmd(DMA1_Stream1, DISABLE );
		DMA_SetCurrDataCounter(DMA1_Stream1,Pwm2_3_Dmasetcounter);	//SK6812_SIZE

		DMA_Cmd(DMA1_Stream1, ENABLE);
		//TIM_Cmd(TIM5, ENABLE);  /* 使能TIM3 */
		TIM2->EGR |= 0x00000001;	/* 由于最后一次ARR值为0，这是为了停止定时器对io口的操作，但是不要忽略了一点：CNT并没有停止计数，而且是不会再停下来，如果没有手动操作的话，所以需要在每次dma使能时加上一句，将EGR里的UG位置1，清零计数器 */
		return true;
	}
	else
	{
		return false;
	}
}
#endif

#if PWM2_4_DMA_ENAB==1
u8 Pwm2_4_DMABuffer[SK6812_SIZE]={0};
void PWM2_4_DMA_Init(void)
{
    DMA_InitTypeDef dma;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);   //使能DMA传输
    delay_ms(1);

	DMA_Cmd(DMA1_Stream7, DISABLE);
	DMA_DeInit(DMA1_Stream7);
    dma.DMA_Channel = DMA_Channel_3;
    dma.DMA_PeripheralBaseAddr = (uint32_t)(&TIM2->CCR4);  /* DMA外设基地址 *///DMA外设TIM5-CCR3地址/
    dma.DMA_Memory0BaseAddr = (uint32_t)Pwm2_4_DMABuffer; ///* DMA内存基地址 */DMA内存基地址/
    dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;/* 数据传输方向，从内存读取发送到外设 */
    dma.DMA_BufferSize = SK6812_SIZE;/* DMA通道的DMA缓存的大小 */
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/* 外设地址寄存器不变 */
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;/* 内存地址寄存器递增 */
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;/* 数据宽度为32位 */
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;/* 数据宽度为32位 */
    dma.DMA_Mode = DMA_Mode_Normal;      ///* 工作在正常模式 */工作在正常缓存模式
    dma.DMA_Priority = DMA_Priority_Medium;   //DMA通道 x拥有中优先级 
    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;   //DMA通道x没有设置为内存到内存传输
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//DMA_FIFOThreshold_HalfFull;//DMA_FIFOThreshold_1QuarterFull;
    dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		
	    /* 配置DMA */
    DMA_Init(DMA1_Stream7, &dma);

    /*使能TIM的DMA接口 */
	//TIM_SelectCCDMA(TIM5,ENABLE);
	//TIM_DMAConfig(TIM5, TIM_DMABase_CCR3, TIM_DMABurstLength_16Bytes);
	
    //TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE);	/* 如果是要调节占空比就把这行去掉注释，注释另一行，再修改相应通道 */
	TIM_DMACmd(TIM2, TIM_DMA_CC4, ENABLE);	/* 如果是要调节频率就把这行去掉注释，注释另一行，再修改相应通道 */

    DMA_Cmd(DMA1_Stream7, DISABLE);	 /*不使能DMA */  
	////TIM_Cmd(TIM2, ENABLE);  /* 使能TIM5 */	//4个通道最后只能使能一次
}

u32 Pwm2_4_Dmasetcounter=0;
bool PWM2_4_DMA_Enable(void)	//DMA1_S6C3
{
	static u8 start_flag=0;	//否则第一次无法启动
	if(DMA_GetFlagStatus(DMA1_Stream7,DMA_FLAG_TCIF7)!= RESET || start_flag!=1)	//传输完成标志，若在传输过程中再次设置将会打乱时序
	{
		start_flag=1;
		DMA_ClearFlag(DMA1_Stream7,DMA_IT_TCIF7);
		DMA_Cmd(DMA1_Stream7, DISABLE );
		DMA_SetCurrDataCounter(DMA1_Stream7,Pwm2_4_Dmasetcounter);	//SK6812_SIZE

		DMA_Cmd(DMA1_Stream7, ENABLE);
		//TIM_Cmd(TIM5, ENABLE);  /* 使能TIM3 */
		TIM2->EGR |= 0x00000001;	/* 由于最后一次ARR值为0，这是为了停止定时器对io口的操作，但是不要忽略了一点：CNT并没有停止计数，而且是不会再停下来，如果没有手动操作的话，所以需要在每次dma使能时加上一句，将EGR里的UG位置1，清零计数器 */
		return true;
	}
	else
	{
		return false;
	}
}
#endif


void PWM3_Init(void)
{
	GPIO_InitTypeDef          gpio;
	TIM_TimeBaseInitTypeDef   tim;
	TIM_OCInitTypeDef         oc;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC ,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   

	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	//CH1,2
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&gpio);

	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;	//CH3,4
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC,&gpio);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);//定时器3 通道1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);//定时器3 通道2
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3);//定时器3 通道3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3);//定时器3 通道4

	/* TIM3 */
	tim.TIM_Prescaler = 19-1;	//18OK
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Period = 1*5;   //4->1us	//0.5HIGH 0.75LOW 0码； 0.75HIGH 0.5LOW 1码
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&tim);	//初始化时间基数单位

	oc.TIM_OCMode = TIM_OCMode_PWM2;
	oc.TIM_OutputState = TIM_OutputState_Enable;
	oc.TIM_OutputNState = TIM_OutputState_Disable;
	oc.TIM_Pulse = 0;
	oc.TIM_OCPolarity = TIM_OCPolarity_Low;
	oc.TIM_OCNPolarity = TIM_OCPolarity_High;
	oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
	oc.TIM_OCNIdleState = TIM_OCIdleState_Set;

	TIM_OC1Init(TIM3,&oc);//定时器3 通道1
	TIM_OC2Init(TIM3,&oc);//定时器3 通道2
	TIM_OC3Init(TIM3,&oc);//定时器3 通道3
	TIM_OC4Init(TIM3,&oc);//定时器3 通道4

	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3,ENABLE);

	TIM_Cmd(TIM3,DISABLE);
	
	TIM3->CCR1=4;
	TIM3->CCR2=4;
	TIM3->CCR3=4;
	TIM3->CCR4=4;
}

#if PWM3_1_DMA_ENAB==1
u16 Pwm3_1_DMABuffer[SK6812_SIZE]={0x1,0x2,0x3,0x4};
void PWM3_1_DMA_Init(void)
{
    DMA_InitTypeDef dma;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);   //使能DMA传输
    delay_ms(5);

	DMA_Cmd(DMA1_Stream4, DISABLE);
	DMA_DeInit(DMA1_Stream4);
    dma.DMA_Channel = DMA_Channel_5;
    dma.DMA_PeripheralBaseAddr = (uint32_t)(&TIM3->CCR1);  /* DMA外设基地址 *///DMA外设TIM5-CCR3地址/
    dma.DMA_Memory0BaseAddr = (uint32_t)Pwm3_1_DMABuffer; ///* DMA内存基地址 */DMA内存基地址/
    dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;/* 数据传输方向，从内存读取发送到外设 */
    dma.DMA_BufferSize = SK6812_SIZE;/* DMA通道的DMA缓存的大小 */
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/* 外设地址寄存器不变 */
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;/* 内存地址寄存器递增 */
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;/* 数据宽度为8位 *///  
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;/* 数据宽度为8位 *///  DMA_MemoryDataSize_Word  DMA_MemoryDataSize_HalfWord  DMA_MemoryDataSize_Byte
    dma.DMA_Mode = DMA_Mode_Normal;      ///* 工作在正常模式 */工作在正常缓存模式
    dma.DMA_Priority = DMA_Priority_Medium;   //DMA通道 x拥有中优先级 
    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;   //DMA通道x没有设置为内存到内存传输
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;//DMA_FIFOThreshold_HalfFull;//DMA_FIFOThreshold_1QuarterFull;
    dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		
	    /* 配置DMA */
    DMA_Init(DMA1_Stream4, &dma);
    /*使能TIM的DMA接口 */
	//TIM_SelectCCDMA(TIM5,ENABLE);
	//TIM_DMAConfig(TIM5, TIM_DMABase_CCR3, TIM_DMABurstLength_16Bytes);
	
    //TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE);	/* 如果是要调节占空比就把这行去掉注释，注释另一行，再修改相应通道 */
	TIM_DMACmd(TIM3, TIM_DMA_CC1, ENABLE);	/* 如果是要调节频率就把这行去掉注释，注释另一行，再修改相应通道 */

    DMA_Cmd(DMA1_Stream4, DISABLE);	 /*不使能DMA */  
	////TIM_Cmd(TIM2, ENABLE);  /* 使能TIM5 */ //4个通道只使能一次
}

u32 Pwm3_1_Dmasetcounter=0;
bool PWM3_1_DMA_Enable(void)	//DMA1_S5C3
{
	static u8 start_flag=0;	//否则第一次无法启动
	if(DMA_GetFlagStatus(DMA1_Stream4,DMA_FLAG_TCIF4)!= RESET || start_flag!=1)	//传输完成标志，若在传输过程中再次设置将会打乱时序
	{
		start_flag=1;
		DMA_ClearFlag(DMA1_Stream4,DMA_IT_TCIF4);
		DMA_Cmd(DMA1_Stream4, DISABLE );
		DMA_SetCurrDataCounter(DMA1_Stream4,Pwm3_1_Dmasetcounter);	//SK6812_SIZE

		DMA_Cmd(DMA1_Stream4, ENABLE);
		//TIM_Cmd(TIM3, ENABLE);  /* 使能TIM3 */
		TIM2->EGR |= 0x00000001;	/* 由于最后一次ARR值为0，这是为了停止定时器对io口的操作，但是不要忽略了一点：CNT并没有停止计数，而且是不会再停下来，如果没有手动操作的话，所以需要在每次dma使能时加上一句，将EGR里的UG位置1，清零计数器 */
		return true;
	}
	else
	{
		return false;
	}
}
#endif

#if PWM3_2_DMA_ENAB==1
u16 Pwm3_2_DMABuffer[SK6812_SIZE]={0};
void PWM3_2_DMA_Init(void)
{
    DMA_InitTypeDef dma;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);   //使能DMA传输
    delay_ms(5);

	DMA_Cmd(DMA1_Stream5, DISABLE);
	DMA_DeInit(DMA1_Stream5);
    dma.DMA_Channel = DMA_Channel_5;
    dma.DMA_PeripheralBaseAddr = (uint32_t)(&TIM3->CCR2);  /* DMA外设基地址 *///DMA外设TIM5-CCR3地址/
    dma.DMA_Memory0BaseAddr = (uint32_t)Pwm3_2_DMABuffer; ///* DMA内存基地址 */DMA内存基地址/
    dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;/* 数据传输方向，从内存读取发送到外设 */
    dma.DMA_BufferSize = SK6812_SIZE;/* DMA通道的DMA缓存的大小 */
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/* 外设地址寄存器不变 */
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;/* 内存地址寄存器递增 */
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;/* 数据宽度为32位 */
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;/* 数据宽度为32位 */
    dma.DMA_Mode = DMA_Mode_Normal;      ///* 工作在正常模式 */工作在正常缓存模式
    dma.DMA_Priority = DMA_Priority_Medium;   //DMA通道 x拥有中优先级 
    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;   //DMA通道x没有设置为内存到内存传输
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//DMA_FIFOThreshold_HalfFull;//DMA_FIFOThreshold_1QuarterFull;
    dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		
	    /* 配置DMA */
    DMA_Init(DMA1_Stream5, &dma);

    /*使能TIM的DMA接口 */
	//TIM_SelectCCDMA(TIM5,ENABLE);
	//TIM_DMAConfig(TIM5, TIM_DMABase_CCR3, TIM_DMABurstLength_16Bytes);
	
    //TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE);	/* 如果是要调节占空比就把这行去掉注释，注释另一行，再修改相应通道 */
	TIM_DMACmd(TIM3, TIM_DMA_CC2, ENABLE);	/* 如果是要调节频率就把这行去掉注释，注释另一行，再修改相应通道 */

    DMA_Cmd(DMA1_Stream5, DISABLE);	 /*不使能DMA */  
	////TIM_Cmd(TIM2, ENABLE);  /* 使能TIM5 */	//4个通道最后只能使能一次
}

u32 Pwm3_2_Dmasetcounter=0;
bool PWM3_2_DMA_Enable(void)	//DMA1_S6C3
{
	static u8 start_flag=0;	//否则第一次无法启动
	if(DMA_GetFlagStatus(DMA1_Stream5,DMA_FLAG_TCIF5)!= RESET || start_flag!=1)	//传输完成标志，若在传输过程中再次设置将会打乱时序
	{
		start_flag=1;
		DMA_ClearFlag(DMA1_Stream5,DMA_IT_TCIF5);
		DMA_Cmd(DMA1_Stream5, DISABLE );
		DMA_SetCurrDataCounter(DMA1_Stream5,Pwm3_2_Dmasetcounter);	//SK6812_SIZE

		DMA_Cmd(DMA1_Stream5, ENABLE);
		//TIM_Cmd(TIM5, ENABLE);  /* 使能TIM3 */
		TIM2->EGR |= 0x00000001;	/* 由于最后一次ARR值为0，这是为了停止定时器对io口的操作，但是不要忽略了一点：CNT并没有停止计数，而且是不会再停下来，如果没有手动操作的话，所以需要在每次dma使能时加上一句，将EGR里的UG位置1，清零计数器 */
		return true;
	}
	else
	{
		return false;
	}
}
#endif

#if PWM3_3_DMA_ENAB==1
u16 Pwm3_3_DMABuffer[SK6812_SIZE]={0};
void PWM3_3_DMA_Init(void)
{
    DMA_InitTypeDef dma;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);   //使能DMA传输
    delay_ms(1);

	DMA_Cmd(DMA1_Stream7, DISABLE);
	DMA_DeInit(DMA1_Stream7);
    dma.DMA_Channel = DMA_Channel_5;
    dma.DMA_PeripheralBaseAddr = (uint32_t)(&TIM3->CCR3);  /* DMA外设基地址 *///DMA外设TIM5-CCR3地址/
    dma.DMA_Memory0BaseAddr = (uint32_t)Pwm3_3_DMABuffer; ///* DMA内存基地址 */DMA内存基地址/
    dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;/* 数据传输方向，从内存读取发送到外设 */
    dma.DMA_BufferSize = SK6812_SIZE;/* DMA通道的DMA缓存的大小 */
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/* 外设地址寄存器不变 */
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;/* 内存地址寄存器递增 */
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;/* 数据宽度为32位 */
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;/* 数据宽度为32位 */
    dma.DMA_Mode = DMA_Mode_Normal;      ///* 工作在正常模式 */工作在正常缓存模式
    dma.DMA_Priority = DMA_Priority_Medium;   //DMA通道 x拥有中优先级 
    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;   //DMA通道x没有设置为内存到内存传输
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//DMA_FIFOThreshold_HalfFull;//DMA_FIFOThreshold_1QuarterFull;
    dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		
	    /* 配置DMA */
    DMA_Init(DMA1_Stream7, &dma);

    /*使能TIM的DMA接口 */
	//TIM_SelectCCDMA(TIM5,ENABLE);
	//TIM_DMAConfig(TIM5, TIM_DMABase_CCR3, TIM_DMABurstLength_16Bytes);
	
    //TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE);	/* 如果是要调节占空比就把这行去掉注释，注释另一行，再修改相应通道 */
	TIM_DMACmd(TIM3, TIM_DMA_CC3, ENABLE);	/* 如果是要调节频率就把这行去掉注释，注释另一行，再修改相应通道 */

    DMA_Cmd(DMA1_Stream7, DISABLE);	 /*不使能DMA */  
	////TIM_Cmd(TIM2, ENABLE);  /* 使能TIM5 */	//4个通道最后只能使能一次
}

u32 Pwm3_3_Dmasetcounter=0;
bool PWM3_3_DMA_Enable(void)	//DMA1_S6C3
{
	static u8 start_flag=0;	//否则第一次无法启动
	if(DMA_GetFlagStatus(DMA1_Stream7,DMA_FLAG_TCIF7)!= RESET || start_flag!=1)	//传输完成标志，若在传输过程中再次设置将会打乱时序
	{
		start_flag=1;
		DMA_ClearFlag(DMA1_Stream7,DMA_IT_TCIF7);
		DMA_Cmd(DMA1_Stream7, DISABLE );
		DMA_SetCurrDataCounter(DMA1_Stream7,Pwm3_3_Dmasetcounter);	//SK6812_SIZE

		DMA_Cmd(DMA1_Stream7, ENABLE);
		//TIM_Cmd(TIM3, ENABLE);  /* 使能TIM3 */
		TIM2->EGR |= 0x00000001;	/* 由于最后一次ARR值为0，这是为了停止定时器对io口的操作，但是不要忽略了一点：CNT并没有停止计数，而且是不会再停下来，如果没有手动操作的话，所以需要在每次dma使能时加上一句，将EGR里的UG位置1，清零计数器 */
		return true;
	}
	else
	{
		return false;
	}
}
#endif

#if PWM3_4_DMA_ENAB==1
u16 Pwm3_4_DMABuffer[SK6812_SIZE]={0};
void PWM3_4_DMA_Init(void)
{
    DMA_InitTypeDef dma;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);   //使能DMA传输
    delay_ms(1);

	DMA_Cmd(DMA1_Stream2, DISABLE);
	DMA_DeInit(DMA1_Stream2);
    dma.DMA_Channel = DMA_Channel_5;
    dma.DMA_PeripheralBaseAddr = (uint32_t)(&TIM3->CCR4);  /* DMA外设基地址 *///DMA外设TIM5-CCR3地址/
    dma.DMA_Memory0BaseAddr = (uint32_t)Pwm3_4_DMABuffer; ///* DMA内存基地址 */DMA内存基地址/
    dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;/* 数据传输方向，从内存读取发送到外设 */
    dma.DMA_BufferSize = SK6812_SIZE;/* DMA通道的DMA缓存的大小 */
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/* 外设地址寄存器不变 */
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;/* 内存地址寄存器递增 */
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;/* 数据宽度为32位 */
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;/* 数据宽度为32位 */
    dma.DMA_Mode = DMA_Mode_Normal;      ///* 工作在正常模式 */工作在正常缓存模式
    dma.DMA_Priority = DMA_Priority_Medium;   //DMA通道 x拥有中优先级 
    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;   //DMA通道x没有设置为内存到内存传输
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//DMA_FIFOThreshold_HalfFull;//DMA_FIFOThreshold_1QuarterFull;
    dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		
	    /* 配置DMA */
    DMA_Init(DMA1_Stream5, &dma);

    /*使能TIM的DMA接口 */
	//TIM_SelectCCDMA(TIM5,ENABLE);
	//TIM_DMAConfig(TIM5, TIM_DMABase_CCR3, TIM_DMABurstLength_16Bytes);
	
    //TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE);	/* 如果是要调节占空比就把这行去掉注释，注释另一行，再修改相应通道 */
	TIM_DMACmd(TIM3, TIM_DMA_CC4, ENABLE);	/* 如果是要调节频率就把这行去掉注释，注释另一行，再修改相应通道 */

    DMA_Cmd(DMA1_Stream5, DISABLE);	 /*不使能DMA */  
	////TIM_Cmd(TIM2, ENABLE);  /* 使能TIM5 */	//4个通道最后只能使能一次
}

u32 Pwm3_4_Dmasetcounter=0;
bool PWM3_4_DMA_Enable(void)	//DMA1_S6C3
{
	static u8 start_flag=0;	//否则第一次无法启动
	if(DMA_GetFlagStatus(DMA1_Stream5,DMA_FLAG_TCIF5)!= RESET || start_flag!=1)	//传输完成标志，若在传输过程中再次设置将会打乱时序
	{
		start_flag=1;
		DMA_ClearFlag(DMA1_Stream5,DMA_IT_TCIF5);
		DMA_Cmd(DMA1_Stream5, DISABLE );
		DMA_SetCurrDataCounter(DMA1_Stream5,Pwm3_4_Dmasetcounter);	//SK6812_SIZE

		DMA_Cmd(DMA1_Stream5, ENABLE);
		//TIM_Cmd(TIM3, ENABLE);  /* 使能TIM3 */
		TIM2->EGR |= 0x00000001;	/* 由于最后一次ARR值为0，这是为了停止定时器对io口的操作，但是不要忽略了一点：CNT并没有停止计数，而且是不会再停下来，如果没有手动操作的话，所以需要在每次dma使能时加上一句，将EGR里的UG位置1，清零计数器 */
		return true;
	}
	else
	{
		return false;
	}
}
#endif


bool PAGE1_UpdateColor(u8 colors[][3],u16 led_nums)	//GRB 高位先发	//PWM2_1
{
	if(led_nums>SK6812_NUMS)	return false;
	for(int i=0;i<led_nums;i++)
	{
		for(int channel=SK6812_GREEN;channel<SK6812_BLUE+1;channel++)	//这两个for可合并为一个
		{
			for(int bit=7;bit>=0;bit--)
			{
				Pwm3_1_DMABuffer[i*24+channel*8+(7-bit)]=SK6812BitDef[*(colors[i] + channel)>>bit&0x01];
			}
		}
	}
	Pwm3_1_DMABuffer[led_nums*24]=0;
	Pwm3_1_Dmasetcounter=led_nums*24+1;
	/////////////////////////////////////////////
//	Pwm3_1_DMABuffer[0]=0x1;
//	Pwm3_1_DMABuffer[1]=0x2;
//	Pwm3_1_DMABuffer[2]=0x3;
//	Pwm3_1_DMABuffer[3]=0x1;
//	Pwm3_1_Dmasetcounter=4;
	PWM3_1_DMA_Enable();
	return true;
}

bool PAGE2_UpdateColor(u8 colors[][3],u16 led_nums)	//GRB 高位先发  //PWM2_2
{
	if(led_nums>SK6812_NUMS)	return false;
	for(int i=0;i<led_nums;i++)
	{
		for(int channel=SK6812_GREEN;channel<SK6812_BLUE+1;channel++)	//这两个for可合并为一个
		{
			for(int bit=7;bit>=0;bit--)
			{
				Pwm3_2_DMABuffer[i*24+channel*8+(7-bit)]=SK6812BitDef[*(colors[i] + channel)>>bit&0x01];
			}
		}
	}
	Pwm3_2_DMABuffer[led_nums*24]=0;
	Pwm3_2_Dmasetcounter=led_nums*24+1;
	/////////////////////////////////////
//	Pwm3_2_DMABuffer[0]=0x1;
//	Pwm3_2_DMABuffer[1]=0x2;
//	Pwm3_2_DMABuffer[2]=0x3;
//	Pwm3_2_DMABuffer[3]=0x4;
//	Pwm3_2_Dmasetcounter=4;
	PWM3_2_DMA_Enable();
	return true;
}

bool PAGE3_UpdateColor(u8 colors[][3],u16 led_nums)	//GRB 高位先发  //PWM2_3
{
	if(led_nums>SK6812_NUMS)	return false;
	for(int i=0;i<led_nums;i++)
	{
		for(int channel=SK6812_GREEN;channel<SK6812_BLUE+1;channel++)	//这两个for可合并为一个
		{
			for(int bit=7;bit>=0;bit--)
			{
				Pwm3_3_DMABuffer[i*24+channel*8+(7-bit)]=SK6812BitDef[*(colors[i] + channel)>>bit&0x01];
			}
		}
	}
	Pwm3_3_DMABuffer[led_nums*24]=0;
	Pwm3_3_Dmasetcounter=led_nums*24+1;
	//////////////////////////////////
//	Pwm3_3_DMABuffer[0]=0x1;
//	Pwm3_3_DMABuffer[1]=0x2;
//	Pwm3_3_DMABuffer[2]=0x3;
//	Pwm3_3_DMABuffer[3]=0x3;
//	Pwm3_3_Dmasetcounter=4;
	PWM3_3_DMA_Enable();
	return true;
}

bool PAGE4_UpdateColor(u8 colors[][3],u16 led_nums)	//GRB 高位先发  //PWM2_3
{

	if(led_nums>SK6812_NUMS)	return false;
	for(int i=0;i<led_nums;i++)
	{
		for(int channel=SK6812_GREEN;channel<SK6812_BLUE+1;channel++)	//这两个for可合并为一个
		{
			for(int bit=7;bit>=0;bit--)
			{
				Pwm2_2_DMABuffer[i*24+channel*8+(7-bit)]=SK6812BitDef[*(colors[i] + channel)>>bit&0x01];
			}
		}
	}
	Pwm2_2_DMABuffer[led_nums*24]=0;
	Pwm2_2_Dmasetcounter=led_nums*24+1;
	////////////////////////////
//	Pwm2_2_DMABuffer[0]=0x1;
//	Pwm2_2_DMABuffer[1]=0x2;
//	Pwm2_2_DMABuffer[2]=0x3;
//	Pwm2_2_DMABuffer[3]=0x2;
//	Pwm2_2_Dmasetcounter=4;
	PWM2_2_DMA_Enable();
	return true;
}

bool PAGE5_UpdateColor(u8 colors[][3],u16 led_nums)	//GRB 高位先发  //PWM2_3
{
	if(led_nums>SK6812_NUMS)	return false;
	for(int i=0;i<led_nums;i++)
	{
		for(int channel=SK6812_GREEN;channel<SK6812_BLUE+1;channel++)	//这两个for可合并为一个
		{
			for(int bit=7;bit>=0;bit--)
			{
				Pwm2_3_DMABuffer[i*24+channel*8+(7-bit)]=SK6812BitDef[*(colors[i] + channel)>>bit&0x01];
			}
		}
	}
	Pwm2_3_DMABuffer[led_nums*24]=0;
	Pwm2_3_Dmasetcounter=led_nums*24+1;
	/////////////////////////////////
//	Pwm2_3_DMABuffer[0]=0x1;
//	Pwm2_3_DMABuffer[1]=0x2;
//	Pwm2_3_DMABuffer[2]=0x3;
//	Pwm2_3_DMABuffer[3]=0x5;
//	Pwm2_3_Dmasetcounter=4;
	PWM2_3_DMA_Enable();
	return true;
}

//////void SK6812_TIM5_3_DMA_Init(void)
//////{
//////	NVIC_InitTypeDef nvic;
//////    DMA_InitTypeDef dma;
//////    
//////    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);   //使能DMA传输
//////    delay_ms(5);

//////	DMA_Cmd(DMA1_Stream0, DISABLE);
//////	DMA_DeInit(DMA1_Stream0);
//////    dma.DMA_Channel = DMA_Channel_6;
//////    dma.DMA_PeripheralBaseAddr = (uint32_t)(&TIM5->CCR3);  /* DMA外设基地址 *///DMA外设TIM5-CCR3地址/
//////    dma.DMA_Memory0BaseAddr = (uint32_t)Pwm5_3_DMABuffer; ///* DMA内存基地址 */DMA内存基地址/
//////    dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;/* 数据传输方向，从内存读取发送到外设 */
//////    dma.DMA_BufferSize = SK6812_SIZE;/* DMA通道的DMA缓存的大小 */
//////    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/* 外设地址寄存器不变 */
//////    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;/* 内存地址寄存器递增 */
//////    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;/* 数据宽度为32位 */
//////    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;/* 数据宽度为32位 */
//////    dma.DMA_Mode = DMA_Mode_Normal;      ///* 工作在正常模式 */工作在正常缓存模式
//////    dma.DMA_Priority = DMA_Priority_Medium;   //DMA通道 x拥有中优先级 
//////    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;   //DMA通道x没有设置为内存到内存传输
//////    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//DMA_FIFOThreshold_HalfFull;//DMA_FIFOThreshold_1QuarterFull;
//////    dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//////    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//////		
//////	    /* 配置DMA */
//////    DMA_Init(DMA1_Stream0, &dma);

//////    /*使能TIM的DMA接口 */
//////	//TIM_SelectCCDMA(TIM5,ENABLE);
//////	//TIM_DMAConfig(TIM5, TIM_DMABase_CCR3, TIM_DMABurstLength_16Bytes);
//////	
//////    TIM_DMACmd(TIM5, TIM_DMA_Update, ENABLE);	/* 如果是要调节占空比就把这行去掉注释，注释另一行，再修改相应通道 */
//////	//TIM_DMACmd(TIM3, TIM_DMA_CC1, ENABLE);	/* 如果是要调节频率就把这行去掉注释，注释另一行，再修改相应通道 */

//////    DMA_Cmd(DMA1_Stream0, DISABLE);	 /*不使能DMA */  
//////	TIM_Cmd(TIM5, ENABLE);  /* 使能TIM5 */
//////}

//////u32 DMAsetcounter=0;
//////bool PWM5_3_DMA_Enable(void)
//////{
//////	static u8 start_flag=0;
//////	if(DMA_GetFlagStatus(DMA1_Stream0,DMA_FLAG_TCIF0)!= RESET || start_flag!=1)	//传输完成标志，若在传输过程中再次设置将会打乱时序
//////	{
//////		start_flag=1;
//////		DMA_ClearFlag(DMA1_Stream0,DMA_IT_TCIF0);
//////		DMA_Cmd(DMA1_Stream0, DISABLE );
//////		DMA_SetCurrDataCounter(DMA1_Stream0,DMAsetcounter);	//SK6812_SIZE

//////		DMA_Cmd(DMA1_Stream0, ENABLE);
//////		//TIM_Cmd(TIM5, ENABLE);  /* 使能TIM3 */
//////		TIM5->EGR |= 0x00000001;	/* 由于最后一次ARR值为0，这是为了停止定时器对io口的操作，但是不要忽略了一点：CNT并没有停止计数，而且是不会再停下来，如果没有手动操作的话，所以需要在每次dma使能时加上一句，将EGR里的UG位置1，清零计数器 */
//////		return true;
//////	}
//////	else
//////	{
//////		return false;
//////	}
//////}

//////bool PAGE1_UpdateColor(u8 colors[][3],u16 led_nums)	//GRB 高位先发
//////{
//////	if(led_nums>SK6812_NUMS)	return false;
//////	for(int i=0;i<led_nums;i++)
//////	{
//////		for(int channel=SK6812_GREEN;channel<SK6812_BLUE+1;channel++)	//这两个for可合并为一个
//////		{
//////			for(int bit=7;bit>=0;bit--)
//////			{
//////				Pwm5_3_DMABuffer[i*24+channel*8+(7-bit)]=SK6812BitDef[*(colors[i] + channel)>>bit&0x01];
//////			}
//////		}
//////	}
//////	Pwm5_3_DMABuffer[led_nums*24]=0;
//////	DMAsetcounter=led_nums*24+1;
//////	PWM5_3_DMA_Enable();
//////	return true;
//////}
