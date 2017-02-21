/*
 * UART.h
 *
 *  Created on: 21 lut 2017
 *      Author: Mac
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f10x.h"

void send_char(char c);
void send_string(const char* s);
char UART_Recieve();

#endif /* UART_H_ */
