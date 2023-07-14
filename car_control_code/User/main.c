#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "nrf24l01.h"
#include "oled.h"
#include "key.h"
#include "AD.h"
#include "Encoder.h"
#include "usart.h"

u8 mode = 1;


void send_action(u8 tmp_buf[3])
{
	if (NRF24L01_TxPacket(tmp_buf) == TX_OK)
	{
		OLED_ShowString(0, 9, "Sended DATA:", 8, 1);
		OLED_ShowString(0, 18, "             ", 8, 1);
		OLED_ShowNum(0, 18, tmp_buf[0], 2, 8, 1);
		OLED_ShowNum(20, 18, tmp_buf[1], 2, 8, 1);
	}
	else
	{
		OLED_ShowString(0, 2, "             ", 8, 1);
		OLED_ShowString(0, 18, "Send Failed ", 8, 1);
	}		
}

int main(void)
{
	u8 key = 0, key_status, key_value ,tmp_val,count;
	u8 tmp_buf[3];
	u8 tmp_buf_tmp[3];
	NRF24L01_Init(); // 初始化NRF24L01
	Key_Init();
	delay_init();
	LED_Init();
	AD_Init();
	OLED_Init();
	OLED_ColorTurn(0);	 // 0正常显示，1 反色显示
	OLED_DisplayTurn(0); // 0正常显示 1 屏幕翻转显示
	Encoder_Init();
	uart_init(115200);
	while (NRF24L01_Check())
	{
		OLED_ShowString(0, 0, "NRF24L01 Error", 8, 1);
		LED = 0;
		delay_ms(200);
		OLED_Clear();
		delay_ms(200);
		LED = 1;
	}

	OLED_ShowString(0, 0, "NRF CHECK OK!", 8, 1);
	delay_ms(1000);

	while (1)
	{
		//	NRF2401 模式控制（收发）
		if (mode == 1)
		{
			NRF24L01_TX_Mode();
			OLED_ShowString(0, 0, "NRF24L01 TX_Mode", 8, 1);
			while (1)
			{
				printf("aaa");
				key = Key_GetNum(&key_status, &key_value);
				printf("ccc");
				if (key != NO_KEY)
				{
					printf("ddd");
					tmp_buf[0] = key_status;
					tmp_buf[1] = key_value;
					printf("key_status = %d key_value = %d\r\n", key_status, key_value);
					if (key_status == KEY_1) {
						if(key_value == LEFT_UP || key_value == RIGHT_UP || key_value == LEFT || key_value == RIGHT || key_value == UP) 
						{
							count = 0;
							if (tmp_val != key_value) {
								// 记录下上一次的val
								tmp_val = key_value;
								
								tmp_buf_tmp[0] = KEY_1;
								tmp_buf_tmp[1] = UP;
								NRF24L01_TxPacket(tmp_buf_tmp);
								delay_ms(1300);
								//先发送前进1.6秒之后再发送转弯
							}
						} 
						else if (key_value == LEFT_DOWN || key_value == RIGHT_DOWN || key_value == DOWN) 
						{
							count = 0;
							if (tmp_val != key_value) {
								// 记录下上一次的val
								tmp_val = key_value;
								
								tmp_buf_tmp[0] = KEY_1;
								tmp_buf_tmp[1] = DOWN;
								NRF24L01_TxPacket(tmp_buf_tmp);
								//先发送前进1秒之后再发送转弯
								delay_ms(1300);
							}
						}
						else 
					  {
							tmp_val = DEFAULT_KEY;
							count++;
						}
						if (count < 2) 
						{
							send_action(tmp_buf);
							LED = !LED;
						} 
						else {
							count = 3; // 防止自加溢出
						}
					}
					else {
						printf("eee");
						count = 0;
						send_action(tmp_buf);
						LED = !LED;
					}
				} 
			}
		}
	}
}
