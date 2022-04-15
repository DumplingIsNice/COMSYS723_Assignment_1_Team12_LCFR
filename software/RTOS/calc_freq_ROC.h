/*
 * calc_freq_ROC.h
 *
 *  Created on: 28 Mar 2022
 *      Author: Hao Lin
 */

#ifndef RTOS_CALC_FREQ_ROC_H_
#define RTOS_CALC_FREQ_ROC_H_

#include "..\customAPI\project.h"

#define SAMPLING_FREQ 				16000
#define FREQ_ADC_QUEUE_SIZE 		1000

SemaphoreHandle_t freq_ADC_received_semaphore;
QueueHandle_t Q_ADC_sample_values;

SemaphoreHandle_t roc_queue_sem;
QueueHandle_t Q_roc_calc_values;

SemaphoreHandle_t freq_queue_sem;
QueueHandle_t Q_freq_calc_values;

void freq_relay();

void init_freq_analyser();

void calc_freq_ROC();

#endif /* RTOS_CALC_FREQ_ROC_H_ */
