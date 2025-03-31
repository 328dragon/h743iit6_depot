#include "tcd1103.h"

uint16_t adc_buffer[ADC_BUFFER_SIZE]={0}; // 用于存储CCD的输出数据
int adc_flag=1;

void Start_Exposure(void)
{
    HAL_GPIO_WritePin(ICG_GPIO_Port, ICG_Pin, GPIO_PIN_RESET);  // ICG 低电平，启动曝光
}

void Stop_Exposure(void)
{
    HAL_GPIO_WritePin(ICG_GPIO_Port, ICG_Pin, GPIO_PIN_SET);  // ICG 高电平，停止曝光
}
void Trigger_Shift_Pulse(void)
{
    HAL_GPIO_WritePin(SH_GPIO_Port, SH_Pin, GPIO_PIN_RESET);  // SH 低电平
    delay_us(2);  //脉冲宽度1us
    HAL_GPIO_WritePin(SH_GPIO_Port, SH_Pin, GPIO_PIN_SET); // SH 高电平
	delay_us(4); 
}
