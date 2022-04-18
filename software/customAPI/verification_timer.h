/*
 * verification_timer.h
 *
 *  Created on: 15 Apr 2022
 *      Author: Hao Lin
 */

#ifndef CUSTOMAPI_VERIFICATION_TIMER_H_
#define CUSTOMAPI_VERIFICATION_TIMER_H_

#include "project.h"

//verification timer interval in ms
#define VERIFICATION_TIMER_PERIOD 500

uint is_verification_elapsed();
void set_verification_flag(uint s);

//set the global variable flag
void verification_timer_callback(TimerHandle_t xTimer);

void verification_timer_start();
void verification_timer_stop();
void verification_timer_reset();
void verification_timer_init();

#endif /* CUSTOMAPI_VERIFICATION_TIMER_H_ */
