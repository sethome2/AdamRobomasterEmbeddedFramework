#include "Arm_Task.h"
#include "UART_data_transmit.h"
#include "stdio.h"
#include "stdlib.h"
#include "Stxxid-Data-Transmission.h"
#include "string.h"
#include "stdio.h"
#include "referee.h"
//DMA控制变量
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart6_rx;
extern DMA_HandleTypeDef hdma_usart6_tx;

//串口控制变量
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;

//将上述串口+DMA整合，并包含缓冲区
transmit_data UART1_data;
transmit_data UART6_data;

//某Stxxid-Data-Transmission变量（暂时不管）
struct serial_data_set_t UART1;             //UART1的接收状态
struct serial_data_set_t UART6;             //UART2的接收状态     
extern struct database_t transmit_database; //接收数据数据库


//初始化串口
void uart_init(void)
{
  UART_DMA_rxtx_start(&UART1_data,&huart1, &hdma_usart1_rx, &hdma_usart1_tx);
  UART_DMA_rxtx_start(&UART6_data,&huart6, &hdma_usart6_rx, &hdma_usart6_tx);
}

//ヾ(?ω?`)o温馨提示，使用 DMA + 中断空闲，缓冲区爆了。。就是爆了 固定256字节，暂时没有给你自定义的打算
//DMA，串口中断启动
void UART_DMA_rxtx_start(transmit_data *data, UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma_usart_rx, DMA_HandleTypeDef *hdma_usart_tx)
{
  data->huart = huart;                                  //串口控制变量
  data->hdma_usart_rx = hdma_usart_rx;                  //DMA接收缓冲
  data->hdma_usart_tx = hdma_usart_tx;                  //DMA发送缓冲
	
  HAL_UART_Receive_DMA(data->huart,data->rev_data,256); //使能DMA

  __HAL_UART_ENABLE_IT(data->huart, UART_IT_RXNE);      //启动接收中断
  __HAL_UART_ENABLE_IT(data->huart, UART_IT_IDLE);      //启动空闲中断
  __HAL_DMA_ENABLE(hdma_usart_rx);                      //启动DMA接收
}




//发送数据（数据别释放了，不然后面收不到）
void UART_send_data(transmit_data uart, uint8_t data[], uint16_t size)
{
  HAL_UART_Transmit_DMA(uart.huart, data, size);//套娃ヾ(?ω?`)o
}

//请放于UARTx_IRQHandler下，即UART全局中断函数
//=.=当然你想放在HAL_UART_RxCpltCallback也不是不行
void UART_rx_IRQHandler(transmit_data *uart)
{
  uint16_t len = 256;//得到的数据长度
	
  if (uart->huart->Instance->SR & UART_FLAG_RXNE) //接受单字节中断
  {
    //如果需要对单个字符处理，在此处
  }
  else if (uart->huart->Instance->SR & UART_FLAG_IDLE) //如果为空闲中断
  {
		HAL_UART_DMAStop(uart->huart); //失效DMA
		
    len = 256 - __HAL_DMA_GET_COUNTER(uart->hdma_usart_rx);//计算获得的字节长度
		
    //此处放置处理函数，蟹蟹
    //数据数组 = uart->rev_data ,长度 = len
    //自发自收示例： UART_send_data(UART1_data,uart->rev_data,len);
		
    if (uart->huart == &huart1)//串口一数据处理
    {
			UART_send_data(UART1_data,uart->rev_data,len);
			
      referee_data_solve(uart->rev_data);
		}
    else if (uart->huart == &huart6)//串口二（板子丝印）数据处理
    {
			UART_send_data(UART6_data,uart->rev_data,len);
		}

		//清除数据
		for(int p = 0;p < 256;p++)
			uart->rev_data[p] = 0;
		
		__HAL_UART_CLEAR_IDLEFLAG(uart->huart); //清除空闲中断标志
		__HAL_UART_CLEAR_PEFLAG(uart->huart);   //清除等待标志
		HAL_UART_Receive_DMA(uart->huart, uart->rev_data, 256);   //重新使能DMA缓冲区
  }
	
	__HAL_UART_CLEAR_PEFLAG(uart->huart);//清除等待标志
}

//end of file
