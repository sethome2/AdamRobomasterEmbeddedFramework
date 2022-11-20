/**
 * @file chassis_move.h
 * @author sethome
 * @brief 底盘控制文件
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "struct_typedef.h"

#define __chassis_move__
#ifdef __chassis_move__

/* motorID 1 %++++++% 0
      			   ++++
			         ++++
		       2 %++++++% 3 */
#define chassis_FR CAN_1_1
#define chassis_FL CAN_1_2
#define chassis_BL CAN_1_3
#define chassis_BR CAN_1_4

struct chassis_status
{
	struct
	{
		float vx, vy, vw;
		float last_vx, last_vy, last_vw;
	} speed;
	struct
	{
		float ax, ay, aw;
	} acc;
};

extern struct chassis_status chassis;

void chassis_move_init(void);								//底盘移动初始化
void chassis_moto_speed_calc(float vx, float vy, float vw); //计算马达速度

#endif

// end of file
