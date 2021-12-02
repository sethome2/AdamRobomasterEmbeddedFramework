#include "struct_typedef.h"

#ifndef LED_CONTROL_H
#define LED_CONTROL_H

//预设颜色表
#define RED    0xFFFF0000
#define GREEN  0xFF00FF00
#define BLUE   0xFF0000FF
#define YELLOW 0xFFFFFF00
#define PINK   0xFFFFC0CB
#define ORANGE 0xFFDA6E00
#define PURPLE 0xFF800080
#define BLANK  0xFF000000

//外部调用
void led_init(void);          //LED彩灯初始化
void led_show(uint32_t aRGB); //设定的颜色
	
#endif
