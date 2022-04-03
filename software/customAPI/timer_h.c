/*
 * timer_h.c
 *
 *  Created on: 29 Mar 2022
 *      Author: Hao Lin
 */

#include "timer_h.h"

static uint timestamp_start_time 	= 0;
static uint timestamp_end_time 		= 0;
static uint timestamp_overhead_time = 0;
static double timestamp_factor_ms = 0;

// Initiates values for time response timing functions
void response_timer_init()
{
	if (alt_timestamp_start() < 0) {
		printf ("No timestamp device is available.\n");
	}

	uint T1 = alt_timestamp();
	uint T2 = alt_timestamp();
	timestamp_overhead_time = T2 - T1;

	timestamp_factor_ms = 1/(((unsigned int)alt_timestamp_freq()/1e3));
}

// Calculates and returns the measured time in ms
uint calc_response_time()
{
	uint t = (unsigned int)((timestamp_end_time - timestamp_start_time) - timestamp_overhead_time);
	t = (unsigned int) (t*timestamp_factor_ms);
	xQueueSendToBack(Q_response_time, (void*) &t, portMAX_DELAY);
	return t;
}

void response_timer_start()
{
	timestamp_start_time = alt_timestamp();
}

void response_timer_end()
{
	timestamp_end_time = alt_timestamp();
}
