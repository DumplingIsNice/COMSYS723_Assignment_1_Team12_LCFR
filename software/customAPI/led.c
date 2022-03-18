/*
 * leds.c
 *
 *  Created on: 18 Mar 2022
 *      Author: Hao Lin
 */

#include "led.h"

void led_init()
{
	// Sets all LED to HIGH
	//SET_LED_RED(LED_R_ALL);
	//SET_LED_GREEN(LED_G_ALL);

	// Sets all LED to LOW
	CLEAR_LED_RED;
	CLEAR_LED_GREEN;
}

/* Inputs:
 * color of led - Macro
 * binary position of led - logic high, rest logic low
 * desired on or off - Macro
*/
void led_write(char colour, uint led_pos, uint is_on)
{

	uint current_led_pos = 0;

	switch (colour)
		{
			case LED_RED:
				current_led_pos = GET_LED_RED;
				current_led_pos = set_on_off(led_pos, current_led_pos, is_on);
				SET_LED_RED(current_led_pos);
				return;
			case LED_GREEN:
				current_led_pos = GET_LED_GREEN;
				current_led_pos = set_on_off(led_pos, current_led_pos, is_on);
				SET_LED_GREEN(current_led_pos);
				return;
			default:
				return;
		}
}

uint set_on_off(uint led_pos, uint current_led_pos, uint is_on)
{
	switch (is_on)
	{
		case HIGH:
			current_led_pos |= led_pos;
			break;
		case LOW:
			current_led_pos &= ~led_pos;
			break;
		default:
			;
	}

	return current_led_pos;
}
