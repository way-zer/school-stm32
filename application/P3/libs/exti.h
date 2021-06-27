#ifndef __EXTI_H
#define __EXTI_H
#include "stm32f4xx_conf.h"

void EXTI_Configure(uint32_t line,uint8_t exti_IRQn,uint8_t priority);


#endif

