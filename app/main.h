/*
 * main.h
 *
 *  Created on: Jul 24, 2022
 *      Author: LECLERCQ Antonin
 */

#ifndef APP_INC_MAIN_H_
#define APP_INC_MAIN_H_

#include <stm32f0xx.h>
#include <ultrasound.h>
#include <usart.h>

// Change this value according to your room temperature
#define SOUND_SPEED 0.340f

void GPIO_status_init(void);

// extern function from "printf-stdarg.c"
extern int m_printf(const char *format, ...);

#endif /* APP_INC_MAIN_H_ */
