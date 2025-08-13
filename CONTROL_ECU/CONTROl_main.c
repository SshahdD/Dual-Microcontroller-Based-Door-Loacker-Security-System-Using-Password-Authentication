#include "buzzer.h"
#include "external_eeprom.h"
#include "dc_motor.h"
#include "PIRsensor.h"
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#define RECEIVE_NEW_PASS   100
#define DOOR_UNLOCK        101
#define WRONG_PASS         102
#define CHECK_PWD          103
#define PEOPLE_DETECT      104


#define PASSWORD_LENGTH    6
#define EEPROM_PASSWORD_ADDRESS 0x0311

uint8 pwd1[PASSWORD_LENGTH], pwd2[PASSWORD_LENGTH];
uint8 eepromPass[PASSWORD_LENGTH];
uint8 g_tick = 0;
uint8 g_motion=0;
void tick(void) {
    g_tick++;
}

void receiveNewPass(void) {
    UART_receiveString(pwd1);
    _delay_ms(15);

    UART_receiveString(pwd2);

    _delay_ms(10);

    if (strcmp(pwd1, pwd2) == 0) {
        UART_sendByte(1);

        _delay_ms(15);
        EEPROM_writeString(PASSWORD_ADDRESS, pwd2);
        _delay_ms(10);
    } else {
    	// Buzzer_on();
        UART_sendByte(0);
    }
}



void checkpwd(void) {
    uint8 i;
    UART_receiveString(pwd2);

    for (i = 0; i < PASSWORD_LENGTH - 1; i++) {
        EEPROM_readByte(EEPROM_PASSWORD_ADDRESS + i, &eepromPass[i]);
        _delay_ms(10);
    }
    eepromPass[PASSWORD_LENGTH - 1] = '\0'; // Null-terminate

    if (strcmp(eepromPass, pwd2) == 0) {
        UART_sendByte(1);
    } else {
        UART_sendByte(0);
    }
}

void openDoor(void) {
    g_tick = 0;
    DcMotor_Rotate(CLOCKWISE, 100);
    while (g_tick <= 15) {} // Wait for the door to open

    g_tick = 0;
    DcMotor_Rotate(STOP, 0);
    g_motion=UART_recieveByte();
    if(g_motion==PEOPLE_DETECT){

    	while (PIR_getState()) {
    	 UART_sendByte(1);
    	  _delay_ms(50);
    	} // Wait for no motion
    	 UART_sendByte(0);
    	  _delay_ms(250);
    }

    g_tick = 0;
    DcMotor_Rotate(ANTI_CLOCKWISE, 100);
    while (g_tick <= 15) {} // Wait for the door to close

    DcMotor_Rotate(STOP, 0);
}

void wrongPass(void) {
    g_tick = 0;
    Buzzer_on();
    while (g_tick <= 60) {} // Beep for some time
    Buzzer_off();
}

int main() {
    uint8 x = 0;
    SREG |= (1 << 7); // Enable global interrupts

    Timer_ConfigType timer_config = {TIMER1, 0, 7812, PRESCALER_1024, COMPARE_MODE};
    UART_ConfigType uart_config = {9600, EVEN_PARITY, ONE_BIT, EIGHT_BIT};

    UART_init(&uart_config);
    Timer_init(&timer_config);
    Timer_setCallBack(tick, TIMER1);

    TWI_ConfigType twi_config = {0x01, 2};
    TWI_init(&twi_config);

    DcMotor_Init();
    PIR_init();
    Buzzer_init();

    while (1) {
        x = UART_recieveByte();

        switch (x) {
            case RECEIVE_NEW_PASS:
                receiveNewPass();
                break;
            case DOOR_UNLOCK:
                openDoor();
                break;
            case WRONG_PASS:
                wrongPass();
                break;
            case CHECK_PWD:
                checkpwd();
                break;

        }
    }
}
