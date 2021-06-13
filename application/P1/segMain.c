/*************************************************************
实验名称：数码管动态扫描实验

硬件模块：计算机原理应用实验箱

硬件接线：ARM P12接口---------数码管 P4接口 
				PF0--------SI
				PF1--------RCK
				PF2--------SCK
				PF3--------A
				PF4--------B
				PF5--------C
         注：可用20P排线直连P12、P4接口
					
实验现象：数码管上显示数字0~7。

更新时间：2018-10-31
**************************************************************/
#define SMG_GPIO GPIOC
#define SMG_RCC_GPIO RCC_AHB1Periph_GPIOC

#include "stm32f4xx_conf.h"
#include "delay.h"
#include "smg.h"

int main(void)
{
	Delay_Init(); //延时初始化
	SMG_Init();	  //数码管初始化
	while(1){
		int8_t i = 0;
		SMG_Sele(i++,1);
		SMG_Sele(i++,9);
		SMG_Sele(i++,2);
		SMG_Sele(i++,1);
		SMG_Sele(i++,0);
		SMG_Sele(i++,1);
		SMG_Sele(i++,1);
		SMG_Sele(i++,5);
	}
}

//end file
