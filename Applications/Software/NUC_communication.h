/*
 * @Author: sethome
 * @Date: 2021-12-23 16:49:11
 * @LastEditTime: 2021-12-23 17:20:10
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\Applications\Software\NUC_communication.c
 */
typedef struct 
{
    enum 
    {
        Red_Team = 0,
        Blue_Team = 1,
    }Team;
} STM32_data_t;

typedef struct
{
    float set_pitch;
    float set_yaw;
		
		char get;//1 get new frame
	
    char shot; //大于1代表开火

    float delay; // ms
} NUC_data_t;

extern NUC_data_t fromNUC;

//对STM32向NUC的信息解/编码
int encodeSTM32(STM32_data_t *target, unsigned char rx_buff[], unsigned int len);
int decodeSTM32(STM32_data_t *target, unsigned char tx_buff[], unsigned int len);

//对NUC向STM32的信息解/编码
int decodeNUC(NUC_data_t *target, unsigned char tx_buff[], unsigned int len);
int encodeNUC(NUC_data_t *target, unsigned char rx_buff[], unsigned int len);

//end of file
