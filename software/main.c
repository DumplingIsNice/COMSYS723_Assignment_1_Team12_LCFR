#include "customAPI/project.h"

//int main()
//{
//  printf("Hello from Nios II!\n");
//  led_init();
////  led_write(LED_RED, LED_0, HIGH);
////  led_write(LED_RED, LED_4, HIGH);
////  led_write(LED_GREEN, LED_6, HIGH);
////  led_write(LED_RED, LED_4, LOW);
//
//  buttons_init();
//  printf("Buttons initiated!\n");
//
//  alt_u32* p_buttons = NULL;
//  p_buttons = buttons_init();
//
//  alt_up_ps2_dev* ps2_device = ps2_keyboard_init();
//  printf("Keyboard initiated!\n");
//
//  while(1)
//  {
//	  handle_buttons((void*)p_buttons);
//	  check_switch(SWITCH_1);
//	  check_switch(SWITCH_2);
//	  check_switch(SWITCH_5);
//	  check_switch(SWITCH_0);
//  }
//  return 0;
//}

int main()
{
	buttons_init();

	response_timer_init();
	ps2_keyboard_init();
	init_freq_analyser();

	init_OS_data_structs();
	init_create_tasks();
	vTaskStartScheduler();
	for (;;);
	return 0;
}
