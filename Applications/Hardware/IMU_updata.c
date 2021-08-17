#include "IMU_updata.h"
#include "BMI088Middleware.h"
#include "BMI088driver.h"
#include "pid.h"
#include "tim.h"
#include "ist8310driver.h"
#include "stm32f4xx_hal_def.h"

//made by sethome
//ʹ��TIM14��Ƶ����

//�������¶�PID
pid_t IMU_tempure_pid;

//�������˲���
Attitude_3D_Kalman filter;

//��ʱ������
extern TIM_HandleTypeDef htim10;

struct IMU_t IMU_data;//IMU���ݽṹ��

//��ʼ��
void IMU_init()
{
	ist8310_init();			//������
	BMI088_init();			//������
	
	pid_set(&IMU_tempure_pid,1100,0.1,0,10000,1520);//PID����
	
  HAL_TIM_Base_Start(&htim10);							//���ȵ���PID
  HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);//���ȵ���PWM
	
  HAL_TIM_Base_Start_IT(&htim14);//ʹ�ܸ����жϣ�200HZ
	
	Kalman_init(&filter,1.0f,0.05f,0.05f,0.05f,0.05f,0.05f);//�������˲�����
}

//IMU���º���
void IMU_updata()//200HZ
{
	//��ȡ�����Ǻ͵شż���Ϣ
	BMI088_read(IMU_data.gyro,IMU_data.accel,&IMU_data.temp);
	ist8310_read_mag(IMU_data.mag);
	
	//������PID����
	IMU_heat_set(pid_cal(&IMU_tempure_pid,IMU_data.temp,45.00));
	
	//�������˲�
	Kalman_update(&IMU_data.filter_result,&filter,IMU_data.accel[0],IMU_data.accel[1],IMU_data.accel[2],IMU_data.gyro[0],IMU_data.gyro[1],IMU_data.gyro[2],IMU_data.mag[0],IMU_data.mag[1],IMU_data.mag[2]);  
}

//�趨���ȵ�PWMռ�ձ�
void IMU_heat_set(uint16_t ccr)
{
	__HAL_TIM_SetCompare(&htim10,TIM_CHANNEL_1,ccr);//HAL��PWM��CCR�趨
  //TIM10->CCR1 = (pwm);
}

//end of file
