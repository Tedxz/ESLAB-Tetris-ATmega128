
#ifndef _UI_H_
#define _UI_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
//#include "pcc32.h"

extern void ui_init(void);
extern void ui_draw_box(uint8_t x, uint8_t y, bool box);
extern void ui_print_preview(uint16_t block);
extern void ui_print_level(uint8_t level);
extern void ui_print_line(uint16_t line);
extern void ui_print_score(uint32_t score);
extern void ui_print_game_over(void);
extern void ui_print_game_pause(void);

#endif



