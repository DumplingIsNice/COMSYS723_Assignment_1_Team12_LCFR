/*
 * handle_keyboard.h
 *
 *  Created on: 25 Mar 2022
 *      Author: Hao Lin
 */

#ifndef RTOS_HANDLE_KEYBOARD_H_
#define RTOS_HANDLE_KEYBOARD_H_

#include "..\customAPI\project.h"

// Definitions for handle_keyboard
#define NO_COMMAND		'N'
#define FREQ_COMMAND	'F'
#define RATE_COMMAND	'R'

#define FREQ_STRING		"FREQ"
#define RATE_STRING		"RATE"

// Definition of Message Queues
#define PS2_KEYBOARD_QUEUE_SIZE  30

// Definition of Semaphores
SemaphoreHandle_t ps2_keyboard_sem;

// QueueHandles
QueueHandle_t ps2_keyboard_inbox;

// Under one of the established commanded modes (defined in handle_keyboard.h)
// A threshold value is expected.
// If valid, accesse threshold resource and updates it with the new value
// Otherwise, the input is invalid and ignored (with a warning)
/* Input:
 * - string that represents input
 * - the threshold type to write to
 * Outputs:
 * - sucess status
 */
uint handle_threshold_string_input(const char *string, char threshold_type);

// Task which handles the input value from ps/2 keyboard
/* Commands:
 * - freq (LED_0)
 * - rate (LED_1)
 * LED_0 & 1 will light up indicating that command has been received.
 *
 * handle_keyboard has three modes:
 * - NO_COMMAND
 * - FREQ_COMMAND
 * - RATE_COMMAND
 *
 * After command is received, task enters respective mode
 * expecting the value of threshold as input:
 *
 * - If valid: threshold is set
 * - If invalid: restes keyboard handling to default mode
*/
void handle_keyboard();

/* Helper Functions */

// Checks string input against a established command
// (defined in handle_keyboard.h)
// And returns the commanded keyboard handling mode
char check_command(char *string);

// Checks if a string contains ONLY numerics
uint is_numbers_only(const char *string);

// Decodes keypad input
void decode_KP();

#endif /* RTOS_HANDLE_KEYBOARD_H_ */
