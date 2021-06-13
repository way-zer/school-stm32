#include "my_timer.h"

/******************************************
功  能：定时器1初始化
参  数：arr  定时器初值  psc  预分频系数
返回值：无
******************************************/
void TIM1_Configure(uint32_t us)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    //定时器初值
    TIM_TimeBaseStructure.TIM_Period = us - 1;
    //预分配系数
    TIM_TimeBaseStructure.TIM_Prescaler = 167;
    //向上计数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //时钟分频因子
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //重复计数次数
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    //选择中断向量
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
    //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    //响应优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
    //使能中断向量
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //TIM1中断使能
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    //TIM1使能
    TIM_Cmd(TIM1, ENABLE);
}

/******************************************
功  能：定时器1中断服务函数
参  数：无
返回值：无
******************************************/
void TIM1_UP_TIM10_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        TIM1Callback();
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}
