#include "smg.h"
#include "delay.h"

/*******************************
��  �ܣ�����ܶ˿ڳ�ʼ��
��  ������
����ֵ����
*******************************/
void SMG_Init(void)
{
	RCC_AHB1PeriphClockCmd(SMG_RCC_GPIO, ENABLE); //����GPIOC��ʱ��
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	  //���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
	GPIO_Init(SMG_GPIO, &GPIO_InitStructure);
}

/*******************************
��  �ܣ�HC595��������
��  ����dat    ����
����ֵ����
*******************************/
void HC595_Send(uint8_t dat)
{
	uint8_t dat_buf = 0, i;
	for (i = 0; i < 8; i++)
	{
		dat_buf = dat & 0x80;
		if (dat_buf) //���1bit����
		{
			HC595_SI(1); //��74HC595��������������������Ϊ�ߵ�ƽ
		}
		else
		{
			HC595_SI(0); //��74HC595��������������������Ϊ�͵�ƽ
		}
		HC595_SCK(0);
		Delay_Us(1);
		HC595_SCK(1);
		Delay_Us(1);
		dat <<= 1;
	}
	HC595_RCK(0);
	Delay_Us(3);
	HC595_RCK(1);
}

//��ʾ���������飬����Ϊ0,1,..,9
uint8_t digivalue[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7f, 0x6f};

/*******************************
��  �ܣ������λ�ο���
��  ����index   ��Ӧ�������
����ֵ����
*******************************/
void SMG_Sele(uint8_t pos, uint8_t index)
{
	HC595_Send(digivalue[index]);
	HC138_A(pos & 0x01);
	HC138_B(pos & 0x02);
	HC138_C(pos & 0x04);
}