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

//���ID
#define FR 0
#define FL 1
#define BL 2
#define BR 3

void chassis_move_init(void);//�����ƶ���ʼ��
void chassis_moto_speed_calc(float vx, float vy, float vw);//��������ٶ�

void track_moto_speed_calc(float vy);//����������ٶ�

#endif
//end of file
