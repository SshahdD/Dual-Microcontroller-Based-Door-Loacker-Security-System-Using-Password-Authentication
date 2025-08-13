/*
 * PIRsensor.c
 *
 *  Created on: ٣١‏/١٠‏/٢٠٢٤
 *      Author: shahd
 */

#include <avr/io.h>
#include "PIRsensor.h"
#include "gpio.h"
#include "common_macros.h"



void PIR_init(void){
	 GPIO_setupPinDirection( PIR_PORT_ID, PIR_PIN_ID,PIN_INPUT);
	 return;
}

uint8 PIR_getState(void){

	return (GPIO_readPin(PIR_PORT_ID ,  PIR_PIN_ID));
}

