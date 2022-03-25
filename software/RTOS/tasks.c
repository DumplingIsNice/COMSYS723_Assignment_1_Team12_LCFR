/*
 * tasks.c
 *
 *  Created on: 25 Mar 2022
 *      Author: Hao Lin
 */

#include "tasks.h"

// Creates message queue, semaphore and other inter-task data packages
int init_OS_data_structs(void)
{
	ps2_keyboard_inbox = xQueueCreate(PS2_KEYBOARD_QUEUE_SIZE, sizeof(char*));
	ps2_keyboard_sem = xSemaphoreCreateCounting(9999, 1);
	return 0;
}

// Creates the tasks in the system
int init_create_tasks(void)
{
	/* xTaskCreate(
		* the function used to be task
		* the name of the task, for debugging purpose
		* the size of the stack
		* the parameter passed to the task
		* the priority of the task
		* task handler, which can be used to delete the task);
	*/

	xTaskCreate(
			handle_keyboard,
			"handle_keyboard",
			TASK_STACKSIZE,
			NULL,
			HANDLE_KEYBOARD_PRIORITY,
			NULL);
	return 0;
}

