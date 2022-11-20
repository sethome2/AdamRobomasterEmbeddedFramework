/**
 * @file Encoder.h
 * @author sethome
 * @brief AB�����������
 * @version 0.1
 * @date 2021-7-12
 * 
 * @copyright Copyright (c) 2022 sethome
 * 
 */

#define __ENCODER_H__
#ifdef __ENCODER_H__

// �������ṹ��
typedef struct
{
	long long count; //�ܼ���

	float speed;				  //�ٶ�
	unsigned long long last_time; //��һ�θ��µ�ʱ��

} Encode_t;

void init_Encoder(Encode_t *target);		  //��ʼ������������
void update_Encoder(Encode_t *target, int B); //���±���������

#endif // __ENCODER_H__
