/*
 * global.h
 *
 *  Created on: 15 Apr 2022
 *      Author: Hao Lin
 */

#ifndef RTOS_GLOBAL_H_
#define RTOS_GLOBAL_H_

#include "..\customAPI\project.h"

/* This file host all global variables used across the project
 * and their parameters
 */

#define DEFAULT_FREQ_THRESHOLD 51
#define DEFAULT_ROC_THRESHOLD 8

// System Status Mux
#define NORMAL 		'N'
#define STABLE 		'S'
#define UNSTABLE 	'U'
#define MAINTAIN 	'M'

// System Status String

// Monitoring
#define SYS_STAT_NORMAL		"Normal"
// Load managing
#define SYS_STAT_STABLE 	"Stable"
#define SYS_STAT_UNSTABLE 	"Unstable"
#define SYS_STAT_MAINTAIN 	"Maintenance"

#define SYS_STATUS_SIZE 20

// Load Data
#define NO_OF_LOADS 5

/* Global Variables */

static char global_system_status = NORMAL;
static double global_threshold_freq = DEFAULT_FREQ_THRESHOLD;
static double global_threshold_roc = DEFAULT_ROC_THRESHOLD;

// "LED Load" data
static uint load_data[NO_OF_LOADS] = {0};

/* Function Declaration */

// Getter and Setters
uint* get_load_data();

void set_global_threshold_freq(double threshold);
void set_global_threshold_roc(double threshold);
double get_global_threshold_freq();
double get_global_threshold_roc();

void toggle_global_maintainence();
void set_global_sys_status(char status);
void set_global_sys_status_from_ISR(char status);
char get_global_sys_status();
void get_string_global_sys_status(char *local_buf);

#endif /* RTOS_GLOBAL_H_ */
