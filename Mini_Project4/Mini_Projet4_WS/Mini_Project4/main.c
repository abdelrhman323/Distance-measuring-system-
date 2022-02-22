/*
 * main.c
 *
 *  Created on: Feb 22, 2022
 *      Author: abdelrahman
 */

#include "ultrasonic.h"
#include "lcd.h"
#include <AVR/io.h>
#include <util/delay.h>

int main(void)
{
/***************Main Function************************
 * 1) Declare uint16 variable distance to store the read distance
 * 2) Enable the global interrupt I-bit
 * 3) Initialize  LCD peripheral
 * 4) Initialize  Ultrasonic sensor
 * 5) Infinite loop to display the distance on the LCD
 */
	uint16 distance=0;
	SREG |= (1<<7);
	LCD_init();
	Ultrasonic_init();
	LCD_displayString("Distance = ");

	while(1)
	{
		distance = Ultrasonic_readDistance();
		LCD_moveCursor(0,10);
		if(distance >= 100)
		{
			LCD_intgerToString(distance);
		}
		else
		{
			LCD_intgerToString(distance);
			LCD_displayCharacter(' '); /*for the two digits number*/

		}
		LCD_displayString(" cm"); /*the measuring unit of the distance*/
	}
	return 0;
}
