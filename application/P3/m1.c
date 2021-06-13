/***********************************************************************
实验名称：中断实验

硬件模块：计算机原理应用实验箱

硬件接线：ARM P12接口---------按键 P1接口 	
           PF8------------K1_N  
				ARM P11接口----------LED P2接口	 
           PC0------------LED1
         注：可用20P排线直连P12、P1接口，可用20P排线直连P11、P2接口。
		   
实验现象：按键KEY1触发外部中断，LED1电平翻转。

更新时间：2018-10-31
***********************************************************************/
#include "stm32f4xx_conf.h"
#include "delay.h"
#include "ledFlow.h"
#include "libs/exti.h"

/*******************************
功  能：外部中断服务函数
参  数：无
返回值：无
*******************************/
void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line8))
    {
        if (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8) == 0)
        {
            LEDFlow_flag = !LEDFlow_flag;
            EXTI_ClearITPendingBit(EXTI_Line8);
        }
    }
}

void main()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    LEDFlow_Init();  //LED端口配置
    EXTI_Configure(); //外部中断配置
    Delay_Init();
    LEDFlow_flag = 1;

    while (1)
    {
        LEDFlow_Update();
    }
}
