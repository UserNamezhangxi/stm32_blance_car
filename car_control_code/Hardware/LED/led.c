#include "stm32f10x.h"                  // Device header
#include "led.h"

//��ʼ��PC13�����.��ʹ������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE); 						  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);

}
