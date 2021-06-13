#include "my_timer.h"

/******************************************
��  �ܣ���ʱ��1��ʼ��
��  ����arr  ��ʱ����ֵ  psc  Ԥ��Ƶϵ��
����ֵ����
******************************************/
void TIM1_Configure(uint32_t us)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    //��ʱ����ֵ
    TIM_TimeBaseStructure.TIM_Period = us - 1;
    //Ԥ����ϵ��
    TIM_TimeBaseStructure.TIM_Prescaler = 167;
    //���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //ʱ�ӷ�Ƶ����
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //�ظ���������
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    //ѡ���ж�����
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
    //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    //��Ӧ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
    //ʹ���ж�����
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //TIM1�ж�ʹ��
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    //TIM1ʹ��
    TIM_Cmd(TIM1, ENABLE);
}

/******************************************
��  �ܣ���ʱ��1�жϷ�����
��  ������
����ֵ����
******************************************/
void TIM1_UP_TIM10_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        TIM1Callback();
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}
