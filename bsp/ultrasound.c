/*
 * ultrasound.c
 *
 *  Created on: 24 juil. 2022
 *      Author: LECLERCQ Antonin
 */


#include <ultrasound.h>

void Timer_one_pulse_mode_init(void)
{
	/*
	 * One pulse mode on TIM3 CH1 --> pin PB4 AF1
	 * APB2 Timers running at 48MHz --> PSC = 47 for 1MHz counting frequency
	 */

	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	// Set to Alternate function mode
	GPIOB->MODER &= ~GPIO_MODER_MODER4_Msk;
	GPIOB->MODER |= (0x02 << GPIO_MODER_MODER4_Pos);

	// Set to AF1
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFRL4_Msk;
	GPIOB->AFR[0] |= (0x01 << GPIO_AFRL_AFRL4_Pos);

	// Enable TIM3 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	// Reset TIM3 configuration
	TIM3->CR1 = 0x0000;
	TIM3->CR2 = 0x0000;

	// Enable auto reload preload
	TIM3->CR1 |= TIM_CR1_ARPE;

	// Enable OnePulse Mode
	TIM3->CR1 |= TIM_CR1_OPM;

	// Set Counting frequency to 1MHz --> 1µs resolution
	TIM3->PSC = (uint16_t)48 -1;

	// Set tDelay to 1000µs
	TIM3->CCR1 = (uint16_t) T_DELAY;

	// Set tPulse to 200µs
	TIM3->ARR = (uint16_t) T_PULSE;

	// Set CH1 to output
	TIM3->CCMR1 &= ~TIM_CCMR1_CC1S_Msk;

	// Set to PWM mode 1 for CH1
	TIM3->CCMR1 &= ~TIM_CCMR1_OC1M_Msk;
	TIM3->CCMR1 |= (0x06 << TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE;

	// Set CH1 active low
	TIM3->CCER |= TIM_CCER_CC1P;

	// Enable CH1 and CH2
	TIM3->CCER |= TIM_CCER_CC1E;

	// Enable Main Output
	TIM3->BDTR |= TIM_BDTR_MOE;

	// Enable Counter
	TIM3->CR1 |= TIM_CR1_CEN;
}

void Timer_input_capture_init(void)
{
	/*
	 * Using TIMER 1
	 * Input capture on CH1 and CH2 (dual edges capture) --> pin PA8 as AF2
	 * APB2 timers run at 48MHz --> PSC = 47 for 1MHz counting frequency
	 * Using reset mode --> when CH1 detects a rising edge, counter is reset
	 */

	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Set PA8 to alternate function
	GPIOA->MODER &= ~GPIO_MODER_MODER8_Msk;
	GPIOA->MODER |= (0x02 << GPIO_MODER_MODER8_Pos);

	// Set PA8 to AF2
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFRH0_Msk;
	GPIOA->AFR[1] |= (0x02 << GPIO_AFRH_AFRH0_Pos);

	// Enable TIM1 clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	// Reset TIM1 configuration
	TIM1->CR1 = 0x0000;
	TIM1->CR2 = 0x0000;

	// Set counting frequency to 1MHz --> 1µs resolution
	TIM1->PSC = (uint16_t) 48 -1;

	// Set ARR to maximum value (default)
	TIM1->ARR = (uint16_t)0xFFFF;

	// Set TIM1 to Master reset mode --> if TIM1 resets, it is used as TRGO to trigger TIM3
	TIM1->CR2 &= ~TIM_CR2_MMS_Msk;

	// Enable CC2IE interrupt
	TIM1->DIER |= TIM_DIER_CC2IE;

	// TIM1 counter trigger is TI1FP1 (echo from ultra-sonic sensor)
	TIM1->SMCR &= ~TIM_SMCR_TS_Msk;
	TIM1->SMCR |= (0x05 << TIM_SMCR_TS_Pos);

	// Enable reset mode (TIM1 resets when it receives echo signal)
	TIM1->SMCR &= ~TIM_SMCR_SMS_Msk;
	TIM1->SMCR |= (0x04 << TIM_SMCR_SMS_Pos);

	// Set CH1 as input mapped on TI1 and CH2 as input mapped on TI1 as well
	TIM1->CCMR1 &= ~(TIM_CCMR1_CC1S_Msk | TIM_CCMR1_CC2S_Msk);
	TIM1->CCMR1 |= (0x01 << TIM_CCMR1_CC1S_Pos) | (0x02 << TIM_CCMR1_CC2S_Pos);

	// Configure input filtering (fCK_INT, N = 8) for both channels
	TIM1->CCMR1 &= ~(TIM_CCMR1_IC1F_Msk | TIM_CCMR1_IC2F_Msk);
	TIM1->CCMR1 |= (0x03 << TIM_CCMR1_IC1F_Pos) | (0x03 << TIM_CCMR1_IC2F_Pos);

	// Set CH1 sensitive to rising edges and CH2 sensitive to falling edge
	TIM1->CCER &= ~(TIM_CCER_CC1P_Msk | TIM_CCER_CC2P_Msk);
	TIM1->CCER |= (0x01 << TIM_CCER_CC2P_Pos);

	// Enable CH1 and CH2
	TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

	// Enable counter
	TIM1->CR1 |= TIM_CR1_CEN;
}

void NVIC_init(void)
{
	// Set priority level to 5
	NVIC_SetPriority(TIM1_CC_IRQn, 5);

	// Enable TIM1 capture / compare interrupt
	NVIC_EnableIRQ(TIM1_CC_IRQn);
}
