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
// String Functions
#include <string.h>

// CPU Generated Symbolic Names
#include <system.h>
// Interrupt library
#include <sys/alt_irq.h>
// PIO register writing and reading operation
#include <altera_avalon_pio_regs.h>

// Delay functions
// #include <unistd.h>

// Timer
#include <sys/alt_alarm.h>

// Scheduler includes
//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
//#include "freertos/queue.h"
//#include "freertos/semphr.h"

// Custom API Libraries
#include "led.h"
#include "switch.h"

// Common macros
#define HIGH 	1
#define LOW 	0
#define TRUE	1
#define FALSE	0

#define RESET 0

#endif /* PROJECT_H_ */
