/* boot_time.h
 * returns  days:hours:minutes:seconds 00:00:00.0 time format, time from the start of the system
 */

#include <stdio.h>
#include "system.h"
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "FreeRTOS/timers.h"

unsigned int millisec;
char *system_uptime[10];
char* boot_timing(); 