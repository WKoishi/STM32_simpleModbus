#include "modbus.h"

// CRC-16/MODBUS校验码计算

uint16_t crctalbeabs[] = { 
    0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401, 
    0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400 
};

// CRC-16/MODBUS 快速查表计算法
// 输出的校验码高位在前，低位在后
uint16_t CRC16_CheckFast(const uint8_t *ptr, uint8_t len)
{
    uint16_t crc = 0xffff; 
    uint8_t i;
    uint8_t ch;

    for (i = 0; i < len; i++)
    {
        ch = *ptr++;
        crc = crctalbeabs[(ch ^ crc) & 15] ^ (crc >> 4);
        crc = crctalbeabs[((ch >> 4) ^ crc) & 15] ^ (crc >> 4);
    }
    return crc;
}


// CRC-16/MODBUS 定义计算法
// 输出的校验码高位在前，低位在后
uint16_t CRC16_Check(const uint8_t *data, uint8_t num) 
{
    uint8_t i,j;
    uint16_t crc_result = 0xFFFF;

    for(i=0; i<num; i++)
    {
        crc_result ^= data[i];
        for(j=0; j<8; j++)
        {
            if(crc_result & 1)
            {
                crc_result >>= 1; 
                crc_result ^= 0xA001; 
            }
            else 
            {
                crc_result >>= 1; 
            }
        }
    }
    return crc_result;
}


