//author:sethome/(cc/相相)
//start time:2019/03/12
//end time:2019/07/02

//ver1.0 bete
#include "stm32f4xx_hal.h"
#include "Stxxid-Data-Transmission.h"
#include "math.h"

volatile struct database_t transmit_database;

//适配不同平台
void send_data(u8 *data, unsigned char len)
{
}

void UART_receive_processing(struct database_t *database, struct serial_data_set_t *UART, uint8_t data) //串口中断处理函数
{
  switch (UART->state) //根据接收状态执行操作
  {
  case wait_data_head: //等待数据头
  {
    if (data == 0xFF) //数据头检测
      UART->data_head_count++;

    if (UART->data_head_count == 2) //OxFF 0xFF数据头
    {
      UART->data_head_count = 0;      //清除计数
      UART->state = wait_data_formet; //确认数据头，接收数据类型
    }
    break;
  }

  case wait_data_formet: //接收数据类型
  {
    if (data != receive_float_data && data != receive_int16_t_data && data != receive_int8_t_data) //异常处理
    {
      UART->data_err_count++;       //错误计数
      UART->state = wait_data_head; //改为等待数据头
      break;
    }
    UART->data_formet = data;  //写入数据类型
    switch (UART->data_formet) //根据数据类型决定长度
    {
    case receive_int16_t_data:
      UART->point = 1;
      break;
    case receive_float_data:
      UART->point = 3;
      break;
    }
    UART->state = wait_data_ID; //修改状态，等待ID
    break;
  }

  case wait_data_ID: //接收数据ID
  {
    if (data == 0xFF) //异常处理
    {
      UART->data_err_count++;       //错误计数
      UART->state = wait_data_head; //改为等待数据头
      break;
    }
    UART->data_ID = data;
    UART->state = wait_data; //修改状态，接收数据
    break;
  }

  case wait_data: //等待接收数据
  {
    switch (UART->data_formet) //根据接收数据类型决定方式
    {
    case receive_int8_t_data:
      database->int8_t_data[UART->data_ID] = data; //写入数据

      UART->recive_data_count++;    //获得数据累加
      UART->state = wait_data_head; //等待接收数据头
      break;
    case receive_int16_t_data:
      UART->data_receive_cache[UART->point] = data;
      UART->point--;       //写入数据到缓存
      if (UART->point < 0) //写入完毕
      {
        database->int16_t_data[UART->data_ID] = u8_to_int16_t(UART->data_receive_cache[1], UART->data_receive_cache[0]); //数据转换

        UART->recive_data_count++;    //获得数据累加
        UART->state = wait_data_head; //等待接收数据头
      }
      break;
    case receive_float_data:
      UART->data_receive_cache[UART->point] = data;
      UART->point--;       //写入数据到缓存
      if (UART->point < 0) //写入完毕
      {
        database->float_data[UART->data_ID] = u8_to_float(UART->data_receive_cache); //数据转换

        UART->recive_data_count++;    //获得数据累加
        UART->state = wait_data_head; //等待接收数据头
      }
      break;
    }
    break;
  }
  }
}

//数据转换
int16_t u8_to_int16_t(u8 data_H, u8 data_L) //u8转换成int16_t
{
  int16_t temp;
  temp = data_H;
  temp <<= 8;
  temp += data_L;
  return temp;
}
void int16_to_u8(u8 *data_H, u8 *data_L, int16_t source_data) //int16_t转换成u8
{
  *data_L = (u16)source_data;
  *data_H = (u16)source_data >> 8;
}
void float_to_u8(float source_data, u8 *HL, u8 *MHL, u8 *MLL, u8 *LL) //float转换成u8
{
  union float_to_u32 temp; //转换联合体
  temp.p = source_data;

  *LL = temp.temp;
  *MLL = temp.temp >> 8;
  *MHL = temp.temp >> 16;
  *HL = temp.temp >> 24;
}
float u8_to_float(u8 cache_u8[4]) //u8转换成float
{
  float target_data;
  target_data = *((float *)cache_u8);
  return target_data;
}

//数据发送
void UART_send_int8_t_data(struct database_t *database, struct serial_data_set_t *UART, int8_t data, u8 data_ID) //发送int8_t类型数据
{
  u8 data_serial[5] = {0};
  data_serial[0] = data_serial[1] = 0xFF;//数据头

  data_serial[2] = receive_int8_t_data; //数据类型

  data_serial[3] = data_ID; //发送数据ID

  data_serial[4] = data; //发送数据

  database->int8_t_data[data_ID] = data; //修改数据库数据

  send_data(data_serial,5);

  UART->send_data_count++; //发送数据累加
}

void UART_send_int16_t_data(struct database_t *database,struct serial_data_set_t *UART, int16_t data, u8 data_ID) //发送int16_t类型数据
{
  u8 data_serial[6] = {0};
  u8 data_H, data_L; //高低位变量

  data_serial[0] = data_serial[1] = 0xFF; //发送数据头

  data_serial[2] = receive_int16_t_data;//发送数据类型

  data_serial[3] = data_ID;//发送数据ID

  int16_to_u8(&data_H, &data_L, data); //数据转换
  data_serial[4] = data_H;//发送数据
  data_serial[5] = data_L;
  
  database->int16_t_data[data_ID] = data;//修改数据库数据
  
  send_data(data_serial,6);

  UART->send_data_count++;//发送数据累加
}

void UART_send_float_data(struct database_t *database,struct serial_data_set_t *UART, float data, u8 data_ID) ////发送float类型
{
  u8 data_serial[8] = {0};
  u8 HL, MHL, MLL, LL; //高，高中，低中，低位

  data_serial[0] = data_serial[1] = 0xFF; //发送数据头

  data_serial[2] = receive_float_data;//发送数据类型

  data_serial[3] = data_ID;//发送数据ID

  float_to_u8(data, &HL, &MHL, &MLL, &LL); //数据转换
  data_serial[4] = HL;
  data_serial[5] = MHL;
  data_serial[6] = MLL;
  data_serial[7] = LL;//发送数据

  database->float_data[data_ID] = data;//修改数据库数据

  send_data(data_serial,8);

  UART->send_data_count++;//发送数据累加
}
//end of file
