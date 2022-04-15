/*
 * handle_load.c
 *
 *  Created on: 15 Apr 2022
 *      Author: Hao Lin
 */

#include "handle_load.h"

// Task to handle all automatic load shedding
void handle_load_auto()
{
	printf("handle_load_auto running\n");
	while (TRUE)
	{
		if (is_threshold_exceeded() && is_verification_elapsed()
			&& get_global_sys_status() != MAINTAIN)
		{
//			printf("Handling Load!!!\n");
			verification_timer_start();
			shed_load();
		}
		vTaskDelay(30);
	}
}

/* Self-contained logic of load shedding
 * - Controlled/depends on system state
 * - Performs only one round of load shedding per call
 *   (shed up to one load per call)
 */
void shed_load()
{
	// "LED Load" data
	static uint d[NO_OF_LOADS] = {0};

	uint next_load = 0;
	char sys_status = get_global_sys_status();

	// If not unstable, perform normal load shedding
	// Otherwise, perform critical load shedding!
	if (sys_status != UNSTABLE)
	{
		update_switch_data(d, NO_OF_LOADS);
		update_load_indication(d, NO_OF_LOADS);
	} else {
		next_load = get_next_load_pos(d, NO_OF_LOADS);
		if (next_load >= 0)
		{
			d[next_load] = LOW;
			led_write(LED_RED, (1 << next_load), LOW);
		}
	}
}

/* Helper Functions */

void update_load_indication(uint d[], uint size)
{
	CLEAR_LED_RED;

	uint led_no = LED_0;

	for (uint i = 0; i < size; i++)
	{
		if (d[i] == HIGH)
			led_write(LED_RED, led_no, HIGH);
		led_no = led_no << 1;
	}
}

void update_switch_data(uint d[], uint size)
{
	uint switch_no = SWITCH_0;

	for (uint i = 0; i < size; i++)
	{
		d[i] = check_switch(switch_no);
		switch_no = switch_no << 1;
	}
}

// Calculates and returns the next load to be shed from
// load data
uint get_next_load_pos(const uint d[], uint size)
{
	uint next_pos = -1;

	for (uint i = 0; i < size; i++)
	{
		if (d[i] == HIGH)
		{
			next_pos = i;
			break;
		}
	}

	return next_pos;
}
