/* boot_time.c
 * returns char days:hours:minutes:seconds 00:00:00.0 time format, time from the start of the system
 */

#include "boot_time.h"

int main(){
	while(1){
        printf("%s\n", boot_timing());   //print the system up time for testing
	}
  return 0;
}
