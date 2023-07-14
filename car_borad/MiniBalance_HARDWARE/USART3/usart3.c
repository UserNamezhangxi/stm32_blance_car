/***********************************************
��˾����Ȥ�Ƽ�����ݸ�����޹�˾
Ʒ�ƣ�WHEELTEC
������wheeltec.net
�Ա����̣�shop114407458.taobao.com
����ͨ: https://minibalance.aliexpress.com/store/4455017
�汾��5.7
�޸�ʱ�䣺2021-04-29


Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version:5.7
Update��2021-04-29

All rights reserved
***********************************************/
#include "usart3.h"
uint8_t Usart3_Receive;
u16 USART_RX_STA = 0;			//����״̬���
u8 USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.
#if 0
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
	int handle;
};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
_sys_exit(int x)
{
	x = x;
}
//�ض���fputc����
int fputc(int ch, FILE *f)
{
	while ((USART3->SR & 0X40) == 0);
	USART3->DR = (u8)ch;
	return ch;
}
#endif

/**************************************************************************
Function: Usart3 initialization
Input   : bound:Baud rate
Output  : none
�������ܣ�����3��ʼ��
��ڲ�����bound:������
����  ֵ����
**************************************************************************/
void uart3_init(u32 bound)
{
	// GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��UGPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //ʹ��USART3ʱ��
														   // USART3_TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			   // PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// USART3_RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			  // PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// Usart3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_InitStructure.USART_BaudRate = bound;										//
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//
	USART_InitStructure.USART_Parity = USART_Parity_No;								//?TD��?��??
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//

	/* Configure USART3 */
	USART_Init(USART3, &USART_InitStructure); //
	/* Enable USART3 Receive and Transmit interrupts */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //
	// USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//
	/* Enable the USART3 */
	USART_Cmd(USART3, ENABLE); //
}

void USART3_Send(uint8_t *s) //�����ַ�������
{
	while (*s)
	{
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
		USART_SendData(USART3, *s);
		s++;
	}
}

void USART3_IRQHandler(void)
{

	u8 Res;

	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(USART3); //��ȡ���յ�������

		if ((USART_RX_STA & 0x8000) == 0) //����δ���
		{
			if (USART_RX_STA & 0x4000) //���յ���0x0d
			{
				if (Res != 0x0a)
					USART_RX_STA = 0; //���մ���,���¿�ʼ
				else
				{
					USART_RX_STA |= 0x8000; //���������
//					USART3_Send(USART_RX_BUF);
//					USART_RX_STA = 0;
				}
			}
			else //��û�յ�0X0D
			{
				if (Res == 0x0d)
					USART_RX_STA |= 0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;
					USART_RX_STA++;
					if (USART_RX_STA > (USART_REC_LEN - 1))
						USART_RX_STA = 0; //�������ݴ���,���¿�ʼ����
				}
			}
		}
	}
}

/**************************************************************************
Function: Receive interrupt function
Input   : none
Output  : none
�������ܣ�����3�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
#if 0
void USART3_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //���յ�����
	{	  
	  static	int uart_receive=0;//����������ر���
		static u8 Flag_PID,i,j,Receive[50];
		static float Data;
  	uart_receive=USART_ReceiveData(USART3); 
		Usart3_Receive=uart_receive;
		if(uart_receive==0x59)  Flag_velocity=2;  //���ٵ���Ĭ��ֵ��
		if(uart_receive==0x58)  Flag_velocity=1;  //���ٵ�
		
	  if(uart_receive>10)  //Ĭ��ʹ��
    {			
			if(uart_receive==0x5A)  //ɲ��
		  { 
				Flag_front=0,
				Flag_back=0,
				Flag_Left=0,
				Flag_Right=0;
			}
			else if(uart_receive==0x41) //ǰ
			{		
				Flag_front=1,
				Flag_back=0;
//				Flag_Left=0,
//				Flag_Right=0;
			}
			else if(uart_receive==0x45) //��
			{
				Flag_front=0,
				Flag_back=1;
//				Flag_Left=0,
//				Flag_Right=0;
			}
			else if(uart_receive==0x42||uart_receive==0x43||uart_receive==0x44)	 //��
			{
//				Flag_front=0,
//				Flag_back=0,
				Flag_Left=0,
				Flag_Right=1;  
			}
			else if(uart_receive==0x46||uart_receive==0x47||uart_receive==0x48)	  //��   
			{
//				Flag_front=0,
//				Flag_back=0,
				Flag_Left=1,
				Flag_Right=0;  
			}
			else if(uart_receive==0x46||uart_receive==0x47||uart_receive==0x5B)	{ // led����
				LED = !LED;
			}
			else  //ɲ��
		  {
			  Flag_front=0,
				Flag_back=0,
				Flag_Left=0,
				Flag_Right=0;
				LED = 1;
			}
  	}
//		if(uart_receive<10)     //����appΪ��MiniBalanceV1.0  ��ΪMiniBalanceV1.0��ң��ָ��ΪA~H ��HEX��С��10
//		{			
//			Flag_velocity=1;//�л������ٵ�
//			if(uart_receive==0x00)	Flag_front=0,Flag_back=0,Flag_Left=0,Flag_Right=0;//ɲ��
//			else if(uart_receive==0x01)	Flag_front=1,Flag_back=0,Flag_Left=0,Flag_Right=0;//ǰ
//			else if(uart_receive==0x05)	Flag_front=0,Flag_back=1,Flag_Left=0,Flag_Right=0;//��
//			else if(uart_receive==0x02||uart_receive==0x03||uart_receive==0x04)	
//														Flag_front=0,Flag_back=0,Flag_Left=0,Flag_Right=1;  //��
//			else if(uart_receive==0x06||uart_receive==0x07||uart_receive==0x08)	    //��
//														Flag_front=0,Flag_back=0,Flag_Left=1,Flag_Right=0;
//			else Flag_front=0,Flag_back=0,Flag_Left=0,Flag_Right=0;//ɲ��
//  	}	

//		
//		if(Usart3_Receive==0x7B) Flag_PID=1;   //APP����ָ����ʼλ
//		if(Usart3_Receive==0x7D) Flag_PID=2;   //APP����ָ��ֹͣλ

//		 if(Flag_PID==1)  //�ɼ�����
//		 {
//				Receive[i]=Usart3_Receive;
//				i++;
//		 }
//		 if(Flag_PID==2)  //��������
//		 {
//			  if(Receive[3]==0x50) 				 PID_Send=1;
//			  else if(Receive[1]!=0x23) 
//				{								
//					for(j=i;j>=4;j--)
//					{
//						Data+=(Receive[j-1]-48)*pow(10,i-j);
//					}
//					switch(Receive[1])
//					{
//						case 0x30:  Balance_Kp=Data;break;
//						case 0x31:  Balance_Kd=Data;break;
//						case 0x32:  Velocity_Kp=Data;break;
//						case 0x33:  Velocity_Ki=Data;break;
//						case 0x34:  Turn_Kp=Data;break; 
//					  case 0x35:  Turn_Kd=Data;break; 
//						case 0x36:  break; //Ԥ��
//						case 0x37:  break; //Ԥ��
//						case 0x38:  break; //Ԥ��
//					}
//				}				 
//			    Flag_PID=0;
//					i=0;
//					j=0;
//					Data=0;
//					memset(Receive, 0, sizeof(u8)*50);//��������
//		 } 
	}  

}
#endif