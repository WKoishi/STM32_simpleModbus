#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "modbus.h"

int main(void)
{ 
	
    HAL_Init();                    	 	//��ʼ��HAL��    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);               		//��ʼ����ʱ����
	//LED_Init();							//��ʼ��LED	
	//KEY_Init();							//��ʼ������
	
	Modbus_Init();
	//LED1 = 0;
	
    while(1)
    {

    }
}

