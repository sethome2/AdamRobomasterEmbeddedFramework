/**
 * @file IMU_updata.c
 * @author sethome
 * @brief IMU updata info
 * @version 0.1
 * @date 2022-11-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "IMU_updata.h"
#include "BMI088Middleware.h"
#include "BMI088driver.h"
#include "pid.h"
#include "tim.h"
#include "ist8310driver.h"
#include "stm32f4xx_hal_def.h"
#include "fdacoefs.h"

#include "MahonyAHRS.h"
#include "madgwick.h"
#include "AHRS.h"
#include "bsxlite_interface.h"
#include "time.h"

// made by sethome
//使用TIM14定频更新

//陀螺仪温度PID
pid_t IMU_tempure_pid;

//卡尔曼滤波器
Attitude_3D_Kalman KF_filter;

//定时器变量
extern TIM_HandleTypeDef htim10;

struct IMU_t IMU_data; // IMU数据结构体
float FIR_state[9][IMU_FIR_FILTER_LENGTH];

extern volatile float q0, q1, q2, q3; // Mahony滤波器变量

extern float SEq_1, SEq_2, SEq_3, SEq_4; // madgwick滤波器变量

// 内部调用
void bsx_init(void);
void bsx_update(void);

void IMU_heat_set(uint16_t ccr); //加热电阻PWM占空比

// 四元数转为欧拉角
// void get_angle(fp32 q[4], fp32 *yaw, fp32 *pitch, fp32 *roll)
//{
//	*yaw = atan2f(2.0f * (q[0] * q[3] + q[1] * q[2]), 2.0f * (q[0] * q[0] + q[1] * q[1]) - 1.0f);
//	*pitch = asinf(-2.0f * (q[1] * q[3] - q[0] * q[2]));
//	*roll = atan2f(2.0f * (q[0] * q[1] + q[2] * q[3]), 2.0f * (q[0] * q[0] + q[3] * q[3]) - 1.0f);
//}

//初始化
void IMU_init()
{
	ist8310_init(); //磁力计
	BMI088_init();	//陀螺仪

	IMU_data.Mahony.q[0] = 1.0f;
	IMU_data.Mahony.q[1] = 0.0f;
	IMU_data.Mahony.q[2] = 0.0f;
	IMU_data.Mahony.q[3] = 0.0f;

	IMU_data.madgwick.q[0] = 1.0f;
	IMU_data.madgwick.q[1] = 0.0f;
	IMU_data.madgwick.q[2] = 0.0f;
	IMU_data.madgwick.q[3] = 0.0f;

	Kalman_init(&KF_filter, 1.0f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f); //卡尔曼滤波

	AHRS_init(IMU_data.AHRS.q, IMU_data.accel, IMU_data.mag); // AHRS滤波参数

	bsx_init();

	pid_set(&IMU_tempure_pid, 300, 0.01, 0, 8000, 1000); // PID设置
	HAL_TIM_Base_Start(&htim10);						 //加热电阻PID
	HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);			 //加热电阻PWM

	HAL_TIM_Base_Start_IT(&htim13); //使能更新中断，1000HZ
	HAL_TIM_Base_Start_IT(&htim14); //使能更新中断，200HZ
}

// IMU更新函数
void IMU_updata() // 200HZ
{
	//读取陀螺仪和地磁计信息
	BMI088_read(IMU_data.gyro, IMU_data.accel, &IMU_data.temp);
	MagUpdate();

	FIR_process((float *)&FIR_state[0], &IMU_data.accel[0], &IMU_data.accel[0], 1,
				IMU_FIR_COFFES, IMU_FIR_FILTER_LENGTH);
	FIR_process((float *)&FIR_state[1], &IMU_data.accel[1], &IMU_data.accel[1], 1,
				IMU_FIR_COFFES, IMU_FIR_FILTER_LENGTH);
	FIR_process((float *)&FIR_state[2], &IMU_data.accel[2], &IMU_data.accel[2], 1,
				IMU_FIR_COFFES, IMU_FIR_FILTER_LENGTH);
	FIR_process((float *)&FIR_state[3], &IMU_data.gyro[0], &IMU_data.gyro[0], 1,
				IMU_FIR_COFFES, IMU_FIR_FILTER_LENGTH);
	FIR_process((float *)&FIR_state[4], &IMU_data.gyro[1], &IMU_data.gyro[1], 1,
				IMU_FIR_COFFES, IMU_FIR_FILTER_LENGTH);
	FIR_process((float *)&FIR_state[5], &IMU_data.gyro[2], &IMU_data.gyro[2], 1,
				IMU_FIR_COFFES, IMU_FIR_FILTER_LENGTH);
	FIR_process((float *)&FIR_state[6], &IMU_data.mag[0], &IMU_data.mag[0], 1,
				IMU_FIR_COFFES, IMU_FIR_FILTER_LENGTH);
	FIR_process((float *)&FIR_state[7], &IMU_data.mag[1], &IMU_data.mag[1], 1,
				IMU_FIR_COFFES, IMU_FIR_FILTER_LENGTH);
	FIR_process((float *)&FIR_state[8], &IMU_data.mag[2], &IMU_data.mag[2], 1,
				IMU_FIR_COFFES, IMU_FIR_FILTER_LENGTH);

	//加热器PID计算
	if (IMU_data.temp < 45.0f)
		IMU_heat_set(pid_cal(&IMU_tempure_pid, IMU_data.temp, 45.0f));
	// else
	// 	IMU_heat_set(5000); //停止加热

	// 卡尔曼滤波
	// Kalman_update(&IMU_data.KF, &KF_filter,
	// 			  IMU_data.accel[0], IMU_data.accel[1], IMU_data.accel[2],
	// 			  IMU_data.gyro[0], IMU_data.gyro[1], IMU_data.gyro[2],
	// 			  IMU_data.mag[0], IMU_data.mag[1], IMU_data.mag[2]);

	// Mahony滤波
	// MahonyAHRSupdate(IMU_data.Mahony.q,
	// 				 IMU_data.gyro[0], IMU_data.gyro[1], IMU_data.gyro[2],
	// 				 IMU_data.accel[0], IMU_data.accel[1], IMU_data.accel[2],
	// 				 IMU_data.mag[0], IMU_data.mag[1], IMU_data.mag[2]);
	// get_angle(IMU_data.Mahony.q, &IMU_data.Mahony.yaw, &IMU_data.Mahony.pitch, &IMU_data.Mahony.roll);

	// madgwick滤波
	madgwickUpdate(IMU_data.gyro[0], IMU_data.gyro[1], IMU_data.gyro[2],
				   IMU_data.accel[0], IMU_data.accel[1], IMU_data.accel[2]);
	IMU_data.madgwick.q[0] = SEq_1;
	IMU_data.madgwick.q[1] = SEq_2;
	IMU_data.madgwick.q[2] = SEq_3;
	IMU_data.madgwick.q[3] = SEq_4;
	get_angle(IMU_data.madgwick.q, &IMU_data.madgwick.yaw, &IMU_data.madgwick.pitch, &IMU_data.madgwick.roll);

	// AHRS.lib
	// 如果失败，返回0
	IMU_data.AHRS.err_code = AHRS_update(IMU_data.AHRS.q, 0.001f,
										 IMU_data.gyro,
										 IMU_data.accel,
										 IMU_data.mag);
	get_angle(IMU_data.AHRS.q, &IMU_data.AHRS.yaw, &IMU_data.AHRS.pitch, &IMU_data.AHRS.roll);

	bsx_update();
}

void MagUpdate() // 更新地磁计
{
	ist8310_read_mag(IMU_data.mag);
}

//设定加热的PWM占空比
void IMU_heat_set(uint16_t ccr)
{
	__HAL_TIM_SetCompare(&htim10, TIM_CHANNEL_1, ccr); // HAL库PWM的CCR设定
													   // TIM10->CCR1 = (pwm);
}

// BSX库
bsxlite_instance_t BSX_instance = 0x00;
bsxlite_out_t bsxlite_fusion_out;
vector_3d_t accel_in, gyro_in;
void bsx_init()
{
	IMU_data.BSX.error_code = bsxlite_init(&BSX_instance);
	
	/** initialize input vars **/
	memset(&accel_in, 0x00, sizeof(accel_in));
	memset(&gyro_in, 0x00, sizeof(gyro_in));

	/** Initialize output variables **/
	memset(&bsxlite_fusion_out, 0x00, sizeof(bsxlite_fusion_out));
}
void bsx_update()
{
	static uint8_t count = 0;
	count++;
	if (count == 10)
		count = 0;
	else
		return;

	uint32_t w_time_stamp = Get_sys_time_ms() * 1000; // 获取系统时间
	
	accel_in.x = IMU_data.accel[0];
	accel_in.y = IMU_data.accel[1];
	accel_in.z = IMU_data.accel[2];

	gyro_in.x = IMU_data.gyro[0];
	gyro_in.y = IMU_data.gyro[1];
	gyro_in.z = IMU_data.gyro[2];

	bsxlite_return_t result = bsxlite_do_step(&BSX_instance,
											  w_time_stamp,
											  &accel_in,
											  &gyro_in,
											  &(bsxlite_fusion_out));

	IMU_data.BSX.error_code = result;
	IMU_data.BSX.accel_calibration_status = bsxlite_fusion_out.accel_calibration_status;
	IMU_data.BSX.gyro_calibration_status = bsxlite_fusion_out.gyro_calibration_status;
	IMU_data.BSX.heading = bsxlite_fusion_out.orientation.heading;
	IMU_data.BSX.pitch = bsxlite_fusion_out.orientation.pitch;
	IMU_data.BSX.yaw = bsxlite_fusion_out.orientation.yaw;
	IMU_data.BSX.roll = bsxlite_fusion_out.orientation.roll;
}

// end of file
