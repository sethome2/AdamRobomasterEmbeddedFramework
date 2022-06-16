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
#define chassis_FR CAN_1_1
#define chassis_FL CAN_1_2
#define chassis_BL CAN_1_3
#define chassis_BR CAN_1_4

// wheel conf
#define WHEEL_RADIUS 0.15240f // m
#define PI 3.1415926f

// car conf
#define ROLLER_DISTANCE 100 // mm  ���
#define WHEELS_DISTANCE 100 // mm  �־�

struct chassis_status chassis;

// mm/s
#define FR 0
#define FL 1
#define BL 2
#define BR 3
float wheel_rpm[4]; // �����ٶ�����

int16_t wheel_current[4]; // PID����ĵ������
int16_t track_pid;

//����ٶ�
#define MAX_VX_SPEED 5 // m/s
#define MAX_VY_SPEED 5
#define MAX_VR_SPEED 5
#define MAX_TRACK_SPEED 5

//�����ٶ�
#define ACC_VX 0.5 // m/s^2
#define ACC_VY 0.5
#define ACC_VW 0xFF

//����ٶȻ�PID
pid_t motor_speed[4];

// ������ʼ������
 void chassis_move_init()
 {
 	//����PID��ʼ��
 	pid_set(&motor_speed[FR], 4000, 0, 0, 8000, 0);
 	pid_set(&motor_speed[FL], 4000, 0, 0, 8000, 0);
 	pid_set(&motor_speed[BL], 4000, 0, 0, 8000, 0);
 	pid_set(&motor_speed[BR], 4000, 0, 0, 8000, 0);
 }

//Ӣ�۳�ʼ������
//void chassis_move_init()
//{
//	//����PID��ʼ��
//	pid_set(&motor_speed[FR], 3500, 0, 0, 6500, 0);
//	pid_set(&motor_speed[FL], 3500, 0, 0, 6500, 0);
//	pid_set(&motor_speed[BL], 3500, 0, 0, 6500, 0);
//	pid_set(&motor_speed[BR], 3500, 0, 0, 6500, 0);
//}

//����ֵ
void val_limit(float *val, float MAX)
{
	if (fabs(*val) > MAX)
		if (*val > 0)
			*val = MAX;
		else
			*val = -MAX;
}

//���Ʊ仯��
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

//�����������ٶ�
void chassis_moto_speed_calc(float vx, float vy, float vw)
{
	// static float last_vx,last_vy,last_vw; //���ٶ�����

	//����ٶ�����
	val_limit(&vx, MAX_VX_SPEED);
	val_limit(&vy, MAX_VY_SPEED);
	val_limit(&vw, MAX_VR_SPEED);

	decode_as_3508(CAN_1_1);
	decode_as_3508(CAN_1_2);
	decode_as_3508(CAN_1_3);
	decode_as_3508(CAN_1_4);

	chassis.speed.vx = vx;
	chassis.speed.vy = vy;
	chassis.speed.vw = vw;

	//���Ƽ��ٶ�
	// change_limit(last_vx,&vx,ACC_VX);
	// change_limit(last_vy,&vy,ACC_VY);
	// change_limit(last_vw,&vw,ACC_VX);

	// last_vx = vx; last_vy = vy; last_vw = vw;

	//�����ٶȷ���
	wheel_rpm[FR] = +chassis.speed.vx - chassis.speed.vy + chassis.speed.vw;
	wheel_rpm[FL] = +chassis.speed.vx + chassis.speed.vy + chassis.speed.vw;
	wheel_rpm[BL] = -chassis.speed.vx + chassis.speed.vy + chassis.speed.vw;
	wheel_rpm[BR] = -chassis.speed.vx - chassis.speed.vy + chassis.speed.vw;

	chassis.speed.now_x = wheel_rpm[FL] / 2.0f - wheel_rpm[BL] / 2.0f;
	chassis.speed.now_y = wheel_rpm[FL] / 2.0f - wheel_rpm[FR] / 2.0f;
	chassis.speed.now_w = wheel_rpm[FR] / 2.0f + wheel_rpm[BL] / 2.0f;

	//����������
	wheel_current[FR] = pid_cal(&motor_speed[FR], get_motor_data(chassis_FR).round_speed * WHEEL_RADIUS * PI, wheel_rpm[FR]);
	wheel_current[FL] = pid_cal(&motor_speed[FL], get_motor_data(chassis_FL).round_speed * WHEEL_RADIUS * PI, wheel_rpm[FL]);
	wheel_current[BL] = pid_cal(&motor_speed[BL], get_motor_data(chassis_BL).round_speed * WHEEL_RADIUS * PI, wheel_rpm[BL]);
	wheel_current[BR] = pid_cal(&motor_speed[BR], get_motor_data(chassis_BR).round_speed * WHEEL_RADIUS * PI, wheel_rpm[BR]);

	//���������� ��Ӧ��freeRTOS���ͣ�
	set_motor(wheel_current[FR], CAN_1_1);
	set_motor(wheel_current[FL], CAN_1_2);
	set_motor(wheel_current[BL], CAN_1_3);
	set_motor(wheel_current[BR], CAN_1_4);
}
