#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"
#include "usart.h"
#include "struct_typedef.h"

#define __UART_DATA_TRANSMIT__
#ifdef  __UART_DATA_TRANSMIT__

//串口数据结构体
typedef struct
{
  UART_HandleTypeDef *huart;
  DMA_HandleTypeDef *hdma_usart_rx;
  DMA_HandleTypeDef *hdma_usart_tx;

  uint8_t rev_data[256];
} transmit_data;

extern transmit_data UART1_data;
extern transmit_data UART6_data;

//外部调用
void uart_init(void);//初始化
void UART_DMA_rxtx_start(transmit_data *data, UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_rx, DMA_HandleTypeDef *hdma_usart_tx);//初始化
void UART_rx_IRQHandler(transmit_data *uart);//接受中断
void UART_send_data(transmit_data uart, uint8_t data[], uint16_t size);//发送

#endif
//end of file
