/*
 * switches.c
 *
 *  Created on: 18 Mar 2022
 *      Author: Hao Lin
 */

#include "switch.h"

// Checks the status of a switch at position and return status
	/* Inputs:
	 * Switch position macro
	 *
	 * Returns: switch status
	 * 2 - Unidentified
	 * 1 - HIGH
	 * 0 - LOW
	 */
uint check_switch(uint switch_pos)
{
	uint switch_status = 2;
	uint switch_value = GET_SWITCHES;

		switch_value = (switch_value & switch_pos);

		switch ((switch_value & switch_pos))
		{
			case LOW:
				led_write(LED_RED, switch_pos, LOW);
				switch_status = LOW;
				break;
			default:
				led_write(LED_RED, switch_pos, HIGH);
				switch_status = HIGH;
				break;
		}
	return switch_status;
}

void handle_switches()
{
	printf("handle_switches running\n");
	while (TRUE)
	{
		shed_load();
		vTaskDelay(300);
	}
}


