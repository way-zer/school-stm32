/*******************************************************************
ʵ�����ƣ�������������ʵ��

Ӳ��ģ�飺�����ԭ��Ӧ��ʵ����

Ӳ�����ߣ�ARM P12�ӿ�---------LED P2�ӿ� 
			PF0~PF7--------LED1~LED8
		 ARM P11�ӿ�---------�������� P1�ӿ�  
			PC0~PC7--------SW1~SW8
         ע������20P����ֱ��P12��P2�ӿڣ�20P����ֱ��P11��P1�ӿڡ� 
		   
ʵ�����󣺲��뿪��״̬ӳ�䵽LED���ϡ�

����ʱ�䣺2018-10-11
********************************************************************/
#include "stm32f4xx_conf.h"
#include "delay.h"

/*******************************
��  �ܣ�LED�˿ڳ�ʼ��
��  ������
����ֵ����
*******************************/

#define KEY_READ() GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)	  // �ϰ������
#define KEY_WRITE() GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) // �°������

#define CLK(val) GPIO_WriteBit(GPIOF, GPIO_Pin_6, (BitAction)val)  // FPGA ʱ���ź�
#define WREN(val) GPIO_WriteBit(GPIOF, GPIO_Pin_7, (BitAction)val) // FPGA ��д�ź�

/*******************************
��  �ܣ�LED�ƶ�д������ʾ
��  ������
����ֵ����
*******************************/

void Led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //����GPIOB��ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	  //���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Write(GPIOB, 0x0000);
}

/*******************************
��  �ܣ����뿪�ض˿ڳ�ʼ��
��  ������
����ֵ����
*******************************/
void Sw_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //����GPIOC��ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_11;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*******************************
��  �ܣ�GPIOF��FPGA���ӳ�ʼ��
��  ������
����ֵ����
*******************************/
void GPIO_FPGA_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); //����GPIOF��ʱ��

	GPIO_InitStructure.GPIO_Pin = 0x0fff;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	 //���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; //�������25MHz
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = 0xf000;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //���ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/************��ȡ����***************/
void Read_Data(uint8_t read_addr, uint8_t *read_date)
{
	uint16_t buf = 0;						   // GPIOF ���ݱ���
	WREN(0);								   // ������
	CLK(0);									   // ʱ���ź�
	buf = GPIO_ReadOutputData(GPIOF) & 0x00C0; // ������6��7λ,��ʱ���źźͶ�д�źŲ���
	buf |= read_addr;						   // ����ȡ��ַ����GPIOF, ����ַ 0-2λ
	GPIO_Write(GPIOF, buf);					   // GPIOF�������
	CLK(1);									   // ʱ�����ڷ��Ͷ����ݵ�ַ
	CLK(0);
	CLK(1);													   // ʱ���źŻ�ȡ�ڴ�����
	*read_date = ((GPIO_ReadInputData(GPIOF) & 0xf000) >> 12); // ���FPGA��������q, ����pin��12-15λ
	WREN(0);
}

/***********д������***************/
void Write_Data(uint8_t write_addr, uint8_t write_date)
{
	uint16_t buf = 0;									  // GPIOF ���ݱ���
	WREN(1);											  // д����
	CLK(0);												  // ʱ���ź�
	buf = GPIO_ReadOutputData(GPIOF) & 0x00C0;			  // ������6��7λ,��ʱ���źźͶ�д�źŲ���
	buf |= ((write_addr & 0x7) << 3) | (write_date << 8); // ����ȡ��ַ����GPIOF, д��ַ��3-5λ��д������8-11λ
	GPIO_Write(GPIOF, buf);								  // GPIOF�������
	CLK(1);
	WREN(0);
}

void Key_Scan(void)
{
	uint8_t gpioc_data;	 // GPIOC���ݶ�ȡ����
	uint8_t read_addr;	 // ���뿪�� 0-2
	uint8_t write_addr;	 // д���ַ
	uint8_t read_date;	 // ���뿪�� 4-7
	uint8_t write_date;	 // д������
	if (KEY_READ() == 0) // ���������
	{
		Delay_Ms(5); // ����
		if (KEY_READ() == 0)
		{
			while (KEY_READ() == 0)
			{
			};
			gpioc_data = ~(GPIO_ReadInputData(GPIOC) & 0x00FF); // ��ȡ���뿪������,��0����1
			read_addr = gpioc_data & 0x07;						// ��ö���ַ
			read_date = (gpioc_data & 0xF0) >> 4;				// �����ݣ����û�ã�������
			Read_Data(read_addr, &read_date);					// ����ָ���ȡFPGA�ڴ�����
			GPIO_Write(GPIOB, read_date);						// ��õ����ݴ�GPIOB���͵�LED
																//LED1(!LED1_DATA());
		}
	}
	if (KEY_WRITE() == 0) // д�������
	{
		Delay_Ms(5); // ����
		if (KEY_WRITE() == 0)
		{

			while (KEY_WRITE() == 0)
			{
			};
			gpioc_data = ~(GPIO_ReadInputData(GPIOC) & 0x00FF); // ��ȡ���뿪������,��0����1
			write_addr = gpioc_data & 0x07;						// ���д��ַ
			write_date = (gpioc_data & 0xF0) >> 4;				// д���ݱ���
			Write_Data(write_addr, write_date);					// ��FPGA�ڴ�д������
			GPIO_Write(GPIOB, write_date);						// ����FPGAд���������ʾ����
																//LED2(!LED2_DATA());
		}
	}
}

int main(void)
{
	Delay_Init();
	Led_Init();		  //LED�Ƴ�ʼ��
	Sw_Init();		  //���뿪�س�ʼ�� ������ʼ��
	GPIO_FPGA_Init(); // GPIOF��ʼ��
	while (1)
	{
		Key_Scan();
	}
}
