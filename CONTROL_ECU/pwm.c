/*
 * pwm.c
 *
 *  Created on: ٠٤‏/١٠‏/٢٠٢٤
 *      Author: shahd
 */
#include <avr/io.h>
#include "common_macros.h"
#include "pwm.h"
#include "gpio.h"
#include "timer.h"


void PWM_Timer0_Start(uint8 duty_cycle)
{


	TCNT0 = 0;
	GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);
	TCCR0 |= (1 << WGM01) | (1 << WGM00) | (1 << COM01) | (1 << CS00) | (1 << CS01);

	/* Sets the required duty cycle for OCR0 */
	OCR0 = duty_cycle;
}
