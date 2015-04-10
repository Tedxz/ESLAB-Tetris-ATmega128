#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
//#include "uart.h"
#include "init.h"
#define RS PD5           //数据/命令控制端 0命令，1数据
#define RW PD6           //读/写选择控制端 0写，1读
#define E PD7            //使能端          下降沿读，高电平写

#define LINE1_SCAN() PORTC = (PORTC | _BV(PC7)) & ~_BV(PC6)
#define LINE2_SCAN() PORTC = (PORTC | _BV(PC6)) & ~_BV(PC7)
#define MASK 0xFF

void LCD12864PortInit(void);   //端口初始化
void LCD12864Init(void);       //LCD初始化
void LCD12864WriteCommand(unsigned char ucCommand);   //LCD写指令
void LCD12864WriteData(unsigned char ucData);  //LCD写数据
void LCD12864BusyCheck(void);   //读写检测函数，每次对液晶操作前都要进行读写检测

void KP_Init();
uint8_t KP_read();
uint8_t KP_scan();

#endif
