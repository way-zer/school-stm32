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
#include "main.h"
#include "delay.h"
#include "smg.h"
#include "led.h"
#include "exti.h"
#include "my_timer.h"

#define KEY  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)
 
void Key_Hardware_Init()
{
	GPIO_InitTypeDef GPIO_TypeDefStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	//开启中断输入端口时钟

	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_IN;    //通用输入模式
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP; 	 //上拉
	GPIO_Init(GPIOB, &GPIO_TypeDefStructure);
}


int main(void)
{
	uint8_t i;
	uint16_t cont = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	Delay_Init();             //延时初始化
	EXTI_Configure();         //外部中断初始化 
	TIM1_Configure(999, 167);  //定时器初始化  1ms
	SMG_Init();               //数码管初始化
	LED_Hardware_Init();       //LED灯初始化 
	Key_Hardware_Init();       //按键检测初始化
	 
	while(1)
	{
		cont++;
		if(cont >= 500)
		{
			cont = 0;
			SMG_Sele(i);  //数码管显示数据
			if(i == 5)    //数码管数据显示到第五位时按键检测
			{
				if(!KEY)
				{
					Delay_Ms(10);
					if(!KEY)
					{
						while(!KEY){};
						led_flow_flag = 1;   //开启流水灯
					}
			    }
			}
			i++;
			i &= 0x07;
	
		}
		Delay_Ms(1);   //需要明显效果时打开延时 
		LED_Flow();   
	}
}

//end file



