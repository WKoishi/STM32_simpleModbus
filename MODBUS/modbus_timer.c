#include "modbus.h"
#include "led.h"

extern UART_HandleTypeDef Modbus_UART_Handler;

TIM_HandleTypeDef Modbus_TIM_Handler;      //定时器句柄 

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void Modbus_TIM_Init(u16 arr,u16 psc)
{
    Modbus_TIM_Handler.Instance = TIM3;                          //通用定时器3
    Modbus_TIM_Handler.Init.Prescaler=psc;                     //分频系数
    Modbus_TIM_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    Modbus_TIM_Handler.Init.Period=arr;                        //自动装载值
    Modbus_TIM_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; //时钟分频因子
    HAL_TIM_Base_Init(&Modbus_TIM_Handler);
    
    __HAL_TIM_ENABLE_IT(&Modbus_TIM_Handler, TIM_IT_UPDATE);  //使能更新中断
	__HAL_TIM_DISABLE(&Modbus_TIM_Handler);  //关闭定时器
}

//定时器底层驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断   
	}
}


void TIM3_IRQHandler(void)
{
	if (Modbus_TIM_Handler.Instance->SR & 0X0001) {
		Modbus_TIM_Handler.Instance->SR &= ~(1<<0);  //清除中断标志位
		
		if (modbus_buffer.receive_state == 1) {
			__HAL_TIM_DISABLE(&Modbus_TIM_Handler);  //关闭定时器
			__HAL_TIM_SET_COUNTER(&Modbus_TIM_Handler, 0);  //计数器清零
			
			modbus_buffer.process_state = 1;  //进入数据处理状态
			Modbus_UART_Handler.Instance->CR1 &= ~(1<<2);  //关闭串口接收
			Modbus_DataProcess(&modbus_buffer);
			
			modbus_buffer.receive_count = 0;
			Modbus_UART_Handler.Instance->CR1 |= 1<<2;  //重新开启串口接收
			modbus_buffer.process_state = 0;
			modbus_buffer.receive_state = 0;
		}
	}
}

//不使用回调函数
#if 0
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == (&Modbus_TIM_Handler)) {
		if (modbus_buffer.receive_state == 1) {
			__HAL_TIM_DISABLE(&Modbus_TIM_Handler);  //关闭定时器
			__HAL_TIM_SET_COUNTER(&Modbus_TIM_Handler, 0);  //计数器清零
			modbus_buffer.process_state = 1;  //进入数据处理状态
			Modbus_DataProcess(&modbus_buffer);
			modbus_buffer.receive_count = 0;
			modbus_buffer.process_state = 0;
			modbus_buffer.receive_state = 0;
		}
	}
}
#endif






