/*
 * tasks.h
 *
 *  Created on: 25 Mar 2022
 *      Author: Hao Lin
 */

#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#include "..\customAPI\project.h"

// Definition of Task Stacks
#define TASK_STACKSIZE       2048

// Definition of Task Priorities
#define HANDLE_KEYBOARD_PRIORITY 14

// Definition of Periods
#define HANDLE_KEYBOARD_DELAY 50

// Used to delete a task
TaskHandle_t xHandle;

// Global variables

/* Task Function Declarations */

// Creates message queue, semaphore and other inter-task data packages
int init_OS_data_structs();

// Creates the tasks in the system
int init_create_tasks();

#endif /* RTOS_TASKS_H_ */
