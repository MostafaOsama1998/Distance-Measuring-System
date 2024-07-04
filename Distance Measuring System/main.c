/*
 * main.c
 *
 *  Created on: Feb 27, 2024
 *      Author: dell
 */
#include "lcd.h"
#include "gpio.h"
#include "common_macros.h"
#include "std_types.h"
#include "ultra_sonic.h"
#include <avr/delay.h>
#include "avr/io.h"
int main (void)
{
	uint16 showDistance=0;/*variable to receive the distance from ultra sonic sensor */
	LCD_init();/*initializing the LCD*/
	Ultrasonic_init();/*initializing the Ultra sonic sensor*/
	SET_BIT(SREG,7);/*general interrupt enable*/
	LCD_displayString("Distance=     CM");
	LCD_moveCursor(0,11);/*moving the cursor to the location to show the distance*/
	LCD_intgerToString(showDistance);
	_delay_ms(30);

	while(1)
	{
		showDistance=Ultrasonic_readDistance(); /*read the distance from the ultra sonic sensor*/
		LCD_moveCursor(0,11);/*move the Cursor to show the distance*/
		LCD_intgerToString(showDistance);/*display the distance on LCD */

		/*
		 * this if condition do delete the value above 99 or 9
		*/
		if(showDistance<100)
		{
			LCD_moveCursor(0,13);
			LCD_displayString(" ");
		}
		if(showDistance<10)
		{
			LCD_moveCursor(0,12);
			LCD_displayString(" ");
		}
		_delay_ms(60);
	}
	return 0;
}

