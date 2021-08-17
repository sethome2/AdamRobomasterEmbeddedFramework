//author:sethome/(cc/����)
//start time:2019/03/12
//end time:2019/07/02

//ver1.0 bete
/*
  ?????????��??
	u8??
	  ?????��            ID   
	0xFF 0xFF   0x00   0x00    0xFA
					  ?????��??        ?????��
	u16??
	  ?????��            ID   
	0xFF 0xFF   0x01   0x00    0xFA  0xFA
					  ?????��??           ?????��
	float:
	  ?????��            ID   
	0xFF 0xFF   0x02   0x00    0xFA 0xFA 0xFA 0xFA
					  ?????��??               ?????��
*/

#include "struct_typedef.h"

#define __UART_data_transmit__
#ifdef 	__UART_data_transmit__

//��������
#define receive_int8_t_data 0
#define receive_int16_t_data 1
#define receive_float_data 2

//�Զ�������ID
//int8_t��

//int16_t��

//float��

//����״̬
#define wait_data_head 0
#define wait_data_formet 1
#define wait_data_ID 2
#define wait_data 3

struct serial_data_set_t//����������ת
{
  u8 data_head_count;//����ͷ������
  u8 data_formet;    //��������
  u8 data_ID;        //����ID
  u8 state;          //����״̬
  
  unsigned long long data_err_count;//����֡�������

  u8 data_receive_cache[4];//���ջ���
  int8_t point;//����ָ��

  unsigned long long send_data_count;
  unsigned long long recive_data_count;//���������ͳ��
};

struct database_t//�������ݴ���
{
  volatile int8_t int8_t_data[10];
  volatile int16_t int16_t_data[50];
  volatile float float_data[50];//�������ݻ���
};

//����ת��
union float_to_u32//��ȡfloat�ڴ�����
{
  u32 temp;
  float p;
};
int16_t u8_to_int16_t(u8 data_H, u8 data_L);//u8ת����int16_t
void int16_to_u8(u8 *data_H, u8 *data_L, int16_t source_data);//int16_tת����u8

void float_to_u8(float source_data, u8 *HL, u8 *MHL, u8 *MLL, u8 *LL);//floatת����u8
float u8_to_float(u8 cache_u8[4]);//u8ת����float

//���ݷ���
void UART_send_int8_t_data(struct database_t *database,struct serial_data_set_t *UART, int8_t data, u8 data_ID); //����int8_t��������
void UART_send_int16_t_data(struct database_t *database,struct serial_data_set_t *UART, int16_t data, u8 data_ID); //����int16_t��������
void UART_send_float_data(struct database_t *database,struct serial_data_set_t *UART, float data, u8 data_ID); //����float����

void UART_receive_processing(struct database_t *database,struct serial_data_set_t *UART,u8 data);//�����жϴ�������Ҫ��

#endif
//end of file
