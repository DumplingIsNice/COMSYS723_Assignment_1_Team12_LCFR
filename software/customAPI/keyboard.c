/*
 * keyboard.c
 *
 *  Created on: 25 Mar 2022
 *      Author: Hao Lin
 */

#include "keyboard.h"

// Private global variable
char global_string[GLOBAL_STRING_SIZE] = "";

// ps2 isr to read the immediate input code and signal
// handle_keyboard task
void ps2_isr (void* context, alt_u32 id)
{
	ps2_read((alt_up_ps2_dev*) context);
}

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
void ps2_read(alt_up_ps2_dev* ps2_device)
{
	char ascii;
	int status = FALSE;
	unsigned char key = 0;
	char string[] = "";
	KB_CODE_TYPE decode_mode;

	status =  decode_scancode (ps2_device, &decode_mode , &key , &ascii);

	if (status == FALSE)
	{
		// print out the result
		switch (decode_mode)
		{
		case KB_ASCII_MAKE_CODE :
			translate_make_code(KB_ASCII_MAKE_CODE, key, string);

			printf("ASCII   : %s\n", string);
			printf("Length  : %d\n", strlen(string));
			strcpy(global_string, string);

			printf("Giving from ISR\n");
			xSemaphoreGiveFromISR(ps2_keyboard_sem, portMAX_DELAY);
			printf("Sending queue from ISR\n");
			xQueueSendToBackFromISR(ps2_keyboard_inbox, (void*) &global_string, portMAX_DELAY);
			printf("All done\n");
			break;
		case KB_LONG_BINARY_MAKE_CODE :
			// do nothing
			printf("KB_LONG_BINARY_MAKE_CODE\n");
			break;
		case KB_BINARY_MAKE_CODE :
			printf ( "MAKE CODE : %x\n", key );
			if (key == KEY_ENTER)
			{
				printf("ASCII   : %s\n", string);
				printf("Length  : %d\n", strlen(string));
				strcpy(global_string, STR_ENTER);
				xSemaphoreGiveFromISR(ps2_keyboard_sem, portMAX_DELAY);
				xQueueSendToBackFromISR(ps2_keyboard_inbox, (void*) &global_string, portMAX_DELAY);
			}
			break;
		case KB_BREAK_CODE :
			// do nothing
			printf("KB_BREAK_CODE\n");
			break;
		default :
			printf ("DEFAULT : %x\n", key) ;
			break ;
		}
	}
}

// Inititalise keyboard setup
alt_up_ps2_dev* ps2_keyboard_init()
{
	alt_up_ps2_dev* ps2_device = alt_up_ps2_open_dev(PS2_NAME);

	if(ps2_device == NULL){
		printf("can't find PS/2 device\n");
	}

	alt_up_ps2_clear_fifo(ps2_device);

	// register the PS/2 interrupt
	alt_irq_register(PS2_IRQ, ps2_device, (alt_isr_func) ps2_isr);
	IOWR_8DIRECT(PS2_BASE,4,1);

	return ps2_device;
}

