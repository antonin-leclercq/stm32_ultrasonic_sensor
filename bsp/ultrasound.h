/*
 * ultrasound.h
 *
 *  Created on: 24 juil. 2022
 *      Author: LECLERCQ Antonin
 */

#ifndef BSP_INC_ULTRASOUND_H_
#define BSP_INC_ULTRASOUND_H_

#include <stm32f0xx.h>

void Timer_one_pulse_mode_init(void);
void Timer_input_capture_init(void);

void NVIC_init(void);

#define T_DELAY 1000
#define T_PULSE (T_DELAY + 200 -1)

#endif /* BSP_INC_ULTRASOUND_H_ */
