#include <stdio.h>

#include "LCD.h"
#include "UI.h"
#include "lcdstring.h"

#define     MAP_WIDTH               10      //!< 地图宽 10 个box
#define     MAP_HEIGHT              20      //!< 地图高 20 个box

#define     MAP_START_COLUMN        51
#define     MAP_START_ROW           2

#define     PREVIEW_START_COLUMN    30
#define     PREVIEW_START_ROW       22

#define     BOX_SIZE                3


static void draw_box(uint8_t x, uint8_t y, bool box)
{

    lcd_draw_box(x, x + BOX_SIZE, y, y + BOX_SIZE, box);
	lcd_draw_pixel(x + BOX_SIZE / 2, y + BOX_SIZE / 2, 0);
	//lcd_paint_scr(cur_scr);
    return;
}

void ui_draw_box(uint8_t x, uint8_t y, bool box)
{
    draw_box(x*3 + MAP_START_COLUMN, y*3 + MAP_START_ROW, box);

    return;
}


void ui_print_score(uint32_t score)
{
    char s[6];
    sprintf(s, "%05d\0", (int)score);
    lcd_draw_string(38, 108, s);


    return;
}


void ui_print_level(uint8_t level)
{
    char s[6];
    sprintf(s, "%05d\0", (int)level);
    lcd_draw_string(17, 108, s);

    return;
}



void ui_print_game_over(void)
{

    return;
}


void ui_print_preview(uint16_t brick)
{
    uint8_t x, y;
    bool bit;
	//lcd_draw_string(10, 1, "Next");
    for (y = 0; y < 4; y++)
    {
        for (x = 0; x < 4; x++)
        {
            bit = ((brick & (1 << (15 - (y * 4 + x)))) >> (15 - (y * 4 + x)));
            draw_box(x*3 + PREVIEW_START_COLUMN, y*3 + PREVIEW_START_ROW, bit);
        }
    }

    return;
}





void ui_print_game_pause(void)
{

    return;
}



void ui_init(void)
{

    Init();
	lcd_clr_scr();
	 lcd_draw_box(MAP_START_COLUMN - 2,
                 MAP_START_COLUMN + BOX_SIZE*MAP_WIDTH + 2,
                 MAP_START_ROW - 2,
                 MAP_START_ROW + BOX_SIZE*MAP_HEIGHT + 2,
                 1);

	 lcd_draw_box(MAP_START_COLUMN - 1,
                  MAP_START_COLUMN + BOX_SIZE*MAP_WIDTH + 1,
                  MAP_START_ROW - 1,
                  MAP_START_ROW + BOX_SIZE*MAP_HEIGHT + 1,
                  0);
     //lcd_draw_string(0, 1, "Tetris\nXieZheng");
	lcd_refresh();

    return;
}


