/*
 * main.c
 *
 *  Created on: Jul 24, 2022
 *      Author: LECLERCQ Antonin
 *
 *
 *  HC-SR04 ultrasonic sensor demo code for STM32F072RB microcontroller
 */


#include <main.h>

static void SystemClock_Config(void);

int main(void)
{
	// CPU runs at 48MHz
	SystemClock_Config();

	Usart_init();
	m_printf("\r\nUltrasonic sensor demo\r\n");

	// Enable interrupts
	NVIC_init();

	Timer_input_capture_init();
	Timer_one_pulse_mode_init();

	uint16_t distance = 0;

	while(1)
	{
		// Convert TIM_CCR2 value to centimeters
		distance = (uint16_t)((float)TIM1->CCR2 * SOUND_SPEED / 20.f);

		m_printf("Distance: %d cm \r", distance);
	}

	return 0;
}

static void SystemClock_Config(void)
{
	uint32_t	HSE_Status = 0;
	uint32_t	PLL_Status = 0;
	uint32_t	SW_Status = 0;
	uint32_t	timeout = 100000;

	// Start HSE in Bypass Mode
	RCC->CR |= RCC_CR_HSEBYP;
	RCC->CR |= RCC_CR_HSEON;

	// Wait until HSE is ready
	do
	{
		HSE_Status = RCC->CR & RCC_CR_HSERDY_Msk;
		timeout--;
	} while ((HSE_Status == 0) && (timeout > 0));

	// Select HSE as PLL input source
	RCC->CFGR &= ~RCC_CFGR_PLLSRC_Msk;
	RCC->CFGR |= (0x02 << RCC_CFGR_PLLSRC_Pos);

	// Set PLL PREDIV to /1
	RCC->CFGR2 = 0x00000000;

	// Set PLL MUL to x6 --> fPLL = 8MHz * 6 = 48MHz (max)
	RCC->CFGR &= ~RCC_CFGR_PLLMUL_Msk;
	RCC->CFGR |= (0x04 << RCC_CFGR_PLLMUL_Pos);

	// Enable the main PLL
	RCC-> CR |= RCC_CR_PLLON;

	// Wait until PLL is ready
	do
	{
		PLL_Status = RCC->CR & RCC_CR_PLLRDY_Msk;
		timeout--;
	} while ((PLL_Status == 0) && (timeout > 0));

        // Set AHB prescaler to /1
	RCC->CFGR &= ~RCC_CFGR_HPRE_Msk;
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	//Set APB1 prescaler to /1
	RCC->CFGR &= ~RCC_CFGR_PPRE_Msk;
	RCC->CFGR |= RCC_CFGR_PPRE_DIV1;

	// Enable FLASH Prefetch Buffer and set Flash Latency
	FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;

	// Select the main PLL as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	// Wait until PLL becomes main switch input
	do
	{
		SW_Status = (RCC->CFGR & RCC_CFGR_SWS_Msk);
		timeout--;
	} while ((SW_Status != RCC_CFGR_SWS_PLL) && (timeout > 0));

	// Update SystemCoreClock global variable
	SystemCoreClockUpdate();
}
