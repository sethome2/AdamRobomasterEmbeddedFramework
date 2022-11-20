/**
 * @file gimbal.h
 * @author sethome
 * @brief 
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#define __GIMBAL_H__
#ifdef __GIMBAL_H__

#include "pid.h"

enum gimbal_status_e
{
    LOCATION = 0,
    SPEED,
};

struct gimbal_status
{
    //�趨��̨����ģʽ
    enum gimbal_status_e pitch_status;
    enum gimbal_status_e yaw_status;

    struct
    {
        float set, now, last, offset;
        float stable;
    } pitch;
    float pitch_speed;
    float set_pitch_speed;

    struct
    {
        float set, now, last, offset;
        float stable;
    } yaw;
    float yaw_speed;
    float set_yaw_speed;

    // struct
    // {
    //     float now, last;
    //     float stable;
    // } roll;
};

extern struct gimbal_status gimbal;

extern pid_t pitch_speed_pid;

//�ⲿ����
void gimbal_init(void);                         //��ʼ����̨
void gimbal_set_offset(float pitch, float yaw); //��ʼ�����

void gimbal_set(float pitch, float yaw); //���ýǶ�
void gimbal_set_pitch(float pitch);      //�趨picth�Ƕ�

void gimbal_set_speed(float pitch, float yaw); //�趨�ٶ�
void gimbal_set_yaw_speed(float yaw);          //�趨yaw�ٶ�

void gimbal_pid_cal(void); //��̨PID����
void gimbal_updata(void);  //������̨����
// end of file

#endif
