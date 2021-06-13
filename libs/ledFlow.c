#include "ledFlow.h"
#include "delay.h"

/*******************************
功  能：LED端口初始化
参  数：无
返回值：无
*******************************/
void LEDFlow_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(LEDFlow_RCC_GPIO, ENABLE); //开启GPIOF的时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	   //输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Medium_Speed; //速率25MHz
	GPIO_Init(LEDFlow_GPIO, &GPIO_InitStructure);
}

void LEDFlow_Update()
{
	static uint32_t lastMillis = 0;
	static uint8_t cont;
	if (LEDFlow_flag == 0)
	{
		LEDFlow_SET(0);
	}
	else if (LEDFlow_flag == 1)
	{
		LEDFlow_SET(1 << cont);
		if (millis - lastMillis > 100)
		{
			lastMillis = millis;
			cont++;
			cont &= 0x07; //大于7后清零
		}
	}
}