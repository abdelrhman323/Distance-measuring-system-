/*
 * icu.c
 *
 *  Created on: Feb 21, 2022
 *      Author: abdelrahman
 */
#include "icu.h"
#include "gpio.h"
#include <AVR/io.h>
#include <AVR/interrupt.h>

/*
 *  global static pointer to function initialized by NULL
 */
 static volatile void (*g_callbackPtr)(void) = NULL_PTR;

 /*
  * this ISR is for ICU to call the callback funtion when ICU do it's task
  */
 ISR(TIMER1_CAPT_vect)
 {
	 if(g_callbackPtr!= NULL_PTR)
	 {
		 /*
		  * call it if the pointer not equal NULL then the application
		  *  detect an edge
		  */
		 (*g_callbackPtr)();
	 }
 }

void ICU_init(const ICU_ConfigurationType * ICU_Config)
{
	/*
	 * PD6 is Input for the input signal
	 */
	GPIO_setupPinDirection(PORTD_ID,PIN6_ID,PIN_INPUT);
	/*
	 * use Normal mode not PWM
	 */
	TCCR1A |= (1<<FOC1A) | (1<<FOC1B);
	/*
	 * Setting rising edge or falling eddge
	 */
	TCCR1B = (TCCR1B & 0xBF) | ((ICU_Config->edge)<<6);
	/*
	 * setting the prescaler frequency
	 */
	TCCR1B |= (TCCR1B & 0xF8) | (ICU_Config->prescale);
	/*
	 * Initializing timer1 counter
	 */
	TCNT1=0;
	/*
	 * Initializing ICU register to count parallel to TCNT1
	 */
	ICR1=0;
	/*
	 * enable interrupt for ICU module
	 */
	TIMSK |= (1<<TICIE1);
}

void ICU_settingCallback(void (*a_Ptr)(void))
{
	/*
	 * update the pointer to point to callback funtion
	 */
	g_callbackPtr = a_Ptr;
}
void ICU_setEdgeDetection(ICU_EdgeType edge)
{
	/*
	 * set the 6th bit in TCCR1B register 0 for falling and 1 for rising
	 */
	TCCR1B = (TCCR1B & 0xBF) | (edge<<6);
}

uint16 ICU_getInputCaptureValue()
{
	/*
	 * getting the timer value
	 */
	return ICR1;
}
void ICU_clearTimerValue()
{
	/*
	 * clearing timer value
	 */
	TCNT1=0;
}
void ICU_DeInit()
{
	/*
	 * clear every thing to disable timer and ICU
	 */
	TCCR1A=0;
	TCCR1B=0;
	TCNT1=0;
	ICR1=0;
	TIMSK &= ~(1<<TICIE1);
}
