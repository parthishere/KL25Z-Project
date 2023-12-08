#include "oled.h"

#define SSD1306_I2C_ADDR         0b0111100 // 0x78 >> 1
//#define SSD1306_I2C_ADDR       0x7A

/* SSD1306 settings */
/* SSD1306 width in pixels */
#define SSD1306_WIDTH            128

/* SSD1306 LCD height in pixels */
#define SSD1306_HEIGHT           64


#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A
#define SSD1306_DEACTIVATE_SCROLL                    0x2E // Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F // Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 // Set scroll range

#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7



#define SSD1306_SEND_COMMAND(command)      writeRegisterI2C(SSD1306_I2C_ADDR, 0x00, (command))
/* Write data */
#define SSD1306_SEND_DATA(data)            writeRegisterI2C(SSD1306_I2C_ADDR, 0x40, (data))

/* SSD1306 data buffer */
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

/* Private SSD1306 structure */
typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} SSD1306_t;

static SSD1306_t SSD1306;

uint8_t init_SSD1306(void) {

	/* Init I2C */
	initI2C();
	
	/* Check if LCD connected to I2C */
	
	/* A little delay */
	uint32_t p = 2500;
	while(p>0)
		p--;
	
	/* Init LCD */
	SSD1306_SEND_COMMAND(0xAE); //display off
	SSD1306_SEND_COMMAND(0x20); //Set Memory Addressing Mode   
	SSD1306_SEND_COMMAND(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	SSD1306_SEND_COMMAND(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	SSD1306_SEND_COMMAND(0xC8); //Set COM Output Scan Direction
	SSD1306_SEND_COMMAND(0x00); //---set low column address
	SSD1306_SEND_COMMAND(0x10); //---set high column address
	SSD1306_SEND_COMMAND(0x40); //--set start line address
	SSD1306_SEND_COMMAND(0x81); //--set contrast control register
	SSD1306_SEND_COMMAND(0xFF);
	SSD1306_SEND_COMMAND(0xA1); //--set segment re-map 0 to 127
	SSD1306_SEND_COMMAND(0xA6); //--set normal display
	SSD1306_SEND_COMMAND(0xA8); //--set multiplex ratio(1 to 64)
	SSD1306_SEND_COMMAND(0x3F); //
	SSD1306_SEND_COMMAND(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	SSD1306_SEND_COMMAND(0xD3); //-set display offset
	SSD1306_SEND_COMMAND(0x00); //-not offset
	SSD1306_SEND_COMMAND(0xD5); //--set display clock divide ratio/oscillator frequency
	SSD1306_SEND_COMMAND(0xF0); //--set divide ratio
	SSD1306_SEND_COMMAND(0xD9); //--set pre-charge period
	SSD1306_SEND_COMMAND(0x22); //
	SSD1306_SEND_COMMAND(0xDA); //--set com pins hardware configuration
	SSD1306_SEND_COMMAND(0x12);
	SSD1306_SEND_COMMAND(0xDB); //--set vcomh
	SSD1306_SEND_COMMAND(0x20); //0x20,0.77xVcc
	SSD1306_SEND_COMMAND(0x8D); //--set DC-DC enable
	SSD1306_SEND_COMMAND(0x14); //
	SSD1306_SEND_COMMAND(0xAF); //--turn on SSD1306 panel
	

	SSD1306_SEND_COMMAND(SSD1306_DEACTIVATE_SCROLL);

	/* Clear screen */
	fill_SSD1306(SSD1306_COLOR_WHITE);
	
	/* Update screen */
	updateScreen_SSD1306();
	
	/* Set default values */
	SSD1306.CurrentX = 0;
	SSD1306.CurrentY = 0;
	
	/* Initialized OK */
	SSD1306.Initialized = 1;
	
	/* Return OK */
	return 1;
}

void fill_SSD1306(SSD1306_COLOR_t color) {
	/* Set memory */
	memset(SSD1306_Buffer, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(SSD1306_Buffer));
}

void updateScreen_SSD1306(void) {
	uint8_t m;
	
	for (m = 0; m < 8; m++) {
		SSD1306_SEND_COMMAND(0xB0 + m);
		SSD1306_SEND_COMMAND(0x00);
		SSD1306_SEND_COMMAND(0x10);
		
		/* Write multi data */
		writeMultRegisterI2C(SSD1306_I2C_ADDR, 0x40, SSD1306_WIDTH, &SSD1306_Buffer[SSD1306_WIDTH * m]);
	}
}


void gotoXY_SSD1306(uint16_t x, uint16_t y) {
	/* Set write pointers */
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}   

char putc_SSD1306(char ch, FontDef_t* Font, SSD1306_COLOR_t color) {
	uint32_t i, b, j;
	
	/* Check available space in LCD */
	if (
		SSD1306_WIDTH <= (SSD1306.CurrentX + Font->FontWidth) ||
		SSD1306_HEIGHT <= (SSD1306.CurrentY + Font->FontHeight)
	) {
		/* Error */
		return 0;
	}
	
	/* Go through font */
	for (i = 0; i < Font->FontHeight; i++) {
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t) color);
			} else {
				SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)!color);
			}
		}
	}
	
	/* Increase pointer */
	SSD1306.CurrentX += Font->FontWidth;
	
	/* Return character written */
	return ch;
}

char puts_SSD1306(char* str, FontDef_t* Font, SSD1306_COLOR_t color) {
	/* Write characters */
	while (*str) {
		/* Write character by character */
		if (putc_SSD1306(*str, Font, color) != *str) {
			/* Return error */
			return *str;
		}
		
		/* Increase string pointer */
		str++;
	}
	
	/* Everything OK, zero should be returned */
	return *str;
}


void SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color) {
	if (
		x >= SSD1306_WIDTH ||
		y >= SSD1306_HEIGHT
	) {
		/* Error */
		return;
	}
	
	/* Check if pixels are inverted */
	if (SSD1306.Inverted) {
		color = (SSD1306_COLOR_t)!color;
	}
	
	/* Set color */
	if (color == SSD1306_COLOR_WHITE) {
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
	} else {
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}