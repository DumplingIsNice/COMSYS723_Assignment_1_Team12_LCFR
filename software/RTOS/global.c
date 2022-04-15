/*
 * global.c
 *
 *  Created on: 15 Apr 2022
 *      Author: Hao Lin
 */

#include "global.h"

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

void get_global_sys_status(char *local_buf)
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

