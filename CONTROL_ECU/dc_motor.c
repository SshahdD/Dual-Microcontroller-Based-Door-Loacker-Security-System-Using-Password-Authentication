 /********************************************************************************
 * Module: DC Motor
 *
 * File Name: dc_motor.c
 *
 * Description: Source file for the DC Motor
 *
 * Author:Shahd Hamad
 *
 *******************************************************************************/

#include "dc_motor.h"
#include "gpio.h" /* to use the gpio functions */
#include "pwm.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


/*
 * Description:
 * 	 The Function responsible for setup the direction for the two motor pins through the GPIO driver.
 * 	 Stop at the DC-Motor at the beginning through the GPIO driver.
 * Inputs: None
 * Return: None
 */
void DcMotor_Init(void){

	GPIO_setupPinDirection(DC_MOTOR_INPUT_1_PORT,DC_MOTOR_INPUT_1_PIN,PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_INPUT_2_PORT,DC_MOTOR_INPUT_2_PIN,PIN_OUTPUT);
	GPIO_writePin(DC_MOTOR_INPUT_1_PORT,DC_MOTOR_INPUT_1_PIN,LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_INPUT_2_PORT,DC_MOTOR_INPUT_2_PIN,LOGIC_LOW);
	PWM_Timer0_Start(0);
}

/*
 * Description:
 * 	 The function responsible for rotate the DC Motor CW/ or A-CW or stop the motor based on the state input state value.
 *	 Send the required duty cycle to the PWM driver based on the required speed value.
 * Inputs:
 *	 state: The required DC Motor state, it should be CW or A-CW or stop. DcMotor_State data type should be declared as enum or uint8.
 *	 speed: decimal value for the required motor speed, it should be from 0 → 100. For example, if the input is 50, The motor should rotate with 50% of its maximum speed.
• Return: None
*/
void DcMotor_Rotate(DcMotor_State state,uint8 speed){
	switch(state)
	{
	case STOP:
		PWM_Timer0_Start(speed);
		GPIO_writePin(DC_MOTOR_INPUT_1_PORT,DC_MOTOR_INPUT_1_PIN,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_INPUT_2_PORT,DC_MOTOR_INPUT_2_PIN,LOGIC_LOW);
		break;
	case CLOCKWISE:
		PWM_Timer0_Start(speed);
		GPIO_writePin(DC_MOTOR_INPUT_1_PORT,DC_MOTOR_INPUT_1_PIN,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_INPUT_2_PORT,DC_MOTOR_INPUT_2_PIN,LOGIC_HIGH);
		break;
	case ANTI_CLOCKWISE:
		PWM_Timer0_Start(speed);
		GPIO_writePin(DC_MOTOR_INPUT_1_PORT,DC_MOTOR_INPUT_1_PIN,LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_INPUT_2_PORT,DC_MOTOR_INPUT_2_PIN,LOGIC_LOW);
		break;
	}
	uint8 _duty = (uint8)(PWM_MAX_VALUE * speed / PWM_OFFSET);
	PWM_Timer0_Start(_duty);
}

