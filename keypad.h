#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
//#include "uart.h"
#include "init.h"
#define RS PD5           //����/������ƶ� 0���1����
#define RW PD6           //��/дѡ����ƶ� 0д��1��
#define E PD7            //ʹ�ܶ�          �½��ض����ߵ�ƽд

#define LINE1_SCAN() PORTC = (PORTC | _BV(PC7)) & ~_BV(PC6)
#define LINE2_SCAN() PORTC = (PORTC | _BV(PC6)) & ~_BV(PC7)
#define MASK 0xFF

void LCD12864PortInit(void);   //�˿ڳ�ʼ��
void LCD12864Init(void);       //LCD��ʼ��
void LCD12864WriteCommand(unsigned char ucCommand);   //LCDдָ��
void LCD12864WriteData(unsigned char ucData);  //LCDд����
void LCD12864BusyCheck(void);   //��д��⺯����ÿ�ζ�Һ������ǰ��Ҫ���ж�д���

void KP_Init();
uint8_t KP_read();
uint8_t KP_scan();

#endif
