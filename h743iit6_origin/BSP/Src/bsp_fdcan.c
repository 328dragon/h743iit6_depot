#include "bsp_can.h"
//先用my_can_init初始化并且配置can过滤器
//再用HAL_CAN_Start(CAN_HandleTypeDef *hcan)开启对应can

uint8_t send_data[8]={0};

/// @brief 用于初始化且配置can的过滤器
/// @param _hcan 
void my_canfilter_init(CAN_HandleTypeDef *_hcan)
{
CAN_FilterTypeDef can_filter_st;   //创建用于配置过滤器的FilterTypeDef类结构体
can_filter_st.FilterBank = 0;    //过滤器编号
can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK; //过滤模式位掩码模式
can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT; //过滤器位宽为32位
can_filter_st.FilterIdHigh = 0x0000;      //设置ID
can_filter_st.FilterIdLow = 0x0000;
can_filter_st.FilterMaskIdHigh = 0x0000;//掩码高位
can_filter_st.FilterMaskIdLow = 0x0000;//掩码低位
can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0; //把接收到的报文放入到FIFO0邮箱中
can_filter_st.SlaveStartFilterBank = 14; //为从属can选择开始的过滤库，对于单个CAN实例，这个参数没有意义，不考虑
can_filter_st.FilterActivation = ENABLE;     //使能过滤器

		if (HAL_CAN_ConfigFilter(_hcan, &can_filter_st) != HAL_OK)//将配置好的结构体作为参数通过函数配置到_hcan的邮箱FIFO0的过滤
		{
			Error_Handler();
		}
		can_filter_st.FilterBank = 14;
		if (HAL_CAN_ConfigFilter(_hcan, &can_filter_st) != HAL_OK)
		{
			Error_Handler();
		}
    
    		HAL_CAN_ActivateNotification(_hcan, CAN_IT_RX_FIFO0_MSG_PENDING);//激活can总线中断的函数

}

//建议放在主函数直接声明，主要为了美观
void my_can_init(CAN_HandleTypeDef *_hcan)
{
my_canfilter_init(_hcan);
HAL_CAN_Start(_hcan);
}



/// @brief 纯发送函数，但是数据需要自己设置
/// @param _hcan 选择的can,例hcan1
/// @param obj_Id 目标id 
/// @param tx_data 发送数据,需要
void can_send_data(CAN_HandleTypeDef *_hcan,uint8_t obj_Id,uint8_t *tx_data)
{
	CAN_TxHeaderTypeDef CAN_TX;
	uint32_t TX_MAILBOX;//邮箱
    uint8_t m_data[8]={0};
	CAN_TX.DLC = 0x08;//长度
	CAN_TX.ExtId = 0x0000;//扩展帧ID，由于IDE配置为标准帧，故EXTID无效
	CAN_TX.StdId = obj_Id;	  // 标准帧ID
	CAN_TX.IDE = CAN_ID_STD;   // 标准帧
	CAN_TX.RTR = CAN_RTR_DATA; // 数据帧, 表示是数据
	CAN_TX.TransmitGlobalTime = DISABLE;//开启后会自动把时间戳添加到最后两字节的数据中,这选关闭

	while (HAL_CAN_GetTxMailboxesFreeLevel(_hcan) == 0);//发送的邮箱是否空闲
		
	HAL_CAN_AddTxMessage(_hcan, &CAN_TX, tx_data, &TX_MAILBOX);//将数据添加到can总线的发送缓冲区
}
 

/* 
//用于具体的发送函数
void tx_data_terminal(CAN_HandleTypeDef *_hcan,uint8_t obj_Id,uint16_t a,uint16_t b,uint16_t c ,uint16_t d,uint16_t e,uint16_t f ,uint16_t g,uint16_t h)
 {
// uint16_t ter_data[8];

// ter_data[0]=a;
// ter_data[1]=b;
// ter_data[2]=c;
// ter_data[3]=d;
// ter_data[4]=e;
// ter_data[5]=f;
// ter_data[6]=g;
// ter_data[7]=h;

// send_data(_hcan,obj_Id,ter_data);
 } */
//中断回调函数
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *_hcan)
{

	CAN_RxHeaderTypeDef RxMessage;
	uint8_t Data[8];

	RxMessage.DLC = 2;
	RxMessage.StdId = 0x00;
	RxMessage.ExtId = 0x0000;
	RxMessage.IDE = CAN_ID_EXT;
	RxMessage.RTR = CAN_RTR_DATA;
	if (_hcan == &hcan)
	{
		if (HAL_CAN_GetState(_hcan) != RESET)
		{
			HAL_CAN_GetRxMessage(_hcan, CAN_RX_FIFO0, &RxMessage, Data);
		}
	}

	
/*
//双can状态
	if (_hcan == &hcan1)
	{
		if (HAL_CAN_GetState(_hcan) != RESET)
		{
			HAL_CAN_GetRxMessage(_hcan, CAN_RX_FIFO0, &RxMessage, Data);

		}
	}
 if (_hcan == &hcan2)
{
		if (HAL_CAN_GetState(_hcan) != RESET)
		{
			HAL_CAN_GetRxMessage(_hcan, CAN_RX_FIFO0, &RxMessage, Data);
		
		}
	} */

	
}
