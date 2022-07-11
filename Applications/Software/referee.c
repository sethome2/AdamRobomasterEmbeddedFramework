#include "referee.h"
#include "CRC8_CRC16.h"
#include "stdio.h"
#include "UART_data_transmit.h"

// STD & STL 本来想用stl的
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

#define HEADER_SOF 0xA5

__packed typedef struct
{
  uint8_t SOF;       //包头
  uint16_t data_len; //数据区域的长度
  uint8_t seq;       //序号
  uint8_t CRC8;      // CRC8校验值
} frame_head;
inline void init_frame_head(frame_head *t)
{
  t->SOF = HEADER_SOF;
  t->data_len = 0;
  t->seq = 0;
  t->CRC8 = 0;
}

referee_t referee; // 裁判系统接受数据函数

#define CMD_MAX_SIZE 25
uint16_t CMD_tail_index = 0;

typedef struct
{
  uint16_t ID;
  uint16_t len;
  uint32_t cnt;
  void *p;
} CMD_callback_t; // 有序存放

CMD_callback_t CMD_callback[CMD_MAX_SIZE];
/**
 * @brief 添加新命令
 *        我知道这很危险，但是一切已经被控制好了
 * @param ID
 * @param len 长度，包含id和CRC16
 * @param data_p
 */
void referee_add_command(unsigned int ID, uint16_t len, void *data_p)
{
  if (CMD_tail_index == CMD_MAX_SIZE - 1)
    return;

  CMD_callback[CMD_tail_index].ID = ID;
  CMD_callback[CMD_tail_index].len = len;
  CMD_callback[CMD_tail_index].p = data_p;
  CMD_callback[CMD_tail_index].cnt = 0;
  CMD_tail_index++;
}

/**
 * @brief 用于接受除帧头后的数据，包括cmdID,data,CRC16区域
 *
 * @param target_data
 * @param rx_data
 * @param len 所有数据的长度 不包括 CRC16
 * @return int
 */
int referee_decode_data(void *target_data, unsigned char rx_data[], int len)
{
  memcpy(target_data, rx_data, len);
  return 0;
}

//外部调用
//从串口接收的解析
uint16_t commandID;
frame_head this_head;
int referee_decode_full_frame(uint8_t rx_data[], unsigned int len)
{
  int start = 0xFFFF;
  for (int i = 0; i < len; i++)
    if (rx_data[i] == HEADER_SOF)
    {
      start = i;
      break;
    }
  if (start == 0xFFFF) // 无帧头
    return -2;

  //	if (len < 9) // 太短了不存在
  //		return -1;

  //复制帧头区域
  memcpy(&this_head, rx_data + start, sizeof(frame_head));
  //此处应有CRC8校验,但是有CRC16，所以没必要
  // if(verify_CRC8_check_sum(rx_data + start,4))
  //  return -5;

  // 获取ID
  commandID = ((uint16_t)rx_data[5 + start] << 8) + rx_data[6 + start];

  // 二分查找 留下一届改进的空间
  // 别删了，留给老学长看看 mark dowm by: sethome
  // 不会一个while都不会写吧

  // 顺序查找命令
  for (int i = 0; i < CMD_tail_index; i++)
    if (CMD_callback[i].ID == commandID)
    {
      if (CMD_callback[i].len + 2 == this_head.data_len)
      {
        // CRC_16校验不正确
        if (!verify_CRC16_check_sum(rx_data + start, 5 + this_head.data_len + 2 + 2))
          break;

        CMD_callback[i].cnt++;
        referee_decode_data(CMD_callback[i].p, rx_data + start + 5 + 2, CMD_callback[i].len);

        return referee_decode_full_frame(rx_data + start + 5 + 2 + CMD_callback[i].len + 2, len - (start + 5 + 2 + CMD_callback[i].len + 2));
      }
      else
      {
        break; // 长度不符合
      }
    }

  return referee_decode_full_frame(rx_data + start + 1, len - (start + 1));
}

void referee_decode_a_data(uint8_t rx_data)
{
  enum status
  {
    WAIT_SOF = 0,
    WAIT_LEN_H = 1,
    WAIT_LEN_L = 2,
    WAIT_SEQ = 3,
    WAIT_CRC8 = 4,
    WAIT_ID_H = 5,
    WAIT_ID_L = 6,
    WAIT_DATA = 7,
    WAIT_CRC16_H = 8,
    WAIT_CRC16_L = 9,
  };
  // 一旦错误，跳转回SOF状态
  static int16_t status = WAIT_SOF;
  static frame_head rev_head;
  static uint16_t id = 0;
  static uint8_t idIndex = 0;
  static uint8_t index = 0;
  static uint8_t buff[255];

  switch (status)
  {
  case WAIT_SOF:
    index = idIndex = 0;
    if (rx_data == HEADER_SOF)
    {
      status = WAIT_LEN_H;
      init_frame_head(&rev_head);
    }
    break;

  case WAIT_LEN_H:
    rev_head.data_len = ((uint16_t)rx_data) << 8;
    status = WAIT_LEN_L;
    break;

  case WAIT_LEN_L:
    rev_head.data_len += rx_data;
    if (rev_head.data_len > 113)
      status = WAIT_SOF;
    else
      status = WAIT_SEQ;
    break;

  case WAIT_SEQ:
    rev_head.seq = rx_data;
    status = WAIT_CRC8;
    break;

  case WAIT_CRC8:
    rev_head.CRC8 = rx_data;
    memcpy(buff, &rev_head, sizeof(rev_head));
    if (verify_CRC8_check_sum(buff, sizeof(rev_head))) // 不正确
      status = WAIT_SOF;
    else
      status = WAIT_ID_H;
    break;

  case WAIT_ID_H:
    id = rx_data << 8;
    status = WAIT_ID_L;
    break;

  case WAIT_ID_L:
    id += rx_data;
    status = WAIT_SOF;
    for (int i = 0; i < CMD_tail_index; i++)
      if (CMD_callback[i].ID == id)
      {
        idIndex = i;
        status = WAIT_DATA;
        break;
      }

    break;

  case WAIT_DATA:
    if (index < rev_head.data_len)
    {
      buff[sizeof(frame_head) + index] = rx_data;
      index++;
    }
    else
      status = WAIT_CRC16_H;
    break;

  case WAIT_CRC16_H:
    buff[sizeof(frame_head) + index] = rx_data;
    status = WAIT_CRC16_L;
    break;

  case WAIT_CRC16_L:
    buff[sizeof(frame_head) + index + 1] = rx_data;
    memcpy(buff, &rev_head, sizeof(frame_head) + index + 2);
    if (!verify_CRC16_check_sum(buff, sizeof(frame_head) + index + 2)) // 校验正确
    {
      CMD_callback[idIndex].cnt++;
      referee_decode_data(CMD_callback[idIndex].p, buff + sizeof(frame_head), CMD_callback[idIndex].len);
    }

    status = WAIT_SOF;
    break;
  }
}

void referee_init()
{
  // 注册数据
  // 命令ID从小到大 节约排序时间 为二分创造条件 也可以使用哈夫曼编码
  referee_add_command(0x0001, 11, &referee.game_status);
  referee_add_command(0x0002, 1, &referee.game_result);
  referee_add_command(0x0003, 28, &referee.game_robot_HP);
  referee_add_command(0x0004, 3, &referee.ext_dart_status_t);
  referee_add_command(0x0005, 11, &referee.ext_ICRA_buff_debuff_zone_status_t);

  referee_add_command(0x0101, 4, &referee.ext_event_data_t);
  referee_add_command(0x0102, 3, &referee.ext_supply_projectile_action_t);
  referee_add_command(0x0104, 2, &referee.ext_referee_warning_t);
  referee_add_command(0x0105, 1, &referee.ext_dart_remaining_time_t);

  referee_add_command(0x0201, 15, &referee.ext_game_robot_status_t);
  referee_add_command(0x0202, 14, &referee.ext_power_heat_data_t);
  referee_add_command(0x0203, 16, &referee.ext_game_robot_pos_t);
  referee_add_command(0x0204, 1, &referee.ext_buff_t);
  referee_add_command(0x0205, 3, &referee.aerial_robot_energy_t);
  referee_add_command(0x0206, 1, &referee.ext_robot_hurt_t);
  referee_add_command(0x0207, 6, &referee.ext_shoot_data_t);
  referee_add_command(0x0208, 2, &referee.ext_bullet_remaining_t);
  referee_add_command(0x0209, 4, &referee.ext_rfid_status_t);
  referee_add_command(0x020A, 4, &referee.ext_dart_client_cmd_t);

  // 初始化图像部分
  char name = '0';
  for (int i = 0; i < IMG_LAYER_NUM; i++)
    referee_layers.img.layers[i].graphic_name[2] = name++;
  for (int i = 0; i < CHARS_LAYER_NUM; i++)
    referee_layers.chars.layers[i].grapic_data_struct.graphic_name[2] = name++;
}


void referee_data_pack_handle(uint16_t cmd_id, uint8_t *p_data, uint16_t len)
{
  // unsigned char i = i;

	static uint8_t seq = 0;
  static uint8_t tx_buff[200];

  uint16_t frame_length = sizeof(frame_head) + 2 + len + 2; //数据帧长度

  memset(tx_buff, 0, frame_length); //存储数据的数组清零

  /*****帧头打包*****/
  frame_head send_header;
  send_header.SOF = HEADER_SOF; //数据帧起始字节
  send_header.data_len = len;
  // memcpy(&tx_buff[1], (uint8_t *)&len, sizeof(len)); //数据帧中data的长度
  send_header.seq = seq;                           //包序号
  memcpy(tx_buff,&send_header,sizeof(frame_head));
  append_CRC8_check_sum(tx_buff, sizeof(frame_head)); //帧头校验CRC8

  /*****命令码打包*****/
  memcpy(&tx_buff[sizeof(frame_head)], (uint8_t *)&cmd_id, 2);

  /*****数据打包*****/
  memcpy(&tx_buff[sizeof(frame_head) + 2], p_data, len);
  append_CRC16_check_sum(tx_buff, frame_length); //一帧数据校验CRC16

  if (seq == 0xff)
    seq = 0;
  else
    seq++;

  UART_send_data(UART6_data, tx_buff, frame_length);
}

// 图层数据
referee_layers_t referee_layers;


void referee_layers_all_updata(void)
{
  uint8_t cache[300];

  for (int i = 0; i < IMG_LAYER_NUM; i++)
    if (referee_layers.update[i] != 0)
    {
      referee_layers.img.data_cmd_id = ADD_7_LAYER;
      referee_layers.img.sender_ID = 3;        //发送者ID，机器人对应ID，此处为红3
      referee_layers.img.receiver_ID = 0x0103; //接收者ID，操作手客户端ID，此处为红3手客户端

      memcpy(cache, &referee_layers.img, sizeof(img_layers_t));
      referee_data_pack_handle(0x301, cache, sizeof(img_layers_t));
      break;
    }

   // 检查字符更新
   for (int i = IMG_LAYER_NUM; i < IMG_LAYER_NUM + CHARS_LAYER_NUM; i++)
     if (referee_layers.update[i] != 0) // 有更新
     {
       break;
     }
}
void referee_layer_updata_img(uint8_t id)
{
  if (id > 9 || id > IMG_LAYER_NUM)
    return;

  referee_layers.update[id] = 1;

  graphic_data_struct_t *this = &referee_layers.img.layers[id];

	if(this->operate_tpye == 1)
		this->operate_tpye = 2; //图形操作，0：空操作；1：增加；2：修改；3：删除；
	else
		this->operate_tpye = 1; //图形操作，0：空操作；1：增加；2：修改；3：删除；
		
  this->graphic_tpye = 0; //图形类型，0为直线，其他的查看用户手册
  this->layer = id;        //图层数
  this->color = 1;        //颜色
  this->start_angle = 0;
  this->end_angle = 0;
  this->width = 1;
  this->width = 1;
  this->start_x = SCREEN_LENGTH / 2;
  this->start_y = SCREEN_WIDTH / 2;
  this->end_x = SCREEN_LENGTH / 2;
  this->end_y = SCREEN_WIDTH / 2 - 300;
  this->radius = 0;
}

void referee_layer_updata_chars(uint8_t id, char str[], uint8_t len)
{
  if (id > 9 || id < IMG_LAYER_NUM)
    return;

  referee_layers.update[id] = 1;

  ext_client_custom_character_t *this = &referee_layers.chars.layers[id - IMG_LAYER_NUM];

  // 拷贝字符
  memcpy((char *)this->data, str, len);

  // 修改操作类型
  if (this->grapic_data_struct.operate_tpye == 0)
    this->grapic_data_struct.operate_tpye = 1;
  if (this->grapic_data_struct.operate_tpye == 1)
    this->grapic_data_struct.operate_tpye = 2;

  this->grapic_data_struct.graphic_tpye = 7; // 字符类型
  this->grapic_data_struct.layer = id;       // 图层
  this->grapic_data_struct.color = 2;        // 绿色

  this->grapic_data_struct.start_angle = 100; // 字号？？
  this->grapic_data_struct.end_angle = 30;    // 长度？？
  this->grapic_data_struct.width = 10;        // 粗细？？
  // 坐标
  this->grapic_data_struct.start_x = SCREEN_LENGTH / 2;
  this->grapic_data_struct.start_y = SCREEN_WIDTH / 2;
}

void referee_layer_color(graphic_data_struct_t *t, enum layer_color_e set)
{
  t->color = set;
}

graphic_data_struct_t *referee_layer_get(uint8_t id)
{
	return NULL;
}