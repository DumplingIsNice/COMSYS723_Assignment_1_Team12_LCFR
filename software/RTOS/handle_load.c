/*
 * handle_load.c
 *
 *  Created on: 15 Apr 2022
 *      Author: Hao Lin
 */

#include "handle_load.h"

// Task to handle all load shedding
void handle_load()
{
	printf("handle_load_auto running\n");
	char sys_status;

	// Load's actual status
	uint load_data[NO_OF_LOADS] = {0};
	// Load's status as demanded by switches
	uint switch_data[NO_OF_LOADS] = {0};
	// Load's status as logged by automatic shedding
	// 1 = shed by relay
	uint auto_data[NO_OF_LOADS] = {0};

	while (TRUE)
	{
		sys_status = get_global_sys_status();

		update_switch_data(switch_data, NO_OF_LOADS);

		if (sys_status == NORMAL || sys_status == MAINTAIN)
		{
			// Perform free load control
			set_array_equal(load_data, switch_data, NO_OF_LOADS, FALSE);
			update_load_indication(load_data, NO_OF_LOADS);
		} else {
			// Perform limited (off) load control
			set_array_equal(load_data, switch_data, NO_OF_LOADS, TRUE);
			update_load_indication(load_data, NO_OF_LOADS);

			if (is_verification_elapsed())
			{
				if (sys_status == UNSTABLE)
				{
		//			printf("Handling Load!!!\n");
					shed_load(load_data, auto_data, NO_OF_LOADS);
					verification_timer_start();
				} else if (sys_status == STABLE) {
					connect_load(load_data, switch_data, auto_data,  NO_OF_LOADS);
					verification_timer_start();
				}
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
void shed_load(uint d[], uint a[], uint size)
{
	int8_t load_index = 0;

	// If not unstable, perform normal load shedding
	// Otherwise, perform critical load shedding!
	load_index = get_next_load_pos(d, NO_OF_LOADS);
	if (load_index >= 0)
	{
		d[load_index] = LOW;
		a[load_index] = HIGH;

		led_write(LED_RED, (1 << load_index), LOW);
		led_write(LED_GREEN, (1 << load_index), HIGH);

		/* Response timing Section */

#ifdef MOCK_RESPONSE

		xSemaphoreTake(response_timer_end_gate_sem, portMAX_DELAY);
		response_timer_end();

        uint time = calc_response_time();

        if (xSemaphoreTake(response_time_sem, portMAX_DELAY) == pdTRUE)
		{
			xQueueSendToBack(Q_response_time, &time, portMAX_DELAY);
			xSemaphoreGive(response_time_sem);
		} else {
			printf("response_time_sem Semaphore cannot be taken!\n");
		}
        xSemaphoreGive(response_timer_start_gate_sem);
#endif
	}
}

void connect_load(uint d[], uint a[], const uint s[], const uint size)
{
	int load_index = get_last_load_pos(d, a, s, NO_OF_LOADS);
	if (load_index >= 0)
	{
		d[load_index] = HIGH;
		a[load_index] = LOW;
		led_write(LED_RED, (1 << load_index), HIGH);
		led_write(LED_GREEN, (1 << load_index), LOW);

		// Clears all higher green LEDS that had yet been turned off
		reconnect_indication_clearing(load_index, size);
	} else {
		set_global_sys_status(NORMAL);
		CLEAR_LED_GREEN;
	}
}

/* Helper Functions */

// Clears all invalid lingering green LED indications
	// of loads that HAD been managed by the relay BUT
	// was switched off manually (despite being off) BEFORE
	// the relay is able to automatically reconnect the load
void reconnect_indication_clearing(const uint load_index, const uint size)
{
	for (uint i = load_index+1; i < size; i++)
	{
		led_write(LED_GREEN, (1 << i), LOW);
	}
}

// Updates normal load indication (red LED)
void update_load_indication(uint d[], uint size)
{
	CLEAR_LED_RED;

	uint led_no = LED_0;

	for (uint i = 0; i < size; i++)
	{
		if (d[i] == HIGH)
		{
			led_write(LED_RED, led_no, HIGH);
			led_write(LED_GREEN, led_no, LOW);	// Insurance
		}
		led_no = led_no << 1;
	}
}

// Updates array with switches input
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
int8_t get_last_load_pos(const uint d[], const uint a[], const uint s[], const uint size)
{
//	/* Note: ANCIENT DEBUGGING FORMAT */
//	// For an unexplained error
//	// Change when got time...

	uint i = 0;
	uint j = size-1-i;
	while (i <= size)
	{
		j = size-1-i;
		if (d[j] == LOW && a[j] == HIGH)
		{
			return j;
		}
		i++;
	}
	return j;
}

/* Intended for loading switch_data into load_data
 * - ld = load_data
 * - sd = switch_data
 * - off_only (boolean) = flag to limit sharing of infomation where
 * 	 the switch can only disconnect load during load management
 */
void set_array_equal(uint* ld, const uint* sd, const uint size, const uint off_only)
{
	if (off_only)
	{
		for (uint i = 0; i <= size; i++)
		{
			if (sd[i] == LOW)
				ld[i] = sd[i];
		}
	} else {
		for (uint i = 0; i <= size; i++)
		{
			ld[i] = sd[i];
		}
	}
}
