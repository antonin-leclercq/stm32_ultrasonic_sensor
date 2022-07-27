/*
 * usart.c
 *
 *  Created on: 26 juil. 2022
 *      Author: LECLERCQ Antonin
 */

#include <usart.h>

void Usart_init(void)
{
	/*
	 * USART2 peripheral
	 * PA2 (TX) and PA3 (RX) as AF1
	 * APB1 peripherals run at 48MHz
	 * Default USART configuration (1 Start bit, 8 data bits, n stop bits)
	 * Baudrate = 9600 bauds --> USART_BRR = 48MHz / 9600 = 5000;
	 */

	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Set PA2 and PA3 as alternate function
	GPIOA->MODER &= ~(GPIO_MODER_MODER2_Msk | GPIO_MODER_MODER3_Msk);
	GPIOA->MODER |= (0x02 << GPIO_MODER_MODER2_Pos) | (0x02 << GPIO_MODER_MODER3_Pos);

	// Set PA2 and PA3 to AF1
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL2_Msk | GPIO_AFRL_AFRL3_Msk);
	GPIOA->AFR[0] |= (0x01 << GPIO_AFRL_AFRL2_Pos) | (0x01 << GPIO_AFRL_AFRL3_Pos);

	// Enable USART2 clock
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	// Reset USART2 configuration
	USART2->CR1 = 0x0000;
	USART2->CR2 = 0x0000;
	USART2->CR3 = 0x0000;

	// Set Baudrate to 9600
	USART2->BRR = (uint16_t) 5000;

	// Enable TX and RX
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;

	// Enable USART2
	USART2->CR1 |= USART_CR1_UE;
}
