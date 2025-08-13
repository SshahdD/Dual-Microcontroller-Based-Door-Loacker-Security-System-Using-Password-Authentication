/*
 * pwm.h
 *
 *  Created on: ٠٤‏/١٠‏/٢٠٢٤
 *      Author: shahd
 */

#ifndef PWM_H_
#define PWM_H_

#include"std_types.h"

#define PWM_MAX_VALUE (255u)
#define PWM_OFFSET	  (100u)

void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_H_ */
