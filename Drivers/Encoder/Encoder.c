#include "Encoder.h"
#include "time.h"

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
