/*
 * ultrasonic.c
 *
 *  Created on: Feb 21, 2022
 *      Author: abdelrahman
 */

#include "ultrasonic.h"
#include "icu.h"
#include "gpio.h"
#include <util/delay.h>
#include <AVR/io.h>

/*
 * flag and time of fall edge global variable
 */
uint16 g_flag=0;
double g_fall=0.0;

void Ultrasonic_init()
{
	/*
	 * initialize the ICU by passing structure config by address
	 */
	ICU_ConfigurationType config={rising,f_cpu_8};
	ICU_init(&config);
	/*
	 *  setting up the callback function
	 */
	ICU_settingCallback(&Ultrasonic_edgeProcessing);
	/*
	 *  set PB5 as a output trigger pin
	 */
	GPIO_setupPinDirection(PORTB_ID,PIN5_ID,PIN_OUTPUT);
	GPIO_writePin(PORTB_ID,PIN5_ID,LOGIC_LOW);
}
void Ultrasonic_Trigger()
{
	/*
	 *  send the trigger signal to Ultrasonic sensor
	 */
	GPIO_writePin(PORTB_ID,PIN5_ID,LOGIC_HIGH);
	_delay_us(15);
	GPIO_writePin(PORTB_ID,PIN5_ID,LOGIC_LOW);
}
uint16 Ultrasonic_readDistance()
{
	/*
	 * sending the trigger signal to Ultrasonic by Ultrasonic_Trigger function
	 */
	double distance;
	/*
	 *  using trigger function to send a 15us pulse to
	 *  make the sensor work
	 */
	Ultrasonic_Trigger();
	_delay_ms(1); /*Delay 1 us to allow the sonar to rebound to ensor*/
	/*
	 * the timer gets incremented after 1 us time elapse.
	 *		= 17000 x (g_fall) x 1 x 10^-6 cm
	 *		= 0.017 x (g_fall) cm
	 *		= (g_fall) / 58.8 cm
	 *	Note: g_fall ---> the time of falling edge of echo pulse
	 *	                  which indicate the double of time taken to cover
	 *	                  the distance
	 */
	distance = (g_fall/57.9);
	return distance;
}
void Ultrasonic_edgeProcessing()
{
	/*
	 * when the echo give the first interrupt as ICU captured a rising
	 * edge, the g_flag=0 so the If statement will be executed
	 * If 	: 1) clear timer value to calculate the pulse width from 0 accurately
	 * 		  2) change the detected edge to be falling to get the pulse end
	 * 		  3) make g_flag = 1 for the second interrupt to execute else statement
	 * 	Else: 1) get the timer1 value to get the time (pulse duration)
	 * 		  2) change the detected edge to be rising for another distance
	 * 		  3) clear timer value for another calculations
	 * 		  4) make g_flag = 0 again for another calculations
	 */
	if(g_flag == 0)
	{
		ICU_clearTimerValue();
		ICU_setEdgeDetection(falling);
		g_flag=1;
	}
	else
	{
		g_fall = ICU_getInputCaptureValue();
		ICU_setEdgeDetection(rising);
		ICU_clearTimerValue();
		g_flag=0;
	}

}
