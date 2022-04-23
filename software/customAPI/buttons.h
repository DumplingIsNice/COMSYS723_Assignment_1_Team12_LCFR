/*
 * buttons.h
 *
 *  Created on: 18 Mar 2022
 *      Author: Hao Lin
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "project.h"

// Pushbutton Peripheral Values

// Pushbutton Binary Positions
#define BUTTON_1	(1 << 0)
#define BUTTON_2	(1 << 1)
#define BUTTON_3	(1 << 2)
#define BUTTON_ALL 	(1 << 3)

// Nios II Pushbutton API Macros
#define GET_BUTTONS 		IORD_ALTERA_AVALON_PIO_EDGE_CAP(PUSH_BUTTON_BASE)
#define CLR_BUTTONS_EDGE 	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PUSH_BUTTON_BASE, 0x7)
#define EN_BUTTONS_IRQ		IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PUSH_BUTTON_BASE, BUTTON_1|BUTTON_2|BUTTON_3)
#define DIS_BUTTONS_IRQ		IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PUSH_BUTTON_BASE, 0x7)

// Function Declarations
	// ISR Routine
void buttons_interrupt_function(void* context, alt_u32 id);
	// Setup buttons irq
alt_u32* buttons_init();

#endif /* BUTTONS_H_ */



