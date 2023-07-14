#include "stm32f10x.h" // Device header
#include "delay.h"
#include "key.h"
#include "ad.h"
#include "usart.h"
#include "encoder.h"

u8 tp = 20;

void Key_Init(void)
{
	// PLAY right left car_mode
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15; // play | right | left | car_mode | down
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// led buzz up
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_5; // led | buzz | up
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

u8 Key_GetNum(u8 *key, u8 *value)
{
	u8 temp_key = NO_KEY;
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)
	{
		printf("1111\r\n");
		delay_ms(10);
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)
			;
		delay_ms(10);
		*key = KEY_2;
		*value = KEY_BEEP;
		return DEFAULT_KEY;
	}
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0)
	{
		printf("2222\r\n");
		delay_ms(10);
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0)
			;
		delay_ms(10);
		*key = KEY_2;
		*value = KEY_LED;
		return DEFAULT_KEY;
	}

	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0)
	{
		printf("3333\r\n");
		delay_ms(10);
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0)
			;
		delay_ms(10);
		*key = KEY_2;
		*value = KEY_UP;
		return DEFAULT_KEY;
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) == 0)
	{
		printf("4444\r\n");
		delay_ms(10);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) == 0)
			;
		delay_ms(10);
		*key = KEY_2;
		*value = KEY_DOWN;
		return DEFAULT_KEY;
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0)
	{
		printf("5555\r\n");
		delay_ms(10);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0)
			;
		delay_ms(10);
		*key = KEY_2;
		*value = KEY_LEFT;
		return DEFAULT_KEY;
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 0)
	{
		printf("66666\r\n");
		delay_ms(10);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 0)
			;
		delay_ms(10);
		*key = KEY_2;
		*value = KEY_RIGHT;
		return DEFAULT_KEY;
	}

	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 0)
	{
		printf("77777\r\n");
		delay_ms(10);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 0)
			;
		delay_ms(10);
		*key = KEY_2;
		*value = KEY_CAR_MODE;
		return DEFAULT_KEY;
	}

	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
	{
		printf("8888\r\n");
		delay_ms(10);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
			;
		delay_ms(10);
		*key = KEY_2;
		*value = KEY_PLAY_PAUSE;
		return DEFAULT_KEY;
	}

//	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0)
//	{
//		printf("99999\r\n");
//		delay_ms(10);
//		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0)
//			printf("99999ccccc\r\n");
//			;
//		delay_ms(10);
//		*key = KEY_2;
//		*value = KEY_NEXT;
//		return DEFAULT_KEY;
//	}

	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)
	{
		printf("1010100\r\n");
		delay_ms(10);
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)
			;
		delay_ms(10);
		*key = KEY_2;
		*value = KEY_PRE;
		return DEFAULT_KEY;
	}

  temp_key = get_music_volume(tp);
	if (temp_key != NO_KEY)
	{
		printf("1111111111111\r\n");
		tp = temp_key;
		*key = KEY_3;
		*value = temp_key;
		return DEFAULT_KEY;
	}
	
	temp_key = Get_AD_Key();
	if (temp_key != NO_KEY)
	{
		printf("11212121211121121211\r\n");
		*key = KEY_1;
		*value = temp_key;
		return DEFAULT_AD_KEY;
	}
	
	printf("bbb\r\n");
	*key = NO_KEY;
	*value = NO_KEY;
	return NO_KEY;
}
