#include "bsp_uart.h"
//这里包含了常见的串口收发操作
uint8_t USARTData;//数据缓冲
uint8_t USARTDataBag[3];//数据存储数组
uint8_t RX_Flag;//数据包接受完成标志位
int mode=0;
  int x=0;
  int y=0;
  int z=0;
  int x_data_last=0;
   int y_data_last=0;


   void delay_us(uint32_t us)
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--)
    {
        __NOP(); // 防止编译器优化
    }
}

uint8_t USART_GetFlag(void)//标志位函数
{
  if(RX_Flag == 1)
  {
    return 1;
    RX_Flag = 0;
  }else
  {
    return 0 ;
  }
}

int32_t get_x()//下位机得到x真实位置
{
	int re_x;

	re_x=(int)USARTDataBag[0];
	return re_x;
}
                                      
int32_t get_y()//下位机得到y真实位置
{
	int re_y;

	re_y=(int)USARTDataBag[1];
	return re_y;
}

int32_t get_z()
{
int re_z=(int)USARTDataBag[2];
return re_z;

}

// void give_x_y_z(int x,int y,int z)
// {
// uint8_t dj[8]={0xFF,0X00,0x05,0x06,0xFE}; 
// memcpy(&dj[1],&x,1);	
//  memcpy(&dj[2],&y,1);	
//  memcpy(&dj[3],&z,1);	
// HAL_UART_Transmit(&huart1,dj,5,0xfff);

// }


   //这直接放在while里面，可以使得先向外发送一个start然后等待数据进行相应动作，最后发stop然后又是start,注意每次接到的数据都不应该一样
void task1()
{

       for (int i=1;i<5;i++)
      {
        printf("start");     
      }
       mode=2;
HAL_UART_Receive_IT(&huart4,&USARTData,1);
      while (1)
      {
      if(USART_GetFlag() == 1)
        {
             x=get_x();
             y=get_y();
             z=get_z();
            if((x!=x_data_last)||(y!=y_data_last))
            {
           for (int i=1;i<5;i++)
            {
              printf("stop");     
            }
             printf("%d,%d,%d",x,y,z);
               printf("\n");   
            x_data_last=x;
            y_data_last=y;
               break;  
            }

        }
mode=2;
      }

}


// //这两个放在usart.c最下面,然后开启microlib，引用stdio.h和strin.h
// int fputc(int ch, FILE *f)
// {
//     HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
//     return ch;
// }
// int fgetc(FILE *f)
// {
//     uint8_t ch = 0;
//     HAL_UART_Receive(&huart1, &ch, 1, 0xffff);
//     return ch;
// }
// //将这个替换原有的串口中断，并改一下对应串口即可，并且可改数据帧长度{0xff,0x0a,0x0b,0x0c,0xfe}中间三个是数据帧
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// { static uint8_t RXState = 0;
//   static uint8_t RXCount = 0;
//   if(mode==2){  if(huart->Instance == USART1)//检测是否是串口1产生中断，也可以（huart==huart1）这样检测
//   {
//     if(USARTData == 0xff) //接收到包头 
//     {
//       RXCount = 0;
//       RXState = 1;
//     }else if(RXState == 1) //如果这里用else if就不包含包头，因为if成立，else if不执行，改if即可。
//     {
//       if(USARTData == 0xfe)//如果检测到包尾
//       {
//         //USARTDataBag[RXCount] = USARTData;想加入包尾就加入这一行
//         RXState = 2;
//       }else
//       {
//         USARTDataBag[RXCount] = USARTData;//数据从缓冲区移入存储区
//         RXCount++;
//       } 
//         if(RXCount == 3) //这里固定包长接收方式 收到3个数据后不再接受数据，一直等待包尾
//       { 
//         if(USARTData == 0xfe)//如果检测到包尾
//       {
//         //USARTDataBag[RXCount] = USARTData;//想加入包尾就加入这一行
//         RXState = 2;
//       }
//     }
//     if(RXState == 2)//状态清零
//     {
//       RX_Flag = 1;
//       RXState = 0; //小细节状态立马成立，所以装态改变放后面，不然标志位不会置1也就接不到数据。       
//     }   
//   }}
//   HAL_UART_Receive_IT(&huart1,&USARTData,1);//等待中断并接收8位数据，没有就不会接收下一次数据。通过main调用一次等待数据接受，接收到后进入中断继续等待下一次的接受，达到重复等待接受。
//   }
//   /* USER CODE END USART2_IRQn 0 */
// 	}
////解码的取的方式，放在while里面，测试用
// 				if(USART_GetFlag() == 1)
//    {
// 			 int x=get_x();
//    			int y=get_y();
//         int z=get_z();
//       if((x!=x_last)||(y!=y_last))
//      {printf("%d,%d,%d",x,y,z);
//      printf("\n");}
// 			x_last=x;
// 			y_last=y;
// 		}