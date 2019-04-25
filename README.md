                        IFR2019程序规范样例405程序说明文档
一、工程名称：IFR_Hero_2018
!!!本程序为样例程序！！！其中内容仅作格式说明
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
　　　











键位操纵（暂定）：
任何模式下，鼠标及遥控器偏航通道仅控制云台目标姿态角，底盘实施跟随
Q,E分别为左转、右转90度


任务日志：
2018.2.30 
待完善：
1.云台漂移问题
解决方案：静态车身位置环+车身矫正或全局陀螺仪位置环或静态陀螺仪位置环+动态速度环
该版本：采用全陀螺仪位置环方案
2.升降信息传入
解决方案：陀螺仪、电机姿态融合或底盘独立陀螺仪
实验中
3.标定问题
解决方案：增加陀螺仪融合输出
已写，存在问题
4.程序跑飞
解决方案：看门狗
暂未添加
5.程序自保护
解决方案：实时监测帧率，模块掉线进入自保护状态，模块上线重新标定检测（遥控器）；检测输出信息，自检程序防疯
已添加
待写：
自动取弹


4.18待解决问题：云台基于电机反馈的控制，用于登岛时看小屏幕或者冗余控制系统
			   高速前进转向时的超调难以回中现象
5.10待解决:云台鼠标控制角度是整形虽然内部累加是浮点但是输出还是整形，导致鼠标进行长时间移动后云台会突然变化1度而不是连续变化  重要！！！！！！！！！！！！！！！！！！		5.17OK

对于底盘跟随的算法猜想：
接近底盘处减小VW，变速PID

对于变速跟随KP与启动自校正的矛盾
可以仅在ERROR绝对值下降区进行变速KP，上升去不去

注意在自动登岛时取消跟随变速PID
/************************/
第一版陀螺仪
typedef struct GYRO_Struct
{
	int16_t acc[3];
	int16_t angvel[3];
	float angle[3];
}GYRO_DATA;

extern GYRO_DATA Gyro_Data;

/************************/
第二版陀螺仪


图传切换，低电平是取弹


5.10待解决，上岛	5.11已解决（传感器插反了，标签纸写错了）


操作设计：
UP-*:全手动模式（NORMAL_STATE）	-UP UP：一起上升下降		-UP DOWN 差值上升下降
MID-MID:STOP_STATE		
MID-UP(跳变)：全自动上岛		
MID-DOWN（跳变）：全自动下岛
DOWN-*:TAKEBULLET_STATE（跳变时旋转180，并将底盘升起）（整个取弹模式摄像头自动切换，操作自动反向（不过还是要写图传向前时操作模式顺应改变））（切出取弹模式自动图传转向，操作模式改变，自动进行取弹一系列流程等等）

CTRL键按住时	待加入智能lift?云台重心扭腰？

取弹后半程退出取弹模式继续操作		5.15已解决

自动下岛开始、结束气动导轮触发时机！！！！！！！！！！！！！！！！			//下岛启动已解决
自动上岛开始、结束气动导轮触发时机！！！！！！！！！！！！！！！！			//下岛收回已经解决


读不到裁判数据时对PWM射速、射频、功率进行特殊限制！！！！！！！！！！！！！！！！！！！！	//5.19ok

普通模式减小智能转向Vy补偿、扭腰模式全部开启

摩擦轮自动开启	//5.17OK




5.18需解决：云台pitch尝试换反馈
CTRL按住进行异种扭腰，开启智能升降、云台目标值高低进行升降处理（shift）	前半部分OK，5.19后半部分OK
普通模式减小智能转向Vy补偿、扭腰模式全部开启
裁判发数、射速反馈	//
裁判丢失数据处理	//ok
键位控制复位

战场版对保护模式进行屏蔽，来保证可以动而不是屏蔽所有CAN_SEND

取弹180度有问题，工程加上C有问题

加一个键位防止小屏幕失效	ok


5.19记录需解决：
战场版对保护模式进行屏蔽，来保证可以动而不是屏蔽所有CAN_SEND	//ok
云台pitch尝试换反馈	
加一个键位防止小屏幕失效	OK
键位控制复位		OK
补给站模式与岛上补弹模式	OK

5.20	
云台pitch尝试换反馈	OK
取弹模式归0时上升不被卡住

5.21	
裁判发数记录：	ok
	第一个数，发弹量
	第二个数，前升降反馈位置
	第三个数，后升降位置
	状态灯：自动取弹模式	裁判LOST	
判断陀螺仪是否疯
普通模式减小智能转向Vy补偿、扭腰模式全部开启	OK

5.21发现陀螺仪解析000是陀螺仪自己数据问题，准备加上速度环控制yaw电机
重新写了一套底盘云台等等各种与陀螺仪相关的算法
1.云台，控制变量发生变化
2.底盘，智能跟随发生变化
3.云台转180度
4.升降融合有陀螺仪的部分
5.底盘智能爬坡（应屏蔽）
6.



5.21 待修改PWM占空比

5.22拨弹的转速
上拨弹使用弹量反馈？

5.23 根据底盘电压决定PWM？
陀螺仪OFFSET?





6.15新英雄
程序架构更改：
反馈结构体定义放在 





7.9待优化：
云台俯仰角度（包含自瞄等一切控制）	//7.10OK
底盘跟随优化		7.10OK
功率控制优化
热量控制优化		7.10OK
自瞄优化
自动取弹

7.10
热量控制		7.10OK
PC控制时，切换到停止模式进行YAW中位锁定	//7.10OK

射速闭环

自动取弹，取弹逻辑
取弹电机标定

扭腰时不开自瞄预测（以此应付敌方扭腰）	7.13OK

热量2级

QE转向

摩擦轮标定

7.13待调节，比较视觉与陀螺仪速度信号同步性

动静态不同策略

跟随速度应该距离越远越小，加以限制

新改的ROI无法识别目标跳变，引入距离参数进行跳变识别（帧差）

7.15调试弹速补偿
标定
取弹SHIFT
自瞄距离

到深圳后：测试2级、3级热量控制

关遥控器启动的BUG 键盘启动？

遥控器数据错乱自动重启	OK

从错误模式强制退出后直接进入取弹模式而不是标定
