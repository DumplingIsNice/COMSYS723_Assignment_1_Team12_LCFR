/*
 * global.c
 *
 *  Created on: 15 Apr 2022
 *      Author: Hao Lin
 */

#include "global.h"

static char global_system_status = STABLE;
static double global_threshold_freq = DEFAULT_FREQ_THRESHOLD;
static double global_threshold_roc = DEFAULT_ROC_THRESHOLD;

void set_global_threshold_freq(double threshold)
{
	global_threshold_freq = (double)threshold;
}
void set_global_threshold_roc(double threshold)
{
	global_threshold_roc = (double)threshold;
}
double get_global_threshold_freq()
{
	return global_threshold_freq;
}
double get_global_threshold_roc()
{
	return global_threshold_roc;
}

void toggle_global_maintainence()
{
	static char last_status = STABLE;

	switch (global_system_status)
	{
	case STABLE:
		last_status = global_system_status;
		global_system_status = MAINTAIN;
		break;
	case UNSTABLE:
		last_status = global_system_status;
		global_system_status = MAINTAIN;
		break;
	case MAINTAIN:
		global_system_status = last_status;
		break;
	default:
		;
	}

}

void set_global_sys_status(char status)
{
	// Needs semaphore
	if (xSemaphoreTake(global_system_status_sem, portMAX_DELAY) == pdTRUE)
	{
		global_system_status = status;
		xSemaphoreGive(global_system_status_sem);
	} else {
		printf("global_system_status_sem Semaphore cannot be taken!\n");
	}
}

void set_global_sys_status_from_ISR(char status)
{
	// Needs semaphore
	if (xSemaphoreTakeFromISR(global_system_status_sem, portMAX_DELAY) == pdTRUE)
	{
		global_system_status = status;
		xSemaphoreGiveFromISR(global_system_status_sem, portMAX_DELAY);
	} else {
		printf("global_system_status_sem Semaphore cannot be taken!\n");
	}
}

char get_global_sys_status()
{
	return global_system_status;
}

void get_string_global_sys_status(char *local_buf)
{
	switch (global_system_status)
	{
	case STABLE:
		strcpy(local_buf, SYS_STAT_STABLE);
		break;
	case UNSTABLE:
		strcpy(local_buf, SYS_STAT_UNSTABLE);
		break;
	case MAINTAIN:
		strcpy(local_buf, SYS_STAT_MAINTAIN);
		break;
	default:
		;
	}

}

