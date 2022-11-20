/**
 * @file Encoder.c
 * @author sethome
 * @brief AB�����������
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022 sethome
 * 
 */

#include "Encoder.h"
#include "Stm32_time.h"

//��ʼ��������
void init_Encoder(Encode_t *target)
{
	target->count = 0;
}

//���±���������
//��A������ʱ���ã���B��ֵ
void update_Encoder(Encode_t *target,int B)
{
	if(B == 1)
		target->count++;
	else
		target->count--;
	
	//�����ٶ�
	target->speed = (float)(1.0f / (float)(Get_sys_time_ms() - target->last_time));
	target->last_time = Get_sys_time_ms();
}
