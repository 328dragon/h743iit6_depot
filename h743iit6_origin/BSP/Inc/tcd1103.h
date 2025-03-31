#ifndef __TCD1103_H
#define __TCD1103_H


#include "main.h"
#define ADC_BUFFER_SIZE 1500
extern uint16_t adc_buffer[ADC_BUFFER_SIZE]; // 用于存储CCD的输出数据
extern int adc_flag;//flag
void Stop_Exposure(void);
void Start_Exposure(void);
void Trigger_Shift_Pulse(void);
#endif
