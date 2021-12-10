#include "modbus.h"

extern TIM_HandleTypeDef Modbus_TIM_Handler;

UART_HandleTypeDef Modbus_UART_Handler;  //UART���

//��ʼ��IO ����1 
//bound:������
void Modbus_UART_Init(u32 bound)
{	
	//UART ��ʼ������
	Modbus_UART_Handler.Instance = USART1;					    //USART1
	Modbus_UART_Handler.Init.BaudRate=bound;				    //������
	Modbus_UART_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	Modbus_UART_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	Modbus_UART_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	Modbus_UART_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	Modbus_UART_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&Modbus_UART_Handler);					    //HAL_UART_Init()��ʹ��UART1
}

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
		__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//ģʽҪ����Ϊ��������ģʽ��	
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA10
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);     //������������ж�
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
				__HAL_TIM_ENABLE(&Modbus_TIM_Handler);  //ʹ�ܼ�����
				modbus_buffer.receive_state = 1;  //�������״̬
			}
			
			__HAL_TIM_CLEAR_FLAG(&Modbus_TIM_Handler, TIM_FLAG_UPDATE);  //�����־λ
			__HAL_TIM_SET_COUNTER(&Modbus_TIM_Handler, 0);  //������ֵ�� 0		
			
			modbus_buffer.receive_buff[modbus_buffer.receive_count++] = res;
		}
	}
	HAL_UART_IRQHandler(&Modbus_UART_Handler);
}








