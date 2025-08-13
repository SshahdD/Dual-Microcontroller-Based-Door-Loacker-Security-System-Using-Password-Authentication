/*
 * PIRsensor.h
 *
 *  Created on: ٣١‏/١٠‏/٢٠٢٤
 *      Author: shahd
 */

#ifndef PIRSENSOR_H_
#define PIRSENSOR_H_


#include "std_types.h"

#define PIR_PORT_ID  PORTC_ID
#define PIR_PIN_ID   PIN2_ID

void PIR_init(void) ;
uint8 PIR_getState(void);

#endif /* PIRSENSOR_H_ */
