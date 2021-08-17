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
	
	Attitude_3D_t filter_result;//�������˲����
};

//�ⲿ����
void IMU_init(void);//IMU��ʼ��
void IMU_updata(void);//IMU���ݸ���

//�ڲ�����
void IMU_heat_set(uint16_t ccr);//���ȵ���PWMռ�ձ�
#endif
//end of flie
