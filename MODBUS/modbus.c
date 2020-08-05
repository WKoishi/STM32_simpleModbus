#include "modbus.h"
#include "led.h"

ModbusBufferType modbus_buffer;
extern UART_HandleTypeDef Modbus_UART_Handler;

// Modbus��ʼ������
void Modbus_Init(void)
{
	modbus_buffer.my_address = 0X01;  //������ַ:0X01
	modbus_buffer.receive_state = 0;
	modbus_buffer.process_state = 0;
	modbus_buffer.receive_count = 0;
	modbus_buffer.send_count = 0;
	
	Modbus_UART_Init(256000);  //���ڳ�ʼ��
	Modbus_TIM_Init(399, 71);  //��ʱ����ʼ��������1ms
}

// Modbus����4������������
void Modbus_Func4(ModbusBufferType *modbus_buff)
{
	u16 crc;
	modbus_buff->send_buff[0] = 0X00;
	modbus_buff->send_buff[1] = 0X01;
	modbus_buff->send_buff[2] = modbus_buff->receive_buff[2];
	modbus_buff->send_buff[3] = 0X01;
	modbus_buff->send_buff[4] = 0XFF;
	
	crc = CRC16_CheckFast(modbus_buff->send_buff, 5);
	modbus_buff->send_buff[5] = (u8)crc;
	modbus_buff->send_buff[6] = (u8)(crc>>8);
	modbus_buff->send_count = 7;
	
	HAL_UART_Transmit(&Modbus_UART_Handler, modbus_buff->send_buff,
						modbus_buff->send_count, 100);
}

// Modbus��������
void Modbus_DataProcess(ModbusBufferType *modbus_buff)
{
	u16 crc, rec_crc;
	if (modbus_buff->receive_state == 0 || modbus_buff->receive_count < 4)
		return;    //�����յ���֡����С��4����
	
	crc = CRC16_CheckFast(modbus_buff->receive_buff, modbus_buff->receive_count - 2);
	// �˴����յ�CRCУ���룬��λ��ǰ����λ�ں�
	rec_crc = (u16)(modbus_buff->receive_buff[modbus_buff->receive_count-2])
			| (u16)(modbus_buff->receive_buff[modbus_buff->receive_count-1]<<8);
	
	if (crc == rec_crc) {
		if (modbus_buff->receive_buff[0] == modbus_buff->my_address) {
			switch (modbus_buff->receive_buff[1]) {
				case 0: {
					break;
				}
				case 1: {
					break;
				}
				case 2: {
					break;
				}
				case 3: {
					break;
				}
				case 4: {
					Modbus_Func4(modbus_buff);  //����
					break;
				}
				case 5: {
					break;
				}
				case 6: {
					break;
				}
				case 7: {
					break;
				}
				case 8: {
					break;
				}
				case 9: {
					break;
				}
			}
		}
	}
}











