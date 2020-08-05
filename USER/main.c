#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "modbus.h"

int main(void)
{ 
	
    HAL_Init();                    	 	//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函数
	//LED_Init();							//初始化LED	
	//KEY_Init();							//初始化按键
	
	Modbus_Init();
	//LED1 = 0;
	
    while(1)
    {

    }
}

