/*
 * handle_load.h
 *
 *  Created on: 15 Apr 2022
 *      Author: Hao Lin
 */

#ifndef RTOS_HANDLE_LOAD_H_
#define RTOS_HANDLE_LOAD_H_

#include "..\customAPI\project.h"

#define NO_OF_LOADS 5

void handle_load();

void shed_load();

void update_load_indication(uint d[], uint size);
void update_switch_data(uint d[], uint size);

/* Helper Functions */

uint get_next_load_pos(const uint d[], uint size);

#endif /* RTOS_HANDLE_LOAD_H_ */
