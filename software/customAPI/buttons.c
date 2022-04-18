/*
 * buttons.c
 *
 *  Created on: 18 Mar 2022
 *      Author: Hao Lin
 */

#include "buttons.h"

void buttons_interrupt_function(void* context, alt_u32 id)
{
	// need to cast the context first before using it
	int* temp = (int*) context;
	(*temp) = GET_BUTTONS;
	printf("Button is: %d\n", *temp);

	if(*temp == BUTTON_3)
		toggle_global_maintainence();

	if(*temp == BUTTON_2)
		set_global_sys_status_from_ISR(UNSTABLE);

	// clears the edge capture register
	CLR_BUTTONS_EDGE;
}

alt_u32* buttons_init()
{
	static alt_u32 flag_btn = 0;
	void* p_flag_btn = (void*) &flag_btn;

	// clears the edge capture register
	CLR_BUTTONS_EDGE;

	// enable interrupts for buttons
	EN_BUTTONS_IRQ;

	// register the ISR, CONTEXT IS THE PASSED POINTER ARGUMENT
	alt_irq_register(PUSH_BUTTON_IRQ, (void*)p_flag_btn, buttons_interrupt_function);

	return p_flag_btn;
}

void handle_buttons(void* p_flag_btn)
{
	alt_u32* flag_btn = (alt_u32*) p_flag_btn;

	// Flag_pro handles an console glitch
	// From printing too fast?
	static uint flag_proc = 0;
	if(flag_proc)
	{
		*flag_btn = 0;
		flag_proc = 0;
	}

	if(*flag_btn)
	{
		CLEAR_LED_GREEN;

		switch (*flag_btn)
			{
				case BUTTON_3:
					led_write(LED_GREEN, LED_6, HIGH);
					break;
				case BUTTON_2:
					led_write(LED_GREEN, LED_4, HIGH);
					break;
				case BUTTON_1:
					led_write(LED_GREEN, LED_2, HIGH);
					break;
				default:
					;
					break;
			}

		flag_proc = 1;
		*flag_btn = 0;
	}
	return;
}




