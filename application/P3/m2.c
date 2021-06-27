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
		 ARM P12接口----------按键 P1接口
             PB8--------KEY1_N
             PB11-------KEY2_N	
          ARM P10接口-----------LED P2接口
             PF0~PF7------LED1~LED8		 
         注：可用20P排线直连P11、P4接口，直连P12、P1接口，直连P10、P2接口。
					
实验现象：数码管上显示数字0~9，
	按键A为高级中断，按下后停止计数，显示流水灯
	按键B为低级中断，按下后倒计数到0

更新时间：2018-10-31
************************************************************************/
#include "stm32f4xx_conf.h"
#include "delay.h"
#include "smg.h"
#include "ledFlow.h"
#include "exti.h"
#include "my_timer.h"

#define KEYA GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8)
#define KEYB GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_11)

void Key_Hardware_Init()
{
	GPIO_InitTypeDef GPIO_TypeDefStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); //开启中断输入端口时钟

	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_IN; //通用输入模式
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOF, &GPIO_TypeDefStructure);
}

uint32_t millis = 0;
void TIM1Callback()
{
	millis++;
}

uint8_t smgNum = 0;
void updateSMG()
{
	static uint32_t lastMillis = 0;
	if (millis - lastMillis > 500)
	{
		lastMillis = millis;
		smgNum++;
		if(smgNum>9)smgNum=0;
		SMG_Sele(0, smgNum); //数码管显示数据
	}
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	TIM1_Configure(1000); //定时器初始化  1ms
	Delay_Init();
	SMG_Init();		//数码管初始化
	LEDFlow_Init(); //LED灯初始化

	Key_Hardware_Init(); //按键检测初始化
	//中断初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource8);
	EXTI_Configure(EXTI_Line8, EXTI9_5_IRQn, 1); //keyA
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource11);
	EXTI_Configure(EXTI_Line11, EXTI15_10_IRQn, 2);  //keyB

	while (1)
	{
		updateSMG();
	}
}

void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line8))
	{
		if (!KEYA)
		{
			uint32_t start = millis;
			LEDFlow_flag = 1;
			while (millis - start < 3000)//流水灯5秒
			{
				LEDFlow_Update();
			}
			LEDFlow_flag = 0;
		}
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
}

void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line11))
	{
		if (!KEYB)
		{
			while(smgNum!=0){
				smgNum--;
				SMG_Sele(0, smgNum);
				Delay_Ms(500);
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
}
//end file
