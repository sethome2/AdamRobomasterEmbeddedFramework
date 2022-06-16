#include "referee.h"
#include "CRC8_CRC16.h"

//STD & STL
#include "map"
#include "stdlib.h"
using namespace std;

#define HEADER 0xA5
typedef struct
{
    uint8_t SOF;       //��ͷ
    uint16_t data_len; //��������ĳ���
    uint8_t seq;       //���
    uint8_t CRC8;      //CRC8У��ֵ
} frame_head;

referee_t referee;

map<unsigned int, void *> commandMem;

template <typename T>
void referee_add_command(unsigned int ID, T *data_p)
{
    commandMem.insert(pair<unsigned int, void *>(ID, (void *)data_p));
}

//���ڽ��ܳ�֡ͷ������ݣ�����cmdID,data,CRC16����
int referee_decode_data(void *target_data, unsigned char rx_data[], unsigned int len)
{
    //������CRC16У��
    //if (!verify_CRC16_check_sum(rx_data, len + 4))
    //		return 1;

    memcpy(target_data, rx_data + 2, len);
    return 0;
}

//�ⲿ����
//�Ӵ��ڽ��յĽ���
int referee_decode_full_frame(unsigned char rx_data[], unsigned int len)
{
    if (len < 9) //̫����   
        return -1;
    else if (rx_data[0] != HEADER) //֡ͷ����
        return -2;

    //����֡ͷ����
    frame_head this_head;
    memcpy(&this_head, rx_data, sizeof(frame_head));
    //�˴�Ӧ��CRC8У��

    uint16_t commandID = rx_data[sizeof(frame_head) + 1] + rx_data[sizeof(frame_head) + 2] * 256;

    map<unsigned int, void *>::iterator iter = commandMem.find(commandID);
    if (iter != commandMem.end())
        return referee_decode_data(iter->second, rx_data + 5, this_head.data_len);
    else
        return -3; //�Ҳ�������
}

/**
 * @brief �����͸�����ϵͳ�����ݴ��
 * 
 * @tparam T ���͵Ľṹ��
 * @param commandID ����ID
 * @param tx_data �������ݵ�����
 * @param len �������ݵĻ��峤�ȣ�������10���ϣ�Ȼ����޸�Ϊ����ʵ�ʳ���
 * @param target ���͵Ľṹ���ڴ��ַ
 */
template <typename T>
void referre_encode_frame(unsigned int commandID, unsigned char tx_data[], unsigned int *len, T *target)
{
    static uint8_t seq = 0;
    if (*len < 10)
        return;

    //����֡ͷ
    frame_head send_head;
    send_head.SOF = HEADER;
    send_head.seq = seq++;
    send_head.data_len = sizeof(T);
    //send_head.CRC8 = ?
    memcpy(&send_head, tx_data, sizeof(frame_head));

    //����ID
    union
    {
        uint8_t uint8_data[2];
        uint16_t ID;
    } ID;
    ID.ID = commandID;
    tx_data[sizeof(frame_head) + 1] = ID.uint8_data[0];
    tx_data[sizeof(frame_head) + 2] = ID.uint8_data[1];

    //�������ݲ���
    memcpy(target, tx_data + sizeof(frame_head) + 3, sizeof(T));

    //CRC16У��

    //���س���
    *len = sizeof(frame_head) + 2 + sizeof(T) + 2; //����Ϊ ֡ͷ + ����ID��2��+ ���ݲ��� + CRC16��2��
}

void referee_init()
{
    referee_add_command(0x0001, &referee.game_status);
    referee_add_command(0x0002, &referee.game_result);
    referee_add_command(0x0003, &referee.game_robot_HP);
}
