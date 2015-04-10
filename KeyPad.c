#include "keypad.h"

void KP_Init()
{
	CHIP_Init();
	PORTC &= 0x3F;
    DDRC |= 0xC0;
	PORTB = 0xFF;
    DDRB = 0x00;
}

uint8_t KP_read()
{
	asm("nop");
    uint8_t retb = PINB;
    return retb;
}

uint8_t KP_scan()
{
    uint8_t code = MASK;
	uint8_t key = 0x00;

    PORTB = 0xFF;//再初始化键盘扫描
    DDRB = 0x00;

    //scan the first line
    LINE1_SCAN();
    LINE1_SCAN();
    code = KP_read();
    if(code != MASK) {
        _delay_ms(10);
        code = KP_read();
        if(code != MASK) {
            switch(code) {
				case 0xFE: key = 0; break;
				case 0xFD: key = 1; break;
				case 0xFB: key = 2; break;
				case 0xF7: key = 3; break;
				case 0xEF: key = 4; break;
				case 0xDF: key = 5; break;
				case 0xBF: key = 6; break;
				case 0x7F: key = 7; break;
			}
        }
		/*
		while(1) {
			code = KP_read();
			if(code == MASK) {
				_delay_ms(10);
				code = KP_read();
				if(code == MASK);
					break;
			}
		}
		*/
    }

    //scan the second line
    LINE2_SCAN();
    LINE2_SCAN();
    code = KP_read();
    if(code != MASK) {
        _delay_ms(10);
        code = KP_read();
        if(code != MASK) {
			switch(code) {
				case 0xFE: key = 8; break;
				case 0xFD: key = 9; break;
				case 0xFB: key = 10; break;
				case 0xF7: key = 11; break;
				case 0xEF: key = 12; break;
				case 0xDF: key = 13; break;
				case 0xBF: key = 14; break;
				case 0x7F: key = 15; break;
			}
        }
		/*
		while(1) {
			code = KP_read();
			if(code == MASK) {
				_delay_ms(10);
				code = KP_read();
				if(code == MASK);
					break;
			}
		}
		*/
    }

    //if no key is pressed, return 0x00
    return key;
}

int main1()
{
    uint8_t keysc;
	KP_Init();
	//UART_Init();
	LCD12864PortInit();   //端口初始化
	LCD12864Init();       //LCD初始化
	//printf("Press a Key:\n");
	unsigned char ucString[]="  你按下了按键";
	PORTB = 0xFF;
    DDRB = 0x00;//初始化键盘扫描
    while(1) {

		keysc = KP_scan();
		if(keysc != 0){
			PORTB = 0x00;
			DDRB = 0xFF;           //初始化LCD显示
			LCD12864WriteCommand(0x90);
			for(int i=0;i<14;i++)
			LCD12864WriteData(ucString[i]);
			LCD12864WriteData(keysc);
			PORTB = 0xFF;//再初始化键盘扫描
    		DDRB = 0x00;
		}


        	//printf("You pressed key: %c\n", keysc);
    }
}


void LCD12864PortInit()
{
	//LCD数据端口设置
	PORTB = 0x00;         //
	DDRB = 0xFF;           //配置端口PB全部为输出口

	PORTD = 0x00;
	DDRD |= (1 << RS) | (1 << RW) | (1 << E);
	_delay_ms(15);
}

void LCD12864Init()
{
	LCD12864WriteCommand(0x01);  //清屏
	_delay_ms(15);
	LCD12864WriteCommand(0x38);  //显示模式
	_delay_ms(5);
	LCD12864WriteCommand(0x0f);  //显示开关控制，开显示，光标显示，光标闪烁
	LCD12864WriteCommand(0x0c);  //显示开关控制，开显示，光标不显示，光标不闪烁
	_delay_ms(5);
	LCD12864WriteCommand(0x06);  //光标设置，读或写一个字符后，地址指针加一，光标加一，整屏不移动
	_delay_ms(5);
}

void LCD12864WriteCommand(unsigned char ucCommand)
{
	LCD12864BusyCheck();

	PORTD &= ~(1 << RS);     //RS=0，写命令
	PORTD &= ~(1 << RW);     //RW=0，写指令
	PORTD |= (1 << E);       //E=1，写操作
	_delay_ms(2);
	PORTB = ucCommand;         //指令送数据端口
	PORTD &= ~(1 << E);     //E=0，停止写操作
    _delay_ms(5);

}

void LCD12864WriteData(unsigned char ucData)
{
	LCD12864BusyCheck();

	PORTD |= (1 << RS);      //RS=1，写数据
	PORTD &= ~(1 << RW);    //RW=0，写指令
	PORTD |= (1 << E);      //E=1，写操作
	_delay_ms(2);
	PORTB = ucData;        // 数据送数据端口
	PORTD &= ~(1 << E);    //E=0，停止写操作
	_delay_ms(5);

}

void LCD12864BusyCheck(void)
{
    unsigned int uiTemp;
	uiTemp = 3000;

	DDRB = 0x00;             //PB口置为输入口，准备读取数据
	PORTD &= ~(1 << RS);      //RS=0，读命令
	PORTD |= (1 << RW);    //RW=1，读指令
	PORTD |= (1 << E);      //E=1，使能

	while((0x80 & PINB) && (uiTemp--));   //监测忙信号，直到忙信号为0，才能进行读写操作

	PORTD &= ~(1 << E);   //E=0
	DDRB = 0xFF;          //PA口置为输出口，准备向端口发送数据

}


