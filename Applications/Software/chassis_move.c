#include "chassis_move.h"
#include "math.h"
#include "CAN_receive&send.h"
#include "pid.h"
#include "UART_data_transmit.h"
#include "stdlib.h"
#include "stdio.h"

/* motorID 1 %++++++% 0
               ++++
               ++++
           2 %++++++% 3 */
//wheel conf
#define WHEEL_RADIUS 0.15240f //m
#define PI 3.1415926f

//car conf
#define ROLLER_DISTANCE 100 //mm  Öá¾à
#define WHEELS_DISTANCE 100 //mm  ÂÖ¾à

struct chassis_status chassis;

//mm/s
float wheel_rpm[4]; //¼ÆËã³öµÄ×ªËÙ

int16_t wheel_current[4]; //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½pidï¿½ï¿½ï¿½ï¿½Öµï¿½ï¿½
int16_t track_pid;
//ï¿½ï¿½ï¿½ï¿½
#define MAX_VX_SPEED 2.5 //m/s
#define MAX_VY_SPEED 2.5
#define MAX_VR_SPEED 3
#define MAX_TRACK_SPEED 3

//ï¿½ï¿½ï¿½Ù¶ï¿½ï¿½ï¿½ï¿½ï¿½
#define ACC_VX 0.01 //m/s^2
#define ACC_VY 0.3
#define ACC_VW 0xFF

//µ×ÅÌPID±äÁ¿
pid_t motor_speed[4];

//ï¿½ï¿½ï¿½Ôµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½PIDÊ¹ï¿½ï¿½
char buff[5] = {0, 0, 0, 0, 0};
uint8_t buff1[100];

//ï¿½ï¿½ï¿½Ì³ï¿½Ê¼ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?4ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä·ï¿½Ö£ï¿?
void chassis_move_init()
{
	//ÉèÖÃµ×ÅÌPID
	pid_set(&motor_speed[FR], 8000, 0, 500, 15000, 0);
	pid_set(&motor_speed[FL], 8000, 0, 500, 15000, 0);
	pid_set(&motor_speed[BL], 8000, 0, 500, 15000, 0);
	pid_set(&motor_speed[BR], 8000, 0, 500, 15000, 0);
}

//ï¿½ï¿½ï¿½Öµï¿½ï¿½ï¿½ï¿?
void val_limit(float *val, float MAX)
{
	if (fabs(*val) > MAX)
		if (*val > 0)
			*val = MAX;
		else
			*val = -MAX;
}

//ï¿½ï¿½ï¿½Æ±ä»¯
void change_limit(float last, float *now, float limit)
{
	float change = *now - last;
	if (fabs(change) > limit)
	{
		if (change > 0)
			*now = last + limit;
		else
			*now = last - limit;
	}
}

//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ù¶È£ï¿½×¢ï¿½â²»ï¿½á·¢ï¿½Í£ï¿?
void chassis_moto_speed_calc(float vx, float vy, float vw)
{
	//static float last_vx,last_vy,last_vw;//ï¿½Ï´Îµï¿½ï¿½Ù¶ï¿½

	//ï¿½Ù¶ï¿½ï¿½ï¿½ï¿½ï¿½
	val_limit(&vx, MAX_VX_SPEED);
	val_limit(&vy, MAX_VY_SPEED);
	val_limit(&vw, MAX_VR_SPEED);

	chassis.speed.vx = vx;
	chassis.speed.vy = vy;
	chassis.speed.vw = vw;
	//ï¿½ï¿½ï¿½Ù¶ï¿½ï¿½ï¿½ï¿½ï¿½
	//change_limit(last_vx,&vx,ACC_VX);
	//change_limit(last_vy,&vy,ACC_VY);
	//change_limit(last_vw,&vw,ACC_VX);

	//last_vx = vx; last_vy = vy; last_vw = vw;

	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ù¶ï¿½
	wheel_rpm[FR] = +chassis.speed.vx - chassis.speed.vy + chassis.speed.vw;
	wheel_rpm[FL] = +chassis.speed.vx + chassis.speed.vy + chassis.speed.vw;
	wheel_rpm[BL] = -chassis.speed.vx + chassis.speed.vy + chassis.speed.vw;
	wheel_rpm[BR] = -chassis.speed.vx - chassis.speed.vy + chassis.speed.vw;

//	//ï¿½Ù¶È»ï¿½PIDï¿½ï¿½ï¿½ï¿½
//	wheel_current[FR] = pid_cal(&motor_speed[FR], get_motor_data(CAN_1, chassis_FR).round_speed * WHEEL_RADIUS * PI, wheel_rpm[FR]);
//	wheel_current[FL] = pid_cal(&motor_speed[FL], get_motor_data(CAN_1, chassis_FL).round_speed * WHEEL_RADIUS * PI, wheel_rpm[FL]);
//	wheel_current[BL] = pid_cal(&motor_speed[BL], get_motor_data(CAN_1, chassis_BL).round_speed * WHEEL_RADIUS * PI, wheel_rpm[BL]);
//	wheel_current[BR] = pid_cal(&motor_speed[BR], get_motor_data(CAN_1, chassis_BR).round_speed * WHEEL_RADIUS * PI, wheel_rpm[BR]);

//	//ï¿½è¶¨ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
//	set_motor_current(wheel_current[FR], CAN_1, chassis_FR);
//	set_motor_current(wheel_current[FL], CAN_1, chassis_FL);
//	set_motor_current(wheel_current[BL], CAN_1, chassis_BL);
//	set_motor_current(wheel_current[BR], CAN_1, chassis_BR);
}

