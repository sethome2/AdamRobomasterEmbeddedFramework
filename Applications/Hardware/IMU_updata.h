/*
 * @Author: your name
 * @Date: 2021-08-18 01:50:38
 * @LastEditTime: 2021-12-09 10:58:23
 * @LastEditors: Please set LastEditors
 * @Description: ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Applications\Hardware\IMU_updata.h
 */
#include "struct_typedef.h"
#include "Kalman_Filter_c.h"

#define __IMU_UPDATA__
#ifdef  __IMU_UPDATA__

struct IMU_t
{
	fp32 gyro[3];			//��/s
	fp32 accel[3];		//m/s^2
	fp32 mag[3];			//ut
	fp32 temp;				//�¶�
	
	Attitude_3D_t KF_result;//�������˲����
	
	struct
	{
		fp32 q[4];//��Ԫ��
		fp32 pitch,yaw,roll;//ŷ����
	}Mahony_result;
		
	struct
	{
		fp32 q[4];//��Ԫ��
		fp32 pitch,yaw,roll;//ŷ����
	}madgwick_result;
		
	struct
	{
		fp32 q[4];//��Ԫ��
		fp32 pitch,yaw,roll;//ŷ����
	}AHRS_result;
};

extern struct IMU_t IMU_data;

//�ⲿ����
void IMU_init(void);//IMU��ʼ��
void IMU_updata(void);//IMU���ݸ���
void MagUpdate(void);

#endif
//end of flie
