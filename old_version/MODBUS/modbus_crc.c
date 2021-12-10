#include "modbus.h"

// CRC-16/MODBUS校验码计算

u16 crctalbeabs[] = { 
	0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401, 
	0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400 
};

// CRC-16/MODBUS 快速查表计算法
// 输出的校验码高位在前，低位在后
u16 CRC16_CheckFast(const u8 *ptr, u8 len)
{
	u16 crc = 0xffff; 
	u8 i;
	u8 ch;
 
	for (i = 0; i < len; i++) {
		ch = *ptr++;
		crc = crctalbeabs[(ch ^ crc) & 15] ^ (crc >> 4);
		crc = crctalbeabs[((ch >> 4) ^ crc) & 15] ^ (crc >> 4);
	}
	return crc;
}


// CRC-16/MODBUS 定义计算法
// 输出的校验码高位在前，低位在后
u16 CRC16_Check(const u8 *data, u8 num) 
{
	u8 i,j;
	u16 crc_result = 0xFFFF;

	for(i=0; i<num; i++) {
		crc_result ^= data[i];
		for(j=0; j<8; j++) {
			if(crc_result & 1) {
				crc_result >>= 1; 
				crc_result ^= 0xA001; 
			} else {
				crc_result >>= 1; 
			}
		}
	}
	return crc_result;
}


