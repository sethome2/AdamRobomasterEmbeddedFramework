#include "mpuiic.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK NANO STM32F103������
//MPU6050 IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2018/7/28
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2018-2028
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
  //MPU IIC ��ʱ����
void MPU_IIC_Delay(void)
{
//	delay_us(2);
}

//��ʼ��IIC
void MPU_IIC_Init(void)
{					     
//	GPIO_InitTypeDef GPIO_Initure;
//	
//	__HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOAʱ��
//	 
//    GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3; //PA2��PA3
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
//    GPIO_Initure.Pull=GPIO_PULLUP;          //����
//    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	//����
//	
//    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2|GPIO_PIN_3,GPIO_PIN_SET);//PB11,PB12 �����	
//	
//	HAL_GPIO_Init(GPIOA,&GPIO_Initure);
 
}
//����IIC��ʼ�ź�
void MPU_IIC_Start(void)
{
//	MPU_SDA_OUT();     //sda�����
//	MPU_IIC_SDA=1;	  	  
//	MPU_IIC_SCL=1;
//	MPU_IIC_Delay();
// 	MPU_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
//	MPU_IIC_Delay();
//	MPU_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void MPU_IIC_Stop(void)
{
//	MPU_SDA_OUT();//sda�����
//	MPU_IIC_SCL=0;
//	MPU_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
// 	MPU_IIC_Delay();
//	MPU_IIC_SCL=1;  
//	MPU_IIC_SDA=1;//����I2C���߽����ź�
//	MPU_IIC_Delay();							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 MPU_IIC_Wait_Ack(void)
{
//	u8 ucErrTime=0;
//	MPU_SDA_IN();      //SDA����Ϊ����  
//	MPU_IIC_SDA=1;MPU_IIC_Delay();	   
//	MPU_IIC_SCL=1;MPU_IIC_Delay();	 
//	while(MPU_READ_SDA)
//	{
//		ucErrTime++;
//		if(ucErrTime>250)
//		{
//			MPU_IIC_Stop();
//			return 1;
//		}
//	}
//	MPU_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void MPU_IIC_Ack(void)
{
//	MPU_IIC_SCL=0;
//	MPU_SDA_OUT();
//	MPU_IIC_SDA=0;
//	MPU_IIC_Delay();
//	MPU_IIC_SCL=1;
//	MPU_IIC_Delay();
//	MPU_IIC_SCL=0;
}
//������ACKӦ��		    
void MPU_IIC_NAck(void)
{
//	MPU_IIC_SCL=0;
//	MPU_SDA_OUT();
//	MPU_IIC_SDA=1;
//	MPU_IIC_Delay();
//	MPU_IIC_SCL=1;
//	MPU_IIC_Delay();
//	MPU_IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void MPU_IIC_Send_Byte(u8 txd)
{                        
//    u8 t;   
//	MPU_SDA_OUT(); 	    
//    MPU_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
//    for(t=0;t<8;t++)
//    {              
//        MPU_IIC_SDA=(txd&0x80)>>7;
//        txd<<=1; 	  
//		MPU_IIC_SCL=1;
//		MPU_IIC_Delay(); 
//		MPU_IIC_SCL=0;	
//		MPU_IIC_Delay();
//    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char receive=0;
//	MPU_SDA_IN();//SDA����Ϊ����
//    for(i=0;i<8;i++ )
//	{
//        MPU_IIC_SCL=0; 
//        MPU_IIC_Delay();
//		MPU_IIC_SCL=1;
//        receive<<=1;
//        if(MPU_READ_SDA)receive++;   
//		MPU_IIC_Delay(); 
//    }					 
//    if (!ack)
//        MPU_IIC_NAck();//����nACK
//    else
//        MPU_IIC_Ack(); //����ACK   
    return receive;
}


















