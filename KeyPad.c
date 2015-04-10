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

    PORTB = 0xFF;//�ٳ�ʼ������ɨ��
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
	LCD12864PortInit();   //�˿ڳ�ʼ��
	LCD12864Init();       //LCD��ʼ��
	//printf("Press a Key:\n");
	unsigned char ucString[]="  �㰴���˰���";
	PORTB = 0xFF;
    DDRB = 0x00;//��ʼ������ɨ��
    while(1) {

		keysc = KP_scan();
		if(keysc != 0){
			PORTB = 0x00;
			DDRB = 0xFF;           //��ʼ��LCD��ʾ
			LCD12864WriteCommand(0x90);
			for(int i=0;i<14;i++)
			LCD12864WriteData(ucString[i]);
			LCD12864WriteData(keysc);
			PORTB = 0xFF;//�ٳ�ʼ������ɨ��
    		DDRB = 0x00;
		}


        	//printf("You pressed key: %c\n", keysc);
    }
}


void LCD12864PortInit()
{
	//LCD���ݶ˿�����
	PORTB = 0x00;         //
	DDRB = 0xFF;           //���ö˿�PBȫ��Ϊ�����

	PORTD = 0x00;
	DDRD |= (1 << RS) | (1 << RW) | (1 << E);
	_delay_ms(15);
}

void LCD12864Init()
{
	LCD12864WriteCommand(0x01);  //����
	_delay_ms(15);
	LCD12864WriteCommand(0x38);  //��ʾģʽ
	_delay_ms(5);
	LCD12864WriteCommand(0x0f);  //��ʾ���ؿ��ƣ�����ʾ�������ʾ�������˸
	LCD12864WriteCommand(0x0c);  //��ʾ���ؿ��ƣ�����ʾ����겻��ʾ����겻��˸
	_delay_ms(5);
	LCD12864WriteCommand(0x06);  //������ã�����дһ���ַ��󣬵�ַָ���һ������һ���������ƶ�
	_delay_ms(5);
}

void LCD12864WriteCommand(unsigned char ucCommand)
{
	LCD12864BusyCheck();

	PORTD &= ~(1 << RS);     //RS=0��д����
	PORTD &= ~(1 << RW);     //RW=0��дָ��
	PORTD |= (1 << E);       //E=1��д����
	_delay_ms(2);
	PORTB = ucCommand;         //ָ�������ݶ˿�
	PORTD &= ~(1 << E);     //E=0��ֹͣд����
    _delay_ms(5);

}

void LCD12864WriteData(unsigned char ucData)
{
	LCD12864BusyCheck();

	PORTD |= (1 << RS);      //RS=1��д����
	PORTD &= ~(1 << RW);    //RW=0��дָ��
	PORTD |= (1 << E);      //E=1��д����
	_delay_ms(2);
	PORTB = ucData;        // ���������ݶ˿�
	PORTD &= ~(1 << E);    //E=0��ֹͣд����
	_delay_ms(5);

}

void LCD12864BusyCheck(void)
{
    unsigned int uiTemp;
	uiTemp = 3000;

	DDRB = 0x00;             //PB����Ϊ����ڣ�׼����ȡ����
	PORTD &= ~(1 << RS);      //RS=0��������
	PORTD |= (1 << RW);    //RW=1����ָ��
	PORTD |= (1 << E);      //E=1��ʹ��

	while((0x80 & PINB) && (uiTemp--));   //���æ�źţ�ֱ��æ�ź�Ϊ0�����ܽ��ж�д����

	PORTD &= ~(1 << E);   //E=0
	DDRB = 0xFF;          //PA����Ϊ����ڣ�׼����˿ڷ�������

}


