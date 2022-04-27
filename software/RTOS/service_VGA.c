/*
 * service_VGA.c
 *
 *  Created on: 30 Mar 2022
 *      Author: Hao Lin
 */

#include "service_VGA.h"

void service_VGA()
{
	/* Intialise Buffers */

	alt_up_char_buffer_dev *char_buf;
	alt_up_pixel_buffer_dma_dev *pixel_buf;

	// No need for an init function, just init in the task!
	pixel_buf = alt_up_pixel_buffer_dma_open_dev(VIDEO_PIXEL_BUFFER_DMA_NAME);
	if(pixel_buf == NULL){
		printf("can't find pixel buffer device\n");
	}

	// Reset the display
	alt_up_pixel_buffer_dma_clear_screen(pixel_buf, 0);

	char_buf = alt_up_char_buffer_open_dev("/dev/video_character_buffer_with_dma");
	if(char_buf == NULL){
		printf("can't find char buffer device\n");
	}

	// Reset the display
	alt_up_char_buffer_clear(char_buf);

	printf("VGA Controllers initialised!\n");

	printf("service_VGA Running\n");

	static uint service_VGA_counter = 0;

	// Reused for VGA Display
	char VGA_print_buffer[100] = "";

	// Local Buffers to hold value for display
	double freq_vals[FREQ_DATA_BUF_SIZE] = {0};
	double roc_vals[ROC_DATA_BUF_SIZE] = {0};

	uint n = FREQ_DATA_BUF_SIZE-1; // Iterator
	uint* n_p = &n;
	uint m = ROC_DATA_BUF_SIZE-1; // Iterator
	uint* m_p = &m;

	uint response_time_vals[RESPONSE_TIME_BUF_SIZE] = {0};

	// Local Variables
	char sys_status[SYS_STATUS_SIZE] = SYS_STAT_STABLE;
	double threshold_freq = DEFAULT_FREQ_THRESHOLD;
	double threshold_roc = DEFAULT_ROC_THRESHOLD;

	char* sys_up_time = NULL;

	uint response_max = 0;
	uint response_min = 99999;
	uint response_avg = 0;

	Line line_freq, line_ROC;

	// Display Delay counter
	uint cnt = 0;

	while(TRUE)
	{
		// Multipurpose iterators
		uint i = 0;
		uint j = 0;

		/* Data Reading */

		threshold_freq = get_global_threshold_freq();
		threshold_roc = get_global_threshold_roc();

		get_string_global_sys_status(sys_status);

		sys_up_time = boot_timing();

		// Empties Freq data queue
		if (uxQueueMessagesWaiting(Q_freq_calc_values) != 0)
		{
			if (xSemaphoreTake(freq_queue_sem, portMAX_DELAY) == pdTRUE)
			{
				empty_queue(EMPTY_FREQ, freq_vals, n_p);
				xSemaphoreGive(freq_queue_sem);
			} else {
				printf("freq_queue_sem Semaphore cannot be taken!\n");
			}
		}

		// Empties RoC data queue
		if (uxQueueMessagesWaiting(Q_roc_calc_values) != 0)
		{
			if (xSemaphoreTake(roc_queue_sem, portMAX_DELAY) == pdTRUE)
			{
				empty_queue(EMPTY_ROC, roc_vals, m_p);
				xSemaphoreGive(roc_queue_sem);
			} else {
				printf("roc_queue_sem Semaphore cannot be taken!\n");
			}
		}

		// Empty and calc response time
#ifdef MOCK_RESPONSE
		if (uxQueueMessagesWaiting(Q_response_time) != 0)
		{
			if (xSemaphoreTake(response_time_sem, portMAX_DELAY) == pdTRUE)
			{
				empty_response_queue(response_time_vals);
				xSemaphoreGive(response_time_sem);
			} else {
				printf("response_time_sem Semaphore cannot be taken!\n");
			}
		}

		calc_response_values(response_time_vals, &response_max, &response_min, &response_avg);
#endif
		/* Draws Graph */
		// Fetched and modified from freq_plot_example in example code

		// Clear old graph
		alt_up_pixel_buffer_dma_draw_box(pixel_buf, 101, 210, 600, 359, 0, 0);
		alt_up_pixel_buffer_dma_draw_box(pixel_buf, 101, 380, 600, 459, 0, 0);

		// Freq
		alt_up_pixel_buffer_dma_draw_hline(pixel_buf, 100, 590, 360, ((0x3ff << 20) + (0x3ff << 10) + (0x3ff)), 0);
		alt_up_pixel_buffer_dma_draw_vline(pixel_buf, 100, 210, 360, ((0x3ff << 20) + (0x3ff << 10) + (0x3ff)), 0);

		// ROC
		alt_up_pixel_buffer_dma_draw_hline(pixel_buf, 100, 590, 460, ((0x3ff << 20) + (0x3ff << 10) + (0x3ff)), 0);
		alt_up_pixel_buffer_dma_draw_vline(pixel_buf, 100, 380, 460, ((0x3ff << 20) + (0x3ff << 10) + (0x3ff)), 0);

		alt_up_char_buffer_string(char_buf, "Freq(Hz)", 0, 36);
		i = 22;
		alt_up_char_buffer_string(char_buf, "52", 10, i+=5);
		alt_up_char_buffer_string(char_buf, "50", 10, i+=5);
		alt_up_char_buffer_string(char_buf, "48", 10, i+=5);
		alt_up_char_buffer_string(char_buf, "46", 10, i+=5);

		alt_up_char_buffer_string(char_buf, "df/dt", 0, 51);
		alt_up_char_buffer_string(char_buf, "(Hz/s)", 0, 52);
		alt_up_char_buffer_string(char_buf, "(Abs)", 0, 53);
		i = 46;
		alt_up_char_buffer_string(char_buf, "60", 10, i+=2);
		alt_up_char_buffer_string(char_buf, "30", 10, i+=2);
		alt_up_char_buffer_string(char_buf, "0", 10, i+=2);
		alt_up_char_buffer_string(char_buf, "-30", 9, i+=2);
		alt_up_char_buffer_string(char_buf, "-60", 9, i+=2);

		// Compensation, for that n actually points to the newest data here
		n++;
		for(uint k=0;k<99;++k){ //i here points to the oldest data, j loops through all the data to be drawn on VGA
			if (((int)(freq_vals[(n+k)%100]) > MIN_FREQ) && ((int)(freq_vals[(n+k+1)%100]) > MIN_FREQ))
			{
				//Calculate coordinates of the two data points to draw a line in between
				//Frequency plot
				line_freq.x1 = FREQPLT_ORI_X + FREQPLT_GRID_SIZE_X * k;
				line_freq.y1 = (int)(FREQPLT_ORI_Y - FREQPLT_FREQ_RES * (freq_vals[(n+k)%100] - MIN_FREQ));

				line_freq.x2 = FREQPLT_ORI_X + FREQPLT_GRID_SIZE_X * (k + 1);
				line_freq.y2 = (int)(FREQPLT_ORI_Y - FREQPLT_FREQ_RES * (freq_vals[(n+k+1)%100] - MIN_FREQ));

				//Frequency RoC plot
				line_ROC.x1 = ROCPLT_ORI_X + ROCPLT_GRID_SIZE_X * k;
				line_ROC.y1 = (int)(ROCPLT_ORI_Y - ROCPLT_ROC_RES * roc_vals[(m+k)%100]);

				line_ROC.x2 = ROCPLT_ORI_X + ROCPLT_GRID_SIZE_X * (k + 1);
				line_ROC.y2 = (int)(ROCPLT_ORI_Y - ROCPLT_ROC_RES * roc_vals[(m+k+1)%100]);

				// Draw
				alt_up_pixel_buffer_dma_draw_line(pixel_buf, line_freq.x1, line_freq.y1, line_freq.x2, line_freq.y2, 0x3ff << 0, 0);
				alt_up_pixel_buffer_dma_draw_line(pixel_buf, line_ROC.x1, line_ROC.y1, line_ROC.x2, line_ROC.y2, 0x3ff << 0, 0);
			}
		}
		n--;
		// Compensation, for that n actually points to the newest data here

		/* Display Information */

		// Any ending spaces is to clear end of display or any lingering character

		alt_up_char_buffer_string(char_buf, "Team 12", 0, 1);
		alt_up_char_buffer_string(char_buf, "LCFR", 10, 1);

		i = 2;
		j = 0;

		// Depreciated code
		sprintf(VGA_print_buffer, "service_VGA Running Counter: %d", service_VGA_counter++);
			alt_up_char_buffer_string(char_buf, (char*)VGA_print_buffer, 0, i+=2);

		i += 2;

		alt_up_char_buffer_string(char_buf, "By:", 0, i+=2);
		alt_up_char_buffer_string(char_buf, "Hao Lin", 1, i+=2);
		alt_up_char_buffer_string(char_buf, "Chamith Nanayakkara", 1, i+=2);

		i += 2;

		alt_up_char_buffer_string(char_buf, "System Infomation: ", 0, i+=2);
		sprintf(VGA_print_buffer, "Status: %s     ", sys_status);
			alt_up_char_buffer_string(char_buf, (char*)VGA_print_buffer, 0, i+=2);

		j = i;
		i = 0;

		alt_up_char_buffer_string(char_buf, "Thresholds:", 40, i+=2);
		sprintf(VGA_print_buffer, "Freq (Hz): %.2f    ", threshold_freq);
			alt_up_char_buffer_string(char_buf, (char*)VGA_print_buffer, 42, i+=2);
		sprintf(VGA_print_buffer, " RoC (Hz/s): %.2f    ", threshold_roc);
			alt_up_char_buffer_string(char_buf, (char*)VGA_print_buffer, 42, i+=2);

		alt_up_char_buffer_string(char_buf, "Threshold Exceed Response Time (s):", 40, i+=2);
		sprintf(VGA_print_buffer, "Max: %d    ", response_max);
			alt_up_char_buffer_string(char_buf, (char*)VGA_print_buffer, 42, i+=2);
		sprintf(VGA_print_buffer, "Min: %d    ", response_min);
			alt_up_char_buffer_string(char_buf, (char*)VGA_print_buffer, 42, i+=2);
		sprintf(VGA_print_buffer, "Avg: %d    ", response_avg);
			alt_up_char_buffer_string(char_buf, (char*)VGA_print_buffer, 42, i+=2);

		sprintf(VGA_print_buffer, "Total System Up-time: %s    ", sys_up_time);
			alt_up_char_buffer_string(char_buf, (char*)VGA_print_buffer, 40, i+=2);
		alt_up_char_buffer_string(char_buf, "(days:hours:seconds)", 40, i+=2);

		i = j;
		i += 2;

		// Problem is n & m actually does not index the current value...
		// the iterator is incremented in empy_queue...
		// But no, this is EXACTLY the LATEST VALUES! (per service_VGA update)

		// Handling display iteration when snapshot of five most recent values reaches
		// end of buffer
		uint n_1, n_2, n_3, n_4;
		n_1 = (n-1)>99 ? 99 : n-1;
		n_2 = (n-2)>99 ? 98 : n-2;
		n_3 = (n-3)>99 ? 97 : n-3;
		n_4 = (n-4)>99 ? 96 : n-4;

		uint m_1, m_2, m_3, m_4;
		m_1 = (m-1)>99 ? 99 : m-1;
		m_2 = (m-2)>99 ? 98 : m-2;
		m_3 = (m-3)>99 ? 97 : m-3;
		m_4 = (m-4)>99 ? 96 : m-4;

		if (cnt > DISPLAY_RATE/10)
		{
		// Ending spaces is to clear end of display or any lingering character
		sprintf(VGA_print_buffer, "Five most recent freq values  (Hz): %.2f, %.2f, %.2f, %.2f, %.2f  ",
								 freq_vals[n],
								 freq_vals[n_1],
								 freq_vals[n_2],
								 freq_vals[n_3],
								 freq_vals[n_4]);
		alt_up_char_buffer_string(char_buf, (char*)VGA_print_buffer, 0, i+=2);

		// Ending spaces is to clear end of display or any lingering character
		sprintf(VGA_print_buffer, "Five most recent roc values (Hz/s): %+5.2f, %+5.2f, %+5.2f, %+5.2f, %+5.2f    ",
									 roc_vals[m],
									 roc_vals[m_1],
									 roc_vals[m_2],
									 roc_vals[m_3],
									 roc_vals[m_4]);
		alt_up_char_buffer_string(char_buf, (char*)VGA_print_buffer, 0, i+=2);

		cnt = 0;
		} else {
			cnt++;
		}

		vTaskDelay(10);
	}
}

// Empties either freq or roc queue
/* Inputs:
 * - freq or roc identifier - see header file define
 * - local buffer array
 * - iterator for the buffer
 */
void empty_queue(char mux, double local_vals[], uint* iterator)
{
	double data = 0;
	uint i = 0;
	QueueHandle_t Q;

	switch (mux)
		{
		case EMPTY_FREQ:
			Q = Q_freq_calc_values;
			break;
		case EMPTY_ROC:
			Q = Q_roc_calc_values;
			break;
		default:
			return;
		}

	while(uxQueueMessagesWaiting(Q) != 0)
	{
		xQueueReceive(Q, &data, portMAX_DELAY);
		(*iterator) = ++(*iterator)%100;
		local_vals[(*iterator)] = data;
		i++;
	}
}

// Same as empty queue, but fixed for response time
void empty_response_queue(uint local_vals[])
{
	uint data = 0;
	uint i = 0;
	static uint j = RESPONSE_TIME_BUF_SIZE-1;

	while(uxQueueMessagesWaiting(Q_response_time) != 0)
	{
		xQueueReceive(Q_response_time, &data, portMAX_DELAY);
		j = ++j%RESPONSE_TIME_BUF_SIZE;
		local_vals[j] = data;
		i++;
	}
}

// Calculates highlight response values to be displayed from local buffer
void calc_response_values(const uint response_time_vals[], uint* max, uint* min, uint* avg)
{
	uint sum = 0;
	uint val = 0;
	uint no_items = 1;

	for (uint i = 0; i < RESPONSE_TIME_BUF_SIZE; i++)
	{
		val = response_time_vals[i];
		(*max) = val > (*max) ? val : (*max);
		if (val != 0)
			(*min) = val < (*min) ? val : (*min);

		if (val != 0)
		{
			sum += val;
			no_items++;
		}
	}
	(*avg) = sum/no_items;
}
