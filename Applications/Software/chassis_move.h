/*
 * @Author: sethome
 * @Date: 2021-08-18 01:50:38
 * @LastEditTime: 2021-12-04 01:38:37
 * @LastEditors: Please set LastEditors
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Applications\Software\chassis_move.h
 */
#include "struct_typedef.h"

#define __chassis_move__
#ifdef __chassis_move__

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
