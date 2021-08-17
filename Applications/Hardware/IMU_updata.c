#include "IMU_updata.h"
#include "BMI088Middleware.h"
#include "BMI088driver.h"
#include "pid.h"
#include "tim.h"
#include "ist8310driver.h"
#include "stm32f4xx_hal_def.h"

//made by sethome
//使用TIM14定频更新

//陀螺仪温度PID
pid_t IMU_tempure_pid;

//卡尔曼滤波器
Attitude_3D_Kalman filter;

//定时器变量
extern TIM_HandleTypeDef htim10;

struct IMU_t IMU_data;//IMU数据结构体

//初始化
void IMU_init()
{
	ist8310_init();			//磁力计
	BMI088_init();			//陀螺仪
	
	pid_set(&IMU_tempure_pid,1100,0.1,0,10000,1520);//PID设置
	
  HAL_TIM_Base_Start(&htim10);							//加热电阻PID
  HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);//加热电阻PWM
	
  HAL_TIM_Base_Start_IT(&htim14);//使能更新中断，200HZ
	
	Kalman_init(&filter,1.0f,0.05f,0.05f,0.05f,0.05f,0.05f);//卡尔曼滤波参数
}

//IMU更新函数
void IMU_updata()//200HZ
{
	//读取陀螺仪和地磁计信息
	BMI088_read(IMU_data.gyro,IMU_data.accel,&IMU_data.temp);
	ist8310_read_mag(IMU_data.mag);
	
	//加热器PID计算
	IMU_heat_set(pid_cal(&IMU_tempure_pid,IMU_data.temp,45.00));
	
	//卡尔曼滤波
	Kalman_update(&IMU_data.filter_result,&filter,IMU_data.accel[0],IMU_data.accel[1],IMU_data.accel[2],IMU_data.gyro[0],IMU_data.gyro[1],IMU_data.gyro[2],IMU_data.mag[0],IMU_data.mag[1],IMU_data.mag[2]);  
}

//设定加热的PWM占空比
void IMU_heat_set(uint16_t ccr)
{
	__HAL_TIM_SetCompare(&htim10,TIM_CHANNEL_1,ccr);//HAL库PWM的CCR设定
  //TIM10->CCR1 = (pwm);
}

//end of file
