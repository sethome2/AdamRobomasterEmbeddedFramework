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
#include "time.h"
#include "PWM_control.h"

//拨弹电机配置
#define TRIGGER_MOTOR CAN_2_7
#define A_BULLET_ANGEL 33.0f
pid_t trigger_speed_pid;
pid_t trigger_location_pid;

//摩擦轮电机配置 3508
#ifdef USE_3508_AS_SHOOT_MOTOR

#define SHOOT_MOTOR1 CAN_2_2
#define SHOOT_MOTOR2 CAN_2_4
pid_t shoot1_speed_pid;
pid_t shoot2_speed_pid;

#endif

shoot_t shoot;
//初始化
void shoot_init()
{
  //步兵
 #ifdef USE_3508_AS_SHOOT_MOTOR
   pid_set(&shoot1_speed_pid, 15, 0.1, 0.1, 2000, 0);
   pid_set(&shoot2_speed_pid, 15, 0.1, 0.1, 2000, 0);
 #endif

   pid_set(&trigger_speed_pid, 3, 0.1, 1, 5000, 300);
   pid_set(&trigger_location_pid, 100, 0.1, 1, 3000, 100);

//英雄
//#ifdef USE_3508_AS_SHOOT_MOTOR
//  pid_set(&shoot1_speed_pid, 20, 0.1, 0.1, 2000, 0);
//  pid_set(&shoot2_speed_pid, 20, 0.1, 0.1, 2000, 0);
//#endif

//  pid_set(&trigger_speed_pid, 8, 0.1, 1, 5000, 300);
//  pid_set(&trigger_location_pid, 50, 0.1, 1, 5000, 300);

  shoot.trigger_status = SPEEDS;
  shoot.remainingBullets = FULL_BULLETS;
  shoot.speed_level = SHOOT_STOP;
  shoot.trigger_location.set = 0;
  shoot.trigger_location.now = 0;
}

//更新拨弹电机数据
void shoot_update()
{
#ifdef USE_3508_AS_SHOOT_MOTOR
  //如果使用3508作为拨弹电机的话
  shoot.shoot_speed[0] = get_motor_data(SHOOT_MOTOR1).speed_rpm;
  shoot.shoot_speed[1] = get_motor_data(SHOOT_MOTOR2).speed_rpm;
#endif

  // 卡弹 退单逻辑处，判断时间，临时后退一个距离

  decode_as_2006(TRIGGER_MOTOR);
  shoot.trigger_location.now = get_motor_data(TRIGGER_MOTOR).angle_cnt;
  shoot.trigger_speed = get_motor_data(TRIGGER_MOTOR).speed_rpm;
}

//摩擦轮速度设定
void shoot_set_shoot_Motor_speed(float speed)
{
#ifdef USE_3508_AS_SHOOT_MOTOR
  set_motor(pid_cal(&shoot1_speed_pid, get_motor_data(SHOOT_MOTOR1).speed_rpm, speed), SHOOT_MOTOR1);
  set_motor(pid_cal(&shoot2_speed_pid, get_motor_data(SHOOT_MOTOR2).speed_rpm, -speed), SHOOT_MOTOR2);
#else
  //适配其他拨弹电机
  PWM_snaill_set(PIN_2, (uint16_t)speed);
  PWM_snaill_set(PIN_3, (uint16_t)speed);
#endif
}

void shoot_pid_cal()
{
  //摩擦轮设定
  shoot_set_shoot_Motor_speed((float)shoot.speed_level);

  //拨弹电机设定
  decode_as_2006(TRIGGER_MOTOR);

  //    float set_speed = pid_cal(&trigger_location_pid, shoot.trigger_location.now, shoot.trigger_location.set);
  if (shoot.trigger_status == LOCATIONS) //位置控制
  {
    shoot.trigger_speed = pid_cal(&trigger_location_pid, shoot.trigger_location.now, shoot.trigger_location.set);
  }
  else if (shoot.trigger_status == SPEEDS) //速度控制
  {
    shoot.trigger_location.now = shoot.trigger_location.set;
    shoot.trigger_speed = shoot.set_trigger_speed;
  }

  //速度环
  set_motor(pid_cal(&trigger_speed_pid, get_motor_data(TRIGGER_MOTOR).speed_rpm, shoot.trigger_speed), TRIGGER_MOTOR);
}

//拨弹轮速度设定
void trigger_set_speed(float set)
{
//  if (shoot.speed_level == SHOOT_STOP)
//  {
//    shoot.speed_level = SHOOT_17;
//  }
  shoot.trigger_status = SPEEDS; // 以速度模式控制
  shoot.set_trigger_speed = set;
}

//内部调用，射出子弹
void shoot_set_trigger_location(int n)
{
  shoot.trigger_status = LOCATIONS; //以位置模式控制

  if (shoot.speed_level != SHOOT_STOP)
  {
    shoot.trigger_location.set += n * A_BULLET_ANGEL;
    shoot.remainingBullets -= n;
  }
}

//发射N颗子弹
int shoot_Bullets(int n)
{
  if (shoot.remainingBullets <= 0 || shoot.speed_level == SHOOT_STOP)
    return -1;
  else
    shoot_set_trigger_location(n);

  return n;
}

//遙控器單發
int shoot_one()
{
  static uint16_t last_time =0;
  if (Get_sys_time_ms() - last_time > 1500)
  {
    last_time = Get_sys_time_ms();
    shoot_set_trigger_location(1);
    return 1;
  }
  return -1;
}

//重设子弹数目
void shoot_reload()
{
  shoot.remainingBullets = FULL_BULLETS;
}

void shoot_change_level(enum shoot_speed set)
{
  shoot.speed_level = set;
}

void trigger_clear_cnt(void)
{
  clear_motor_cnt(TRIGGER_MOTOR);
}

void trigger_set_offset(float set)                                           //8.拨弹轮初始位置设定
{
	shoot.trigger_location.off_set=set;
	float set_speed = pid_cal(&trigger_location_pid, shoot.trigger_location.now, shoot.trigger_location.off_set);
  set_motor(pid_cal(&trigger_speed_pid, get_motor_data(TRIGGER_MOTOR).speed_rpm, set_speed), TRIGGER_MOTOR);
}
// end of file
