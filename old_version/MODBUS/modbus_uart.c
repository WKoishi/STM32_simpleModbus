#include "modbus.h"

extern TIM_HandleTypeDef Modbus_TIM_Handler;

UART_HandleTypeDef Modbus_UART_Handler;  //UART句柄

//初始化IO 串口1 
//bound:波特率
void Modbus_UART_Init(u32 bound)
{	
	//UART 初始化设置
	Modbus_UART_Handler.Instance = USART1;					    //USART1
	Modbus_UART_Handler.Init.BaudRate=bound;				    //波特率
	Modbus_UART_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	Modbus_UART_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	Modbus_UART_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	Modbus_UART_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	Modbus_UART_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&Modbus_UART_Handler);					    //HAL_UART_Init()会使能UART1
}

//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
		__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//模式要设置为复用输入模式！	
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA10
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//抢占优先级3，子优先级3
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);     //开启接收完成中断
	}
}


void USART1_IRQHandler(void)
{
	u8 res;
	if (__HAL_UART_GET_FLAG(&Modbus_UART_Handler, UART_FLAG_RXNE) != RESET) {
		
		//HAL_UART_Receive(&Modbus_UART_Handler, &res, 1, 4);
		res = Modbus_UART_Handler.Instance->DR;
		
		if (modbus_buffer.process_state == 0) {
			if (modbus_buffer.receive_state == 0) {
				__HAL_TIM_ENABLE(&Modbus_TIM_Handler);  //使能计数器
				modbus_buffer.receive_state = 1;  //进入接收状态
			}
			
			__HAL_TIM_CLEAR_FLAG(&Modbus_TIM_Handler, TIM_FLAG_UPDATE);  //清除标志位
			__HAL_TIM_SET_COUNTER(&Modbus_TIM_Handler, 0);  //计数器值归 0		
			
			modbus_buffer.receive_buff[modbus_buffer.receive_count++] = res;
		}
	}
	HAL_UART_IRQHandler(&Modbus_UART_Handler);
}








