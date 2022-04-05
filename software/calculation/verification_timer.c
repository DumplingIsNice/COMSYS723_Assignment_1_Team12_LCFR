/* verification_timer function
 * set the global variable verification_timer_elapsed once the 500ms has elapsed
 * after start_verification_timer() is called
 */
#include <stdio.h>
#include "system.h"
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "FreeRTOS/timers.h"

static volatile int verification_timer_elapsed; //global timer elapsed flag
int interval = 500; //verification timer interval in ms
TimerHandle_t timer;

//set the global variable flag
void verification_timer_callback( TimerHandle_t xTimer ){ 
  verification_timer_elapsed = 1;
}

void start_verification_timer(){
  xTimerStart(timer, 0);
}

int main(){ 
    //create a timer for 500ms 
    timer = xTimerCreate("Verification Timer", pdMS_TO_TICKS(interval), pdFALSE, NULL, &verification_timer_callback);   
	while(1){   
        //start the verification timer from inside handle_load task
        start_verification_timer();
	}
  return 0;
}