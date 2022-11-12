/*
 * @Author: sethome
 * @Date: 2021-12-23 12:29:35
 * @LastEditTime: 2021-12-23 22:56:34
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Applications\Software\shoot.c
 */
#include "shoot.h"

#include "main.h"

#include "pid.h"

#include "CAN_receive&send.h"
#include "PWM_control.h"

//�����������
#define TRIGGER_MOTOR CAN_1_3
#define A_BULLET_ANGEL 90.0f
pid_t trigger_speed_pid;
pid_t trigger_location_pid;

//Ħ���ֵ������ 3508
#ifdef USE_3508_AS_SHOOT_MOTOR

#define SHOOT_MOTOR1 CAN_2_3
#define SHOOT_MOTOR2 CAN_2_4
pid_t shoot1_speed_pid;
pid_t shoot2_speed_pid;

#endif

shoot_t shoot;

//��ʼ��
void shoot_init()
{
#ifdef USE_3508_AS_SHOOT_MOTOR
    pid_set(&shoot1_speed_pid, 1000, 0, 0, 2000, 0);
    pid_set(&shoot2_speed_pid, 1000, 0, 0, 2000, 0);
#endif

    pid_set(&trigger_speed_pid, 4.5, 0.01, 0, 5000, 300);
    pid_set(&trigger_location_pid, 100, 0, 1, 3000, 0);

    shoot.remainingBullets = FULL_BULLETS;
    shoot.speed_level = SHOOT_STOP;
    shoot.trigger_location.set = 0;
    shoot.trigger_location.now = 0;
}

//���²����������
void shoot_update()
{
#ifdef USE_3508_AS_SHOOT_MOTOR
    //���ʹ��3508��Ϊ��������Ļ�
    shoot.shoot_speed[0] = get_motor_data(SHOOT_MOTOR1).speed_rpm;
    shoot.shoot_speed[1] = get_motor_data(SHOOT_MOTOR2).speed_rpm;
#endif

    // ���� �˵��߼������ж�ʱ�䣬��ʱ����һ������
    
    decode_as_2006(TRIGGER_MOTOR);
    shoot.trigger_location.now = get_motor_data(TRIGGER_MOTOR).angle_cnt;
}

void shoot_set_shoot_Motor_speed(float speed)
{
#ifdef USE_3508_AS_SHOOT_MOTOR
    set_motor(pid_cal(&shoot1_speed_pid, get_motor_data(SHOOT_MOTOR1).speed_rpm, speed), SHOOT_MOTOR1);
    set_motor(pid_cal(&shoot2_speed_pid, get_motor_data(SHOOT_MOTOR2).speed_rpm, speed), SHOOT_MOTOR2);
#else
    //���������������
		PWM_snaill_set(PIN_2, (uint16_t)speed);
		PWM_snaill_set(PIN_3, (uint16_t)speed);
#endif
}

void shoot_pid_cal()
{
    //Ħ�����趨
    shoot_set_shoot_Motor_speed((float)shoot.speed_level);

    //��������趨
    decode_as_2006(TRIGGER_MOTOR);
    float set_speed = pid_cal(&trigger_location_pid, shoot.trigger_location.now, shoot.trigger_location.set);
    set_motor(pid_cal(&trigger_speed_pid, get_motor_data(TRIGGER_MOTOR).speed_rpm, set_speed), TRIGGER_MOTOR);
}

//�ڲ����ã�����ӵ�
void shoot_set_trigger_location(int n)
{
	if(shoot.speed_level != SHOOT_STOP)
	{
    shoot.trigger_location.set += n * A_BULLET_ANGEL;
		shoot.remainingBullets -= n;
	}
}

//����N���ӵ�
int shoot_Bullets(int n)
{
    if (shoot.remainingBullets <= 0 || shoot.speed_level == SHOOT_STOP)
        return -1;
    else
        shoot_set_trigger_location(n);
		
		return n;
}

//�����ӵ���Ŀ
void shoot_reload()
{
    shoot.remainingBullets = FULL_BULLETS;
}

void shoot_change_level(enum shoot_speed set)
{
    shoot.speed_level = set;
}
//end of file
