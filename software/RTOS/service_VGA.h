/*
 * service_VGA.h
 *
 *  Created on: 30 Mar 2022
 *      Author: Hao Lin
 */

#ifndef RTOS_SERVICE_VGA_H_
#define RTOS_SERVICE_VGA_H_

#include "..\customAPI\project.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"

/* Graphing Parameters & Structs */

#define MIN_FREQ 45

#define FREQPLT_ORI_X 		101		//x axis pixel position at the plot origin
#define FREQPLT_GRID_SIZE_X 5		//pixel separation in the x axis between two data points
#define FREQPLT_ORI_Y 		359.0	//y axis pixel position at the plot origin
#define FREQPLT_FREQ_RES 	20.0	//number of pixels per Hz (y axis scale)

#define ROCPLT_ORI_X 		101
#define ROCPLT_GRID_SIZE_X 	5
#define ROCPLT_ORI_Y 		420 	//Center of plot (0 at center) instead of left bottom
#define ROCPLT_ROC_RES 		0.5		//number of pixels per Hz/s (y axis scale)

typedef struct{
	unsigned int x1;
	unsigned int y1;
	unsigned int x2;
	unsigned int y2;
}Line;

/* Data Sizes */

// Inter-task queues
#define FREQ_RESPONSE_QUEUE_SIZE 	1000
#define FREQ_DATA_QUEUE_SIZE		1000
#define ROC_DATA_QUEUE_SIZE 		1000

// Local buffers
#define RESPONSE_TIME_BUF_SIZE		100
#define FREQ_DATA_BUF_SIZE			100
#define ROC_DATA_BUF_SIZE			100

// Mux Identifiers
#define EMPTY_RESPONSE 	'P'
#define EMPTY_FREQ 	 	'F'
#define EMPTY_ROC 	 	'R'

// Definition of Semaphores
SemaphoreHandle_t response_time_sem;
SemaphoreHandle_t response_timer_end_gate_sem;
SemaphoreHandle_t response_timer_start_gate_sem;

// QueueHandles
QueueHandle_t Q_response_time;

void service_VGA();

// Empties either freq or roc queue
/* Inputs:
 * - freq or roc identifier - see header file define
 * - local buffer array
 * - iterator for the buffer
 */
void empty_queue(char mux, double* local_vals, uint* n);
// Same as empty queue, but fixed for response time
void empty_response_queue(uint local_vals[]);

// Calculates highlight response values to be displayed from local buffer
void calc_response_values(const uint response_time_vals[], uint* max, uint* min, uint* avg);

#endif /* RTOS_SERVICE_VGA_H_ */
