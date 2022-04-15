/* verification_timer function

 * set the global variable verification_timer_elapsed once the 500ms has elapsed
 * after start_verification_timer() is called
 */
#include "verification_timer.h"

//global timer elapsed flag
static volatile uint verification_timer_elapsed = TRUE;

static TimerHandle_t ver_timer;

uint is_verification_elapsed()
{
	return verification_timer_elapsed;
}

void set_verification_flag(uint s)
{
	verification_timer_elapsed = s;
}

//set the global variable flag
void verification_timer_callback(TimerHandle_t xTimer)
{
	verification_timer_elapsed = TRUE;
	verification_timer_stop();
}

void verification_timer_start()
{
	verification_timer_elapsed = FALSE;
	xTimerStart(ver_timer, 0);
}

void verification_timer_stop()
{
	xTimerStop(ver_timer, 0);
}

void verification_timer_reset()
{
	xTimerReset(ver_timer, 0);
}

void verification_timer_init()
{
	ver_timer = xTimerCreate(
					"Verification Timer",
					pdMS_TO_TICKS(VERIFICATION_TIMER_PERIOD),
					pdFALSE,
					NULL,
					&verification_timer_callback);
}
