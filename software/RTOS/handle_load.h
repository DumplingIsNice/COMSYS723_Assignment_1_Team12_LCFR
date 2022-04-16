/*
 * handle_load_auto.h
 *
 *  Created on: 15 Apr 2022
 *      Author: Hao Lin
 */

#ifndef RTOS_HANDLE_LOAD_H_
#define RTOS_HANDLE_LOAD_H_

#include "..\customAPI\project.h"

/* Parameters */

// Load Data
#define NO_OF_LOADS 5

/* Primary Functions */

// Task to handle all automatic load shedding
void handle_load_auto();

/* Self-contained logic of load shedding
 * - Controlled/depends on system state
 * - Performs only one round of load shedding per call
 *   (shed up to one load per call)
 */
void shed_load(uint d[], uint a[], uint size);

/* Self-contained logic of load reconnection
 * Controlled/depends on relationship of
 * - load_data
 * - switch_data
 * - auto_data
 * (see handle_load.c -> handle_load())
 */
void connect_load(uint d[], uint a[], const uint s[], const uint size);

/* Helper Functions */

// Clears all invalid lingering green LED indications
	// of loads that HAD been managed by the relay BUT
	// was switched off manually (despite being off) BEFORE
	// the relay is able to automatically reconnect the load
void reconnect_indication_clearing(const uint load_index, const uint size);

// Updates normal load indication (red LED)
void update_load_indication(uint d[], uint size);
// Updates array with switches input
void update_switch_data(uint d[], uint size);

// Calculates and returns the next load position to be shed from
// load data (aka get next highest priority that is loaded)
int8_t get_next_load_pos(const uint d[], const uint size);

// Calculates and returns the last load position to be shed from
// load data (aka get next lowest priority that is loaded)
int8_t get_last_load_pos(const uint d[], const uint a[], const uint s[], const uint size);

/* Intended for loading switch_data into load_data
 * - ld = load_data
 * - sd = switch_data
 * - off_only (boolean) = flag to limit sharing of infomation where
 * 	 the switch can only disconnect load during load management
 */
void set_array_equal(uint* ld, const uint* sd, const uint size, const uint off_only);
#endif /* RTOS_HANDLE_LOAD_H_ */
