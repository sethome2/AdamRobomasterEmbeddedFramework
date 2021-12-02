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
		
};

//外部调用
void IMU_init(void);//IMU初始化
void IMU_updata(void);//IMU数据更新
void MagUpdate(void);

//内部调用
void IMU_heat_set(uint16_t ccr);//加热电阻PWM占空比
#endif
//end of flie
