/*
 * calc_freq_ROC.h
 *
 *  Created on: 28 Mar 2022
 *      Author: Hao Lin
 */

#ifndef RTOS_CALC_FREQ_ROC_H_
#define RTOS_CALC_FREQ_ROC_H_

#include "..\customAPI\project.h"
#include "math.h"

#define SAMPLING_FREQ 				16000
#define FREQ_ADC_QUEUE_SIZE 		1000

SemaphoreHandle_t freq_ADC_received_semaphore;
QueueHandle_t Q_ADC_sample_values;

SemaphoreHandle_t roc_queue_sem;
QueueHandle_t Q_roc_calc_values;

SemaphoreHandle_t freq_queue_sem;
QueueHandle_t Q_freq_calc_values;

SemaphoreHandle_t freq_threshold_sem;
SemaphoreHandle_t roc_threshold_sem;

/* Calculation Functions */

// ISR function to receive ACD sample value
void freq_relay();

void init_freq_analyser();

void calc_freq_ROC();

/* Threshold Functions */

/* Checks the most recent values of freq and roc against threshold
 * Then updates threshold exceeded flag.
 * Note: This is also the top-most level (gatekeeping) of unstable
 * loop of control flow.
 *
 * Gated By the Following Conditions:
 *
 * - is_verification_elapsed()
 * - global variable of system status (global_sys_status)
 * - is_threshold_exceeded()
 */
void threshold_monitor(const double current_freq, const double current_roc);

#endif /* RTOS_CALC_FREQ_ROC_H_ */
