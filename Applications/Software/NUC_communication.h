/*
 * @Author: sethome
 * @Date: 2021-12-23 16:49:11
 * @LastEditTime: 2021-12-23 17:20:10
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Applications\Software\NUC_communication.c
 */
typedef struct 
{
    float acc[3];
    float gyo[3];
    float mag[3];

    float move_speed;
    float location;
    float now_pitch;
    float now_yaw;

    int remainingBullets;
    enum 
    {
        Red = 0,
        Blue,
    }Team;

    char customStr[2]; //�ڴ����
} STM32_data_t;

typedef struct
{
    float set_pitch;
    float set_yaw;

    char shot; //����1��������

    float delay; // ms
    int HP;      //ʣ��Ѫ��

    char customStr[10]; //�ڴ����
} NUC_data_t;

//��STM32��NUC����Ϣ��/����
int encodeSTM32(STM32_data_t *target, unsigned char rx_buff[], unsigned int len);
int decodeSTM32(STM32_data_t *target, unsigned char tx_buff[], unsigned int len);

//��NUC��STM32����Ϣ��/����
int decodeNUC(NUC_data_t *target, unsigned char tx_buff[], unsigned int len);
int encodeNUC(NUC_data_t *target, unsigned char rx_buff[], unsigned int len);

//end of file