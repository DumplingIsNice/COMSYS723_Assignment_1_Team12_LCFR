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

	Q_ADC_sample_values = xQueueCreate(FREQ_ADC_QUEUE_SIZE, sizeof(unsigned int));
	freq_ADC_received_semaphore = xSemaphoreCreateCounting(9999, 1);

	Q_response_time = xQueueCreate(FREQ_RESPONSE_QUEUE_SIZE, sizeof(unsigned int));
	response_time_sem = xSemaphoreCreateMutex();
	response_timer_end_gate_sem = xSemaphoreCreateBinary();
	response_timer_start_gate_sem = xSemaphoreCreateBinary();

	Q_roc_calc_values = xQueueCreate(ROC_DATA_QUEUE_SIZE, sizeof(double));
	roc_queue_sem = xSemaphoreCreateMutex();

	Q_freq_calc_values = xQueueCreate(FREQ_DATA_QUEUE_SIZE, sizeof(double));
	freq_queue_sem = xSemaphoreCreateMutex();

	global_system_status_sem = xSemaphoreCreateMutex();

	printf("Data Structs Created!\n");
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

	xTaskCreate(
			calc_freq_ROC,
			"calc_freq_ROC",
			TASK_STACKSIZE,
			NULL,
			HANDLE_KEYBOARD_PRIORITY,
			NULL);

	xTaskCreate(
			service_VGA,
			"service_VGA",
			TASK_STACKSIZE,
			NULL,
			SERVICE_VGA_PRIORITY,
			NULL);

	xTaskCreate(
			handle_load,
			"handle_load_auto",
			TASK_STACKSIZE,
			NULL,
			HANDLE_LOAD_PRIORITY,
			NULL);

	printf("Tasks Created!\n");
	return 0;
}

