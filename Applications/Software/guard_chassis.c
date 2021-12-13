/*
 * @Author: sethome
 * @Date: 2021-12-02 11:15:52
 * @LastEditTime: 2021-12-13 18:02:23
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Applications\Software\guard_chassis_move.c
 */
#include "guard_chassis.h"

#include "CAN_receive&send.h"

#include "time.h"

//algorithm
#include "pid.h"

//std
#include "math.h"
#include "stdlib.h"
#include "stdio.h"

/* motorID 1 %++++++% 0
    ___________++++__________
    ___________++++__________
             %++++++%  */

can_id Motor1 = CAN_1_1;
can_id Motor2 = CAN_1_2;
//wheel conf
#define WHEEL_RADIUS 0.15240f //m
#define PI 3.1415926f

struct guard_chassis_status chassis;

//m/s
float wheel_rpm[2];       //����ٶȣ�����ֵ��
int16_t wheel_current[2]; //��������pid����ֵ��

//����
#define MAX_SPEED 2.5 //m/s
#define MAX_ACC 1.0   //m/s^2

//���PID���Ʊ�������
pid_t speed_pid[2];
pid_t location_pid;

//���̳�ʼ��
void guard_chassis_move_init()
{
    //�ٶȻ�
    pid_set(&speed_pid[0], 1000, 0, 10, 2000, 0);
    pid_set(&speed_pid[1], 1000, 0, 10, 2000, 0);
    //λ�û�
    pid_set(&location_pid, 10, 0, 1, 2000, 0);
}

//�����������ٶȣ�ע�ⲻ�ᷢ�ͣ�
void guard_chassis_pid_calc()
{
    //λ�û�
    float cal_speed = pid_cal(&location_pid, chassis.location.now, chassis.location.set);

    //�ٶȻ�
    wheel_current[0] = pid_cal(&speed_pid[0], get_motor_data(Motor1).speed_rpm, cal_speed);
    wheel_current[1] = pid_cal(&speed_pid[1], get_motor_data(Motor2).speed_rpm, cal_speed);

    //�趨������
    set_motor(wheel_current[0], Motor1);
    set_motor(wheel_current[1], Motor2);
}

void guard_chassis_set_location(float location)
{
}
void guard_chassis_offset()
{
}

void guard_chassis_updata_location()
{
    static float last_time = 0;
    float time = Get_sys_time_s();
    
    decode_as_3508(Motor1);
    decode_as_3508(Motor2);
    //ʹ�õ����������λ
    chassis.location.last = chassis.location.now;
    chassis.location.now = (get_motor_data(Motor1).angle_cnt + get_motor_data(Motor1).angle_cnt) / 2.0;

    //�����ٶ�
    chassis.speed.last = chassis.speed.now;
    chassis.speed.last = (chassis.location.now - chassis.location.set) / (time - last_time);
    
    last_time = time;
}
