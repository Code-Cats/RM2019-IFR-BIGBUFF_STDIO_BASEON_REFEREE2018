#ifndef __LED_CONTROL_H__
#define __LED_CONTROL_H__

#include "sys.h"
#include <stdbool.h>

#define GREEN_LED_ON()      GPIO_ResetBits(GPIOC, GPIO_Pin_1)
#define GREEN_LED_OFF()     GPIO_SetBits(GPIOC, GPIO_Pin_1)
#define GREEN_LED_TOGGLE()      GPIO_ToggleBits(GPIOC, GPIO_Pin_1)

#define RED_LED_ON()            GPIO_ResetBits(GPIOC, GPIO_Pin_0)
#define RED_LED_OFF()           GPIO_SetBits(GPIOC, GPIO_Pin_0)
#define RED_LED_TOGGLE()        GPIO_ToggleBits(GPIOC, GPIO_Pin_0)

#define LASER_ON()  GPIO_SetBits(GPIOC, GPIO_Pin_5)
#define LASER_OFF()  GPIO_ResetBits(GPIOC, GPIO_Pin_5)

#define DELAY_CNT  200000    //闪烁时间间隔

#define BLINK_CYCLE 150
#define BLINK_INTERVAL (BLINK_CYCLE*2*8)
void LED_Blink_Run(void);
void LED_Blink_Set(u8 frequency_green,u8 frequency_red);	//2s内闪烁次数//LED闪烁运行函数10h处理频率

#define PAGE1_ARMORSTART 93
#define PAGE1_ARMOREND 207
#define PAGE1_ALLEND 284
#define PAGE2_ARMORSTART 94
#define PAGE2_ARMOREND 211
#define PAGE2_ALLEND 287
#define PAGE3_ARMORSTART 94
#define PAGE3_ARMOREND 205
#define PAGE3_ALLEND 284
#define PAGE4_ARMORSTART 94
#define PAGE4_ARMOREND 206
#define PAGE4_ALLEND 284
#define PAGE5_ARMORSTART 91
#define PAGE5_ARMOREND 208
#define PAGE5_ALLEND 286

#define CYAN 0
#define ORANGE 1
#define BIGBUFF_ORANGE_R 0x7F
#define BIGBUFF_ORANGE_G 0x20
#define BIGBUFF_ORANGE_B 0x00
#define BIGBUFF_CYAN_R 0x00
#define BIGBUFF_CYAN_G 0x60
#define BIGBUFF_CYAN_B 0x43

void SK6812_Run(void);
void SK6812_SetStart(void);	//调用周期100ms
void SK6812_SetNormal(void);
void SK6812_SetError(void);

void SK6812_BIGBUFF_Set(void);

void SK6812_Draw_ColorSegmentation(u8 allcolors[][3],u16 seg_node[],u8 seg_color[][3],u16 seg_nums,bool smooth_flag,u8 smooth_factor);
#endif
