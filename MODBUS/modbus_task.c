#include "modbus.h"
#include "modbus_port.h"

extern void DMA_switch_buffer(void);

uint8_t error_count = 0;
uint16_t send_count = 0;

//test function
void modbus_Func4(struct ModbusHandler* handler)
{
    uint16_t crc_val;
    uint8_t send_buf[50];
    
    send_count++;
    
//    uint32_t address = (uint32_t)handler->buf_processing->data;
//    
//    send_buf[0] = 0X00;
//    send_buf[1] = 0X01;
//    send_buf[2] = handler->buf_processing->data[2];
//    send_buf[3] = 0X05;
//    send_buf[4] = (uint8_t)(address>>24);
//    send_buf[5] = (uint8_t)(address>>16);
//    send_buf[6] = (uint8_t)(address>>8);
//    send_buf[7] = (uint8_t)(address);
//    send_buf[8] = (uint8_t)it_count;
//    
//    crc_val = CRC16_CheckFast(send_buf, 9);
//    send_buf[9] = (uint8_t)crc_val;
//    send_buf[10] = (uint8_t)(crc_val>>8);
//    
//    HAL_UART_Transmit(&modbus_huart, send_buf, 11, 0XFF);
    
    handler->buf_processing->data[4] = (uint8_t)(send_count>>8);
    handler->buf_processing->data[5] = (uint8_t)send_count;
    
    crc_val = CRC16_CheckFast(handler->buf_processing->data, handler->buf_processing->receive_count - 2);
    handler->buf_processing->data[9] = (uint8_t)crc_val;
    handler->buf_processing->data[10] = (uint8_t)(crc_val>>8);
    
    HAL_UART_Transmit(&modbus_huart, handler->buf_processing->data, handler->buf_processing->receive_count, 0XFF);
}

// Modbus任务处理函数
void modbus_data_process(struct ModbusHandler* handler)
{
	uint16_t crc, rec_crc;
    struct BufferHandler* modbus_buff = handler->buf_processing;
    
	if (modbus_buff->receive_count < 4)
		return;    //若接收到的帧长度小于4则丢弃
	
	crc = CRC16_CheckFast(modbus_buff->data, modbus_buff->receive_count - 2);
	// 此处接收的CRC校验码，低位在前，高位在后
	rec_crc = (uint16_t)(modbus_buff->data[modbus_buff->receive_count-2])
			| (uint16_t)(modbus_buff->data[modbus_buff->receive_count-1]<<8);
	
	if (crc == rec_crc) {
		if (modbus_buff->data[0] == handler->local_address) {
			switch (modbus_buff->data[1]) {
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
					modbus_Func4(handler);  //测试
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

void modbus_main_task(void)
{
    if (read_buffer_state() == BUFFER_PROCESSING)
    {
        modbus_data_process(&modbus_handler);
        release_buffer_processing();
    }
    
    if (check_buffer_switch_error() == BUFFER_SWITCH_ERROR)
    {
        DMA_switch_buffer();
        HAL_GPIO_TogglePin(LED0_ON_BOARD_GPIO_Port, LED0_ON_BOARD_Pin);
        error_count++;
    }
}


