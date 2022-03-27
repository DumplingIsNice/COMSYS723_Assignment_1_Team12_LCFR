// Retrieve the ADC sample values and calculate the Frequency and Frequency ROC
// NOT tested!!

#include <stdio.h>
#include <unistd.h>

#include "system.h"
#include "sys/alt_irq.h"
#include "io.h"
#include "altera_avalon_pio_regs.h"
#define SAMPLING_FREQ 16000

static QueueHandle_t Q_ADC_sample_values;

void freq_relay(){
	unsigned int temp = IORD(FREQUENCY_ANALYSER_BASE, 0);
    xQueueSendToBackFromISR(Q_ADC_sample_values, &temp, pdFALSE);
    xSemaphoreGiveFromISR(freq_ADC_received_semaphore, pdFALSE); // call the semaphore
	return;
}

int main(){
	// initialize the frequency relay IRQ
	alt_irq_register(FREQUENCY_ANALYSER_IRQ, 0, freq_relay);
	// create the queue
	Q_ADC_sample_values = xQueueCreate(10, sizeof(unsigned int));
	
	double new_freq_value;
	double old_freq_value;
	
	double old_freq_ADC_samples;
	double new_freq_ADC_samples;
	
	double freq_ROC_value;
	
	while(1){
		
		//---- This has to go underneath the freq_calculation task -----------//
		
		// check for the freq_ADC_received_semaphore
        xSemaphoreTake(freq_ADC_received_semaphore, portMAX_DELAY);

		// store the old freq value
		old_freq_value = new_freq_value;

		//store the old freq ADC samples value
		old_freq_ADC_samples= new_freq_ADC_samples;

        // Retrieve the new ADC sample value
        xQueueReceive(Q_ADC_sample_values, &new_freq_ADC_samples, 0);

        // calculate the new freq value
        new_freq_value = SAMPLING_FREQ / new_freq_ADC_samples;

        // calculate the new frequency ROC value
        freq_ROC_value = ((new_freq_value - old_freq_value) * SAMPLING_FREQ) / (new_freq_ADC_samples + old_freq_ADC_samples );

         
	}

  return 0;
}
