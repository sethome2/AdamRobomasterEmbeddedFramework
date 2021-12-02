#include "CAN_receive&send.h"
#include "math.h"

//�������
motor_measure_t Can1_motor_data[16];
motor_measure_t Can2_motor_data[16];

//CAN�Ĵ�����������
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2; //����ԭ����can.c�ļ�

//������ݶ�ȡ
#define get_motor_measure(ptr, data)                               \
  {                                                                \
    (ptr)->last_ecd = (ptr)->ecd;                                  \
    (ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);           \
    (ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);     \
    (ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]); \
    (ptr)->temperate = (data)[6];                                  \
  }

//ʹ�ú���ָ��
//������ݴ���
void process_motor_data(CAN_HandleTypeDef *hcan, motor_measure_t *motor_data, int motor_ID)
{
  //���ӵ�������жϣ���ͬ���ٱ�
  UNUSED(motor_ID);
  UNUSED(hcan); //���ⱨ�� �����ӹ���

  if (motor_data->last_ecd > 7000 && motor_data->ecd < 2000)
    motor_data->ecd_cnt += ((ECD_MAX - motor_data->last_ecd) + motor_data->ecd);
  else if (motor_data->last_ecd < 2000 && motor_data->ecd > 7000)
    motor_data->ecd_cnt -= ((ECD_MAX - motor_data->ecd) + motor_data->last_ecd);
  else
    motor_data->ecd_cnt += (motor_data->ecd - motor_data->last_ecd);

  //��������ٶ�
  motor_data->round_speed = motor_data->speed_rpm / 60.0f / M3508_P;
  //�����ܽǶ�
  motor_data->angle_cnt = motor_data->ecd_cnt / ECD_MAX / M3508_P * 360.00f;
}

//HAL���жϻص�ָ��
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  CAN_RxHeaderTypeDef rx_header; //CAN ����ָ��
  motor_measure_t *motor_data;   //�������ָ��
  uint8_t rx_data[8];            //��ȡ��������

  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data); //ȡ����Ϣ

  if (hcan == &hcan1) //CAN1/2�ж�
    motor_data = Can1_motor_data;
  else
    motor_data = Can2_motor_data;

  switch (rx_header.StdId)
  {
  case CAN_ID1:
  {
    get_motor_measure(motor_data, rx_data);
    process_motor_data(hcan, motor_data, 0);
    break;
  }
  case CAN_ID2:
  {
    get_motor_measure(motor_data + 1, rx_data);
    process_motor_data(hcan, motor_data + 1, 1);
    break;
  }
  case CAN_ID3:
  {
    get_motor_measure(motor_data + 2, rx_data);
    process_motor_data(&hcan1, motor_data + 2, 2);
    break;
  }
  case CAN_ID4:
  {
    get_motor_measure(motor_data + 3, rx_data);
    process_motor_data(&hcan1, motor_data + 3, 2);
    break;
  }
  case CAN_ID5:
  {
    get_motor_measure(motor_data + 4, rx_data);
    process_motor_data(&hcan1, motor_data + 4, 2);
    break;
  }
  case CAN_ID6:
  {
    get_motor_measure(motor_data + 5, rx_data);
    process_motor_data(&hcan1, motor_data + 5, 2);
    break;
  }
  case CAN_ID7:
  {
    get_motor_measure(motor_data + 6, rx_data);
    process_motor_data(&hcan1, motor_data + 6, 2);
    break;
  }
  case CAN_ID8:
  {
    get_motor_measure(motor_data + 7, rx_data);
    process_motor_data(&hcan1, motor_data + 7, 2);
    break;
  }
  default:
  {
    break;
  }
  }
}

//�����������ָ�루��������ȫ��������
motor_measure_t get_motor_data(uint8_t CAN_x, int motorID) //��ȡ�������
{
  motor_measure_t *target_data;
  motorID -= 0x201;
  if (CAN_x == CAN_1)
    target_data = &Can1_motor_data[motorID];
  else if (CAN_x == CAN_2)
    target_data = &Can2_motor_data[motorID];
  return *target_data;
}

//����������
void set_motor_current(int16_t current, uint8_t CAN_x, int motorID) //�趨������
{
  motorID -= 0x201;
  if (CAN_x == CAN_1)
    Can1_motor_data[motorID].set_current = current;
  else if (CAN_x == CAN_2)
    Can2_motor_data[motorID].set_current = current;
}

//CAN1���͵���
void CAN1_send_current() //���͵�����Ƶ���
{
  uint8_t can_send_data[8];
  static CAN_TxHeaderTypeDef tx_message;
  uint32_t send_mail_box;

  //����ǰ4��
  tx_message.StdId = CAN_1_4_SIGN_ID;
  tx_message.IDE = CAN_ID_STD;
  tx_message.RTR = CAN_RTR_DATA;
  tx_message.DLC = 0x08;

  can_send_data[0] = (Can1_motor_data[0].set_current >> 8);
  can_send_data[1] = Can1_motor_data[0].set_current;

  can_send_data[2] = (Can1_motor_data[1].set_current >> 8);
  can_send_data[3] = Can1_motor_data[1].set_current;

  can_send_data[4] = (Can1_motor_data[2].set_current >> 8);
  can_send_data[5] = Can1_motor_data[2].set_current;

  can_send_data[6] = (Can1_motor_data[3].set_current >> 8);
  can_send_data[7] = Can1_motor_data[3].set_current;
  HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, &send_mail_box);

  //���ͺ�4��
  tx_message.StdId = CAN_5_8_SIGN_ID;

  can_send_data[0] = (Can1_motor_data[4].set_current >> 8);
  can_send_data[1] = Can1_motor_data[4].set_current + 4;

  can_send_data[2] = (Can1_motor_data[5].set_current >> 8);
  can_send_data[3] = Can1_motor_data[5].set_current;

  can_send_data[4] = (Can1_motor_data[6].set_current >> 8);
  can_send_data[5] = Can1_motor_data[6].set_current;

  can_send_data[6] = (Can1_motor_data[7].set_current >> 8);
  can_send_data[7] = Can1_motor_data[7].set_current;
  HAL_CAN_AddTxMessage(&hcan1, &tx_message, can_send_data, &send_mail_box);
}
//CAN2 ���͵���
void CAN2_send_current() //���͵�����Ƶ���
{
  uint8_t can_send_data[8];
  static CAN_TxHeaderTypeDef tx_message;
  uint32_t send_mail_box;

  //����ǰ4��
  tx_message.StdId = CAN_1_4_SIGN_ID;
  tx_message.IDE = CAN_ID_STD;
  tx_message.RTR = CAN_RTR_DATA;
  tx_message.DLC = 0x08;

  can_send_data[0] = (Can2_motor_data[0].set_current >> 8);
  can_send_data[1] = Can2_motor_data[0].set_current;

  can_send_data[2] = (Can2_motor_data[1].set_current >> 8);
  can_send_data[3] = Can2_motor_data[1].set_current;

  can_send_data[4] = (Can2_motor_data[2].set_current >> 8);
  can_send_data[5] = Can2_motor_data[2].set_current;

  can_send_data[6] = (Can2_motor_data[3].set_current >> 8);
  can_send_data[7] = Can2_motor_data[3].set_current;
  HAL_CAN_AddTxMessage(&hcan2, &tx_message, can_send_data, &send_mail_box);

  //���ͺ�4��
  tx_message.StdId = CAN_5_8_SIGN_ID;

  can_send_data[0] = (Can2_motor_data[4].set_current >> 8);
  can_send_data[1] = Can2_motor_data[4].set_current + 4;

  can_send_data[2] = (Can2_motor_data[5].set_current >> 8);
  can_send_data[3] = Can2_motor_data[5].set_current;

  can_send_data[4] = (Can2_motor_data[6].set_current >> 8);
  can_send_data[5] = Can2_motor_data[6].set_current;

  can_send_data[6] = (Can2_motor_data[7].set_current >> 8);
  can_send_data[7] = Can2_motor_data[7].set_current;
  HAL_CAN_AddTxMessage(&hcan2, &tx_message, can_send_data, &send_mail_box);
}
