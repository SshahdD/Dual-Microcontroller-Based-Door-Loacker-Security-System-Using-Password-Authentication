 /******************************************************************************
 *
 * Module: Timer
 *
 * File Name: Timer.c
 *
 * Description: Source file for Timer
 *
 * Author:Shahd Hamad
 *
 *******************************************************************************/

#include "timer.h"

#include <avr/io.h> /* To use ICU/Timer1 Registers */
#include <avr/interrupt.h> /* For ICU ISR */


/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr0)(void) = NULL_PTR;
static volatile void (*g_callBackPtr1)(void) = NULL_PTR;
static volatile void (*g_callBackPtr2)(void) = NULL_PTR;
/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after overflow*/
		(*g_callBackPtr0)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after reaching the compare value*/
		(*g_callBackPtr0)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after overflow*/
		(*g_callBackPtr1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after reaching the compare value*/
		(*g_callBackPtr1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_OVF_vect)
{
	if(g_callBackPtr2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after overflow*/
		(*g_callBackPtr2)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_COMP_vect)
{
	if(g_callBackPtr2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after reaching the compare value*/
		(*g_callBackPtr2)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description
 	 * Function to initialize the Timer driver
* Inputs: pointer to the configuration structure with type Timer1_ConfigType.
* Return: None
 */
void Timer_init(const Timer_ConfigType * Config_Ptr){

	switch(Config_Ptr->timer_ID)
	{
		case TIMER0:
			TCCR0 = (Config_Ptr->mode == NORMAL_MODE) ? (1 << FOC0) : ((1 << FOC0) | (1 << WGM01));
			OCR0 = (Config_Ptr->compare_value);
			TCNT0 = (Config_Ptr->initial_value);
			TCCR0 |= (Config_Ptr->timer_clock);
			TIMSK |= (Config_Ptr->mode == NORMAL_MODE) ? (1 << TOIE0) : (1 << OCIE0);
			break;

		case TIMER1:
			TCCR1A = (1 << FOC1A);
			TCCR1B = (Config_Ptr->mode == COMPARE_MODE) ? (1 << WGM12) | (Config_Ptr->timer_clock) : (Config_Ptr->timer_clock) ;
			OCR1A = (Config_Ptr->compare_value);
			TIMSK |= (Config_Ptr->mode == NORMAL_MODE) ? (1 << TOIE1) : (1 << OCIE1A) ;
			TCNT1 = (Config_Ptr->initial_value);
			break;

		case TIMER2:
			TCCR2 = (Config_Ptr->mode == NORMAL_MODE) ? (1 << FOC2) : ((1 << FOC2) | (1 << WGM21));
			OCR2 = (Config_Ptr->compare_value);
			TCNT2 = (Config_Ptr->initial_value);
			TCCR2 |= (Config_Ptr->timer_clock);
			TIMSK |= (Config_Ptr->mode == NORMAL_MODE) ? (1 << TOIE2) : (1 << OCIE2) ;
			break;
	}
}

/*
 * Description
 	 * Function to disable the Timer1.
* Inputs: None
* Return: None
 */
void Timer_deInit(Timer_ID timer_type){

	switch (timer_type) {
	 case 0:
		 TCCR0 = 0;
	   	 TCNT0 = 0;
		 OCR0 = 0;
		 TIMSK &= 0xFC;
    break;
	 case 1:
		    TCCR1A = 0;
		 	TCCR1B = 0;
		 	TCNT1 = 0;
		 	OCR1A = 0;
		 	TIMSK = 0xC3;
		 	break;
	 case 2:
		  TCCR0 = 0;
	      TCNT0 = 0;
		  OCR0 = 0;
		  TIMSK &= 0x3F;
	 break;
}
}

/*
 * Description
 	 *Function to set the Call Back function address.
* Inputs: pointer to Call Back function.
* Return: None
 */
void Timer_setCallBack(void(*a_ptr)(void),Timer_ID a_timer_ID){

		switch (a_timer_ID) {
			 case 0:
				 /* Save the address of the Call back function in a global variable */
				 		g_callBackPtr0 = a_ptr;
				 		break;
			 case 1:
				 /* Save the address of the Call back function in a global variable */
				 		g_callBackPtr1 = a_ptr;
				 	    break;
			 case 2:
				 /* Save the address of the Call back function in a global variable */
						g_callBackPtr2 = a_ptr;
						break;
		}
}
