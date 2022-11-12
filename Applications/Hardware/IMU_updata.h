/*
 * @Author: your name
 * @Date: 2021-08-18 01:50:38
 * @LastEditTime: 2021-12-09 10:58:23
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Applications\Hardware\IMU_updata.h
 */
#include "struct_typedef.h"
#include "Kalman_Filter_c.h"

#define __IMU_UPDATA__
#ifdef  __IMU_UPDATA__

struct IMU_t
{
	fp32 gyro[3];			//度/s
	fp32 accel[3];		//m/s^2
	fp32 mag[3];			//ut
	fp32 temp;				//温度
	
	Attitude_3D_t KF_result;//卡尔曼滤波结果
	
	struct
	{
		fp32 q[4];//四元数
		fp32 pitch,yaw,roll;//欧拉角
	}Mahony_result;
		
	struct
	{
		fp32 q[4];//四元数
		fp32 pitch,yaw,roll;//欧拉角
	}madgwick_result;
		
	struct
	{
		fp32 q[4];//四元数
		fp32 pitch,yaw,roll;//欧拉角
	}AHRS_result;
};

extern struct IMU_t IMU_data;

//外部调用
void IMU_init(void);//IMU初始化
void IMU_updata(void);//IMU数据更新
void MagUpdate(void);

#endif
//end of flie
