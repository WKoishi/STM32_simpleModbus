#ifndef __MODBUS_INSIDE_H
#define __MODBUS_INSIDE_H

#include "sys.h"

typedef struct {
	u8 my_address;  //������ַ
	u8 receive_count;  //�˿ڽ��յ������ݸ���
	u8 send_count;  //��Ҫ���͵������ܸ���
	u8 receive_state;  //���ݽ���״̬
	u8 process_state;  //���ݴ���״̬
	u8 receive_buff[50];  //���ջ�����1
	u8 send_buff[50];  //���ͻ�����
} ModbusBufferType;

extern ModbusBufferType modbus_buffer;

void Modbus_Init(void);
void Modbus_UART_Init(u32 bound);
void Modbus_TIM_Init(u16 arr,u16 psc);
u16 CRC16_CheckFast(const u8 *ptr, u8 len);
u16 CRC16_Check(const u8 *data, u8 num);
void Modbus_DataProcess(ModbusBufferType *modbus_buff);


#endif


