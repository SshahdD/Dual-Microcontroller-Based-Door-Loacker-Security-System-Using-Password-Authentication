/*
 * HMI_main.c
 *
 *      Author: shahd
 */

#include "lcd.h"
#include "keypad.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "util/delay.h"

#define RECEIVE_NEW_PASS   100
#define DOOR_UNLOCK          101
#define WRONG_PASS         102
#define CHECK_PWD          103
#define PEOPLE_DETECT      104

#define PASSWORD_LENGTH    7

uint8 pwd1[PASSWORD_LENGTH], pwd2[PASSWORD_LENGTH];
uint8 g_compare = 0;
uint8 g_checkpwd = 0;
uint8 g_unlock = 0;
uint8 g_tick = 0;
uint8 g_motion = 0;
uint8 g_changePass;

void MainOptions(void);

void tick(void) {
	g_tick++;
}

void CreatePwd(void) {

	uint8 i;

	uint8 key;

	UART_sendByte(RECEIVE_NEW_PASS);
	LCD_clearScreen();
	LCD_displayString("Enter New Pass");
	LCD_moveCursor(1, 0);
	_delay_ms(250);

	for (i = 0; i < 5; i++) {
		key = KEYPAD_getPressedKey() + 48;
		UART_sendByte(key);
		_delay_ms(250);
		LCD_displayCharacter('*');
		_delay_ms(250);
	}
	_delay_ms(250);
	while (KEYPAD_getPressedKey() != '=')
		;
	UART_sendByte('#');

	LCD_clearScreen();
	LCD_moveCursor(0, 0);
	LCD_displayString("Plz Re-enter The");
	LCD_moveCursor(1, 0);
	LCD_displayString("Same Pass: ");
	_delay_ms(250);

	for (i = 0; i < 5; i++) {
		key = KEYPAD_getPressedKey() + 48;
		UART_sendByte(key);
		_delay_ms(250);
		LCD_displayCharacter('*');
		_delay_ms(250);
	}
	_delay_ms(250);
	while (KEYPAD_getPressedKey() != '=')
		;
	UART_sendByte('#');

	LCD_clearScreen();
	g_compare = UART_recieveByte();
	_delay_ms(15);
	if (g_compare == 1) {

		return;
	} else if (g_compare == 0)
		CreatePwd();

}

void EnterPwd(void) {
	UART_sendByte(CHECK_PWD);

	LCD_clearScreen();
	LCD_moveCursor(0, 0);
	LCD_displayString("Plz Enter Old");
	LCD_moveCursor(1, 0);
	LCD_displayString("Pass: ");


	_delay_ms(100);
	LCD_moveCursor(1, 6);
	uint8 i;
	_delay_ms(250);
	for (i = 0; i < 5; i++) {
		pwd1[i] = KEYPAD_getPressedKey() + 48;
		UART_sendByte(pwd1[i]);

		_delay_ms(250);
		LCD_displayCharacter('*');
		_delay_ms(250);
	}

	_delay_ms(250);
	while (KEYPAD_getPressedKey() != '=')
		;
	UART_sendByte('#');
	_delay_ms(250);


	//UART_sendString(pwd1);

	g_compare = UART_recieveByte();

	while (g_checkpwd < 2) {
		if (g_compare == 1) {
			g_checkpwd = 0;
			LCD_clearScreen();
			LCD_moveCursor(0, 0);
			LCD_displayString("Right Password");
			_delay_ms(250);
			return;
		} else if (g_compare == 0) {
			g_checkpwd++;
			    LCD_clearScreen();
				LCD_moveCursor(0, 0);
				LCD_displayString("Wrong password");
				_delay_ms(100);

			EnterPwd();
		}
		if (g_checkpwd == 2) {
			UART_sendByte(WRONG_PASS);
					_delay_ms(250);
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "ERROR, wait!");
			LCD_displayStringRowColumn(1, 3, "seconds");
			g_tick = 0;
			while (g_tick < 60) {
				LCD_displayStringRowColumn(0, 0, "ERROR, wait!");
				LCD_displayStringRowColumn(1, 3, "seconds");
				LCD_moveCursor(1, 0);
				if ((60 - g_tick) > 10) {
					LCD_intgerToString(60 - g_tick);
				} else {
					LCD_displayStringRowColumn(1, 0, "  ");
					LCD_moveCursor(1, 0);
					LCD_intgerToString(60 - g_tick);
				}
			}
			g_checkpwd = 0;
			LCD_clearScreen();
			MainOptions();

		}

	}

}

void MainOptions(void) {

	g_changePass = 1;

	LCD_displayString("+ : OPEN DOOR");
	LCD_moveCursor(1, 0);
	LCD_displayString("- : CHANGE PASS");

	if (KEYPAD_getPressedKey() == '+') {

		_delay_ms(200);
		EnterPwd();

		if (g_changePass == 0) {
			return;
		}

		UART_sendByte(DOOR_UNLOCK);

		g_tick = 0;

		LCD_clearScreen();

		LCD_displayString("Door Unlocking");

		while (g_tick <= 15) {

		}

		g_tick = 0;
		LCD_clearScreen();
		UART_sendByte(PEOPLE_DETECT);
	    _delay_ms(250);

		LCD_moveCursor(0, 0);
		LCD_displayString("Wait For People ");
		LCD_moveCursor(1, 0);
		LCD_displayString("   To Enter ");
		g_motion=1;
	   while(g_motion==UART_recieveByte()){}

		g_motion=0;

		g_tick = 0;
		LCD_clearScreen();
		LCD_displayString("Door Locking");
		while (g_tick <= 15) {
		}

		LCD_clearScreen();

	}

	else if (KEYPAD_getPressedKey() == '-') {

		g_changePass = 0;
		EnterPwd();
		LCD_clearScreen();
		CreatePwd();

	}

}

int main() {
	SREG |= (1 << 7);

	LCD_init();
	sei();

	UART_ConfigType uart_config = { 9600, dISABLED, ONE_BIT, EIGHT_BIT };
	UART_init(&uart_config);
	Timer_ConfigType timer_config = { TIMER1, 0, 7812, PRESCALER_1024,
			COMPARE_MODE };
	Timer_init(&timer_config);
	Timer_setCallBack(tick, TIMER1);

	LCD_moveCursor(0, 0);
	LCD_displayString("Door Lock System");
	_delay_ms(250);
	CreatePwd();

	while (1) {

		MainOptions();

	}
}
