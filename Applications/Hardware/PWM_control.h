#include "struct_typedef.h"

#ifndef PWM_CONTROL_H
#define PWM_CONTROL_H


//�ⲿ����
void PWM_servo_control_init(void);                   //��ʼ��
void set_servo_angle(uint8_t channel,float angle);   //�趨PWM����Ƕ�
void set_PIN_PWM(uint8_t channel,uint16_t CCR);    //�趨PWMռ�ձȣ�0-20000��

#endif
