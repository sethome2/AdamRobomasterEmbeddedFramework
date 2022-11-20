//author:sethome/(cc/相相)
//start time:2019/03/12
//end time:2019/07/02

//ver1.0 bete
/*
  ?????????á??
	u8??
	  ?????·            ID   
	0xFF 0xFF   0x00   0x00    0xFA
					  ?????à??        ?????é
	u16??
	  ?????·            ID   
	0xFF 0xFF   0x01   0x00    0xFA  0xFA
					  ?????à??           ?????é
	float:
	  ?????·            ID   
	0xFF 0xFF   0x02   0x00    0xFA 0xFA 0xFA 0xFA
					  ?????à??               ?????é
*/

#include "struct_typedef.h"

#define __UART_data_transmit__
#ifdef 	__UART_data_transmit__

//数据类型
#define receive_int8_t_data 0
#define receive_int16_t_data 1
#define receive_float_data 2

//自定义数据ID
//int8_t类

//int16_t类

//float类

//接收状态
#define wait_data_head 0
#define wait_data_formet 1
#define wait_data_ID 2
#define wait_data 3

struct serial_data_set_t//串口数据中转
{
  u8 data_head_count;//数据头检测计数
  u8 data_formet;    //数据类型
  u8 data_ID;        //数据ID
  u8 state;          //接收状态
  
  unsigned long long data_err_count;//数据帧错误计数

  u8 data_receive_cache[4];//接收缓存
  int8_t point;//虚拟指针

  unsigned long long send_data_count;
  unsigned long long recive_data_count;//发送与接收统计
};

struct database_t//接受数据储存
{
  volatile int8_t int8_t_data[10];
  volatile int16_t int16_t_data[50];
  volatile float float_data[50];//接收数据缓存
};

//数据转换
union float_to_u32//提取float内存数据
{
  u32 temp;
  float p;
};
int16_t u8_to_int16_t(u8 data_H, u8 data_L);//u8转换成int16_t
void int16_to_u8(u8 *data_H, u8 *data_L, int16_t source_data);//int16_t转换成u8

void float_to_u8(float source_data, u8 *HL, u8 *MHL, u8 *MLL, u8 *LL);//float转换成u8
float u8_to_float(u8 cache_u8[4]);//u8转换成float

//数据发送
void UART_send_int8_t_data(struct database_t *database,struct serial_data_set_t *UART, int8_t data, u8 data_ID); //发送int8_t类型数据
void UART_send_int16_t_data(struct database_t *database,struct serial_data_set_t *UART, int16_t data, u8 data_ID); //发送int16_t类型数据
void UART_send_float_data(struct database_t *database,struct serial_data_set_t *UART, float data, u8 data_ID); //发送float类型

void UART_receive_processing(struct database_t *database,struct serial_data_set_t *UART,u8 data);//串口中断处理（最重要）

#endif
//end of file
