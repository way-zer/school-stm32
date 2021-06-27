#ifndef __LEDFlow_H
#define __LEDFlow_H
#include "stm32f4xx_conf.h"

//LED端口
#define LEDFlow_GPIO GPIOB
#define LEDFlow_RCC_GPIO RCC_AHB1Periph_GPIOB
#define LEDFlow_SET(dat) GPIO_Write(LEDFlow_GPIO, dat)

extern uint32_t millis;
int8_t LEDFlow_flag;

void LEDFlow_Init(void);
void LEDFlow_Update(void);

#endif
