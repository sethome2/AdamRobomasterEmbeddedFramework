#include "struct_typedef.h"

#define __chassis_move__
#ifdef  __chassis_move__

struct chassis_status
{
	struct
	{
		float vx,vy,vw;
		float last_vx,last_vy,last_vw;
	}speed;
	struct
	{
		float ax,ay,aw;
	}acc;
};

extern struct chassis_status chassis;

//马达ID
#define FR 0
#define FL 1
#define BL 2
#define BR 3

void chassis_move_init(void);//底盘移动初始化
void chassis_moto_speed_calc(float vx, float vy, float vw);//计算马达速度

void track_moto_speed_calc(float vy);//计算轨道马达速度

#endif
//end of file
