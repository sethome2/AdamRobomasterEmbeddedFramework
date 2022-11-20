/**
 * @file referee.c
 * @author sethome 
 * @brief 裁判系统通信
 * @version 0.1
 * @date 2022-05-14
 * 
 * 
 * @copyright Copyright (c) 2022 sethome
 *        
 */

#include "referee.h"
// #include "CRC8_CRC16.h"
#include "UART_data_transmit.h"

// STD & STL 本来想用stl的
#include "stdlib.h"
#include "string.h"

#define HEADER_SOF 0xA5
__packed typedef struct
{
  uint8_t SOF;       //包头
  uint16_t data_len; //数据区域的长度
  uint8_t seq;       //序号
  uint8_t CRC8;      // CRC8校验值
} frame_head;

referee_t referee; // 裁判系统接受数据函数

#define CMD_MAX_SIZE 200
uint16_t CMD_tail_index = 0;

typedef struct
{
  uint16_t ID;
  uint16_t len;
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
  CMD_callback[CMD_tail_index].ID = ID;
  CMD_callback[CMD_tail_index].len = len;
  CMD_callback[CMD_tail_index].p = data_p;
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
int referee_decode_data(void *target_data, unsigned char rx_data[], unsigned int len)
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
  int start = -1;
  for (int i = 0; i < len; i++)
    if (rx_data[i] == HEADER_SOF)
    {
      start = i;
      break;
    }
  if (start == -1) //帧头不对
    return -2;

  if (len < 9) //太短了不存在
    return -1;

  //复制帧头区域
  memcpy(&this_head, rx_data + start, sizeof(frame_head));
  //此处应有CRC8校验,但是有CRC16，所以没必要
  // if(verify_CRC8_check_sum(rx_data + start,4))
  //  return -5;

  // 获取ID
  commandID = ((uint16_t)rx_data[5 + start] << 8) + rx_data[6 + start];

  // 二分查找 留下一届改进的空间

  // 查找命令
  for (int i = 0; i < CMD_tail_index; i++)
    if (CMD_callback[i].ID == commandID)
    {
      if (CMD_callback[i].len + 2 == this_head.data_len)
      {
        // 不想写CRC_16校验
//        if (!verify_CRC16_check_sum(rx_data + start, 5 + this_head.data_len + 2 + 2))
//          return -6;

        referee_decode_full_frame(rx_data + start + 5 + this_head.data_len + 2 + 2, len - (start + 5 + this_head.data_len + 2 + 2)); // 递归检查更多帧
        return referee_decode_data(CMD_callback[i].p, rx_data + start + 5 + 2, CMD_callback[i].len);
      }
      else
        return -3; // 长度不符合
    }
  return -4; //找不到命令
}

/**
 * @brief 将发送给裁判系统的数据打包
 *
 * @param commandID 命令ID
 * @param play_load 有效载荷
 * @param len 载荷长度
 */
void referre_encode_frame(uint16_t commandID, unsigned char play_load[], uint8_t len)
{
  static uint8_t send_data[512]; // 发送数据暂存
  static uint8_t seq = 0;             // 序列号

  //设置帧头
  frame_head send_head;
  send_head.SOF = HEADER_SOF;
  send_head.seq = seq++;
  send_head.data_len = len;
  memcpy(&send_head, send_data, sizeof(frame_head));
  // append_CRC8_check_sum(send_data, sizeof(frame_head)); // 添加CRC8校验

  //设置ID
  union
  {
    uint8_t uint8_data[2];
    uint16_t ID;
  } ID;
  ID.ID = commandID;
  send_data[sizeof(frame_head)] = ID.uint8_data[0];
  send_data[sizeof(frame_head) + 1] = ID.uint8_data[1];

  //拷贝数据部分
  memcpy(play_load, send_data + 5 + 2, len);

  // CRC16校验
  // append_CRC16_check_sum(send_data, 5 + 2 + len + 2);
  UART_send_data(UART1_data, send_data, 5 + 2 + len + 2);
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

  // referee_add_command(0x0304, 14, &referee.???); 图传下发信息
}


layer_node referee_layer[10]; // 10个图层
