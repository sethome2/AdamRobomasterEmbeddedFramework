#ifndef __CAN_RECEIVE__
#define __CAN_RECEIVE__

#include "struct_typedef.h"
#include "main.h"

#define CAN_1 1
#define CAN_2 2

/* CAN send and receive ID */
typedef enum
{
  CAN_1_4_SIGN_ID = 0x200,
  CAN_ID1 = 0x201,
  CAN_ID2 = 0x202,
  CAN_ID3 = 0x203,
  CAN_ID4 = 0x204,

  CAN_5_8_SIGN_ID = 0x1FF,
  CAN_ID5 = 0x205,
  CAN_ID6 = 0x206,
  CAN_ID7 = 0x207,
  CAN_ID8 = 0x208,
	
	CAN_6020_SIGN_ID = 0x204,
	
} can_msg_id_e;

//������궨�����ĵ��ID
//���޸�can_msg_id_e
//motorID
#define chassis_FR CAN_ID1
#define chassis_FL CAN_ID2
#define chassis_BL CAN_ID3
#define chassis_BR CAN_ID4
#define rotator1   CAN_ID5
#define rotator2   CAN_ID6
#define RollTrack  CAN_ID7
#define motor_8    CAN_ID8

//����������
#define ECD_MAX 8191  //���������ֵ
#define M3508_P 19.0f //M3508������ٱ�
#define M2006_P 36.0f //M2006������ٱ�

//��д sethome
//��������޸� Ĭ��ΪM3508
//#define CAN1_0_M2006
//#define CAN1_1_M2006
//#define CAN1_2_M2006
//#define CAN1_3_M2006
//#define CAN1_4_M2006
//#define CAN1_5_M2006
//#define CAN1_6_M2006
//#define CAN1_7_M2006

//#define CAN2_0_M2006
//#define CAN2_1_M2006
//#define CAN2_2_M2006
//#define CAN2_3_M2006
//#define CAN2_4_M2006
//#define CAN2_5_M2006
//#define CAN2_6_M2006
//#define CAN2_7_M2006

//rm motor data
typedef struct
{
  int16_t set_current; //�趨�ĵ���

  uint16_t ecd;          //������
  int16_t speed_rpm;     //ת��
  int16_t given_current; //������ĵ���
  uint8_t temperate;     //�¶ȣ���ȡ������
  int16_t last_ecd;      //��һ�α���������ֵ

  long long ecd_cnt; //������������
  float angle_cnt;   //ת�����ܽǶ�
  float round_speed; //������ת�ٶ�

} motor_measure_t;

typedef struct
{
  int16_t set_voltage; //�趨�ĵ�ѹ

  uint16_t ecd;          //������
  int16_t speed_rpm;     //ת��
  int16_t given_current; //������ĵ���
  uint8_t temperate;     //�¶ȣ���ȡ������
  int16_t last_ecd;      //��һ�α���������ֵ

  long long ecd_cnt; //������������
  float angle_cnt;   //ת�����ܽǶ�
  float round_speed; //������ת�ٶ�

} motor_6020_measure_t;

//�ⲿ����
void CAN1_send_current(void);                                        //���͵�����Ƶ���
void CAN2_send_current(void);                                        //���͵�����Ƶ���
void set_motor_current(int16_t current, uint8_t CAN_x, int motorID); //�趨������
motor_measure_t get_motor_data(uint8_t CAN_x, int motorID);          //��ȡ�����Ϣ

#endif
