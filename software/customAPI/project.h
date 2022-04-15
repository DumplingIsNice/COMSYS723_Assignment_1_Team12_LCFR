/*
 * project.h
 *
 *  Created on: 18 Mar 2022
 *      Author: Hao Lin
 */

// Contains Common Declarations
#ifndef PROJECT_H_
#define PROJECT_H_

// Standard includes
#include <stddef.h>
// Standard I/O
#include <stdio.h>
// Standard Library Functions
#include <stdlib.h>
// String Functions
#include <string.h>
// Character Functions
#include <ctype.h>


// CPU Generated Symbolic Names
#include <system.h>
// Interrupt library
#include <sys/alt_irq.h>
// PIO register writing and reading operation
#include <altera_avalon_pio_regs.h>
// Altera system defined types
#include <alt_types.h>

// ps/2 Libaries
#include <altera_up_avalon_ps2.h>
#include <altera_up_ps2_keyboard.h>

#include <io.h>

// Delay functions
// #include <unistd.h>

// Timer
#include <sys/alt_alarm.h>

// Custom API Libraries
#include "led.h"
#include "switch.h"
#include "buttons.h"
#include "keyboard.h"
#include "timer_h.h"

#include "../RTOS/global.h"

// Scheduler includes
#include "../freertos/FreeRTOS.h"
#include "../freertos/task.h"
#include "../freertos/queue.h"
#include "../freertos/semphr.h"

#include "../RTOS/tasks.h"
#include "../RTOS/handle_keyboard.h"
#include "../RTOS/calc_freq_ROC.h"
#include "../RTOS/service_VGA.h"
#include "../RTOS/handle_load.h"

// Common macros
#define HIGH 	1
#define LOW 	0
#define TRUE	1
#define FALSE	0

#define RESET   0

#define DEBUG

// Mock Debugging Defined
// Enable, disable dummy response timing functionality
#define MOCK_RESPONSE

// Prints calculated values in calc_freq_roc
//#define PRINT_CALC_VAL

#endif /* PROJECT_H_ */
