/*
 * calc_freq_ROC.c
 *
 *  Created on: 28 Mar 2022
 *      Author: Hao Lin
 */

#include "calc_freq_ROC.h"

void freq_relay(){
	uint temp = IORD(FREQUENCY_ANALYSER_BASE, 0);
//	printf("%f Hz\n", 16000/(double)temp);
    xQueueSendToBackFromISR(Q_ADC_sample_values, (void*) &temp, portMAX_DELAY);
    xSemaphoreGiveFromISR(freq_ADC_received_semaphore, portMAX_DELAY); // call the semaphore
}

void init_freq_analyser()
{
	// initialize the frequency analyser IRQ
	alt_irq_register(FREQUENCY_ANALYSER_IRQ, 0, (alt_isr_func) freq_relay);
	printf("init_freq_analyser Completed\n");

}

void calc_freq_ROC()
{
	printf("calc_freq_ROC Running\n");

	double new_freq_value = 0;
	double old_freq_value = 0;

	uint old_freq_ADC_samples = 0;
	uint new_freq_ADC_samples = 0;

	double freq_ROC_value = 0;

	uint flag_first_run = FALSE;

	while(TRUE){

		//---- This has to go underneath the freq_calculation task -----------//

		// check for the freq_ADC_received_semaphore
        xSemaphoreTake(freq_ADC_received_semaphore, portMAX_DELAY);

		// store the old freq value
		old_freq_value = new_freq_value;

		//store the old freq ADC samples value
		old_freq_ADC_samples = new_freq_ADC_samples;

        // Retrieve the new ADC sample value
        xQueueReceive(Q_ADC_sample_values, &new_freq_ADC_samples, 0);

        // calculate the new freq value
        new_freq_value = SAMPLING_FREQ / (double) new_freq_ADC_samples;

        if (flag_first_run)
        {
			// calculate the new frequency ROC value
        	printf("Calc ROC:\n");
        	printf("new_freq_value: %f\n", new_freq_value);
        	printf("old_freq_value: %f\n", old_freq_value);
        	printf("new_freq_ADC_samples: %d\n", new_freq_ADC_samples);
        	printf("old_freq_ADC_samples: %d\n", old_freq_ADC_samples);

			freq_ROC_value = abs((new_freq_value - old_freq_value) * SAMPLING_FREQ) / (new_freq_ADC_samples + old_freq_ADC_samples );
        }

        printf("Report:\n");
        printf("new_freq_value: %f\nfreq_ROC_value: %f\n", new_freq_value, freq_ROC_value);
        printf("#########################\n");
        flag_first_run = TRUE;
	}
}
