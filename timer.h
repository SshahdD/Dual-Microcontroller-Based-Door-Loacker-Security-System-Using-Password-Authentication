 /******************************************************************************
 *
 * Module: Timer
 *
 * File Name: Timer.h
 *
 * Description: Header file for Timer
 *
 * Author: Shahd Hamad
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum{
	NO_CLOCK, PRESCALER_1, PRESCALER_8, PRESCALER_64, PRESCALER_256, PRESCALER_1024,
	EXTERNAL_FALLING_EDGE, EXTERNAL_RISING_EDGE
}Timer_Clock;

typedef enum{
	NORMAL_MODE,COMPARE_MODE,FAST_PWM_MODE
}Timer_Mode;

typedef enum{
	TIMER0,TIMER1,TIMER2
}Timer_ID;

typedef struct {
Timer_ID  timer_ID;
uint16 initial_value;
uint16 compare_value; // it will be used in compare mode only.
Timer_Clock timer_clock;
Timer_Mode mode;

} Timer_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description
 	 * Function to initialize the Timer driver
* Inputs: pointer to the configuration structure with type Timer1_ConfigType.
* Return: None
 */
void Timer_init(const Timer_ConfigType * Config_Ptr);

/*
 * Description
 	 * Function to disable the Timer via Timer_ID.
* Inputs:  Timer_ID
* Return: None
 */
void Timer_deInit(Timer_ID timer_type);

/*  Description: Function to set the Call Back function address to the required
Timer.
 Inputs: pointer to Call Back function and Timer Id you want to set The
Callback to it.
 Return: None
*/
void Timer_setCallBack(void(*a_ptr)(void),Timer_ID a_timer_ID);

#endif /* TIMER_H_ */
