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
//ʹ��TIM14��Ƶ����

//�������¶�PID
pid_t IMU_tempure_pid;

//�������˲���
Attitude_3D_Kalman KF_filter;

//��ʱ������
extern TIM_HandleTypeDef htim10;

struct IMU_t IMU_data; // IMU���ݽṹ��
float FIR_state[9][IMU_FIR_FILTER_LENGTH];

extern volatile float q0, q1, q2, q3; // Mahony�˲�������

extern float SEq_1, SEq_2, SEq_3, SEq_4; // madgwick�˲�������

// �ڲ�����
void bsx_init(void);
void bsx_update(void);

void IMU_heat_set(uint16_t ccr); //���ȵ���PWMռ�ձ�

// ��Ԫ��תΪŷ����
// void get_angle(fp32 q[4], fp32 *yaw, fp32 *pitch, fp32 *roll)
//{
//	*yaw = atan2f(2.0f * (q[0] * q[3] + q[1] * q[2]), 2.0f * (q[0] * q[0] + q[1] * q[1]) - 1.0f);
//	*pitch = asinf(-2.0f * (q[1] * q[3] - q[0] * q[2]));
//	*roll = atan2f(2.0f * (q[0] * q[1] + q[2] * q[3]), 2.0f * (q[0] * q[0] + q[3] * q[3]) - 1.0f);
//}

//��ʼ��
void IMU_init()
{
	ist8310_init(); //������
	BMI088_init();	//������

	IMU_data.Mahony.q[0] = 1.0f;
	IMU_data.Mahony.q[1] = 0.0f;
	IMU_data.Mahony.q[2] = 0.0f;
	IMU_data.Mahony.q[3] = 0.0f;

	IMU_data.madgwick.q[0] = 1.0f;
	IMU_data.madgwick.q[1] = 0.0f;
	IMU_data.madgwick.q[2] = 0.0f;
	IMU_data.madgwick.q[3] = 0.0f;

	Kalman_init(&KF_filter, 1.0f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f); //�������˲�

	AHRS_init(IMU_data.AHRS.q, IMU_data.accel, IMU_data.mag); // AHRS�˲�����

	bsx_init();

	pid_set(&IMU_tempure_pid, 300, 0.01, 0, 8000, 1000); // PID����
	HAL_TIM_Base_Start(&htim10);						 //���ȵ���PID
	HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);			 //���ȵ���PWM

	HAL_TIM_Base_Start_IT(&htim13); //ʹ�ܸ����жϣ�1000HZ
	HAL_TIM_Base_Start_IT(&htim14); //ʹ�ܸ����жϣ�200HZ
}

// IMU���º���
void IMU_updata() // 200HZ
{
	//��ȡ�����Ǻ͵شż���Ϣ
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

	//������PID����
	if (IMU_data.temp < 45.0f)
		IMU_heat_set(pid_cal(&IMU_tempure_pid, IMU_data.temp, 45.0f));
	// else
	// 	IMU_heat_set(5000); //ֹͣ����

	// �������˲�
	// Kalman_update(&IMU_data.KF, &KF_filter,
	// 			  IMU_data.accel[0], IMU_data.accel[1], IMU_data.accel[2],
	// 			  IMU_data.gyro[0], IMU_data.gyro[1], IMU_data.gyro[2],
	// 			  IMU_data.mag[0], IMU_data.mag[1], IMU_data.mag[2]);

	// Mahony�˲�
	// MahonyAHRSupdate(IMU_data.Mahony.q,
	// 				 IMU_data.gyro[0], IMU_data.gyro[1], IMU_data.gyro[2],
	// 				 IMU_data.accel[0], IMU_data.accel[1], IMU_data.accel[2],
	// 				 IMU_data.mag[0], IMU_data.mag[1], IMU_data.mag[2]);
	// get_angle(IMU_data.Mahony.q, &IMU_data.Mahony.yaw, &IMU_data.Mahony.pitch, &IMU_data.Mahony.roll);

	// madgwick�˲�
	madgwickUpdate(IMU_data.gyro[0], IMU_data.gyro[1], IMU_data.gyro[2],
				   IMU_data.accel[0], IMU_data.accel[1], IMU_data.accel[2]);
	IMU_data.madgwick.q[0] = SEq_1;
	IMU_data.madgwick.q[1] = SEq_2;
	IMU_data.madgwick.q[2] = SEq_3;
	IMU_data.madgwick.q[3] = SEq_4;
	get_angle(IMU_data.madgwick.q, &IMU_data.madgwick.yaw, &IMU_data.madgwick.pitch, &IMU_data.madgwick.roll);

	// AHRS.lib
	// ���ʧ�ܣ�����0
	IMU_data.AHRS.err_code = AHRS_update(IMU_data.AHRS.q, 0.001f,
										 IMU_data.gyro,
										 IMU_data.accel,
										 IMU_data.mag);
	get_angle(IMU_data.AHRS.q, &IMU_data.AHRS.yaw, &IMU_data.AHRS.pitch, &IMU_data.AHRS.roll);

	bsx_update();
}

void MagUpdate() // ���µشż�
{
	ist8310_read_mag(IMU_data.mag);
}

//�趨���ȵ�PWMռ�ձ�
void IMU_heat_set(uint16_t ccr)
{
	__HAL_TIM_SetCompare(&htim10, TIM_CHANNEL_1, ccr); // HAL��PWM��CCR�趨
													   // TIM10->CCR1 = (pwm);
}

// BSX��
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

	uint32_t w_time_stamp = Get_sys_time_ms() * 1000; // ��ȡϵͳʱ��
	
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
