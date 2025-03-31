#ifndef    __BSP_UART_H
#define    __BSP_UART_H

#include <stdio.h>
#include "main.h"
#include "usart.h"
#include "string.h"
extern uint8_t USARTData;//数据缓冲
extern uint8_t USARTDataBag[3];//数据存储数组
extern uint8_t RX_Flag;//数据包接受完成标志位
uint8_t USART_GetFlag(void);//标志位函数
extern int mode;
int get_x();
int get_y();
int32_t get_z();
void task1();
// void give_x_y_z(int x,int y,int z);
  void delay_us(uint32_t us);
extern  int x;
 extern int y;
 extern int z;
extern  int x_data_last;
extern   int y_data_last;
#endif
