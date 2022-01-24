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
        Red_Team = 0,
        Blue_Team,
    }Team;

    char customStr[2]; //内存对齐
} STM32_data_t;

typedef struct
{
    float set_pitch;
    float set_yaw;

    char shot; //大于1代表开火

    float delay; // ms
    int HP;      //剩余血量

    char customStr[10]; //内存对齐
} NUC_data_t;

extern NUC_data_t fromNUC;

//对STM32向NUC的信息解/编码
int encodeSTM32(STM32_data_t *target, unsigned char rx_buff[], unsigned int len);
int decodeSTM32(STM32_data_t *target, unsigned char tx_buff[], unsigned int len);

//对NUC向STM32的信息解/编码
int decodeNUC(NUC_data_t *target, unsigned char tx_buff[], unsigned int len);
int encodeNUC(NUC_data_t *target, unsigned char rx_buff[], unsigned int len);

//end of file
