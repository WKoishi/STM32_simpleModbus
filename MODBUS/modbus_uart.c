#include "modbus_port.h"
#include "modbus.h"

struct BufferHandler receive_buffer_1 = {
    .buffer_length = RX_BUFFER_SIZE,
};
struct BufferHandler receive_buffer_2 = {
    .buffer_length = RX_BUFFER_SIZE,
};

struct ModbusHandler modbus_handler = {
    .local_address = 0X01,
};

uint16_t it_count = 0;

HAL_StatusTypeDef Modbus_init(void)
{
    HAL_StatusTypeDef ret_val = HAL_OK;
    
    modbus_handler.buf_receiving = &receive_buffer_1;
    modbus_handler.buf_processing = &receive_buffer_2;
    
    ret_val = HAL_UART_Receive_DMA(&modbus_huart, modbus_handler.buf_receiving->data, 
                modbus_handler.buf_receiving->buffer_length);
    __HAL_UART_ENABLE_IT(&modbus_huart, UART_IT_IDLE);
    
    return ret_val;
}

void DMA_switch_buffer(void)
{
    struct BufferHandler* temp;
    volatile BufferState flag = modbus_handler.buf_processing->state_flag;
    
    if (BUFFER_IDLE == flag)
    {
        temp = modbus_handler.buf_receiving;
        modbus_handler.buf_receiving = modbus_handler.buf_processing;
        modbus_handler.buf_processing = temp;
        
        if (HAL_UART_Receive_DMA(&modbus_huart, modbus_handler.buf_receiving->data, 
                modbus_handler.buf_receiving->buffer_length) == HAL_OK)
        {
            modbus_handler.buffer_switch_state = BUFFER_SWITCH_NORMAL;
        }
        else
        {
            modbus_handler.buffer_switch_state = BUFFER_SWITCH_ERROR;
        }
    }
    else
    {
        modbus_handler.buffer_switch_state = BUFFER_SWITCH_ERROR;
    }
}

void USART1_IRQHandler(void)
{
    uint32_t temp = 0;
    
    if ((__HAL_UART_GET_FLAG(&modbus_huart, UART_FLAG_IDLE) != RESET))
    {
        __HAL_UART_CLEAR_IDLEFLAG(&modbus_huart);
        HAL_UART_DMAStop(&modbus_huart);
        
        set_buffer_processing();
        temp = __HAL_DMA_GET_COUNTER(modbus_huart.hdmarx);
        modbus_handler.buf_receiving->receive_count = modbus_handler.buf_receiving->buffer_length - temp;

        DMA_switch_buffer();
        it_count++;
    }
    HAL_UART_IRQHandler(&modbus_huart);
}






