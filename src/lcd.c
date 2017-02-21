/**
 *  ILI9163 128x128 LCD Driver (main file) , 1.44" Serial 128X128 SPI Color TFT LCD Module 16bit Display (RGB16)
 *
 **/
#include "lcd.h"
#include "delay.h"
#include "stm32f10x.h"

GPIO_InitTypeDef gpio;
SPI_InitTypeDef spi;
TIM_TimeBaseInitTypeDef bl_tim;
TIM_OCInitTypeDef  bl_channel;

uint8_t _abs(int16_t n){
	return (n < 0)? -n : n;
}

void LCD_Cmd(uint8_t address) {
    LCD_CMD;
    SPI1->DR = address;
}

void LCD_Data(uint8_t data) {
    LCD_DATA;
    SPI1->DR = data;
}

void LCD_Data16(uint16_t word) {
	LCD_DATA;
	SPI1->DR = word >> 8;
	SPI1->DR = word;
}

void LCD_Init() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	SysTick_Config(SystemCoreClock / 1000);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7; // SCK, MOSI
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = LCD_RST|LCD_A0|LCD_CS;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_PORT, &gpio);

	GPIO_SetBits(LCD_PORT, LCD_RST | LCD_A0);
	GPIO_ResetBits(LCD_PORT, LCD_CS);

	gpio.GPIO_Pin = GPIO_Pin_6;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &gpio);

	SPI_StructInit(&spi);
	spi.SPI_Mode = SPI_Mode_Master;
	spi.SPI_NSS = SPI_NSS_Soft;
	spi.SPI_CPHA = SPI_CPHA_1Edge;
	spi.SPI_CPOL = SPI_CPOL_Low;
	spi.SPI_DataSize = SPI_DataSize_8b;
	spi.SPI_FirstBit = SPI_FirstBit_MSB;
	//spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_Init(SPI1, &spi);
	SPI_Cmd(SPI1, ENABLE);

	TIM_TimeBaseStructInit(&bl_tim);
	bl_tim.TIM_CounterMode = TIM_CounterMode_Up;
	bl_tim.TIM_Prescaler = 64 - 1;
	bl_tim.TIM_Period = 1000 - 1;
	TIM_TimeBaseInit(TIM4, &bl_tim);

	TIM_OCStructInit(&bl_channel);
	bl_channel.TIM_OCMode = TIM_OCMode_PWM2;
	bl_channel.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC1Init(TIM4, &bl_channel);

	TIM_Cmd(TIM4, ENABLE);


	GPIO_SetBits(LCD_PORT, LCD_RST);

    // Hardware reset the LCD
    LCD_Reset();

    LCD_Cmd(LCD_CMD_EXIT_SLEEP_MODE);
    delay_ms(15); // Wait for the screen to wake up

    LCD_Cmd(LCD_CMD_SET_PIXEL_FORMAT);
    LCD_Data(0x05); // 12 bits per pixel

    LCD_Cmd(LCD_CMD_SET_GAMMA_CURVE);
    LCD_Data(0x04); // Select gamma curve 3

    LCD_Cmd(LCD_CMD_GAM_R_SEL);
    LCD_Data(0x01); // Gamma adjustment enabled

    LCD_Cmd(LCD_CMD_POSITIVE_GAMMA_CORRECT);
    LCD_Data(0x3f); // 1st Parameter
    LCD_Data(0x25); // 2nd Parameter
    LCD_Data(0x1c); // 3rd Parameter
    LCD_Data(0x1e); // 4th Parameter
    LCD_Data(0x20); // 5th Parameter
    LCD_Data(0x12); // 6th Parameter
    LCD_Data(0x2a); // 7th Parameter
    LCD_Data(0x90); // 8th Parameter
    LCD_Data(0x24); // 9th Parameter
    LCD_Data(0x11); // 10th Parameter
    LCD_Data(0x00); // 11th Parameter
    LCD_Data(0x00); // 12th Parameter
    LCD_Data(0x00); // 13th Parameter
    LCD_Data(0x00); // 14th Parameter
    LCD_Data(0x00); // 15th Parameter

    LCD_Cmd(LCD_CMD_NEGATIVE_GAMMA_CORRECT);
    LCD_Data(0x20); // 1st Parameter
    LCD_Data(0x20); // 2nd Parameter
    LCD_Data(0x20); // 3rd Parameter
    LCD_Data(0x20); // 4th Parameter
    LCD_Data(0x05); // 5th Parameter
    LCD_Data(0x00); // 6th Parameter
    LCD_Data(0x15); // 7th Parameter
    LCD_Data(0xa7); // 8th Parameter
    LCD_Data(0x3d); // 9th Parameter
    LCD_Data(0x18); // 10th Parameter
    LCD_Data(0x25); // 11th Parameter
    LCD_Data(0x2a); // 12th Parameter
    LCD_Data(0x2b); // 13th Parameter
    LCD_Data(0x2b); // 14th Parameter
    LCD_Data(0x3a); // 15th Parameter

    LCD_Cmd(LCD_CMD_FRAME_RATE_CONTROL1);
    LCD_Data(0x08); // DIVA = 8
    LCD_Data(0x08); // VPA = 8

    LCD_Cmd(LCD_CMD_DISPLAY_INVERSION);
    LCD_Data(0x07); // NLA = 1, NLB = 1, NLC = 1 (all on Frame Inversion)

    LCD_Cmd(LCD_CMD_POWER_CONTROL1);
    LCD_Data(0x1D); // VRH = 10:  GVDD = 3.35
    LCD_Data(0x02); // VC = 2: VCI1 = 2.65

    LCD_Cmd(LCD_CMD_POWER_CONTROL2);
    LCD_Data(0x02); // BT = 2: AVDD = 2xVCI1, VCL = -1xVCI1, VGH = 5xVCI1, VGL = -2xVCI1

    LCD_Cmd(LCD_CMD_VCOM_CONTROL1);
    LCD_Data(0x20); // VMH = 80: VCOMH voltage = 3.3
    LCD_Data(0x5b); // VML = 91: VCOML voltage = -0.225

    LCD_Cmd(LCD_CMD_VCOM_OFFSET_CONTROL);
    LCD_Data(0x40); // nVM = 0, VMF = 64: VCOMH output = VMH, VCOML output = VML

    LCD_Cmd(LCD_CMD_SET_COLUMN_ADDRESS);
    LCD_Data(0x00); // XSH
    LCD_Data(0x00); // XSL
    LCD_Data(0x00); // XEH
    LCD_Data(0x7f); // XEL (128 pixels x)

    LCD_Cmd(LCD_CMD_SET_PAGE_ADDRESS);
    LCD_Data(0x00);
    LCD_Data(0x00);
    LCD_Data(0x00);
    LCD_Data(0x7f); // 128 pixels y

    // Select display orientation
   	//LCD_Cmd(LCD_CMD_SET_ADDRESS_MODE);
	//LCD_Data(0x00);


    // Set the display to on
    LCD_Cmd(LCD_CMD_SET_DISPLAY_ON);
    LCD_Cmd(LCD_CMD_WRITE_MEMORY_START);
}

void LCD_Backlight(uint16_t brightness){
	 TIM_SetCompare1(TIM4, 1000-brightness);
}

// Reset the LCD hardware
void LCD_Reset(void) {
    // Reset pin is active low (0 = reset, 1 = ready)
	GPIO_ResetBits(LCD_PORT, LCD_RST);
    delay_ms(50);

    GPIO_SetBits(LCD_PORT, LCD_RST);
    delay_ms(120);
}

void LCD_SetPixel(uint8_t x, uint8_t y, uint16_t color) {
	LCD_CMD;
	SPI1->DR = LCD_CMD_SET_COLUMN_ADDRESS;

	LCD_DATA;
	SPI1->DR = 0x00;
	SPI1->DR = x ;

	LCD_CMD;
	SPI1->DR = LCD_CMD_SET_PAGE_ADDRESS;

	LCD_DATA;
	SPI1->DR = 0x00;
	SPI1->DR = y ;

	LCD_CMD;
	SPI1->DR = LCD_CMD_WRITE_MEMORY_START;

	LCD_DATA;
	SPI1->DR = (color >> 8);
	SPI1->DR = color ;
}

void _LCD_Rect(uint8_t x1, uint8_t y1, uint8_t x2,uint8_t y2, uint16_t color){
	LCD_CMD;
	SPI1->DR = LCD_CMD_SET_COLUMN_ADDRESS;

	LCD_DATA;
	SPI1->DR = 0;
	SPI1->DR = x1;
	LCD_DATA;
	SPI1->DR = 0;
	SPI1->DR = x2-1;

	LCD_CMD;
	SPI1->DR = LCD_CMD_SET_PAGE_ADDRESS;

	LCD_DATA;
	SPI1->DR = 0;
	SPI1->DR = y1;
	LCD_DATA;
	SPI1->DR = 0;
	SPI1->DR = y2-1;

	LCD_CMD;
	SPI1->DR = LCD_CMD_WRITE_MEMORY_START;

	LCD_DATA;
	int i,j;
	for(i = x1; i < x2; i++){
		for(j = y1; j < y2; j++){
			SPI1->DR = (color >> 8);
			SPI1->DR = color;
		}
	}
}

void LCD_Clear() {
	_LCD_Rect(0,0,LCD_WIDTH, LCD_HEIGHT, LCD.fill.color);
}

void LCD_MoveTo(uint8_t X, uint8_t Y){
	LCD.posX = X;
	LCD.posY = Y;
}

void LCD_LineTo(uint8_t toX, uint8_t toY) {

	  int16_t dx = _abs(toX-LCD.posX), sx = LCD.posX<toX ? 1 : -1;
	  int16_t dy = _abs(toY-LCD.posY), sy = LCD.posY<toY ? 1 : -1;
	  int16_t err = (dx>dy ? dx : -dy)/2, e2;

	  for(;;){
		if(LCD.brush.width == 1) LCD_SetPixel(LCD.posX, LCD.posY, LCD.brush.color);
		else _LCD_Rect(LCD.posX-LCD.brush.width/2, LCD.posY-LCD.brush.width/2, LCD.posX+LCD.brush.width/2, LCD.posY+LCD.brush.width/2, LCD.brush.color);
	    if (LCD.posX==toX && LCD.posY==toY) break;
	    e2 = err;
	    if (e2 >-dx) { err -= dy; LCD.posX += sx; }
	    if (e2 < dy) { err += dx; LCD.posY += sy; }
	  }
	}


void LCD_Rectangle(uint8_t x, uint8_t y, uint8_t w,uint8_t h) {
	if(LCD.brush.width){
		if(LCD.brush.style == BS_SOLID){
			uint8_t bw = LCD.brush.width;
			_LCD_Rect(x-bw, y-bw, x+w+bw, y, LCD.brush.color);
			_LCD_Rect(x-bw, y+h, x+w+bw, y+h+bw, LCD.brush.color);
			_LCD_Rect(x-bw, y, x, y+h, LCD.brush.color);
			_LCD_Rect(x+w, y, x+w+bw, y+h, LCD.brush.color);
		}
	}
	if(LCD.fill.style == FS_SOLID){
		_LCD_Rect(x, y, x+w, y+h, LCD.fill.color);
	}
}

void LCD_Circle(uint8_t x, uint8_t y, uint8_t r){

	int16_t x0, y0, d, xChange, yChange, radiusError;

	if(LCD.fill.style == FS_SOLID){
		x0 = r;
		y0 = 0;
		xChange = 1 - (r << 1);
		yChange = 0;
		radiusError = 0;

		while (x0 >= y0)
		{
			for (int i = x - x0; i <= x + x0; i++)
			{
				LCD_SetPixel(i, y + y0, LCD.fill.color);
				LCD_SetPixel(i, y - y0, LCD.fill.color);
			}
			for (int i = x - y0; i <= x + y0; i++)
			{
				LCD_SetPixel(i, y + x0, LCD.fill.color);
				LCD_SetPixel(i, y - x0, LCD.fill.color);
			}

			y0++;
			radiusError += yChange;
			yChange += 2;
			if (((radiusError << 1) + xChange) > 0)
			{
				x0--;
				radiusError += xChange;
				xChange += 2;
			}
		}
	}

	if(LCD.brush.style == BS_SOLID && LCD.brush.width > 0){
		x0 = 0;
		y0 = r+LCD.brush.width/2;
		d = 3 - (2 * (r+LCD.brush.width));

		while(x0 <= y0) {
			if(LCD.brush.width > 1){
				_LCD_Rect(x + x0 - LCD.brush.width/2, y + y0 - LCD.brush.width/2, x + x0 + LCD.brush.width/2, y + y0 + LCD.brush.width/2, LCD.brush.color);
				_LCD_Rect(x + y0 - LCD.brush.width/2, y + x0 - LCD.brush.width/2, x + y0 + LCD.brush.width/2, y + x0 + LCD.brush.width/2, LCD.brush.color);
				_LCD_Rect(x - x0 - LCD.brush.width/2, y + y0 - LCD.brush.width/2, x - x0 + LCD.brush.width/2, y + y0 + LCD.brush.width/2, LCD.brush.color);
				_LCD_Rect(x + y0 - LCD.brush.width/2, y - x0 - LCD.brush.width/2, x + y0 + LCD.brush.width/2, y - x0 + LCD.brush.width/2, LCD.brush.color);
				_LCD_Rect(x - x0 - LCD.brush.width/2, y - y0 - LCD.brush.width/2, x - x0 + LCD.brush.width/2, y - y0 + LCD.brush.width/2, LCD.brush.color);
				_LCD_Rect(x - y0 - LCD.brush.width/2, y - x0 - LCD.brush.width/2, x - y0 + LCD.brush.width/2, y - x0 + LCD.brush.width/2, LCD.brush.color);
				_LCD_Rect(x + x0 - LCD.brush.width/2, y - y0 - LCD.brush.width/2, x + x0 + LCD.brush.width/2, y - y0 + LCD.brush.width/2, LCD.brush.color);
				_LCD_Rect(x - y0 - LCD.brush.width/2, y + x0 - LCD.brush.width/2, x - y0 + LCD.brush.width/2, y + x0 + LCD.brush.width/2, LCD.brush.color);
			}
			else{
				LCD_SetPixel(x + x0, y + y0, LCD.brush.color);
				LCD_SetPixel(x + y0, y + x0, LCD.brush.color);
				LCD_SetPixel(x - x0, y + y0, LCD.brush.color);
				LCD_SetPixel(x + y0, y - x0, LCD.brush.color);
				LCD_SetPixel(x - x0, y - y0, LCD.brush.color);
				LCD_SetPixel(x - y0, y - x0, LCD.brush.color);
				LCD_SetPixel(x + x0, y - y0, LCD.brush.color);
				LCD_SetPixel(x - y0, y + x0, LCD.brush.color);
			}
			if (d < 0)
				d += (4 * x0) + 6;
			else {
				d += (4 * (x0 - y0)) + 10;
				y0 -= 1;
			}

			x0++;
		}
	}
}

void LCD_Char(char c) {
	uint16_t n;
	const uint8_t w = LCD.font.data[0];
	const uint8_t h = LCD.font.data[1];
	const uint16_t size = w*h;
	const uint16_t start = 3*8+((c-LCD.font.data[2])*size);
	uint8_t *bitmap = &LCD.font.data[start/8];

	if(c == 13){
		LCD.posY+= h;
		LCD.posX = 0;
		return;
	}

	if(c == 8){
		if(LCD.posX-w < 0){
			if(LCD.posY - h >= 0){
				LCD.posY-= h;
				LCD.posX = LCD_WIDTH - (LCD_WIDTH % w) - w;
			}
		}
		else
			LCD.posX-= w;
		LCD_Char(' ');
		LCD.posX-= w;
		return;
	}

	if(LCD.posX+w > LCD_WIDTH){
		LCD.posX = 0;
		LCD.posY += h;
	}

	if(LCD.posY+h > LCD_HEIGHT){
		LCD.posX = 0;
		LCD.posY = 0;
	}

	LCD_CMD;
	SPI1->DR = LCD_CMD_SET_COLUMN_ADDRESS;

	LCD_DATA;
	SPI1->DR = 0;
	SPI1->DR = LCD.posX;
	LCD_DATA;
	SPI1->DR = 0;
	SPI1->DR = LCD.posX+w-1;

	LCD_CMD;
	SPI1->DR = LCD_CMD_SET_PAGE_ADDRESS;

	LCD_DATA;
	SPI1->DR = 0;
	SPI1->DR = LCD.posY;
	LCD_DATA;
	SPI1->DR = 0;
	SPI1->DR = LCD.posY+h-1;

	LCD.posX+= w;

	LCD_CMD;
	SPI1->DR = LCD_CMD_WRITE_MEMORY_START;

	LCD_DATA;
	n = start;
	while(n < start+size ){

		if(*bitmap & (1 << (n % 8))){
			SPI1->DR = LCD.brush.color >> 8;
			SPI1->DR = LCD.brush.color;
		}
		else{
			SPI1->DR = LCD.fill.color >> 8;
			SPI1->DR = LCD.fill.color;
		}
		n++;

		if((n % 8 == 0))
			bitmap++;
	}

}

void LCD_Text(const char *str) {

    unsigned char c;
    while ( (c = *str++) ) {
    	LCD_Char(c);
    }
}
