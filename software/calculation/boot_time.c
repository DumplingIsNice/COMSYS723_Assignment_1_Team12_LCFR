/* boot_time.c
 * returns char days:hours:minutes:seconds 00:00:00.0 time format, time from the start of the system
 */

#include "boot_time.h"

unsigned int millisec;
char* system_uptime[10];
char* boot_timing(); 

char* boot_timing(){
  millisec = xTaskGetTickCount();
	sprintf(system_uptime, "%02d:%02d:%02d.%1d", (millisec/3600000) % 24, (millisec/60000) % 60, (millisec/1000) % 60, (millisec/100) % 10);
  return system_uptime;
}

int main(){
	while(1){
        printf("%s\n", boot_timing());   //print the system up time for testing
	}
  return 0;
}
