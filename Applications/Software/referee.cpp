#include "referee.h"
#include "CRC8_CRC16.h"

//STD & STL
#include "map"
#include "stdlib.h"
using namespace std;

#define HEADER 0xA5
typedef struct
{
    uint8_t SOF;       //包头
    uint16_t data_len; //数据区域的长度
    uint8_t seq;       //序号
    uint8_t CRC8;      //CRC8校验值
} frame_head;

referee_t referee;

map<unsigned int, void *> commandMem;

template <typename T>
void referee_add_command(unsigned int ID, T *data_p)
{
    commandMem.insert(pair<unsigned int, void *>(ID, (void *)data_p));
}

//用于接受除帧头后的数据，包括cmdID,data,CRC16区域
int referee_decode_data(void *target_data, unsigned char rx_data[], unsigned int len)
{
    //不想做CRC16校验
    //if (!verify_CRC16_check_sum(rx_data, len + 4))
    //		return 1;

    memcpy(target_data, rx_data + 2, len);
    return 0;
}

//外部调用
//从串口接收的解析
int referee_decode_full_frame(unsigned char rx_data[], unsigned int len)
{
    if (len < 9) //太短了   
        return -1;
    else if (rx_data[0] != HEADER) //帧头不对
        return -2;

    //复制帧头区域
    frame_head this_head;
    memcpy(&this_head, rx_data, sizeof(frame_head));
    //此处应有CRC8校验

    uint16_t commandID = rx_data[sizeof(frame_head) + 1] + rx_data[sizeof(frame_head) + 2] * 256;

    map<unsigned int, void *>::iterator iter = commandMem.find(commandID);
    if (iter != commandMem.end())
        return referee_decode_data(iter->second, rx_data + 5, this_head.data_len);
    else
        return -3; //找不到命令
}

/**
 * @brief 将发送给裁判系统的数据打包
 * 
 * @tparam T 发送的结构体
 * @param commandID 命令ID
 * @param tx_data 发送数据的数组
 * @param len 发送数据的缓冲长度，至少在10以上，然后会修改为发送实际长度
 * @param target 发送的结构体内存地址
 */
template <typename T>
void referre_encode_frame(unsigned int commandID, unsigned char tx_data[], unsigned int *len, T *target)
{
    static uint8_t seq = 0;
    if (*len < 10)
        return;

    //设置帧头
    frame_head send_head;
    send_head.SOF = HEADER;
    send_head.seq = seq++;
    send_head.data_len = sizeof(T);
    //send_head.CRC8 = ?
    memcpy(&send_head, tx_data, sizeof(frame_head));

    //设置ID
    union
    {
        uint8_t uint8_data[2];
        uint16_t ID;
    } ID;
    ID.ID = commandID;
    tx_data[sizeof(frame_head) + 1] = ID.uint8_data[0];
    tx_data[sizeof(frame_head) + 2] = ID.uint8_data[1];

    //拷贝数据部分
    memcpy(target, tx_data + sizeof(frame_head) + 3, sizeof(T));

    //CRC16校验

    //返回长度
    *len = sizeof(frame_head) + 2 + sizeof(T) + 2; //长度为 帧头 + 命令ID（2）+ 数据部分 + CRC16（2）
}

void referee_init()
{
    referee_add_command(0x0001, &referee.game_status);
    referee_add_command(0x0002, &referee.game_result);
    referee_add_command(0x0003, &referee.game_robot_HP);
}
