#ifndef __BSP_CAN_H
#define __BSP_CAN_H

#include "main.h"
#include "fdcan.h"
void can_send_data(CAN_HandleTypeDef *_hcan,uint8_t obj_Id,uint8_t *tx_data);
void my_canfilter_init(CAN_HandleTypeDef *_hcan);
void my_can_init(CAN_HandleTypeDef *_hcan);

typedef struct
{
	uint32_t mailbox;
	CAN_TxHeaderTypeDef hdr;
	uint8_t payload[8];
}CAN_TxPacketTypeDef;

typedef struct
{
	CAN_RxHeaderTypeDef hdr;
	uint8_t payload[8];
}CAN_RxPacketTypeDef;



#endif