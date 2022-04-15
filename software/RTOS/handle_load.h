/*
 * handle_load_auto.h
 *
 *  Created on: 15 Apr 2022
 *      Author: Hao Lin
 */

#ifndef RTOS_HANDLE_LOAD_H_
#define RTOS_HANDLE_LOAD_H_

#include "..\customAPI\project.h"

#define NO_OF_LOADS 5

// Task to handle all automatic load shedding
void handle_load_auto();

/* Self-contained logic of load shedding
 * - Controlled/depends on system state
 * - Performs only one round of load shedding per call
 *   (shed up to one load per call)
 */

// Note: Might need to implement two different load shedding function...
// One geen, one red... Modify helper function instead?
void shed_load();

void update_load_indication(uint d[], uint size);
void update_switch_data(uint d[], uint size);

/* Helper Functions */

uint get_next_load_pos(const uint d[], uint size);

#endif /* RTOS_HANDLE_LOAD_H_ */
