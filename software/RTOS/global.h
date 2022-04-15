/*
 * global.h
 *
 *  Created on: 15 Apr 2022
 *      Author: Hao Lin
 */

#ifndef RTOS_GLOBAL_H_
#define RTOS_GLOBAL_H_

#include "..\customAPI\project.h"

/* Global Variables */

/* This file host all global variables used across the project
 * and their parameters
 */

// System Status Mux
#define STABLE 		'S'
#define UNSTABLE 	'U'
#define MAINTAIN 	'M'

// System Status String
#define SYS_STAT_STABLE 	"Stable"
#define SYS_STAT_UNSTABLE 	"Unstable"
#define SYS_STAT_MAINTAIN 	"Maintenance"

#define SYS_STATUS_SIZE 20

static char global_system_status = STABLE;
static double global_threshold_freq = 0;
static double global_threshold_roc = 0;

/* Function Declaration */

// Getter and Setters

void set_global_threshold_freq(double threshold);
void set_global_threshold_roc(double threshold);
double get_global_threshold_freq();
double get_global_threshold_roc();

void toggle_global_maintainence();
void set_global_sys_status(char status);
void get_global_sys_status(char *local_buf);

#endif /* RTOS_GLOBAL_H_ */
