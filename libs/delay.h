#ifndef __DELAY_H
#define __DELAY_H
#include "stm32f4xx_conf.h"

void Delay_Init(void);
void Delay_Us(uint16_t ctr);
void Delay_Ms(uint16_t ctr);

#endif
