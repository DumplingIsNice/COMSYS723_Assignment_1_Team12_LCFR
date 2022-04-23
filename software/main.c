#include "customAPI/project.h"

int main()
{
	led_init();
	buttons_init();

	verification_timer_init();

	response_timer_init();
	ps2_keyboard_init();
	init_freq_analyser();

	init_OS_data_structs();
	init_create_tasks();
	vTaskStartScheduler();
	for (;;);
	return 0;
}
