/**
 * @file IMU_updata.h
 * @author sethome
 * @brief
 * @version 0.1
 * @date 2022-11-19
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "struct_typedef.h"
#include "Kalman_Filter_c.h"

#define __IMU_UPDATA__
#ifdef __IMU_UPDATA__

struct IMU_t
{
	fp32 gyro[3];  // degree/s
	fp32 accel[3]; // m/s^2
	fp32 mag[3];   // ut
	fp32 temp;	   // �¶�

	Attitude_3D_t KF; //�������˲����

	struct
	{
		fp32 q[4];			   //��Ԫ��
		fp32 pitch, yaw, roll; //ŷ����
	} Mahony;

	struct
	{
		fp32 q[4];			   //��Ԫ��
		fp32 pitch, yaw, roll; //ŷ����
	} madgwick;

	struct
	{
		fp32 q[4];			   //��Ԫ��
		fp32 pitch, yaw, roll; //ŷ����
		int8_t err_code;
	} AHRS;

	struct
	{
		fp32 q[4];			   //��Ԫ��
		fp32 pitch, yaw, roll; //ŷ����
		fp32 heading;		   //�����(0-360)
		int16_t error_code;
		uint8_t accel_calibration_status;
		uint8_t gyro_calibration_status;
	} BSX;
};

extern struct IMU_t IMU_data;

//�ⲿ����
void IMU_init(void);   // IMU��ʼ��
void IMU_updata(void); // IMU���ݸ���
void MagUpdate(void);  // ���������ݸ���

#endif
// end of flie
