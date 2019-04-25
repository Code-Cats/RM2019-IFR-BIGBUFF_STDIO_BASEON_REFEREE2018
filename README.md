                        IFR2019程序规范样例405程序说明文档
一、工程名称：RM-2019_BIGGBUFF_IFR-YuXin

二、文件夹：
　　　KEIL：编译环境IDE
　　　STM32MCU:：存放stm32顶层文件，如stm32f4xx_it.c
      --- FWLIB
	  --- inc：存放库文件头文件
	  --- src：存放库文件c文件
      --- CORE：存放core核心文件
      
　　　USER:存放主函数main.c
　　　ROBOT:
　　　  --- BSP:板级初始化文件	如usart1_remote.c  !!!要求命名：外设名+功能名.c 例：usart1_remote.c 
　　　  --- APP：机器人控制应用文件 如chassis.c
　　　  --- ALGO:机器人算法文件，如pid.c filter.c
	--- ANALYSIS:底层数据解析文件，如对遥控器数据的解码  要求命名：外设名+功能名+analysis.c

说明：初始化函数调用请添加在BSP.C的BSP_Init()中。初始化GPIO函数请写上对应功能名，并放在gpio.c中

请勿修改 hit_recognition.c/.h 以及 bigbuff_6623.c/.h中的内容！！！

			（装甲板）						（装甲板下面的架子）
TIM3 CH1  --	ID0  			TIM2 CH1  --	ID0  
TIM3 CH2  --	ID1  			TIM2 CH2  --	ID1  
TIM3 CH3  --	ID2  			TIM2 CH3  --	ID2  
TIM3 CH4  --	ID3  			TIM2 CH4  --	ID3  
