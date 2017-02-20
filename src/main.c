#include "stm32f10x.h"
#include "font.h"
#include "lcd.h"

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

	LCD.font.scale = 1;
	LCD.font.data = FONT_BIG;
	LCD_MoveTo(5,5);
	LCD_Text("maciej!");

	LCD_Circle(64,64,32);
	int i= 0;
	int d= 1;
	while(1){
		LCD_MoveTo(i,5);
		LCD_Text("maciej");
			delay_ms(10);
		if(i+d > 128-72 || i+d < 0)
			d*= -1;
		i+=d;
	}


}
