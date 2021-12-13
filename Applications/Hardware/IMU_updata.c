#include "IMU_updata.h"
#include "BMI088Middleware.h"
#include "BMI088driver.h"
#include "pid.h"
#include "tim.h"
#include "ist8310driver.h"
#include "stm32f4xx_hal_def.h"

#include "MahonyAHRS.h"
#include "madgwick.h"

//made by sethome
//使用TIM14定频更新

//陀螺仪温度PID
pid_t IMU_tempure_pid;

//卡尔曼滤波器
Attitude_3D_Kalman filter;

//定时器变量
extern TIM_HandleTypeDef htim10;

struct IMU_t IMU_data; //IMU数据结构体

extern volatile float q0, q1, q2, q3;	 //Mahony滤波器变量
extern float SEq_1, SEq_2, SEq_3, SEq_4; //madgwick滤波器变量

//内部调用
void IMU_heat_set(uint16_t ccr); //加热电阻PWM占空比

//四元数转为欧拉角
void get_angle(fp32 q[4], fp32 *yaw, fp32 *pitch, fp32 *roll)
{
	*yaw = atan2f(2.0f * (q[0] * q[3] + q[1] * q[2]), 2.0f * (q[0] * q[0] + q[1] * q[1]) - 1.0f);
	*pitch = asinf(-2.0f * (q[1] * q[3] - q[0] * q[2]));
	*roll = atan2f(2.0f * (q[0] * q[1] + q[2] * q[3]), 2.0f * (q[0] * q[0] + q[3] * q[3]) - 1.0f);
}

//初始化
void IMU_init()
{
	ist8310_init(); //磁力计
	BMI088_init();	//陀螺仪

	IMU_data.Mahony_result.q[0] = 1.0f;
	IMU_data.Mahony_result.q[1] = 0.0f;
	IMU_data.Mahony_result.q[2] = 0.0f;
	IMU_data.Mahony_result.q[3] = 0.0f;

	IMU_data.madgwick_result.q[0] = 1.0f;
	IMU_data.madgwick_result.q[1] = 0.0f;
	IMU_data.madgwick_result.q[2] = 0.0f;
	IMU_data.madgwick_result.q[3] = 0.0f;

	pid_set(&IMU_tempure_pid, 1100, 0.1, 0, 10000, 1520); //PID设置

	HAL_TIM_Base_Start(&htim10);			   //加热电阻PID
	HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1); //加热电阻PWM

	HAL_TIM_Base_Start_IT(&htim13); //使能更新中断，1000HZ
	HAL_TIM_Base_Start_IT(&htim14); //使能更新中断，200HZ

	Kalman_init(&filter, 1.0f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f); //卡尔曼滤波参数
}

//IMU更新函数
void IMU_updata() //1000HZ
{
	//读取陀螺仪和地磁计信息
	BMI088_read(IMU_data.gyro, IMU_data.accel, &IMU_data.temp);
	//ist8310_read_mag(IMU_data.mag);

	//加热器PID计算
	//if (IMU_data.temp < 45.0f)
	IMU_heat_set(pid_cal(&IMU_tempure_pid, IMU_data.temp, 45.0f));
	//	else
	//		IMU_heat_set(5000); //停止加热

	//卡尔曼滤波
	Kalman_update(&IMU_data.KF_result, &filter, IMU_data.accel[0], IMU_data.accel[1], IMU_data.accel[2], IMU_data.gyro[0], IMU_data.gyro[1], IMU_data.gyro[2], IMU_data.mag[0], IMU_data.mag[1], IMU_data.mag[2]);

	//Mahony滤波
	MahonyAHRSupdate(IMU_data.Mahony_result.q, IMU_data.gyro[0], IMU_data.gyro[1], IMU_data.gyro[2], IMU_data.accel[0], IMU_data.accel[1], IMU_data.accel[2], IMU_data.mag[0], IMU_data.mag[1], IMU_data.mag[2]);
	get_angle(IMU_data.Mahony_result.q, &IMU_data.Mahony_result.yaw, &IMU_data.Mahony_result.pitch, &IMU_data.Mahony_result.roll);

	//madgwick滤波
	madgwickUpdate(IMU_data.gyro[0], IMU_data.gyro[1], IMU_data.gyro[2], IMU_data.accel[0], IMU_data.accel[1], IMU_data.accel[2]);
	IMU_data.madgwick_result.q[0] = SEq_1;
	IMU_data.madgwick_result.q[1] = SEq_2;
	IMU_data.madgwick_result.q[2] = SEq_3;
	IMU_data.madgwick_result.q[3] = SEq_4;
	get_angle(IMU_data.madgwick_result.q, &IMU_data.madgwick_result.yaw, &IMU_data.madgwick_result.pitch, &IMU_data.madgwick_result.roll);
}
void MagUpdate() //200Hz
{
	ist8310_read_mag(IMU_data.mag);
}

//设定加热的PWM占空比
void IMU_heat_set(uint16_t ccr)
{
	__HAL_TIM_SetCompare(&htim10, TIM_CHANNEL_1, ccr); //HAL库PWM的CCR设定
													   //TIM10->CCR1 = (pwm);
}

//end of file
