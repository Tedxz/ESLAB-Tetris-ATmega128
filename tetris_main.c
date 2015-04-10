#include <stdlib.h>
//#include <time.h>
#include "ui.h"
#include "LCD.h"
#include "Tetris.h"

#include "keypad.h"
#include "lcdstring.h"

#include <util/delay.h>

#define KEY_UP			2
#define KEY_DOWN 		10
#define KEY_LEFT 		5
#define KEY_RIGHT 		6
#define KEY_HARDDROP 	9
#define KEY_PAUSE	 	1

#define DELAY_KEY		3



static bool pause = false;          // æ¸¸æˆæš‚åœ
static uint8_t level = 1;           // çº§åˆ«
static uint16_t lines = 0;          // æ¶ˆé™¤çš„è¡Œæ•°
static uint16_t score = 0;          // åˆ†æ•°
static uint16_t time_count = 0;

static uint8_t delay = 0;


void game_over(void)
{
    ui_print_game_over();

    return;
}



void game_info_update(void)
{

    ui_print_level(level);
    ui_print_score(score);

    return;
}



void get_preview_brick(const void *info)
{
    uint16_t dat = *((uint16_t *)info);

    ui_print_preview(dat);

    return;
}



void draw_box(uint8_t x, uint8_t y, uint8_t color)
{
    if (color == 0)
        ui_draw_box(x, y, false);
    else
        ui_draw_box(x, y, true);

    return;
}



uint8_t random_num(void)
{
    return (uint8_t)rand();
}


void get_remove_line_num(uint8_t line)
{
    lines += line;

    switch (line)
    {
    case 1:
        score += 10;
        break;
    case 2:
        score += 25;
        break;
    case 3:
        score += 45;
        break;
    case 4:
        score += 80;
        break;
    default:
        break;
    }

    level = lines / 25 + 1;

    return;
}



void game_pause(void)
{
    pause = !pause;

    if (pause)
        ui_print_game_pause();
    else
        tetris_sync_all();      // å› ä¸ºæ‰“å°æš‚åœç ´åäº†åœ°å›¾åŒºæ˜¾ç¤º
                                // æ‰€ä»¥é€€å‡ºæ—¶è¦åˆ·æ–°æ•´ä¸ªåœ°å›¾åŒº
    return;
}


void game_run(void)
{
    uint16_t key;
    static bool refresh = false;
	uint8_t i;

    //delayMS(50);
	_delay_ms(30);

    if (!pause)
        time_count++;

    // çº§åˆ«è¶Šé«˜é€Ÿåº¦è¶Šå¿«
    if (time_count >= (12 - level))
    {
        time_count = 0;

        refresh = true;
        tetris_move(dire_down);
    }
	//refresh = true;
	key = KP_scan();
	
	if (delay > 0)
		--delay;

    if (key != 0 && delay == 0)
    {
		delay = DELAY_KEY;
        // æš‚åœæ—¶åªå“åº”å›žè½¦é”®
        if (pause && key != KEY_PAUSE)
            return;
		switch (key)
        {
        case KEY_UP:
            tetris_move(dire_rotate);
            break;
        case KEY_DOWN:
            tetris_move(dire_down);
            break;
        case KEY_LEFT:
            tetris_move(dire_left);
            break;
        case KEY_RIGHT:
            tetris_move(dire_right);
            break;
        case KEY_HARDDROP:
            while (tetris_move(dire_down));
            break;
        case KEY_PAUSE:
            game_pause();
			_delay_ms(100);
            break;
        default:
            break;
        }

        refresh = true;
    }

    if (refresh)
    {
        refresh = !refresh;
        tetris_sync();
        // æ›´æ–°è¡Œæ•°, åˆ†æ•°ç­‰ä¿¡æ¯

		lcd_refresh();
        game_info_update();
    }

    return;
}


int main(void)
{
    // éšæœºæ•°ç§å­
//    srand((int32_t)time(NULL));

	ui_init();
    KP_Init();
	PORTB = 0xFF;
    DDRB = 0x00;
	tetris_init(&draw_box, &random_num, &get_preview_brick, &get_remove_line_num);
    
	lcd_clr_scr();
	
    lcd_clr_scr();
	lcd_paint_scr(title);

    while (KP_scan() == 0) ;

    lcd_paint_scr(game);

    //game_pause();

    while (!tetris_is_game_over())
    {
        game_run();
    }

    game_over();

    // æŒ‰å›žè½¦é€€å‡º
    //while (getch() != 13);

    return 0;
}


