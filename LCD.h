#ifndef _LCD_H_
#define _LCD_H_

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "init.h"

#include <stdint.h>
#include <stdbool.h>

#define D_PORT        PORTB
#define SET_RS() PORTD |= _BV(PD5)
#define CLR_RS() PORTD &= ~_BV(PD5)
#define SET_RW() PORTD |= _BV(PD6)
#define CLR_RW() PORTD &= ~_BV(PD6)
#define SET_E() PORTD |= _BV(PD7)
#define CLR_E() PORTD &= ~_BV(PD7)
#define NOP() asm("nop")

#define DATA 1
#define CMD 0

extern uint8_t cur_scr[16*64];
extern uint8_t hztab[16*64];
extern uint8_t game[16*64];
extern uint8_t title[16*64];
void lcd_refresh(void);
void lcd_clr_scr(void);
void wait_free(void);
void write_comm(uint8_t cmd_dat,uint8_t cd);
void LCD_Init(void);
void Init(void);

void lcd_draw_pixel(uint8_t x, uint8_t y, bool c);
void lcd_draw_box(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, bool c);

#endif

