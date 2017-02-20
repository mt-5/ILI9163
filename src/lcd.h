#ifndef LCD_H_
#define LCD_H_

#include "stdint.h"


// RGB24 to RGB16: Translates a 3 byte RGB value into a 2 byte value for the LCD (values should be 0-31)
// Radu Motisan: we need to scale the 8bit values to 5bit (for blue or red) / 6bit (for green)
#define rgb(r, g, b)  ((b>>3)<<11)|((g>>2)<< 5)|(r>>3)

//connections
#define LCD_PORT GPIOA
#define LCD_RST GPIO_Pin_1
#define LCD_A0 GPIO_Pin_0
#define LCD_CS GPIO_Pin_4

#define LCD_DATA GPIO_SetBits(LCD_PORT, LCD_A0)
#define LCD_CMD GPIO_ResetBits(LCD_PORT, LCD_A0)

// define size
#define LCD_HEIGHT 						128
#define LCD_WIDTH 						128

// LCD LCD Controller Commands
#define LCD_CMD_NOP                     0x00
#define LCD_CMD_SOFT_RESET              0x01
#define LCD_CMD_GET_RED_CHANNEL         0x06
#define LCD_CMD_GET_GREEN_CHANNEL       0x07
#define LCD_CMD_GET_BLUE_CHANNEL        0x08
#define LCD_CMD_GET_PIXEL_FORMAT        0x0C
#define LCD_CMD_GET_POWER_MODE          0x0A
#define LCD_CMD_GET_ADDRESS_MODE        0x0B
#define LCD_CMD_GET_DISPLAY_MODE        0x0D
#define LCD_CMD_GET_SIGNAL_MODE         0x0E
#define LCD_CMD_GET_DIAGNOSTIC_RESULT   0x0F
#define LCD_CMD_ENTER_SLEEP_MODE        0x10
#define LCD_CMD_EXIT_SLEEP_MODE         0x11
#define LCD_CMD_ENTER_PARTIAL_MODE      0x12
#define LCD_CMD_ENTER_NORMAL_MODE       0x13
#define LCD_CMD_EXIT_INVERT_MODE        0x20
#define LCD_CMD_ENTER_INVERT_MODE       0x21
#define LCD_CMD_SET_GAMMA_CURVE         0x26
#define LCD_CMD_SET_DISPLAY_OFF         0x28
#define LCD_CMD_SET_DISPLAY_ON          0x29
#define LCD_CMD_SET_COLUMN_ADDRESS      0x2A
#define LCD_CMD_SET_PAGE_ADDRESS        0x2B
#define LCD_CMD_WRITE_MEMORY_START      0x2C
#define LCD_CMD_WRITE_LUT               0x2D
#define LCD_CMD_READ_MEMORY_START       0x2E
#define LCD_CMD_SET_PARTIAL_AREA        0x30
#define LCD_CMD_SET_SCROLL_AREA         0x33
#define LCD_CMD_SET_TEAR_OFF            0x34
#define LCD_CMD_SET_TEAR_ON             0x35
#define LCD_CMD_SET_ADDRESS_MODE        0x36
#define LCD_CMD_SET_SCROLL_START        0X37
#define LCD_CMD_EXIT_IDLE_MODE          0x38
#define LCD_CMD_ENTER_IDLE_MODE         0x39
#define LCD_CMD_SET_PIXEL_FORMAT        0x3A
#define LCD_CMD_WRITE_MEMORY_CONTINUE   0x3C
#define LCD_CMD_READ_MEMORY_CONTINUE    0x3E
#define LCD_CMD_SET_TEAR_SCANLINE       0x44
#define LCD_CMD_GET_SCANLINE            0x45
#define LCD_CMD_READ_ID1                0xDA
#define LCD_CMD_READ_ID2                0xDB
#define LCD_CMD_READ_ID3                0xDC
#define LCD_CMD_FRAME_RATE_CONTROL1     0xB1
#define LCD_CMD_FRAME_RATE_CONTROL2     0xB2
#define LCD_CMD_FRAME_RATE_CONTROL3     0xB3
#define LCD_CMD_DISPLAY_INVERSION       0xB4
#define LCD_CMD_SOURCE_DRIVER_DIRECTION 0xB7
#define LCD_CMD_GATE_DRIVER_DIRECTION   0xB8
#define LCD_CMD_POWER_CONTROL1          0xC0
#define LCD_CMD_POWER_CONTROL2          0xC1
#define LCD_CMD_POWER_CONTROL3          0xC2
#define LCD_CMD_POWER_CONTROL4          0xC3
#define LCD_CMD_POWER_CONTROL5          0xC4
#define LCD_CMD_VCOM_CONTROL1           0xC5
#define LCD_CMD_VCOM_CONTROL2           0xC6
#define LCD_CMD_VCOM_OFFSET_CONTROL     0xC7
#define LCD_CMD_WRITE_ID4_VALUE         0xD3
#define LCD_CMD_NV_MEMORY_FUNCTION1     0xD7
#define LCD_CMD_NV_MEMORY_FUNCTION2     0xDE
#define LCD_CMD_POSITIVE_GAMMA_CORRECT  0xE0
#define LCD_CMD_NEGATIVE_GAMMA_CORRECT  0xE1
#define LCD_CMD_GAM_R_SEL               0xF2

enum LCD_Brush_Style{BS_TRANSPARENT, BS_SOLID, BS_DOTS, BS_STRPIES, BS_DOTSTRIPES};
enum LCD_Fill_Style{FS_TRANSPARENT, FS_SOLID, FS_DOTS, FS_STRPIES, FS_BACKSTRIPES};

typedef struct {
 	uint16_t color;
 	uint8_t width;
 	enum LCD_Brush_Style style;
 }Brush;

typedef struct{
	uint16_t color;
	enum LCD_Fill_Style style;
}Fill;

typedef struct{
	uint8_t scale;
	uint8_t *data;
}Font;

typedef struct{
	Brush brush;
	Fill fill;
	Font font;
	uint8_t posX;
	uint8_t posY;
}LCD_prop;

LCD_prop LCD;

	void LCD_Init(void);
	void LCD_Cmd(uint8_t address);
	void LCD_Data(uint8_t data);
	void LCD_Data16(uint16_t word);
	void LCD_SetAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
	void LCD_Reset(void);

	void LCD_Clear();
	void LCD_MoveTo(uint8_t X, uint8_t Y);
	void LCD_Backlight(uint16_t brightness);
	void LCD_SetPixel(uint8_t x, uint8_t y, uint16_t color);
	void LCD_LineTo(uint8_t toX, uint8_t toY);
	void LCD_Rectangle(uint8_t x, uint8_t y, uint8_t w,uint8_t h);
	void LCD_Circle(uint8_t x, uint8_t y, uint8_t r);

	void LCD_Text(const char *str);

#endif /* LCD_H_ */
