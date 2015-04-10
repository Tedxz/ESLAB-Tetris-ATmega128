#include <stdint.h>
#include "font.h"
#include "LCD.h"

void lcd_draw_char(uint8_t x, uint8_t y, char ch)
{
    uint16_t index = (uint16_t)(ch - '0') * 3;
    int i, j;
    for (i = 0; i < 3; ++i)
    {
        for (j = 0; j < 8; ++j)
        {
            lcd_draw_pixel(y + i, x + j, 1 & (digits_font[index + i] >> j));
        }
    }
}

void lcd_draw_string(uint8_t x, uint8_t y, char* s)
{
    int tx = x, ty = y;
    int i;
    for (i = 0; s[i]; ++i)
    {
        if (s[i] == '\n')
        {
            tx += 5;
            ty = y;
            continue;
        }
        if (s[i] == '\r')
        {
            continue;
        }
        lcd_draw_char(tx, ty, s[i]);
        ty += 4;

    }
}
