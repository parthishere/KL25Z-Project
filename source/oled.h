#ifndef __OLED_H__
#define __OLED_H__

#include "MKL25Z4.h"
#include "i2c.h"
#include <string.h>
#include "fonts.h"

typedef enum {
	SSD1306_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
	SSD1306_COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD */
} SSD1306_COLOR_t;


uint8_t init_SSD1306(void);
void fill_SSD1306(SSD1306_COLOR_t color);
void updateScreen_SSD1306(void);



/**
 * @brief  Sets cursor pointer to desired location for strings
 * @param  x: X location. This parameter can be a value between 0 and SSD1306_WIDTH - 1
 * @param  y: Y location. This parameter can be a value between 0 and SSD1306_HEIGHT - 1
 * @retval None
 */
void gotoXY_SSD1306(uint16_t x, uint16_t y);

/**
 * @brief  Puts character to internal RAM
 * @note   @ref updateScreen_SSD1306() must be called after that in order to see updated LCD screen
 * @param  ch: Character to be written
 * @param  *Font: Pointer to @ref FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval Character written
 */
char putc_SSD1306(char ch, FontDef_t* Font, SSD1306_COLOR_t color);

/**
 * @brief  Puts string to internal RAM
 * @note   @ref updateScreen_SSD1306() must be called after that in order to see updated LCD screen
 * @param  *str: String to be written
 * @param  *Font: Pointer to @ref FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval Zero on success or character value when function failed
 */
char puts_SSD1306(char* str, FontDef_t* Font, SSD1306_COLOR_t color);

void drawPixel_SSD1306(uint16_t x, uint16_t y, SSD1306_COLOR_t color);

#endif // !__OLED_H__