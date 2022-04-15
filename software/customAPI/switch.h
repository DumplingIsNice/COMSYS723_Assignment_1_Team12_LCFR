/*
 * switches.h
 *
 *  Created on: 18 Mar 2022
 *      Author: Hao Lin
 */

#ifndef SWITCHES_H_
#define SWITCHES_H_

#include "project.h"

// Switches Binary Positions
#define SWITCH_0	(1 << 0)
#define SWITCH_1	(1 << 1)
#define SWITCH_2 	(1 << 2)
#define SWITCH_3 	(1 << 3)
#define SWITCH_4 	(1 << 4)
#define SWITCH_5 	(1 << 5)
#define SWITCH_6 	(1 << 6)
#define SWITCH_7 	(1 << 7)
#define SWITCH_ALL 	0x03fff

// Nios II Switchs API Macros
#define GET_SWITCHES IORD_ALTERA_AVALON_PIO_DATA(SLIDE_SWITCH_BASE)

// Handles the switching between two modes based on the switch position
	/*
	 * Inputs:
	 * load data package
	 */
void handle_switches();

// Checks the status of a switch at position and return status
	/* Inputs:
	 * Switch position macro
	 *
	 * Returns: switch status
	 * 2 - Unidentified
	 * 1 - HIGH
	 * 0 - LOW
	 */
uint check_switch(uint switch_pos);

#endif /* SWITCHES_H_ */
