/********************************************************************
实验名称：数码管动态扫描实验

硬件模块：计算机原理应用实验箱

硬件接线：ARM P11接口---------数码管 P4接口 
			PC0--------SI
			PC1--------RCK
			PC2--------SCK
			PC3--------A
			PC4--------B
			PC5--------C
		 ARM P10接口----------按键 P1接口
             PB8--------KEY1_N
             PB11-------KEY2_N	
          ARM P12接口-----------LED P2接口
             PF0~PF7------LED1~LED8		 
         注：可用20P排线直连P11、P4接口，直连P10、P1接口，直连P12、P2接口。
					
实验现象：数码管上显示数字0~7，可通过按键检测或者外部中断触发流水灯。

更新时间：2018-10-31
************************************************************************/
#include "stm32f4xx_conf.h"
#include "delay.h"
#include "smg.h"
#include "ledFlow.h"
#include "exti.h"
#include "my_timer.h"

#define KEY GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)

void Key_Hardware_Init()
{
	GPIO_InitTypeDef GPIO_TypeDefStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //开启中断输入端口时钟

	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_IN; //通用输入模式
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB, &GPIO_TypeDefStructure);
}

uint32_t millis = 0;

void TIM1Callback()
{
	millis++;

	static cont_time = 0;
	if (LEDFlow_flag)
	{
		cont_time++;
		if (cont_time >= 5000)
		{
			cont_time = 0;
			LEDFlow_flag = 0;
		}
	}
}

uint8_t smgNum = 0;

void updateSMG()
{
	static uint32_t lastMillis = 0;
	if (millis - lastMillis > 500)
	{
		lastMillis = millis;
		SMG_Sele(0, smgNum); //数码管显示数据

		//数码管数据显示到第五位时按键检测
		if (smgNum == 5 && !KEY)
		{
			Delay_Ms(20); //20ms 消抖
			if (!KEY)
			{
				while (!KEY)
					;
				LEDFlow_flag = 1; //开启流水灯
			}
		}

		smgNum++;
		smgNum &= 0x07;
	}
}

// //异步检测版
// //数码管数据显示到第五位时按键检测
// void checkKey()
// {
// 	static uint8_t last = 0;
// 	static uint32_t lastMillis = 0;
// 	if (millis - lastMillis > 20) //20ms 消抖
// 	{
// 		lastMillis = millis;
// 		uint8_t new = !KEY &&smgNum == 5;
// 		if (last && new)
// 		{
// 			while (!KEY)
// 				;
// 			LEDFlow_flag = 1; //开启流水灯
// 		}
// 		last = new;
// 	}
// }

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	Delay_Init();		  //延时初始化
	EXTI_Configure();	  //外部中断初始化
	TIM1_Configure(1000); //定时器初始化  1ms
	SMG_Init();			  //数码管初始化
	LEDFlow_Init();		  //LED灯初始化
	Key_Hardware_Init();  //按键检测初始化

	while (1)
	{
		updateSMG();
		// checkKey();
		LEDFlow_Update();
	}
}

//end file
