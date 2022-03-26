/*
 * keyboard.h
 *
 *  Created on: 25 Mar 2022
 *      Author: Hao Lin
 */

#ifndef CUSTOMAPI_KEYBOARD_H_
#define CUSTOMAPI_KEYBOARD_H_

#include "project.h"

#define KEY_ENTER 	0x5a
#define STR_ENTER   "ENT"

// ps2 isr to read the immediate input code and signal
// handle_keyboard task
void ps2_isr (void* context, alt_u32 id);

/* Input:
 * LED_0 will light up indicating that command has been received.
 * - freq
 * - rate
 * After command is received, input the value of threshold.
 *
 * handle_keyboard has three modes:
 * - no command
 * - freq
 * - rate
 * After entering a mode, handle_keyboard will expect the threshold value (check validity)
*/
void ps2_read(alt_up_ps2_dev* ps2_device);

// Inititalise keyboard setup
alt_up_ps2_dev* ps2_keyboard_init();

#endif /* CUSTOMAPI_KEYBOARD_H_ */
