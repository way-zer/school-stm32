#ifndef  __MY_TIMER_H_
#define  __MY_TIMER_H_

#include "stm32f4xx_conf.h"

void TIM1_Configure(uint32_t us);
extern void TIM1Callback();

#endif
