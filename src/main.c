#include "stm32f10x.h"
#include "font.h"
#include "lcd.h"
#include "delay.h"
#include "UART.h"

int main(void){

	LCD_Init();
	LCD_Backlight(1000);
	LCD.fill.style = FS_SOLID;
	LCD.fill.color = 0x0000;
	LCD_Clear();

	//LCD.fill.color = rgb(123,43,200);
	LCD.brush.style = BS_SOLID;
	LCD.brush.color = rgb(255,255,0);
	LCD.brush.width = 1;

	LCD.font.data = &FONT_BIG;

	UART_Init();

	LCD_MoveTo(0,0);


	while(1){
		LCD_Char(UART_Recieve());
	}


}
