/*
 * timer_h.h
 *
 *  Created on: 29 Mar 2022
 *      Author: Hao Lin
 */

#ifndef CUSTOMAPI_TIMER_H_H_
#define CUSTOMAPI_TIMER_H_H_

#include "project.h"
#include "sys/alt_timestamp.h"

/* Hardware Timer Functions */

// Calculates and returns the measured time in ms
void check_timer_h();

// Initiates values for time response timing functions
uint calc_response_time();

void response_timer_start();
void response_timer_end();

#endif /* CUSTOMAPI_TIMER_H_H_ */
