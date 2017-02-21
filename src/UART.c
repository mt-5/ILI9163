#include "UART.h"

USART_InitTypeDef uart;
GPIO_InitTypeDef gpio;

void UART_Init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_3;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio);

	USART_StructInit(&uart);
	uart.USART_BaudRate = 115200;
	USART_Init(USART2, &uart);

	USART_Cmd(USART2, ENABLE);
}

void UART_Send_Char(char c)
{
 while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
 USART_SendData(USART2, c);
}

void UART_Send(const char* s)
{
 while (*s)
	 UART_Send_Char(*s++);
}

char UART_Recieve(){
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
	return USART_ReceiveData(USART2);
}
