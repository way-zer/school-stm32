#include "stm32f4xx_conf.h"

void EXTI_Configure(uint32_t line,uint8_t exti_IRQn,uint8_t priority){
	EXTI_InitTypeDef EXTI_TypeDefStructure;
	EXTI_TypeDefStructure.EXTI_Line = line;
	EXTI_TypeDefStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_TypeDefStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_TypeDefStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_TypeDefStructure);

	//EXTI2_IRQn中断向量优先级设置
	NVIC_InitTypeDef NVIC_TypeDefStructure;
	NVIC_TypeDefStructure.NVIC_IRQChannel = exti_IRQn;
	NVIC_TypeDefStructure.NVIC_IRQChannelPreemptionPriority = priority;
	NVIC_TypeDefStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_TypeDefStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_TypeDefStructure);
}