#include "modbus.h"
#include "led.h"

extern UART_HandleTypeDef Modbus_UART_Handler;

TIM_HandleTypeDef Modbus_TIM_Handler;      //��ʱ����� 

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void Modbus_TIM_Init(u16 arr,u16 psc)
{
    Modbus_TIM_Handler.Instance = TIM3;                          //ͨ�ö�ʱ��3
    Modbus_TIM_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    Modbus_TIM_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    Modbus_TIM_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    Modbus_TIM_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; //ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&Modbus_TIM_Handler);
    
    __HAL_TIM_ENABLE_IT(&Modbus_TIM_Handler, TIM_IT_UPDATE);  //ʹ�ܸ����ж�
	__HAL_TIM_DISABLE(&Modbus_TIM_Handler);  //�رն�ʱ��
}

//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //����ITM3�ж�   
	}
}


void TIM3_IRQHandler(void)
{
	if (Modbus_TIM_Handler.Instance->SR & 0X0001) {
		Modbus_TIM_Handler.Instance->SR &= ~(1<<0);  //����жϱ�־λ
		
		if (modbus_buffer.receive_state == 1) {
			__HAL_TIM_DISABLE(&Modbus_TIM_Handler);  //�رն�ʱ��
			__HAL_TIM_SET_COUNTER(&Modbus_TIM_Handler, 0);  //����������
			
			modbus_buffer.process_state = 1;  //�������ݴ���״̬
			Modbus_UART_Handler.Instance->CR1 &= ~(1<<2);  //�رմ��ڽ���
			Modbus_DataProcess(&modbus_buffer);
			
			modbus_buffer.receive_count = 0;
			Modbus_UART_Handler.Instance->CR1 |= 1<<2;  //���¿������ڽ���
			modbus_buffer.process_state = 0;
			modbus_buffer.receive_state = 0;
		}
	}
}

//��ʹ�ûص�����
#if 0
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == (&Modbus_TIM_Handler)) {
		if (modbus_buffer.receive_state == 1) {
			__HAL_TIM_DISABLE(&Modbus_TIM_Handler);  //�رն�ʱ��
			__HAL_TIM_SET_COUNTER(&Modbus_TIM_Handler, 0);  //����������
			modbus_buffer.process_state = 1;  //�������ݴ���״̬
			Modbus_DataProcess(&modbus_buffer);
			modbus_buffer.receive_count = 0;
			modbus_buffer.process_state = 0;
			modbus_buffer.receive_state = 0;
		}
	}
}
#endif






