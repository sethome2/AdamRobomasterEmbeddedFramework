/*
 * @Author: sethome
 * @Date: 2021-12-06 11:32:52
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Applications\Software\gimbal.c
 */
#include "gimbal.h"
#include "IMU_updata.h"
#include "CAN_receive&send.h"
#include "small_tools.h"

#include "USB_VirCom.h"

struct gimbal_status gimbal;

pid_t pitch_speed_pid;
pid_t pitch_location_pid;

pid_t yaw_speed_pid;
pid_t yaw_location_pid;

//��̨�������
#define PITCH_MOTOR CAN_2_2
#define YAW_MOTOR CAN_2_1

//�ȶ��̶�
sliding_variance pitch_variance;
sliding_variance yaw_variance;

//��̨��ʼ��
void gimbal_init()
{
  pid_set(&yaw_speed_pid, 3.45, 0.1f, 0.1f, 2000.0f, 200.0f);
  pid_set(&yaw_location_pid, 40, 0, 0.245f, 3600.0f, 0.0f);

  pid_set(&pitch_speed_pid, 5.5f, 0.05f, 0.1f, 2500.0f, 300.0f);
  pid_set(&pitch_location_pid, 60.0f, 0.0f, 0.0f, 3000.0f, 0.0f);

  gimbal.pitch.now = 0;
  gimbal.pitch.set = 0;
  gimbal.pitch.offset = 0;
  gimbal.pitch.stable = 0;
  gimbal.set_pitch_speed = 0;

  gimbal.yaw.now = 0;
  gimbal.yaw.set = 0;
  gimbal.yaw.offset = 0;
  gimbal.yaw.stable = 0;
  gimbal.set_yaw_speed = 0;

  gimbal.yaw_status = gimbal.pitch_status = LOCATION; // Ĭ��Ϊλ�ÿ���ģʽ

  sliding_variance_init(&pitch_variance);
  sliding_variance_init(&yaw_variance);
}

void gimbal_set_offset(float pitch, float yaw)
{
  gimbal.pitch.offset = pitch;
  gimbal.yaw.offset = yaw;
}

void gimbal_updata()
{
  //�������ǻ�����ȡ����
  // yaw�����
  // decode_as_6020(YAW_MOTOR);
  decode_as_3508(YAW_MOTOR);
  gimbal.yaw.now = (get_motor_data(YAW_MOTOR).angle_cnt) - gimbal.yaw.offset;

  // pitch�����
  // decode_as_6020(PITCH_MOTOR);
  decode_as_3508(PITCH_MOTOR);
  gimbal.pitch.now = (get_motor_data(PITCH_MOTOR).angle_cnt) - gimbal.pitch.offset;

  gimbal.pitch.stable = sliding_variance_cal(&pitch_variance, gimbal.pitch.now);
  gimbal.yaw.stable = sliding_variance_cal(&yaw_variance, gimbal.yaw.now);
}

//�趨�Ƕ�
void gimbal_set(float pitch, float yaw)
{
  gimbal.yaw_status = gimbal.pitch_status = LOCATION; // ��λ��ģʽ����

  //��Χ�޶�
  if (fabs(pitch) < 180)
    gimbal.pitch.set = pitch;

  gimbal.yaw.set = yaw;
}

void gimbal_set_pitch(float pitch)
{
  gimbal.pitch_status = LOCATION; // ��λ��ģʽ����

  if (fabs(pitch) < 180)
    gimbal.pitch.set = pitch;
}

//�趨�ٶ�
void gimbal_set_speed(float pitch, float yaw)
{
  gimbal.yaw_status = gimbal.pitch_status = SPEED; // ���ٶ�ģʽ����

  gimbal.set_pitch_speed = pitch;
  gimbal.set_yaw_speed = yaw;
}
void gimbal_set_yaw_speed(float yaw)
{
  gimbal.yaw_status = SPEED; // ���ٶ�ģʽ����
  
  gimbal.set_yaw_speed = yaw;
}

void gimbal_pid_cal()
{
  // λ�û� ��λ�ÿ���ģʽ�£�
  if (gimbal.yaw_status == LOCATION)
    gimbal.yaw_speed = pid_cal(&yaw_location_pid, gimbal.yaw.now, gimbal.yaw.set);
  else
    gimbal.yaw_speed = gimbal.set_yaw_speed;

  if (gimbal.pitch_status == LOCATION)
    gimbal.pitch_speed = pid_cal(&pitch_location_pid, gimbal.pitch.now, gimbal.pitch.set);
  else
    gimbal.pitch_speed = gimbal.set_pitch_speed;

  //�ٶȻ�
  set_motor(pid_cal(&yaw_speed_pid, get_motor_data(YAW_MOTOR).speed_rpm, gimbal.yaw_speed), YAW_MOTOR);
  set_motor(pid_cal(&pitch_speed_pid, get_motor_data(PITCH_MOTOR).speed_rpm, gimbal.pitch_speed), PITCH_MOTOR);
}
