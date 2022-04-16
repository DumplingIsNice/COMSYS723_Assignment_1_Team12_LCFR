/*
 * handle_load.c
 *
 *  Created on: 15 Apr 2022
 *      Author: Hao Lin
 */

#include "handle_load.h"

extern uint load_data[];

// Task to handle all automatic load shedding
void handle_load_auto()
{
	printf("handle_load_auto running\n");
	char sys_status;
	while (TRUE)
	{
		sys_status = get_global_sys_status();
		if (is_verification_elapsed())
		{
			if (sys_status == UNSTABLE)
			{
	//			printf("Handling Load!!!\n");
				shed_load(get_load_data(), NO_OF_LOADS);
				verification_timer_start();
			} else if (sys_status == STABLE) {
				connect_load(get_load_data(), NO_OF_LOADS);
				verification_timer_start();
			}
		}
		vTaskDelay(30);
	}
}

/* Self-contained logic of load shedding
 * - Controlled/depends on system state
 * - Performs only one round of load shedding per call
 *   (shed up to one load per call)
 */
void shed_load(uint d[], uint size)
{
	int8_t load_index = 0;
	char sys_status = get_global_sys_status();

	// If not unstable, perform normal load shedding
	// Otherwise, perform critical load shedding!
	if (sys_status == NORMAL)
	{
		update_switch_data(d, NO_OF_LOADS);
		update_load_indication(d, NO_OF_LOADS);
	} else {

		load_index = get_next_load_pos(d, NO_OF_LOADS);
		if (load_index >= 0)
		{
			d[load_index] = LOW;
			led_write(LED_RED, (1 << load_index), LOW);
			led_write(LED_GREEN, (1 << load_index), HIGH);
		}
	}
}

void connect_load(uint d[], uint size)
{
	int load_index = get_last_load_pos(d, NO_OF_LOADS);
	if (load_index >= 0)
	{
		// Note: Don't write high if switch position is off!!!
//		printf("Connect Load called for index %d\n", load_index);
		d[load_index] = HIGH;
		led_write(LED_RED, (1 << load_index), HIGH);
		led_write(LED_GREEN, (1 << load_index), LOW);
	} else {
		set_global_sys_status(NORMAL);
		return;
	}
}

void shed_load_off_only(uint d[], uint size)
{
	;
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

// Calculates and returns the next load position to be shed from
// load data (aka get next highest priority that is loaded)
int8_t get_next_load_pos(const uint d[], const uint size)
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

// Calculates and returns the last load position to be shed from
// load data (aka get next lowest priority that is loaded)
int8_t get_last_load_pos(const uint d[], const uint size)
{
	/* Note: ANCIENT DEBUGGING FORMAT */
	// For an unexplained error
	// Change when got time...
	uint i = 0;
	uint j = size-1-i;
	while (i <= size)
	{
		j = size-1-i;
//		printf("Looping through d[%d] = %d\n", j, d[j]);
		if (d[j] == LOW)
		{
			return j;
		}
		i++;
	}
	return j;
}
