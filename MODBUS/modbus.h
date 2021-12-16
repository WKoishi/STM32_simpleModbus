#ifndef _MODBUS_H_
#define _MODBUS_H_

#include "stdint.h"
#include "usart.h"
#include "string.h"

#define RX_BUFFER_SIZE 256

typedef enum {
    BUFFER_IDLE = 0X00U,
    BUFFER_PROCESSING = 0X01U
} BufferState;

typedef enum {
    BUFFER_SWITCH_NORMAL = 0X00U,
    BUFFER_SWITCH_ERROR = 0X01U
} BufferSwitchState;

struct BufferHandler {
    const uint16_t buffer_length;
    BufferState state_flag;
    uint16_t receive_count;
    uint8_t data[RX_BUFFER_SIZE];
};

struct ModbusHandler {
    const uint8_t local_address;
    uint8_t received_flag;
    BufferSwitchState buffer_switch_state;
    struct BufferHandler* buf_receiving;
    struct BufferHandler* buf_processing;
};

extern struct ModbusHandler modbus_handler;

static inline void set_buffer_processing(void)
{
    modbus_handler.buf_receiving->state_flag = BUFFER_PROCESSING;
}

static inline void release_buffer_processing(void)
{
    modbus_handler.buf_processing->state_flag = BUFFER_IDLE;
    //memset(modbus_handler.buf_processing->data, 0, modbus_handler.buf_processing->receive_count);
}

static inline BufferState read_buffer_state(void)
{
    return modbus_handler.buf_processing->state_flag;
}

static inline BufferSwitchState check_buffer_switch_error(void)
{
    return modbus_handler.buffer_switch_state;
}


uint16_t CRC16_CheckFast(const uint8_t *ptr, uint8_t len);
uint16_t CRC16_Check(const uint8_t *data, uint8_t num);
HAL_StatusTypeDef Modbus_init(void);
void modbus_main_task(void);

#endif /* _MODBUS_H_ */

