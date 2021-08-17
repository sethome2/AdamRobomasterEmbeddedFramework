#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"
#include "usart.h"
#include "struct_typedef.h"

#define __UART_DATA_TRANSMIT__
#ifdef  __UART_DATA_TRANSMIT__

//�������ݽṹ��
typedef struct
{
  UART_HandleTypeDef *huart;
  DMA_HandleTypeDef *hdma_usart_rx;
  DMA_HandleTypeDef *hdma_usart_tx;

  uint8_t rev_data[256];
} transmit_data;

extern transmit_data UART1_data;
extern transmit_data UART6_data;

//�ⲿ����
void uart_init(void);//��ʼ��
void UART_DMA_rxtx_start(transmit_data *data, UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_rx, DMA_HandleTypeDef *hdma_usart_tx);//��ʼ��
void UART_rx_IRQHandler(transmit_data *uart);//�����ж�
void UART_send_data(transmit_data uart, uint8_t data[], uint16_t size);//����

#endif
//end of file
