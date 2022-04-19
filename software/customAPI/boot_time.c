/* boot_time.c
 * returns char days:hours:minutes:seconds 00:00:00 time format, time from the start of the system
 */

#include "boot_time.h"

char* boot_timing(){
	uint millisec;
	static char *system_uptime[10];

	millisec = xTaskGetTickCount();
	sprintf(system_uptime, "%02d:%02d:%02d", (millisec/3600000) % 24, (millisec/60000) % 60, (millisec/1000) % 60);
	return system_uptime;
}
