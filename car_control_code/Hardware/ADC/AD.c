#include "stm32f10x.h" // Device header
#include "AD.h"
#include "usart.h"

uint16_t AD_Value[2];

void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_55Cycles5);

	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_NbrOfChannel = 2;
	ADC_Init(ADC1, &ADC_InitStructure);

	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 4;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	DMA_Cmd(DMA1_Channel1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1) == SET)
		;
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET)
		;

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

uint16_t Get_AD_Key()
{
	int ver = Vertical();
	int hor = Horizontal();

	printf("ver = %d  , hor = %d\r\n", ver, hor);

	if (ver == UP)
	{
		if (hor == LEFT)
		{
			return LEFT_UP;
		}
		else if (hor == RIGHT)
		{
			return RIGHT_UP;
		}
		else
		{
			return UP;
		}
	}
	else if (ver == DOWN)
	{
		if (hor == LEFT)
		{
			return LEFT_DOWN;
		}
		else if (hor == RIGHT)
		{
			return RIGHT_DOWN;
		}
		else
		{
			return DOWN;
		}
	}
	else if (hor == LEFT)
	{
		if (ver == UP)
		{
			return LEFT_UP;
		}
		else if (ver == DOWN)
		{
			return LEFT_DOWN;
		}
		else
		{
			return LEFT;
		}
	}
	else if (hor == RIGHT)
	{
		if (ver == UP)
		{
			return RIGHT_UP;
		}
		else if (ver == DOWN)
		{
			return RIGHT_DOWN;
		}
		else
		{
			return RIGHT;
		}
	}
	else
	{
		return DEFAULT_AD_KEY;
	}
}

uint16_t Vertical()
{
	int sum = 0;
	for (int i = 0; i < 10; i++)
	{
		sum += AD_Value[0];
	}
	sum = sum / 10;

	if (4096 > sum && sum >= 3000) // ��2
	{
		return UP;
	}
	else if (1000 > sum && sum >= 0) // ��1
	{
		return DOWN;
	}
	else
	{
		return NO_KEY;
	}
}

uint16_t Horizontal()
{
	int sum = 0;
	for (int i = 0; i < 10; i++)
	{
		sum += AD_Value[1];
	}
	sum = sum / 10;

	if (4096 > sum && sum >= 3000) // ��2
	{
		return LEFT;
	}
	else if (1000 > sum && sum >= 0) // ��1
	{
		return RIGHT;
	}
	else
	{
		return NO_KEY;
	}
}
