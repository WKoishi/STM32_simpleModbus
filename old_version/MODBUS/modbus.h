#ifndef __MODBUS_INSIDE_H
#define __MODBUS_INSIDE_H

#include "sys.h"

typedef struct {
	u8 my_address;  //本机地址
	u8 receive_count;  //端口接收到的数据个数
	u8 send_count;  //想要发送的数据总个数
	u8 receive_state;  //数据接收状态
	u8 process_state;  //数据处理状态
	u8 receive_buff[50];  //接收缓冲区1
	u8 send_buff[50];  //发送缓冲区
} ModbusBufferType;

extern ModbusBufferType modbus_buffer;

void Modbus_Init(void);
void Modbus_UART_Init(u32 bound);
void Modbus_TIM_Init(u16 arr,u16 psc);
u16 CRC16_CheckFast(const u8 *ptr, u8 len);
u16 CRC16_Check(const u8 *data, u8 num);
void Modbus_DataProcess(ModbusBufferType *modbus_buff);


#endif


