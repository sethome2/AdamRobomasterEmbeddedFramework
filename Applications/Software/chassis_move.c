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
#define ROLLER_DISTANCE 100 //mm  ���
#define WHEELS_DISTANCE 100 //mm  �־�

struct chassis_status chassis;

//mm/s
float wheel_rpm[4]; //�������ת��

int16_t wheel_current[4]; //���������pid����ֵ��
int16_t track_pid;
//����
#define MAX_VX_SPEED 2.5 //m/s
#define MAX_VY_SPEED 2.5
#define MAX_VR_SPEED 3
#define MAX_TRACK_SPEED 3

//���ٶ�����
#define ACC_VX 0.01 //m/s^2
#define ACC_VY 0.3
#define ACC_VW 0xFF

//����PID����
pid_t motor_speed[4];

//���Ե�������PIDʹ��
char buff[5] = {0, 0, 0, 0, 0};
uint8_t buff1[100];

//���̳�ʼ���������?4�������ķ�֣�?
void chassis_move_init()
{
	//���õ���PID
	pid_set(&motor_speed[FR], 8000, 0, 500, 15000, 0);
	pid_set(&motor_speed[FL], 8000, 0, 500, 15000, 0);
	pid_set(&motor_speed[BL], 8000, 0, 500, 15000, 0);
	pid_set(&motor_speed[BR], 8000, 0, 500, 15000, 0);
}

//���ֵ����?
void val_limit(float *val, float MAX)
{
	if (fabs(*val) > MAX)
		if (*val > 0)
			*val = MAX;
		else
			*val = -MAX;
}

//���Ʊ仯
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

//������������ٶȣ�ע�ⲻ�ᷢ�ͣ�?
void chassis_moto_speed_calc(float vx, float vy, float vw)
{
	//static float last_vx,last_vy,last_vw;//�ϴε��ٶ�

	//�ٶ�����
	val_limit(&vx, MAX_VX_SPEED);
	val_limit(&vy, MAX_VY_SPEED);
	val_limit(&vw, MAX_VR_SPEED);

	chassis.speed.vx = vx;
	chassis.speed.vy = vy;
	chassis.speed.vw = vw;
	//���ٶ�����
	//change_limit(last_vx,&vx,ACC_VX);
	//change_limit(last_vy,&vy,ACC_VY);
	//change_limit(last_vw,&vw,ACC_VX);

	//last_vx = vx; last_vy = vy; last_vw = vw;

	//���������ٶ�
	wheel_rpm[FR] = +chassis.speed.vx - chassis.speed.vy + chassis.speed.vw;
	wheel_rpm[FL] = +chassis.speed.vx + chassis.speed.vy + chassis.speed.vw;
	wheel_rpm[BL] = -chassis.speed.vx + chassis.speed.vy + chassis.speed.vw;
	wheel_rpm[BR] = -chassis.speed.vx - chassis.speed.vy + chassis.speed.vw;

//	//�ٶȻ�PID����
//	wheel_current[FR] = pid_cal(&motor_speed[FR], get_motor_data(CAN_1, chassis_FR).round_speed * WHEEL_RADIUS * PI, wheel_rpm[FR]);
//	wheel_current[FL] = pid_cal(&motor_speed[FL], get_motor_data(CAN_1, chassis_FL).round_speed * WHEEL_RADIUS * PI, wheel_rpm[FL]);
//	wheel_current[BL] = pid_cal(&motor_speed[BL], get_motor_data(CAN_1, chassis_BL).round_speed * WHEEL_RADIUS * PI, wheel_rpm[BL]);
//	wheel_current[BR] = pid_cal(&motor_speed[BR], get_motor_data(CAN_1, chassis_BR).round_speed * WHEEL_RADIUS * PI, wheel_rpm[BR]);

//	//�趨�������?
//	set_motor_current(wheel_current[FR], CAN_1, chassis_FR);
//	set_motor_current(wheel_current[FL], CAN_1, chassis_FL);
//	set_motor_current(wheel_current[BL], CAN_1, chassis_BL);
//	set_motor_current(wheel_current[BR], CAN_1, chassis_BR);
}

