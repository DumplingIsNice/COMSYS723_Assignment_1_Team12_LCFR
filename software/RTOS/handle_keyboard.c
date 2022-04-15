/*
 * handle_keyboard.c
 *
 *  Created on: 25 Mar 2022
 *      Author: Hao Lin
 */

#include "handle_keyboard.h"

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
uint handle_threshold_string_input(const char *string, char threshold_type)
{
	uint status = FALSE;
	char* pEnd;
	double threshold = strtod(string, &pEnd);
	printf("Threshold: %f\n", threshold);
	if (threshold)
		{
			switch(threshold_type)
			{
			case FREQ_COMMAND:
				// Mutex, perform threshold setting
				printf("Freq Threshold: %f\n", threshold);
				set_global_threshold_freq(threshold);
				status = TRUE;
				break;
			case RATE_COMMAND:
				// Mutex, perform threshold setting
				printf("Rate Threshold: %f\n", threshold);
				set_global_threshold_roc(threshold);
				status = TRUE;
				break;
			default:
				printf("NO THRESHOLD SET!!!");
				return 0;
				break;
			}
		} else {
			// Raise some warning!!
			// ENABLE Flash LED TASK?
			printf("INVALID THRESHOLD INPUT!!!\n");
		}
	return status;
}

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
 * - If invalid: resets keyboard handling to default mode
*/
void handle_keyboard()
{
	uint status = FALSE;
	char received_string[] = "";

	static char logged_string[GLOBAL_STRING_SIZE] = "";
	static char mode = NO_COMMAND;

	while(TRUE)
	{
		printf("Keyboard running\n");
		status = xSemaphoreTake(ps2_keyboard_sem, portMAX_DELAY);
		printf("Semaphore taken\n");
		if (status == pdPASS)
		{
			xQueueReceive(ps2_keyboard_inbox, received_string, portMAX_DELAY);
			printf("Received %s\n", received_string);
			printf("Handling\n");

			// When enter is pressed:
				// if string is one of the recognised commands:
					// activate the mode to receive
				// if not:
					// erase log
			if (!strcmp(received_string, STR_ENTER))
			{
				switch (mode)
				{
				case NO_COMMAND:
					mode = check_command(logged_string);
					strcpy(logged_string, "");
					break;
				case FREQ_COMMAND:
					mode = NO_COMMAND;
					handle_threshold_string_input(logged_string, FREQ_COMMAND);
					strcpy(logged_string, "");
					led_write(LED_GREEN, LED_0, LOW);
					break;
				case RATE_COMMAND:
					mode = NO_COMMAND;
					handle_threshold_string_input(logged_string, RATE_COMMAND);
					strcpy(logged_string, "");
					led_write(LED_GREEN, LED_1, LOW);
					break;
				default:
					strcat(logged_string, received_string);
					printf("default logged_string %s\n", logged_string);
					break;
				}
			// Otherwise, always log input
			} else {
				strcat(logged_string, received_string);
				printf("logged_string %s\n", logged_string);
			}

			printf("mode is %c\n", mode);

		} else if (status == pdFAIL) {
			printf("FAILED TO HANDLE!!!");
		}
		status = FALSE;
	}
}

/* Helper Functions */

// Checks string input against a established command
// (defined in handle_keyboard.h)
// And returns the commanded keyboard handling mode
char check_command(char *string)
{
	char mode = NO_COMMAND;
	printf("Received command is: %s\n", string);
	if (!strcmpi(string, FREQ_STRING))
	{
		mode = FREQ_COMMAND;
		led_write(LED_GREEN, LED_0, HIGH);
	} else if (!strcmpi(string, RATE_STRING))
	{
		mode = RATE_COMMAND;
		led_write(LED_GREEN, LED_1, HIGH);
	}
	return mode;
}

// Checks if a string contains ONLY numerics
uint is_numbers_only(const char *string)
{
	while (*string) {
		if (isdigit(*string++) == 0)
			return 0;
	}
	return 1;
}

// Decodes keypad input
void decode_KP()
{
	;
}

void print_to_LCD()
{
	;
}


