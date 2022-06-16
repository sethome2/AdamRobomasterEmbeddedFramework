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
#define PITCH_MOTOR CAN_2_6
#define YAW_MOTOR CAN_2_5

//�ȶ��̶�
sliding_variance pitch_variance;
sliding_variance yaw_variance;

//��̨��ʼ��
void gimbal_init()
{
  //����
   pid_set(&yaw_speed_pid, 60, 0.1f, 0.1f, 10000.0f, 0.0f);
   pid_set(&yaw_location_pid, 20, 0, 0.01f, 3600.0f, 0.0f);

   pid_set(&pitch_speed_pid, 15.0f, 1.0f, 1.0f, 30000.0f, 300.0f);

   pid_set(&pitch_location_pid, 50.0f, 1.5f, 0.5f, 2000.0f, 0.0f);

  //Ӣ��
//  pid_set(&yaw_speed_pid, 80, 0.0f, 10.0f, 10000.0f, 0.0f);
//  pid_set(&yaw_location_pid, 5, 0, 30.0f, 3600.0f, 0.0f);

//  pid_set(&pitch_speed_pid, 60.0f, 0.0f, 160.0f, 30000.0f, 300.0f);
//  pid_set(&pitch_location_pid, 15.0f, 0.0f, 0.001f, 2000.0f, 0.0f);
  

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

//��ʼ�� ��ʼλ��
void gimbal_set_offset(float pitch, float yaw)
{
  gimbal.pitch.offset = pitch;
  gimbal.yaw.offset = yaw;
}

void gimbal_updata()
{
  //�������ǻ�����ȡ����
  // yaw�����
  decode_as_6020(YAW_MOTOR);
  // decode_as_3508(YAW_MOTOR);
  gimbal.yaw.now = (get_motor_data(YAW_MOTOR).angle_cnt) - gimbal.yaw.offset;
  //  gimbal.yaw.now = (get_motor_data(YAW_MOTOR).angle_cnt);
  // pitch�����
  decode_as_6020(PITCH_MOTOR);
  // decode_as_3508(PITCH_MOTOR);
  gimbal.pitch.now = (get_motor_data(PITCH_MOTOR).angle_cnt) - gimbal.pitch.offset;
  //  gimbal.pitch.now = (get_motor_data(PITCH_MOTOR).angle_cnt);
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

// char arduino[128];
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

  //    // ����ʹ��
  //		int len = sprintf(arduino,"%f,%f,%f,%d,%d\n",gimbal.pitch.now,gimbal.pitch.set,gimbal.pitch_speed,get_motor_data(PITCH_MOTOR).speed_rpm,get_motor_data(PITCH_MOTOR).set);
  //		VirCom_send(arduino,len);
}

void gimbal_clear_cnt(void)
{
  clear_motor_cnt(YAW_MOTOR);
}
